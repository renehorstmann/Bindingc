#include "bindingc/parse.h"


void bc_ParsedParameter_kill(bc_ParsedParameter *self) {
    free(self->name);
    free(self->type);
    self->name = self->type = NULL;
}

void bc_ParsedParameterInfo_kill(bc_ParsedParameterInfo *self) {
    free(self->name);
    free(self->default_value);
    free(self->info);
    self->name = self->default_value = self->info = NULL;
}

void bc_ParsedInfo_kill(bc_ParsedInfo *self) {
    for (int p = 0; self->parameterInfos[p].name; p++)
        bc_ParsedParameterInfo_kill(&self->parameterInfos[p]);
    free(self->text);
    free(self->return_info);
    free(self->parameterInfos);
    self->text = self->return_info = NULL;
    self->parameterInfos = NULL;
}

void bc_ParsedFunction_kill(bc_ParsedFunction *self) {
    for (int p = 0; self->parameters[p].name; p++)
        bc_ParsedParameter_kill(&self->parameters[p]);
    bc_ParsedInfo_kill(&self->info);
    free(self->name);
    free(self->return_type);
    free(self->parameters);
    self->name = self->return_type = NULL;
    self->parameters = NULL;
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

bc_ParsedInfo bc_parse_info_text(StrViu viu) {
    bc_ParsedInfo res = {0};
    res.parameterInfos = calloc(1, sizeof(bc_ParsedParameterInfo));
    size_t text_size = 0;
    while (viu.begin < viu.end) {
        viu = sv_lstrip(viu, ' ');
        viu = sv_lstrip(viu, '/');
        viu = sv_lstrip(viu, '*');
        StrViu next_line = sv_eat_until(viu, '\n');
        StrViu line = sv_strip((StrViu) {viu.begin, next_line.begin}, ' ');
        viu = next_line;
        if (sv_empty(line))
            continue;
        if (*line.begin != '@') {
            size_t len = sv_length(line);
            res.text = realloc(res.text, text_size + len + 1);  // if this this returns NULL, we have bigger problems
            sv_cpy(res.text + text_size, line);
            res.text[text_size + len] = 0;
            text_size += len + 1;
            continue;
        }
        line.begin++;
        if (strncmp(line.begin, "return", 6) == 0) {
            line.begin += 6;
            int start = sv_find_first(line, ':');
            if (start == -1)
                start = sv_find_first(line, ' ');
            line.begin += start + 1;
            line = sv_lstrip(line, ' ');
            int info_len = sv_length(line);
            res.return_info = malloc(info_len + 1);
            strncpy(res.return_info, line.begin, info_len);
            res.return_info[info_len] = 0;
            continue;
        }
        if(strncmp(line.begin, "error", 5) == 0) {
            line.begin += 5;
            line = sv_lstrip(line, ' ');
            int info_len = sv_length(line);
            res.error_info = malloc(info_len + 1);
            strncpy(res.error_info, line.begin, info_len);
            res.error_info[info_len] = 0;
            continue;
        }
        if (strncmp(line.begin, "param", 5) == 0) {
            line.begin += 5;
            line = sv_lstrip(line, ' ');
            int params = 0;
            while (res.parameterInfos[params].name)
                params++;

            res.parameterInfos = realloc(res.parameterInfos, sizeof(bc_ParsedParameterInfo) * (params + 2));
            res.parameterInfos[params] = bc_parse_parameter_info_text(line);
            memset(&res.parameterInfos[params + 1], 0, sizeof(bc_ParsedParameterInfo));
            continue;
        }
    }

    return res;
}

bc_ParsedParameter bc_parse_parameter(StrViu viu) {
    // string view such as "const char *name" or "string IN s", ...
    bc_ParsedParameter res = {0};

    viu = sv_strip(viu, ' ');

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
    res.type = sv_heap_cpy(type);
    return res;
}

bc_ParsedFunction bc_parse_function(StrViu info, StrViu function) {
    bc_ParsedFunction res = {0};


    int params_start = sv_find_first(function, '(');
    StrViu type_name = {function.begin, function.begin + params_start};
    function.begin += params_start+1;

    type_name = sv_strip(type_name, ' ');
    int name_pos = sv_find_last(type_name, ' ');

    StrViu type = {type_name.begin, type_name.begin+name_pos};
    type = sv_rstrip(type, ' ');

    StrViu name = {type_name.begin+name_pos+1, type_name.end};

    function = sv_eat_back_until(function, ')');
    function.end--;

    StrViuArray params = sv_split(function, ',');

    res.name = sv_heap_cpy(name);
    res.return_type = sv_heap_cpy(type);

    res.parameters = (bc_ParsedParameter*) calloc(params.size+1, sizeof(bc_ParsedParameter));
    for(size_t i=0; i<params.size; i++)
        res.parameters[i] = bc_parse_parameter(params.array[i]);

    res.info = bc_parse_info_text(info);

    return res;
}



void bc_parse_file(bc_ParsedFunction **functions, StrViu filetext) {


}
