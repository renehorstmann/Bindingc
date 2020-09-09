#ifndef BINDINGC_PARSETYPES_H
#define BINDINGC_PARSETYPES_H

#include "base.h"

typedef struct {
    bc_shortstring name;
    bc_shortstring type;
} BcParsedParameter_s;

typedef struct {
    bc_shortstring name;
    bc_shortstring default_value;
    bc_string info;
} BcParsedParameterInfo_s;

typedef struct {
    bc_longstring text;
    bc_string return_info;
    bc_string error_info;
    BcParsedParameterInfo_s parameter_infos[MAX_PARAMETERS];
    size_t parameter_infos_len;
} BcParsedInfo_s;

typedef struct {
    bc_shortstring name;
    bc_shortstring return_type;
    BcParsedParameter_s parameters[MAX_PARAMETERS];
    size_t parameters_len;
    BcParsedInfo_s info;
} BcParsedFunction_s;

typedef struct {
    BcParsedFunction_s *array;
    size_t size;
} BcParsedFunctionArray;

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

void bc_parsed_function_array_kill(BcParsedFunctionArray *self);

//void bc_ParsedTypeDeclarationArray_kill(bc_ParsedTypeDeclarationArray *self);
//
//void bc_ParsedStruct_kill(bc_ParsedStruct *self);

#endif //BINDINGC_PARSETYPES_H
