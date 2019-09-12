#ifndef BINDINGC_PARSE_H
#define BINDINGC_PARSE_H

#include "utilc/strviu.h"

typedef struct {
    char *name;
    char *type;
} bc_ParsedParameter;

typedef struct {
    char *name;
    char *default_value;
    char *info;
} bc_ParsedParameterInfo;

typedef struct {
    char *text;
    char *return_info;
    char *error_info;
    bc_ParsedParameterInfo *parameter_infos; // ends with a NULL component
} bc_ParsedInfo;

typedef struct {
    char *name;
    char *return_type;
    bc_ParsedParameter *parameters; // ends with a NULL component
    bc_ParsedInfo info;
} bc_ParsedFunction;


void bc_ParsedParameter_kill(bc_ParsedParameter *self);
void bc_ParsedParameterInfo_kill(bc_ParsedParameterInfo *self);
void bc_ParsedInfo_kill(bc_ParsedInfo *self);
void bc_ParsedFunction_kill(bc_ParsedFunction *self);


bc_ParsedParameterInfo bc_parse_parameter_info_text(StrViu viu);

bc_ParsedInfo bc_parse_info_text(StrViu viu);

char *bc_parse_type(StrViu viu);

bc_ParsedParameter bc_parse_parameter(StrViu viu);

bc_ParsedFunction bc_parse_function(StrViu info, StrViu function);

void bc_parse_file(bc_ParsedFunction **functions, StrViu filetext);

#endif //BINDINGC_PARSE_H
