#ifndef BINDINGC_PARSEDTYPES_H
#define BINDINGC_PARSEDTYPES_H

#include "base.h"

typedef struct {
    bc_shortstring name;
    bc_shortstring type;
    bc_shortstring default_value;
    bc_string info;
} BcParameter_s;


typedef struct {
    BcParameter_s array[BC_MAX_PARAMETERS];
    int size;
} BcParameterArray_s;

typedef struct {
    bc_shortstring name;
    bc_longstring info;
    bc_string error_info;
    BcParameter_s return_parameter;
    BcParameterArray_s parameters;
} BcFunction_s;

typedef struct {
    BcFunction_s *array;
    int size;
} BcFunctionArray;



#endif //BINDINGC_PARSEDTYPES_H
