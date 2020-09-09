#ifndef BINDINGC_PARSE_H
#define BINDINGC_PARSE_H

#include "utilc/strviu.h"

#include "parsetypes.h"


BcParsedParameterInfo_s bc_parse_parameter_info_text(strviu viu);

BcParsedInfo_s bc_parse_info_text(strviu viu);

char *bc_parse_type(strviu viu);

BcParsedParameter_s bc_parse_parameter(strviu viu);

BcParsedFunction_s bc_parse_function(strviu info, strviu definition);

//bc_ParsedTypeDeclaration bc_parse_type_declaration(strviu viu);

//bc_ParsedStruct bc_parse_struct(strviu info, strviu definition);

BcParsedFunctionArray bc_parse_file(strviu filetext);

#endif //BINDINGC_PARSE_H
