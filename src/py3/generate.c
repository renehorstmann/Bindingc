#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include <bindingc/utilc/strviu.h>
#include "bindingc/utilc/dynarray.h"
#include "bindingc/py3/generate.h"

DynArray(char, StrArray, str_array)



static void append(StrArray *arr, const char *str) {
    str_array_push_array(arr, str, strlen(str));
}

static void append_indent(StrArray *arr, int level) {
    int old_size = arr->size;
    str_array_resize(arr, old_size + level);
    for (int i = 0; i < level; i++)
        arr->array[old_size + i] = ' ';
}


char *bc_py3_generate_function_head(const char *name, BcParameterArray_s parameter, BcTypeArray types, int indent) {
    StrArray res = {0};

    append_indent(&res, indent);
    append(&res, "def ");
    append(&res, name);
    append(&res, "(");

    // parameters
    int in_cnt = 0;
    int out_cnt = 0;
    for (int i = 0; i < parameter.size; i++) {
        BcParameter_s param = parameter.array[i];
        if(sv_begins_with_cstring(ToStrViu(param.name), "out_")) {
            out_cnt++;
            continue;
        }
        in_cnt++;

        if (in_cnt > 1)
            append(&res, ", ");

        append(&res, param.name);
        append(&res, ": ");
        append(&res, bc_type_array_get(types, param.type)->out_name);
    }

    append(&res, ")");

    // return types
    if (out_cnt > 0) {
        append(&res, "\n");
        append_indent(&res, indent + 8);
        append(&res, "-> ");

        if (out_cnt > 1)
            append(&res, "Tuple[");

        out_cnt = 0;
        for (int i = 0; i < parameter.size; i++) {
            BcParameter_s param = parameter.array[i];
            if(!sv_begins_with_cstring(ToStrViu(param.name), "out_"))
                continue;
            out_cnt++;
            if (out_cnt > 1)
                append(&res, ", ");
            append(&res, bc_type_array_get(types, param.type)->out_name);
        }
        if (out_cnt > 1)
            append(&res, "]");
    }
    append(&res, ":\n");

    str_array_push(&res, '\0');
    return res.array;
}

char *bc_py3_generate_info(const char *text, const char *return_info,
                           BcParameterArray_s parameter, int indent) {
    StrArray res = {0};

    append_indent(&res, indent);
    append(&res, "\"\"\"\n");

    if (text && *text != 0) {
        append_indent(&res, indent);
        strviu viu = ToStrViu(text);
        int next_line;
        do {
            strviu line;
            next_line = sv_find_first(viu, '\n');
            if (next_line == -1)
                line = viu;
            else
                line = (strviu) {viu.begin, viu.begin + next_line + 1};

            char *line_string = sv_heap_cpy(line);
            append(&res, line_string);
            append_indent(&res, indent);
            free(line_string);
            viu.begin += next_line + 1;
        } while (next_line >= 0);

        append(&res, "\n\n");
    }

    for (int i = 0; i < parameter.size; i++) {
        BcParameter_s param = parameter.array[i];
        if(sv_begins_with_cstring(ToStrViu(param.name), "out_"))
            continue;
        if (*param.info != 0) {
            append_indent(&res, indent);
            append(&res, ":param ");
            append(&res, param.name);
            append(&res, " ");
            append(&res, param.info);
            append(&res, "\n");
        }
    }

    if (return_info && *return_info != 0) {
        append_indent(&res, indent);
        append(&res, ":return ");
        append(&res, return_info);
        append(&res, "\n");
    }

    append_indent(&res, indent);
    append(&res, "\"\"\"\n");

    str_array_push(&res, '\0');
    return res.array;
}

char *bc_py3_generate_function(BcFunction_s function, BcTypeArray types, int indent) {
    StrArray res = {0};

    // todo
    bool return_is_error = false;
    bool return_is_output = false;
    bool info_available = *function.info != 0 || *function.return_parameter.info != 0;
    if (!info_available) {
        for (int i = 0; i < function.parameters.size; i++) {
            if (*function.parameters.array[i].info != 0)
                info_available = true;
        }
    }

    // head
    char *function_head = bc_py3_generate_function_head(function.name, function.parameters, types, indent);
    append(&res, function_head);
    free(function_head);

    // info
    char *info = bc_py3_generate_info(function.info,
                                      function.return_parameter.info, function.parameters, indent + 4);
    append(&res, info);
    free(info);

    // creations
    // todo
    BcParameterArray_s parameters = function.parameters;

    // call
    append_indent(&res, indent + 4);
    append(&res, "_ret_val_ = _lib.");
    append(&res, function.name);
    append(&res, "(");
    for(int i=0; i<function.parameters.size; i++) {
        if(i>=1)
            append(&res, ", ");
        char param_call[128];
        bc_apply_template(param_call, 128,
                          bc_type_array_get(types, parameters.array[i].type)->to_c_template,
                          parameters.array[i].name);
        append(&res, param_call);
    }
    append(&res, ")\n");

    // todo check errors n this stuff (return ?)

    // return
    append_indent(&res, indent + 4);
    append(&res, "return _ret_val_\n");

    str_array_push(&res, '\0');
    return res.array;
}


char *bc_py3_generate_interface(BcFunction_s function, BcTypeArray types, int indent) {
    StrArray res = {0};

    // parameters
    if(function.parameters.size>0) {
        append_indent(&res, indent);
        append(&res, "_lib.");
        append(&res, function.name);
        append(&res, ".argtypes = [");
        for(int i=0; i<function.parameters.size; i++) {
            if(i>=1)
                append(&res, ", ");
            append(&res, bc_type_array_get(types, function.parameters.array[i].type)->c_name);
        }
        append(&res, "]\n");
    }

    // return value
    if(*function.return_parameter.type) {
        append_indent(&res, indent);
        append(&res, "_lib.");
        append(&res, function.name);
        append(&res, ".restype = ");
        append(&res, bc_type_array_get(types, function.return_parameter.type)->out_name);
        append(&res, "\n");
    }

    str_array_push(&res, '\0');
    return res.array;
}


char *bc_py3_generate_functions(BcFunctionArray functions, BcTypeArray types, int indent) {
    StrArray res = {0};

    for(int i=0; i<functions.size; i++) {
        char *iface = bc_py3_generate_interface(functions.array[i], types, indent);
        append(&res, iface);
        free(iface);

        char *fun = bc_py3_generate_function(functions.array[i], types, indent);
        append(&res, fun);
        free(fun);
        append(&res, "\n\n");
    }

    str_array_push(&res, '\0');
    return res.array;
}
