#ifndef BINDINGC_PY3_GENERATE_H
#define BINDINGC_PY3_GENERATE_H

#include "../types.h"
#include "../bindingtypes.h"


char *bc_py3_generate_function_head(const char *name,
                                    const BcParameterArray_s *in_parameter, const BcParameterArray_s *out_parameter,
                                    BcTypeArray types, int indent);

char *bc_py3_generate_info(const char *text, const char *returun_info,
                           const BcParameterArray_s *parameter, int indent);

char *bc_py3_generate_function(const BcFunction_s *function, BcTypeArray types, int indent);

char *bc_py3_generate_interface(const BcFunction_s *function, BcTypeArray types, int indent);

#endif //BINDINGC_PY3_GENERATE_H
