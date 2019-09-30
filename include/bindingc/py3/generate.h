#ifndef BINDINGC_PY3_GENERATE_H
#define BINDINGC_PY3_GENERATE_H

#include "../types.h"
#include "../bindingtypes.h"


char *bc_py3_generate_function_head(const char *name,
                                    const bc_parameterarray *in_parameter, const bc_parameterarray *out_parameter,
                                    bc_TypeArray types, int indent);

char *bc_py3_generate_info(const char *text, const char *returun_info,
                           const bc_parameterarray *parameter, int indent);

char *bc_py3_generate_function(const bc_function *function, bc_TypeArray types, int indent);

char *bc_py3_generate_interface(const bc_function *function, bc_TypeArray types, int indent);

#endif //BINDINGC_PY3_GENERATE_H
