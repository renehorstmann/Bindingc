#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

#define BC_MAX_SSTR_LEN 80
#define BC_MAX_MSTR_LEN 160
#define BC_MAX_LSTR_LEN 320

typedef struct {
    char name[BC_MAX_SSTR_LEN];
    char type[BC_MAX_SSTR_LEN];
} bc_ParsedParameter;

typedef struct {
    char name[BC_MAX_SSTR_LEN];
    char default_value[BC_MAX_SSTR_LEN];
    char info[BC_MAX_MSTR_LEN];
} bc_ParsedParameterInfo;

typedef struct {
    char text[BC_MAX_LSTR_LEN];
    char return_info[BC_MAX_MSTR_LEN];
    char error_info[BC_MAX_MSTR_LEN];
    bc_ParsedParameterInfo *parameter_infos;
    size_t parameter_infos_len;
} bc_ParsedInfo;

typedef struct {
    char name[BC_MAX_SSTR_LEN];
    char return_type[BC_MAX_SSTR_LEN];
    bc_ParsedParameter *parameters;
    size_t parameters_len;
    bc_ParsedInfo info;
} bc_ParsedFunction;

typedef struct {
    bc_ParsedFunction *functions;
    size_t functions_len;
} bc_ParsedFunctionArray;

void bc_ParsedInfo_kill(bc_ParsedInfo *self);
void bc_ParsedFunction_kill(bc_ParsedFunction *self);
void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self);

#endif //BINDINGC_PARSETYPES_H
