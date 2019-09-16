#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

typedef char ShortString[80];
typedef char String[160];
typedef char LongString[320];

typedef struct {
    ShortString name;
    ShortString type;
} bc_ParsedParameter;

typedef struct {
    ShortString name;
    ShortString default_value;
    String info;
} bc_ParsedParameterInfo;

typedef struct {
    LongString text;
    String return_info;
    String error_info;
    bc_ParsedParameterInfo *parameter_infos;
    size_t parameter_infos_len;
} bc_ParsedInfo;

typedef struct {
    ShortString name;
    ShortString return_type;
    bc_ParsedParameter *parameters;
    size_t parameters_len;
    bc_ParsedInfo info;
} bc_ParsedFunction;

typedef struct {
    bc_ParsedFunction *array;
    size_t size;
} bc_ParsedFunctionArray;

void bc_ParsedInfo_kill(bc_ParsedInfo *self);
void bc_ParsedFunction_kill(bc_ParsedFunction *self);
void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self);

#endif //BINDINGC_PARSETYPES_H
