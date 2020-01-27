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
} bc_parameter;

typedef struct {
    bc_parameter array[32];
    size_t size;
} bc_parameterarray;

typedef struct {
    bc_shortstring c_name;
    bc_shortstring out_name;
    bc_longstring info;
    bc_string error_info;
    bc_parameter return_parameter;
    bc_parameterarray parameters;
} bc_function;

typedef struct {
    bc_function *array;
    size_t size;
} bc_FunctionArray;

//typedef struct {
//    shortstring name;
//    longstring info;
//    bc_function constructor;
//    bc_function destructor;
//    bc_function methods[64];
//    size_t methods_len;
//} bc_Class;


void bc_FunctionArray_kill(bc_FunctionArray *self);

#endif //BINDINGC_TYPES_H
