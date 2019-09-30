#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

typedef char shortstring[128];
typedef char string[256];
typedef char longstring[512];

typedef struct {
    shortstring name;
    shortstring type;
} bc_parsedparameter;

typedef struct {
    shortstring name;
    shortstring default_value;
    string info;
} bc_parsedparameterinfo;

typedef struct {
    longstring text;
    string return_info;
    string error_info;
    bc_parsedparameterinfo *parameter_infos;
    size_t parameter_infos_len;
} bc_ParsedInfo;

typedef struct {
    shortstring name;
    shortstring return_type;
    bc_parsedparameter *parameters;
    size_t parameters_len;
    bc_ParsedInfo info;
} bc_ParsedFunction;

typedef struct {
    bc_ParsedFunction *array;
    size_t size;
} bc_ParsedFunctionArray;

//typedef struct {
//    shortstring name;
//    shortstring type;   // char (*)() for function ptr?, char [10] for arrays
//} bc_ParsedTypeDeclaration;
//
//typedef struct {
//    bc_ParsedTypeDeclaration *array;
//    size_t size;
//} bc_ParsedTypeDeclarationArray;
//
//typedef struct {
//    shortstring name;
//    longstring info;
//    bc_ParsedTypeDeclarationArray members;
//} bc_ParsedStruct;

void bc_ParsedInfo_kill(bc_ParsedInfo *self);

void bc_ParsedFunction_kill(bc_ParsedFunction *self);

void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self);

//void bc_ParsedTypeDeclarationArray_kill(bc_ParsedTypeDeclarationArray *self);
//
//void bc_ParsedStruct_kill(bc_ParsedStruct *self);

#endif //BINDINGC_PARSETYPES_H
