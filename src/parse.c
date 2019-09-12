#include <limits.h>

#include "bindingc/utilc/alloc.h"

#include "bindingc/parse.h"

void bc_ParsedParameter_kill(bc_ParsedParameter *self) {
    Free0(self->name);
    Free0(self->type);
}

void bc_ParsedParameterInfo_kill(bc_ParsedParameterInfo *self) {
    Free0(self->name);
    Free0(self->default_value);
    Free0(self->info);
}

void bc_ParsedInfo_kill(bc_ParsedInfo *self) {
    for (int p = 0; self->parameter_infos[p].name; p++)
        bc_ParsedParameterInfo_kill(&self->parameter_infos[p]);
    Free0(self->text);
    Free0(self->return_info);
    Free0(self->parameter_infos);
}

void bc_ParsedFunction_kill(bc_ParsedFunction *self) {
    for (int p = 0; self->parameters[p].name; p++)
        bc_ParsedParameter_kill(&self->parameters[p]);
    bc_ParsedInfo_kill(&self->info);
    Free0(self->name);
    Free0(self->return_type);
    Free0(self->parameters);
}


bc_ParsedParameterInfo bc_parse_parameter_info_text(StrViu viu) {
    // string view such as "name=\"Peter\": the name of some guy."
    bc_ParsedParameterInfo res = {0};

    viu = sv_strip(viu, ' ');
    StrViu name = {viu.begin};


    StrViu info = sv_eat_until_multiple(viu, "=: ");
    if (sv_empty(info))
        return res;

    name.end = info.begin;
    name = sv_rstrip(name, ' ');

    if (*info.begin == '=') {
        StrViu behind_def = sv_eat_until_multiple(info, ": ");
        StrViu def = sv_strip((StrViu) {info.begin + 1, behind_def.begin}, ' ');
        res.default_value = sv_heap_cpy(def);
        info = behind_def;
    }

    // ignore "=:"
    if (!isspace(*info.begin))
        info.begin++;

    info = sv_lstrip(info, ' ');

    res.name = sv_heap_cpy(name);
    if (!sv_empty(info))
        res.info = sv_heap_cpy(info);
    return res;
}


static char *get_info_text_on_heap_(StrViu viu) {
    char *text = New0(char, 1);
    size_t text_len = 0;
    while (viu.begin < viu.end) {
        viu = sv_lstrip(viu, ' ');
        viu = sv_lstrip(viu, '/');
        viu = sv_lstrip(viu, '*');
        StrViu next_line = sv_eat_until(viu, '\n');
        StrViu line = sv_strip((StrViu) {viu.begin, next_line.begin}, ' ');
        viu = next_line;

        if (sv_empty(line))
            continue;

        if(text_len>0)
            text[text_len++] = ' ';   // space between each line

        size_t line_len = sv_length(line);
        text = ReNew(char, text, text_len + line_len + 1);
        sv_cpy(text + text_len, line);
        text_len += line_len;
    }
    return text;
}

bc_ParsedInfo bc_parse_info_text(StrViu viu) {
    bc_ParsedInfo res = {0};

    // remove leading and heading white spaces
    viu = sv_strip(viu, ' ');
    if (sv_empty(viu))
        return res;

    // if the last char is a /, remove smth like ****///
    if (*(viu.end - 1) == '/') {
        viu = sv_rstrip(viu, '/');
        viu = sv_rstrip(viu, '*');
    }
    if (sv_empty(viu))
        return res;


    res.parameter_infos = New0(bc_ParsedParameterInfo, 1);
    while (viu.begin < viu.end) {

        int next;
        {
            viu.begin++;    //ignore first @
            next = sv_find_first_cstring(viu, "@param");
            next = next < 0 ? INT_MAX : next+1;

            int next_return = sv_find_first_cstring(viu, "@return");
            next_return = next_return < 0 ? INT_MAX : next_return+1;
            if (next_return < next)
                next = next_return;

            int next_error = sv_find_first_cstring(viu, "@error");
            next_error = next_error < 0 ? INT_MAX : next_error+1;
            if (next_error < next)
                next = next_error;

            viu.begin--;
            int next_end = sv_length(viu);
            if (next_end < next)
                next = next_end;
        }

        StrViu item = {viu.begin, viu.begin + next};
        viu.begin = item.end;

        if (*item.begin != '@') {
            res.text = get_info_text_on_heap_(item);
            continue;
        }
        item.begin++;
        if (strncmp(item.begin, "param", 5) == 0) {
            item.begin += 5;
            item = sv_strip(item, ' ');
            int params = 0;
            while (res.parameter_infos[params].name)
                params++;

            res.parameter_infos = ReNew(bc_ParsedParameterInfo, res.parameter_infos, params + 2);  //2=new+end

            char *item_text = get_info_text_on_heap_(item);
            res.parameter_infos[params] = bc_parse_parameter_info_text(ToStrViu(item_text));
            free(item_text);

            // send end to 0
            memset(&res.parameter_infos[params + 1], 0, sizeof(bc_ParsedParameterInfo));
            continue;
        }
        if (strncmp(item.begin, "return", 6) == 0) {
            item.begin += 6;
            int start = sv_find_first(item, ':');
            if (start == -1)
                start = sv_find_first(item, ' ');
            item.begin += start + 1;
            item = sv_strip(item, ' ');
            res.return_info = get_info_text_on_heap_(item);
            continue;
        }
        if (strncmp(item.begin, "error", 5) == 0) {
            item.begin += 5;
            item = sv_strip(item, ' ');
            res.error_info = get_info_text_on_heap_(item);
            continue;
        }
    }

    // NULL if no parameters
    if(!res.parameter_infos[0].name)
        Free0(res.parameter_infos);

    return res;
}

bc_ParsedParameter bc_parse_parameter(StrViu viu) {
    // string view such as "const char *name" or "string IN s", ...
    bc_ParsedParameter res = {0};

    viu = sv_strip(viu, ' ');

    if(sv_empty(viu))
        return res;

    StrViu name = {viu.end, viu.end};
    StrViu type = {viu.begin};
    do {
        name.begin--;
    } while (name.begin > viu.begin && !isspace(*name.begin) && *name.begin != '*');
    if (name.begin == viu.begin)
        return res;

    name.begin++;
    type.end = name.begin;
    type = sv_rstrip(type, ' ');

    res.name = sv_heap_cpy(name);

    assert(!sv_empty(type));
    StrViuArray components = {0};
    while(type.begin < type.end) {
        int next = sv_find_first_multiple(type, "* ");
        if(next<0) {
            // rest of type is last component
            components.array = ReNew(StrViu, components.array, ++components.size);
            components.array[components.size - 1] = type;
            break;
        } else if(next == 0) {
            // should be a *
            components.array = ReNew(StrViu, components.array, ++components.size);
            components.array[components.size - 1] = (StrViu) {type.begin, type.begin + 1};
            type.begin++;
        } else {
            components.array = ReNew(StrViu, components.array, ++components.size);
            components.array[components.size - 1] = (StrViu) {type.begin, type.begin + next};
            type.begin+=next;
        }

        type = sv_lstrip(type, ' ');
    }

    size_t length = components.size;   // spaces + \0
    for(size_t i=0; i < components.size; i++)
        length += sv_length(components.array[i]);

    res.type = New(char, length);
    char *fill_type = res.type;
    for(size_t i=0; i < components.size; i++) {
        sv_cpy(fill_type, components.array[i]);
        fill_type+=sv_length(components.array[i]);
        *fill_type++ = ' ';
    }
    res.type[length-1] = '\0';

    return res;
}

bc_ParsedFunction bc_parse_function(StrViu info, StrViu function) {
    bc_ParsedFunction res = {0};


    int params_start = sv_find_first(function, '(');
    StrViu type_name = {function.begin, function.begin + params_start};
    function.begin += params_start + 1;

    type_name = sv_strip(type_name, ' ');
    int name_pos = sv_find_last(type_name, ' ');

    StrViu type = {type_name.begin, type_name.begin + name_pos};
    type = sv_rstrip(type, ' ');

    StrViu name = {type_name.begin + name_pos + 1, type_name.end};

    function = sv_eat_back_until(function, ')');
    function.end--;

    StrViuArray params = sv_split(function, ',');

    res.name = sv_heap_cpy(name);
    res.return_type = sv_heap_cpy(type);

    res.parameters = (bc_ParsedParameter *) New0(bc_ParsedParameter, params.size + 1);
    for (size_t i = 0; i < params.size; i++)
        res.parameters[i] = bc_parse_parameter(params.array[i]);

    res.info = bc_parse_info_text(info);

    return res;
}


void bc_parse_file(bc_ParsedFunction **functions, StrViu filetext) {


}
