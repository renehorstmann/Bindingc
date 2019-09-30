#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

typedef char ShortString[128];
typedef char String[256];
typedef char LongString[512];

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

//typedef struct {
//    ShortString name;
//    ShortString type;   // char (*)() for function ptr?, char [10] for arrays
//} bc_ParsedTypeDeclaration;
//
//typedef struct {
//    bc_ParsedTypeDeclaration *array;
//    size_t size;
//} bc_ParsedTypeDeclarationArray;
//
//typedef struct {
//    ShortString name;
//    LongString info;
//    bc_ParsedTypeDeclarationArray members;
//} bc_ParsedStruct;

void bc_ParsedInfo_kill(bc_ParsedInfo *self);

void bc_ParsedFunction_kill(bc_ParsedFunction *self);

void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self);

//void bc_ParsedTypeDeclarationArray_kill(bc_ParsedTypeDeclarationArray *self);
//
//void bc_ParsedStruct_kill(bc_ParsedStruct *self);

#endif //BINDINGC_PARSETYPES_H
