#include <limits.h>
#include <stdio.h>

#include "bindingc/utilc/alloc.h"
#include "bindingc/parse.h"

#include "bindingc/log.h"

BcParsedParameterInfo_s bc_parse_parameter_info_text(strviu viu) {
    // string view such as "c_name=\"Peter\": the c_name of some guy."
    BcParsedParameterInfo_s res = {0};

    viu = sv_strip(viu, ' ');
    strviu name = {viu.begin};


    strviu info = sv_eat_until_multiple(viu, "=: ");
    if (sv_empty(info))
        return res;

    name.end = info.begin;
    name = sv_rstrip(name, ' ');

    if (*info.begin == '=') {
        strviu behind_def = sv_eat_until_multiple(info, ": ");
        strviu def = sv_strip((strviu) {info.begin + 1, behind_def.begin}, ' ');
        if(sv_length(def) >= sizeof(res.default_value))
            log_warn("Parameter default value string is too long: %.*s", sv_length(def), def.begin);
        sv_ncpy(res.default_value, def, sizeof(res.default_value));
        info = behind_def;
    }

    // ignore "=:"
    if (!isspace(*info.begin))
        info.begin++;

    info = sv_lstrip(info, ' ');

    if(sv_length(name) >= sizeof(res.name))
        log_warn("Parameter name string is too long: %.*s", sv_length(name), name.begin);
    sv_ncpy(res.name, name, sizeof(res.name));

    if(sv_length(info) >= sizeof(res.info))
        log_warn("Parameter info string is too long: %.*s", sv_length(info), info.begin);
    sv_ncpy(res.info, info, sizeof(res.info));
    return res;
}


static char *get_info_text_on_heap_(strviu viu) {
    char *text = New0(char, 1);
    size_t text_len = 0;
    while (viu.begin < viu.end) {
        viu = sv_lstrip(viu, ' ');
        viu = sv_lstrip(viu, '/');
        viu = sv_lstrip(viu, '*');
        strviu next_line = sv_eat_until(viu, '\n');
        strviu line = sv_strip((strviu) {viu.begin, next_line.begin}, ' ');
        viu = next_line;

        if (sv_empty(line))
            continue;

        if (text_len > 0)
            text[text_len++] = ' ';   // space between each line

        size_t line_len = sv_length(line);
        text = ReNew(char, text, text_len + line_len + 1);
        sv_cpy(text + text_len, line);
        text_len += line_len;
    }
    return text;
}

BcParsedInfo_s bc_parse_info_text(strviu viu) {
    BcParsedInfo_s res = {0};

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

    while (viu.begin < viu.end) {

        int next;
        {
            viu.begin++;    //ignore first @
            next = sv_find_first_cstring(viu, "@param");
            next = next < 0 ? INT_MAX : next + 1;

            int next_return = sv_find_first_cstring(viu, "@return");
            next_return = next_return < 0 ? INT_MAX : next_return + 1;
            if (next_return < next)
                next = next_return;

            int next_error = sv_find_first_cstring(viu, "@error");
            next_error = next_error < 0 ? INT_MAX : next_error + 1;
            if (next_error < next)
                next = next_error;

            viu.begin--;
            int next_end = sv_length(viu);
            if (next_end < next)
                next = next_end;
        }

        strviu item = {viu.begin, viu.begin + next};
        viu.begin = item.end;

        if (*item.begin != '@') {
            char *info = get_info_text_on_heap_(item);
            if(strlen(info) >= sizeof(res.text))
                log_warn("Info test string is too long: %s", info);
            strncpy(res.text, info, sizeof(res.text));
            res.text[sizeof(res.text)-1] = '\0';
            free(info);
            continue;
        }
        item.begin++;
        if (strncmp(item.begin, "param", 5) == 0) {
            item.begin += 5;
            item = sv_strip(item, ' ');

            res.parameter_infos_len++;
            if (res.parameter_infos_len > MAX_PARAMETERS) {
                res.parameter_infos_len = MAX_PARAMETERS;
                log_warn("Too many parameter infos, only %d are allowed", MAX_PARAMETERS);
                continue;
            }

            char *item_text = get_info_text_on_heap_(item);
            res.parameter_infos[res.parameter_infos_len - 1] = bc_parse_parameter_info_text(ToStrViu(item_text));
            free(item_text);
            continue;
        }
        if (strncmp(item.begin, "return", 6) == 0) {
            item.begin += 6;
            int start = sv_find_first(item, ':');
            if (start == -1)
                start = sv_find_first(item, ' ');
            item.begin += start + 1;
            item = sv_strip(item, ' ');
            char *info = get_info_text_on_heap_(item);
            if(strlen(info) >= sizeof(res.return_info))
                log_warn("Return info text string is too long: %s", info);
            strncpy(res.return_info, info, sizeof(res.return_info));
            res.return_info[sizeof(res.return_info)-1] = '\0';
            free(info);
            continue;
        }
        if (strncmp(item.begin, "error", 5) == 0) {
            item.begin += 5;
            item = sv_strip(item, ' ');
            char *info = get_info_text_on_heap_(item);
            if(strlen(info) >= sizeof(res.error_info))
                log_warn("Error info text string is too long: %s", info);
            strncpy(res.error_info, info, sizeof(res.error_info));
            res.error_info[sizeof(res.error_info)-1] = '\0';
            free(info);
            continue;
        }
    }

    return res;
}


char *bc_parse_type(strviu viu) {
    char *res;

    strviuarray components = {0};
    while (viu.begin < viu.end) {
        int next = sv_find_first_multiple(viu, "* ");
        if (next < 0) {
            // rest of viu is last component
            components.size++;
            if(components.size >= STRVIUARRAY_SIZE) {
                log_error("Too many type components");
                return NULL;
            }
            components.array[components.size - 1] = viu;
            break;
        } else if (next == 0) {
            // should be a *
            components.size++;
            if(components.size >= STRVIUARRAY_SIZE) {
                log_error("Too many type components");
                return NULL;
            }
            components.array[components.size - 1] = (strviu) {viu.begin, viu.begin + 1};
            viu.begin++;
        } else {
            components.size++;
            if(components.size >= STRVIUARRAY_SIZE) {
                log_error("Too many type components");
                return NULL;
            }
            components.array[components.size - 1] = (strviu) {viu.begin, viu.begin + next};
            viu.begin += next;
        }

        viu = sv_lstrip(viu, ' ');
    }

    size_t length = components.size;   // spaces + \0
    for (size_t i = 0; i < components.size; i++)
        length += sv_length(components.array[i]);

    res = New(char, length);
    char *fill_viu = res;
    for (size_t i = 0; i < components.size; i++) {
        sv_cpy(fill_viu, components.array[i]);
        fill_viu += sv_length(components.array[i]);
        *fill_viu++ = ' ';
    }
    res[length - 1] = '\0';
    return res;
}

BcParsedParameter_s bc_parse_parameter(strviu viu) {
    // string view such as "const char *c_name" or "string IN s", ...
    BcParsedParameter_s res = {0};

    viu = sv_strip(viu, ' ');

    if (sv_empty(viu))
        return res;

    strviu name = {viu.end, viu.end};
    strviu type = {viu.begin};
    do {
        name.begin--;
    } while (name.begin > viu.begin && !isspace(*name.begin) && *name.begin != '*');
    if (name.begin == viu.begin)
        return res;

    name.begin++;
    type.end = name.begin;
    type = sv_rstrip(type, ' ');
    if(sv_empty(type)) {
        log_error("Parameter type is not available: %.*s", sv_length(viu), viu.begin);
        return res;
    }

    if(sv_length(name) >= sizeof(res.name))
        log_warn("Parameter name string is too long: %.*s", sv_length(name), name.begin);
    sv_ncpy(res.name, name, sizeof(res.name));

    char *type_string = bc_parse_type(type);
    if(strlen(type_string) >= sizeof(res.type))
        log_warn("Parameter name string is too long: %s", type_string);
    strncpy(res.type, type_string, sizeof(res.type));
    res.type[sizeof(res.type)-1] = '\0';
    free(type_string);

    return res;
}

BcParsedFunction_s bc_parse_function(strviu info, strviu definition) {
    BcParsedFunction_s res = {0};

    res.info = bc_parse_info_text(info);

    int params_start = sv_find_first(definition, '(');
    if (params_start < 0)
        return res;

    strviu type_name = {definition.begin, definition.begin + params_start};
    definition.begin += params_start + 1;

    type_name = sv_strip(type_name, ' ');
    int name_pos = sv_find_last_multiple(type_name, "* ");

    strviu type = {type_name.begin, type_name.begin + name_pos + 1};
    type = sv_rstrip(type, ' ');

    strviu name = {type_name.begin + name_pos + 1, type_name.end};

    definition = sv_eat_back_until(definition, ')');
    if (sv_empty(definition))
        return res;

    definition.end--;

    strviuarray params = sv_split(definition, ',');

    if(params.size >= MAX_PARAMETERS) {
        log_error("Function has to many parameters: %.*s", sv_length(name), name.begin);
        return res;
    }

    if(sv_length(name) >= sizeof(res.name))
        log_warn("Function name string is too long: %.*s", sv_length(name), name.begin);
    sv_ncpy(res.name, name, sizeof(res.name));


    char *type_string = bc_parse_type(type);
    if(strlen(type_string) >= sizeof(res.return_type))
        log_warn("Function return type string is too long: %s", type_string);
    strncpy(res.return_type, type_string, sizeof(res.return_type));
    res.return_type[sizeof(res.return_type)-1] = '\0';
    free(type_string);

    res.parameters_len = params.size;
    for (size_t i = 0; i < res.parameters_len; i++)
        res.parameters[i] = bc_parse_parameter(params.array[i]);

    return res;
}


//bc_ParsedTypeDeclaration bc_parse_type_declaration(strviu viu) {
//    bc_ParsedTypeDeclaration res = {0};
//
//    viu = sv_strip(viu, ' ');
//    if(sv_empty(viu))
//        return res;
//
//    shortstring add_type = "";
//    if(*(viu.end-1) == ']') {
//        // array
//        int end_pos = sv_length(viu);
//        do {
//            viu = sv_eat_back_until(viu, '[');
//            viu.end--;
//            viu = sv_rstrip(viu, ' ');
//        } while(!sv_empty(viu) && *(viu.end-1) == ']');
//        strviu add = {viu.end, viu.end + end_pos};
//        add_type[0] = ' ';
//        assert(sv_length(add)+1 < sizeof(add_type));
//        sv_cpy(add_type+1, add);
//
//    } else if(*(viu.end-1) == ')') {
//        // function ptr
//        int fun_ptr_close = sv_find_first(viu, ')');
//        viu.end = viu.begin+fun_ptr_close;
//        strcpy(add_type, " (*)");
//    }
//
//    assert(sv_find_first(viu, ',') == -1);
//
//    int name_start = sv_find_last_multiple(viu, "* ") + 1;
//    strviu name = {viu.begin+name_start, viu.end};
//
//    if(strcmp(add_type, " (*)") == 0) {
//        int fun_ptr_open = sv_find_last(viu, '(');
//        name_start = fun_ptr_open-1;
//    }
//    strviu type = {viu.begin, viu.begin + name_start};
//
//    assert(sv_length(name) < sizeof(res.name));
//    sv_cpy(res.name, name);
//
//    char *type_string = bc_parse_type(type);
//    size_t type_string_len = strlen(type_string);
//    assert(type_string_len + strlen(add_type) < sizeof(res.type));
//    strcpy(res.type, type_string);
//    strcpy(res.type+type_string_len, add_type);
//    free(type_string);
//
//    return res;
//}

//bc_ParsedStruct bc_parse_struct(strviu info, strviu definition) {
//    bc_ParsedStruct res = {0};
//
//    bc_ParsedInfo parsed_info = bc_parse_info_text(info);
//    strcpy(res.info, parsed_info.text);
//    bc_ParsedInfo_kill(&parsed_info);
//
//
//
//    return res;
//}


