#ifndef BINDINGC_PARSE_H
#define BINDINGC_PARSE_H

#include "utilc/strviu.h"

#include "parsetypes.h"


bc_ParsedParameterInfo bc_parse_parameter_info_text(strviu viu);

bc_ParsedInfo bc_parse_info_text(strviu viu);

char *bc_parse_type(strviu viu);

bc_ParsedParameter bc_parse_parameter(strviu viu);

bc_ParsedFunction bc_parse_function(strviu info, strviu definition);

//bc_ParsedTypeDeclaration bc_parse_type_declaration(strviu viu);

//bc_ParsedStruct bc_parse_struct(strviu info, strviu definition);

bc_ParsedFunctionArray bc_parse_file(strviu filetext);

#endif //BINDINGC_PARSE_H
