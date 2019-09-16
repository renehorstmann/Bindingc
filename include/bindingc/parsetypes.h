#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

#define BC_PT_SHORT_STR 80
#define BC_PT_MEDIUM_STR 160
#define BC_PT_LONG_STR 320

typedef struct {
    char name[BC_PT_SHORT_STR];
    char type[BC_PT_SHORT_STR];
} bc_ParsedParameter;

typedef struct {
    char name[BC_PT_SHORT_STR];
    char default_value[BC_PT_SHORT_STR];
    char info[BC_PT_MEDIUM_STR];
} bc_ParsedParameterInfo;

typedef struct {
    char text[BC_PT_LONG_STR];
    char return_info[BC_PT_MEDIUM_STR];
    char error_info[BC_PT_MEDIUM_STR];
    bc_ParsedParameterInfo *parameter_infos;
    size_t parameter_infos_len;
} bc_ParsedInfo;

typedef struct {
    char name[BC_PT_SHORT_STR];
    char return_type[BC_PT_SHORT_STR];
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
