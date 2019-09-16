#ifndef BINDINGC_FILTER_H
#define BINDINGC_FILTER_H

#include <stdbool.h>

#include "parsetypes.h"
#include "types.h"

bc_FunctionArray bc_filter_function_non_static(bc_FunctionArray array, bool free_array);

bc_FunctionArray bc_filter_function_name_prefix(bc_FunctionArray array, const char *name_prefix, bool free_array);

bc_ParameterArray bc_filter_parameter_name_prefix(bc_ParameterArray array, const char *name_prefix, bool free_array);

bc_ParameterArray bc_filter_parameter_diff(bc_ParameterArray a, bc_ParameterArray b);

#endif //BINDINGC_FILTER_H
