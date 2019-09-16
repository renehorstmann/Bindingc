#ifndef BINDINGC_PARSE_H
#define BINDINGC_PARSE_H

#include "utilc/strviu.h"

#include "parsetypes.h"


bc_ParsedParameterInfo bc_parse_parameter_info_text(StrViu viu);

bc_ParsedInfo bc_parse_info_text(StrViu viu);

char *bc_parse_type(StrViu viu);

bc_ParsedParameter bc_parse_parameter(StrViu viu);

bc_ParsedFunction bc_parse_function(StrViu info, StrViu function);

bc_ParsedFunctionArray bc_parse_file(StrViu filetext);

#endif //BINDINGC_PARSE_H
