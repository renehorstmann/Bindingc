#ifndef BINDINGC_SORT_H
#define BINDINGC_SORT_H

#include "parsetypes.h"
#include "types.h"

BcParameterArray_s bc_get_parameters(const BcParsedFunction_s *function);

BcFunction_s bc_get_function_without_paramaters(const BcParsedFunction_s *function);

BcFunctionArray bc_get_function_array_without_paramaters(const BcParsedFunctionArray *array);

#endif //BINDINGC_SORT_H
