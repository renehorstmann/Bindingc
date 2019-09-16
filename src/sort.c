#include <string.h>

#include <bindingc/utilc/alloc.h>

#include "bindingc/sort.h"

bc_ParameterArray bc_get_parameters(const bc_ParsedFunction *function) {
    bc_ParameterArray res = {0};
    if(function->parameters_len>0) {
        res.array = New0(bc_Parameter, function->parameters_len);
        for(size_t p=0; p < res.size; p++) {
            strcpy(res.array[p].name, function->parameters[p].name);
            strcpy(res.array[p].type, function->parameters[p].type);
            for(size_t i=0; i<function->info.parameter_infos_len; i++) {
                if(strcmp(res.array[p].name, function->info.parameter_infos[i].name) == 0) {
                    strcpy(res.array[p].default_value, function->info.parameter_infos[i].default_value);
                    strcpy(res.array[p].info, function->info.parameter_infos[i].info);
                }
            }
        }
    }
    return res;
}

bc_Function bc_get_function_without_paramaters(const bc_ParsedFunction *function) {
    bc_Function res = {""};
    strcpy(res.name, function->name);
    strcpy(res.return_type, function->return_type);
    strcpy(res.info, function->info.text);
    strcpy(res.return_info, function->info.return_info);
    strcpy(res.error_info, function->info.error_info);
    return res;
}

bc_FunctionArray bc_get_function_array_without_paramaters(const bc_ParsedFunctionArray *array) {
    bc_FunctionArray res = {0};
    if(array->size>0) {
        res.array = New0(bc_Function, array->size);
        res.size = array->size;
        for(size_t i=0; i<array->size; i++)
            res.array[i] = bc_get_function_without_paramaters(&array->array[i]);
    }
    return res;
}
