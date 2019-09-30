#ifndef BINDINGC_PY3_GENERATE_H
#define BINDINGC_PY3_GENERATE_H

#include "../types.h"
#include "../bindingtypes.h"


char *bc_py3_generate_function_head(const char *name,
        const bc_ParameterArray *in_parameter, const bc_ParameterArray *out_parameter,
        bc_TypeArray types, int indent);

char *bc_py3_generate_info(const char *text, const char *returun_info,
        const bc_ParameterArray *parameter, int indent);

char *bc_py3_generate_function(const bc_Function *function, bc_TypeArray types, int indent);

char *bc_py3_generate_interface(const bc_Function *function, bc_TypeArray types, int indent);

#endif //BINDINGC_PY3_GENERATE_H
