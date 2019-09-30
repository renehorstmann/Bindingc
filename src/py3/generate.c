#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include <bindingc/utilc/strviu.h>
#include "bindingc/utilc/dynarray.h"
#include "bindingc/py3/generate.h"

DynArray(char, StrArray)



static void append(StrArray *arr, const char *str) {
    size_t old_size = arr->size;
    StrArray_resize(arr, old_size + strlen(str));
    strcpy(arr->array + old_size - 1, str);
}

static void append_indent(StrArray *arr, int level) {
    size_t old_size = arr->size;
    StrArray_resize(arr, old_size + level);
    for (int i = 0; i < level; i++)
        arr->array[old_size + i - 1] = ' ';
    arr->array[arr->size - 1] = '\0';
}


char *bc_py3_generate_function_head(const char *name,
                                    const bc_ParameterArray *in_parameter, const bc_ParameterArray *out_parameter,
                                    bc_TypeArray types, int indent) {
    StrArray res = {0};
    StrArray_resize(&res, 1);
    res.array[0] = '\0';

    append_indent(&res, indent);
    append(&res, "def ");
    append(&res, name);
    append(&res, "(");

    // parameters
    for (size_t i = 0; i < in_parameter->size; i++) {
        if (i > 0)
            append(&res, ", ");

        const bc_Parameter *param = &in_parameter->array[i];
        if (i > 1)
            append(&res, ", ");
        append(&res, param->name);
        append(&res, ": ");
        append(&res, bc_TypeArray_get(types, param->type)->out_name);
    }

    append(&res, ")");

    // return types
    if (out_parameter->size > 0) {
        append(&res, "\n");
        append_indent(&res, indent + 8);
        append(&res, "-> ");

        if (out_parameter->size > 1)
            append(&res, "Tuple[");

        for (size_t i = 0; i < out_parameter->size; i++) {
            if (i > 0)
                append(&res, ", ");
            append(&res, bc_TypeArray_get(types, out_parameter->array[i].type)->out_name);
        }
        if (out_parameter->size > 1)
            append(&res, "]");
    }
    append(&res, ":\n");

    return res.array;
}

char *bc_py3_generate_info(const char *text, const char *returun_info,
                           const bc_ParameterArray *parameter, int indent) {
    StrArray res = {0};
    StrArray_resize(&res, 1);
    res.array[0] = '\0';

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

        append(&res, "\n");
    }

    for (size_t i = 0; i < parameter->size; i++) {
        const bc_Parameter *param = &parameter->array[i];
        if (*param->info != 0) {
            append_indent(&res, indent);
            append(&res, ":param ");
            append(&res, param->name);
            append(&res, " ");
            append(&res, param->info);
            append(&res, "\n");
        }
    }

    if (returun_info && *returun_info != 0) {
        append_indent(&res, indent);
        append(&res, ":return ");
        append(&res, returun_info);
        append(&res, "\n");
    }

    append_indent(&res, indent);
    append(&res, "\"\"\"\n");

    return res.array;
}

char *bc_py3_generate_function(const bc_Function *function, bc_TypeArray types, int indent) {
    StrArray res = {0};
    StrArray_resize(&res, 1);
    res.array[0] = '\0';

    // todo
    bool return_is_error = false;
    bool return_is_output = false;
    bool info_available = *function->info != 0 || *function->return_parameter.info != 0;
    if (!info_available) {
        for (size_t i = 0; i < function->parameters.size; i++) {
            if (*function->parameters.array[i].info != 0)
                info_available = true;
        }
    }

    bc_ParameterArray in_params = {0};
    bc_ParameterArray out_params = {0};

    if (function->return_parameter.is_output)
        out_params.array[out_params.size++] = function->return_parameter;

    for (size_t i = 0; i < function->parameters.size; i++) {
        if (function->parameters.array[i].is_input)
            in_params.array[in_params.size++] = function->parameters.array[i];
        if (function->parameters.array[i].is_output)
            out_params.array[out_params.size++] = function->parameters.array[i];
    }

    // head
    char *function_head = bc_py3_generate_function_head(function->c_name, &in_params, &out_params, types, indent);
    append(&res, function_head);
    free(function_head);

    // info
    char *info = bc_py3_generate_info(function->info,
                                      function->return_parameter.info, &in_params, indent + 4);
    append(&res, info);
    free(info);

    // creations
    // todo

    // call
    append_indent(&res, indent + 4);
    append(&res, "_ret_val_ = _lib.");
    append(&res, function->c_name);
    append(&res, "(");
    for(size_t i=0; i<function->parameters.size; i++) {
        if(i>=1)
            append(&res, ", ");
        append(&res, function->parameters.array[i].name);
    }
    append(&res, ")\n");

    // todo check errors n this stuff (return ?)

    // return
    append(&res, "return _ret_val_\n");

    return res.array;
}


char *bc_py3_generate_interface(const bc_Function *function, bc_TypeArray types, int indent) {
    StrArray res = {0};
    StrArray_resize(&res, 1);
    res.array[0] = '\0';

    // parameters
    if(function->parameters.size>0) {
        append_indent(&res, indent);
        append(&res, "_lib.");
        append(&res, function->c_name);
        append(&res, ".argtypes = [");
        for(size_t i=0; i<function->parameters.size; i++) {
            if(i>=1)
                append(&res, ", ");
            append(&res, bc_TypeArray_get(types, function->parameters.array[i].type)->c_name);
        }
        append(&res, "]\n");
    }

    // return value
    if(*function->return_parameter.type) {
        append_indent(&res, indent);
        append(&res, "_lib.");
        append(&res, function->c_name);
        append(&res, ".restype = ");
        append(&res, bc_TypeArray_get(types, function->return_parameter.type)->out_name);
        append(&res, "\n");
    }

    return res.array;
}
