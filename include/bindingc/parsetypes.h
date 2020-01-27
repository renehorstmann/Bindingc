#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

#include "base.h"

typedef struct {
    bc_shortstring name;
    bc_shortstring type;
} bc_parsedparameter;

typedef struct {
    bc_shortstring name;
    bc_shortstring default_value;
    bc_string info;
} bc_parsedparameterinfo;

typedef struct {
    bc_longstring text;
    bc_string return_info;
    bc_string error_info;
    bc_parsedparameterinfo parameter_infos[MAX_PARAMETERS];
    size_t parameter_infos_len;
} bc_parsedinfo;

typedef struct {
    bc_shortstring name;
    bc_shortstring return_type;
    bc_parsedparameter parameters[MAX_PARAMETERS];
    size_t parameters_len;
    bc_parsedinfo info;
} bc_parsedfunction;

typedef struct {
    bc_parsedfunction *array;
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

void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self);

//void bc_ParsedTypeDeclarationArray_kill(bc_ParsedTypeDeclarationArray *self);
//
//void bc_ParsedStruct_kill(bc_ParsedStruct *self);

#endif //BINDINGC_PARSETYPES_H
