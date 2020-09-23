#include <string.h>
#include "bindingc/utilc/alloc.h"
#include "bindingc/reflectionc.h"

BcFunctionArray bc_function_array_from_reflectionc(HrFunctionArray array, bool kill_array) {
    BcFunctionArray res = {0};

    res.array = New0(BcFunction_s, array.size);
    res.size = array.size;

    for(int i=0; i<res.size; i++) {
        BcFunction_s *to = &res.array[i];
        HrFunction_s *from = &array.array[i];

        strcpy(to->name, from->name);
        strcpy(to->info, from->info.text);
        strcpy(to->error_info, from->info.error_info);
        strcpy(to->return_parameter.info, from->info.return_info);
        if(!strstr(from->return_type, "void"))
            strcpy(to->return_parameter.type, from->return_type);
        // return_parameter .name .default_value will be 0 (New0)

        to->parameters.size = from->parameters_size;
        for(int p=0; p<to->parameters.size; p++) {
            BcParameter_s *p_to = &to->parameters.array[p];
            HrParameter_s *p_from = &from->parameters[p];

            strcpy(p_to->name, p_from->name);
            strcpy(p_to->type, p_from->type);

            HrParameterInfo_s *pi_from = NULL;
            for(int pi=0; pi<from->info.parameter_infos_size; pi++) {
                if(strcmp(p_to->name, from->info.parameter_infos[pi].name) == 0)
                    pi_from = &from->info.parameter_infos[pi];
            }

            if(pi_from) {
                strcpy(p_to->default_value, pi_from->default_value);
                strcpy(p_to->info, pi_from->info);
            }
        }
    }


    if(kill_array)
        hr_function_array_kill(&array);
    return res;
}
