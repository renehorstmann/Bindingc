#include <string.h>

#include <bindingc/utilc/alloc.h>

#include "bindingc/sort.h"

bc_parameterarray bc_get_parameters(const bc_ParsedFunction *function) {
    bc_parameterarray res = {0};
    if(function->parameters_len>0) {
        res.size = function->parameters_len;
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

bc_function bc_get_function_without_paramaters(const bc_ParsedFunction *function) {
    bc_function res = {0};
    strcpy(res.c_name, function->name);
    strcpy(res.out_name, function->name);
    strcpy(res.info, function->info.text);
    strcpy(res.error_info, function->info.error_info);
    strcpy(res.return_parameter.type, function->return_type);
    strcpy(res.return_parameter.info, function->info.return_info);
    strcpy(res.return_parameter.name, "return");
    return res;
}

bc_FunctionArray bc_get_function_array_without_paramaters(const bc_ParsedFunctionArray *array) {
    bc_FunctionArray res = {0};
    if(array->size>0) {
        res.array = New0(bc_function, (res.size = array->size));
        for(size_t i=0; i<array->size; i++)
            res.array[i] = bc_get_function_without_paramaters(&array->array[i]);
    }
    return res;
}
