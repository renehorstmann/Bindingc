#include <stdlib.h>
#include <string.h>

#include "bindingc/utilc/dynarray.h"
#include "bindingc/utilc/alloc.h"

#include "bindingc/filter.h"


DynArray(int, IntArray)


static bc_FunctionArray apply_indices_function_array(bc_FunctionArray array, IntArray indices) {
    bc_FunctionArray res = {0};
    if (indices.size > 0) {
        res.array = New0(bc_function, (res.size = indices.size));
        for (size_t i = 0; i < indices.size; i++)
            res.array[i] = array.array[indices.array[i]];
    }
    return res;
}

static bc_parameterarray apply_indices_parameter_array(const bc_parameterarray *array, IntArray indices) {
    bc_parameterarray res = {0};
    if (indices.size > 0) {
        res.size = indices.size;
        for (size_t i = 0; i < indices.size; i++)
            res.array[i] = array->array[indices.array[i]];
    }
    return res;
}


bc_FunctionArray bc_filter_function_non_static(bc_FunctionArray array, bool free_array) {
    IntArray indices = {0};
    IntArray_set_capacity(&indices, array.size);

    for (size_t i = 0; i < array.size; i++) {
        if (strstr(array.array[i].return_parameter.type, "static") == NULL)
            IntArray_push(&indices, i);
    }

    bc_FunctionArray res = apply_indices_function_array(array, indices);
    IntArray_kill(&indices);
    if (free_array)
        bc_FunctionArray_kill(&array);
    return res;
}


bc_FunctionArray bc_filter_function_name_prefix(bc_FunctionArray array, const char *name_prefix, bool free_array) {
    IntArray indices = {0};
    IntArray_set_capacity(&indices, array.size);

    for (size_t i = 0; i < array.size; i++) {
        if (strncmp(array.array[i].c_name, name_prefix, strlen(name_prefix)) == 0)
            IntArray_push(&indices, i);
    }

    bc_FunctionArray res = apply_indices_function_array(array, indices);
    IntArray_kill(&indices);
    if (free_array)
        bc_FunctionArray_kill(&array);
    return res;
}

bc_parameterarray bc_filter_parameter_name_prefix(bc_parameterarray array, const char *name_prefix) {
    IntArray indices = {0};
    IntArray_set_capacity(&indices, array.size);

    for (size_t i = 0; i < array.size; i++) {
        if (strncmp(array.array[i].name, name_prefix, strlen(name_prefix)) == 0)
            IntArray_push(&indices, i);
    }

    bc_parameterarray res = apply_indices_parameter_array(&array, indices);
    IntArray_kill(&indices);
    return res;
}

bc_parameterarray bc_filter_parameter_diff(bc_parameterarray a, bc_parameterarray b) {
    bc_parameterarray res = {0};
    if (a.size > 0) {
        for (size_t a_i = 0; a_i < a.size; a_i++) {
            bool found = false;
            for (size_t b_i = 0; b_i < b.size; b_i++) {
                if (strcmp(a.array[a_i].name, b.array[b_i].name) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found)
                res.array[res.size++] = a.array[a_i];
        }
    }

    return res;
}
