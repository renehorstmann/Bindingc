#ifndef BINDINGC_PY3_GENERATE_H
#define BINDINGC_PY3_GENERATE_H

#include "../parsedtypes.h"
#include "../bindingtypes.h"

char *bc_py3_generate_function_head(const char *name, BcParameterArray_s parameter, BcTypeArray types, int indent);

char *bc_py3_generate_info(const char *text, const char *return_info,
                           BcParameterArray_s parameter, int indent);

char *bc_py3_generate_function(BcFunction_s function, BcTypeArray types, int indent);

char *bc_py3_generate_interface(BcFunction_s function, BcTypeArray types, int indent);

char *bc_py3_generate_functions(BcFunctionArray functions, BcTypeArray types, int indent);

#endif //BINDINGC_PY3_GENERATE_H
