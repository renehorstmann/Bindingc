#ifndef BINDINGC_FILTER_H
#define BINDINGC_FILTER_H

#include <stdbool.h>

#include "parsetypes.h"

void bc_filter_non_static(bc_ParsedFunctionArray *inout_array);

void bc_filter_name_prefix(bc_ParsedFunctionArray *inout_array, const char *name_prefix);

#endif //BINDINGC_FILTER_H
