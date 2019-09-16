#ifndef BINDINGC_SORT_H
#define BINDINGC_SORT_H

#include "parsetypes.h"
#include "types.h"

bc_ParameterArray bc_get_parameters(const bc_ParsedFunction *function);

bc_Function bc_get_function_without_paramaters(const bc_ParsedFunction *function);

bc_FunctionArray bc_get_function_array_without_paramaters(const bc_ParsedFunctionArray *array);


#endif //BINDINGC_SORT_H
