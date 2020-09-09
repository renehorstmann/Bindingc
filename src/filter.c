#include <stdlib.h>
#include <string.h>

#include "bindingc/utilc/dynarray.h"
#include "bindingc/utilc/alloc.h"

#include "bindingc/filter.h"


DynArray(int, IntArray, int_array)


static BcFunctionArray apply_indices_function_array(BcFunctionArray array, IntArray indices) {
    BcFunctionArray res = {0};
    if (indices.size > 0) {
        res.array = New0(BcFunction_s, (res.size = indices.size));
        for (size_t i = 0; i < indices.size; i++)
            res.array[i] = array.array[indices.array[i]];
    }
    return res;
}

static BcParameterArray_s apply_indices_parameter_array(const BcParameterArray_s *array, IntArray indices) {
    BcParameterArray_s res = {0};
    if (indices.size > 0) {
        res.size = indices.size;
        for (size_t i = 0; i < indices.size; i++)
            res.array[i] = array->array[indices.array[i]];
    }
    return res;
}


BcFunctionArray bc_filter_function_non_static(BcFunctionArray array, bool free_array) {
    IntArray indices = {0};
    int_array_set_capacity(&indices, array.size);

    for (size_t i = 0; i < array.size; i++) {
        if (strstr(array.array[i].return_parameter.type, "static") == NULL)
            int_array_push(&indices, i);
    }

    BcFunctionArray res = apply_indices_function_array(array, indices);
    int_array_kill(&indices);
    if (free_array)
        bc_function_array_kill(&array);
    return res;
}


BcFunctionArray bc_filter_function_name_prefix(BcFunctionArray array, const char *name_prefix, bool free_array) {
    IntArray indices = {0};
    int_array_set_capacity(&indices, array.size);

    for (size_t i = 0; i < array.size; i++) {
        if (strncmp(array.array[i].c_name, name_prefix, strlen(name_prefix)) == 0)
            int_array_push(&indices, i);
    }

    BcFunctionArray res = apply_indices_function_array(array, indices);
    int_array_kill(&indices);
    if (free_array)
        bc_function_array_kill(&array);
    return res;
}

BcParameterArray_s bc_filter_parameter_name_prefix(BcParameterArray_s array, const char *name_prefix) {
    IntArray indices = {0};
    int_array_set_capacity(&indices, array.size);

    for (size_t i = 0; i < array.size; i++) {
        if (strncmp(array.array[i].name, name_prefix, strlen(name_prefix)) == 0)
            int_array_push(&indices, i);
    }

    BcParameterArray_s res = apply_indices_parameter_array(&array, indices);
    int_array_kill(&indices);
    return res;
}

BcParameterArray_s bc_filter_parameter_diff(BcParameterArray_s a, BcParameterArray_s b) {
    BcParameterArray_s res = {0};
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
