#ifndef BINDINGC_TYPES_H
#define BINDINGC_TYPES_H

#include <stdbool.h>
#include <stdlib.h>

typedef char ShortString[128];
typedef char String[256];
typedef char LongString[512];


typedef struct {
    ShortString name;
    ShortString type;
    ShortString default_value;
    String info;
    bool is_input;
    bool is_output;
} bc_Parameter;

typedef struct {
    bc_Parameter array[32];
    size_t size;
} bc_ParameterArray;

typedef struct {
    ShortString c_name;
    ShortString out_name;
    LongString info;
    String error_info;
    bc_Parameter return_parameter;
    bc_ParameterArray parameters;
} bc_Function;

typedef struct {
    bc_Function *array;
    size_t size;
} bc_FunctionArray;

//typedef struct {
//    ShortString name;
//    LongString info;
//    bc_Function constructor;
//    bc_Function destructor;
//    bc_Function methods[64];
//    size_t methods_len;
//} bc_Class;


void bc_FunctionArray_kill(bc_FunctionArray *self);

#endif //BINDINGC_TYPES_H
