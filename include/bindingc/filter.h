#ifndef BINDINGC_FILTER_H
#define BINDINGC_FILTER_H

#include <stdbool.h>

#include "parsetypes.h"
#include "types.h"

BcFunctionArray bc_filter_function_non_static(BcFunctionArray array, bool free_array);

BcFunctionArray bc_filter_function_name_prefix(BcFunctionArray array, const char *name_prefix, bool free_array);

BcParameterArray_s bc_filter_parameter_name_prefix(BcParameterArray_s array, const char *name_prefix);

BcParameterArray_s bc_filter_parameter_diff(BcParameterArray_s a, BcParameterArray_s b);

#endif //BINDINGC_FILTER_H
