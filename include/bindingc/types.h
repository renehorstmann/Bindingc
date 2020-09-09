#ifndef BINDINGC_TYPES_H
#define BINDINGC_TYPES_H

#include <stdbool.h>
#include <stdlib.h>


#include "base.h"


typedef struct {
    bc_shortstring name;
    bc_shortstring type;
    bc_shortstring default_value;
    bc_string info;
    bool is_input;  // todo: could also be an enum IN, OUT, INOUT, OPT, ERROR, ...
    bool is_output;
} BcParameter_s;

typedef struct {
    BcParameter_s array[32];
    size_t size;
} BcParameterArray_s;

typedef struct {
    bc_shortstring c_name;
    bc_shortstring out_name;
    bc_longstring info;
    bc_string error_info;
    BcParameter_s return_parameter;
    BcParameterArray_s parameters;
} BcFunction_s;

typedef struct {
    BcFunction_s *array;
    size_t size;
} BcFunctionArray;

//typedef struct {
//    shortstring name;
//    longstring info;
//    BcFunction_s constructor;
//    BcFunction_s destructor;
//    BcFunction_s methods[64];
//    size_t methods_len;
//} bc_Class;


void bc_function_array_kill(BcFunctionArray *self);

#endif //BINDINGC_TYPES_H
