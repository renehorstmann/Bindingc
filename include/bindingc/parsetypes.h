#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

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
    bc_ParsedParameterInfo *parameter_infos;
    size_t parameter_infos_len;
} bc_ParsedInfo;

typedef struct {
    char *name;
    char *return_type;
    bc_ParsedParameter *parameters;
    size_t parameters_len;
    bc_ParsedInfo info;
} bc_ParsedFunction;

typedef struct {
    bc_ParsedFunction *functions;
    size_t functions_len;
} bc_ParsedFunctionArray;

void bc_ParsedParameter_kill(bc_ParsedParameter *self);
void bc_ParsedParameterInfo_kill(bc_ParsedParameterInfo *self);
void bc_ParsedInfo_kill(bc_ParsedInfo *self);
void bc_ParsedFunction_kill(bc_ParsedFunction *self);
void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self);

#endif //BINDINGC_PARSETYPES_H
