#include <stdlib.h>
#include <string.h>

#include "bindingc/utilc/dynarray.h"
#include "bindingc/utilc/alloc.h"

#include "bindingc/filter.h"


DynArray(int, IntArray)


static void apply_indices(bc_ParsedFunctionArray *inout_array, IntArray indices) {
    int kill_id = 0;
    for(size_t i=0; i<indices.size; i++) {
        while(kill_id < indices.array[i])
            bc_ParsedFunction_kill(&inout_array->functions[kill_id++]);
        kill_id = indices.array[i] + 1;
    }

    bc_ParsedFunction *filtered = NULL;
    if(indices.size>0) {
        filtered = New(bc_ParsedFunction, indices.size);
        for (size_t i = 0; i < indices.size; i++)
            filtered[i] = inout_array->functions[indices.array[i]];
    }
    free(inout_array->functions);
    inout_array->functions = filtered;
    inout_array->functions_len = indices.size;
}


void bc_filter_non_static(bc_ParsedFunctionArray *inout_array) {
    IntArray indices = {0};
    IntArray_set_capacity(&indices, inout_array->functions_len);

    for(size_t i=0; i < inout_array->functions_len; i++) {
        if(strstr(inout_array->functions[i].return_type, "static") == NULL)
            IntArray_push(&indices, i);
    }

    apply_indices(inout_array, indices);
    IntArray_kill(&indices);
}


void bc_filter_name_prefix(bc_ParsedFunctionArray *inout_array, const char *name_prefix) {
    IntArray indices = {0};
    IntArray_set_capacity(&indices, inout_array->functions_len);

    for(size_t i=0; i < inout_array->functions_len; i++) {
        if(strncmp(inout_array->functions[i].name, name_prefix, strlen(name_prefix)) == 0)
            IntArray_push(&indices, i);
    }

    apply_indices(inout_array, indices);
    IntArray_kill(&indices);
}


