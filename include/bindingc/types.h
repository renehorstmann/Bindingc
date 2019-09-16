#ifndef BINDINGC_SORTEDTYPES_H
#define BINDINGC_SORTEDTYPES_H

#include <glob.h>

#define BC_T_SHORT_STR 80
#define BC_T_MEDIUM_STR 160
#define BC_T_LONG_STR 320

typedef struct {
    char name[BC_T_SHORT_STR];
    char type[BC_T_SHORT_STR];
    char default_value[BC_T_SHORT_STR];
    char info[BC_T_MEDIUM_STR];
} bc_Parameter;

typedef struct {
    bc_Parameter *array;
    size_t size;
} bc_ParameterArray;

typedef struct {
    char name[BC_T_SHORT_STR];
    char return_type[BC_T_SHORT_STR];
    char info[BC_T_LONG_STR];
    char return_info[BC_T_MEDIUM_STR];
    char error_info[BC_T_MEDIUM_STR];
    bc_ParameterArray output_parameters;
    bc_ParameterArray input_parameters;
    bc_ParameterArray optional_parameters;
} bc_Function;

typedef struct {
    bc_Function *array;
    size_t size;
} bc_FunctionArray;

typedef struct {
    char name[BC_T_SHORT_STR];
    char info[BC_T_LONG_STR];
    bc_Function constructor;
    bc_Function destructor;
    bc_FunctionArray methods;
} bc_Class;

void bc_ParameterArray_kill(bc_ParameterArray *self);

void bc_Function_kill(bc_Function *self);

void bc_FunctionArray_kill(bc_FunctionArray *self);

void bc_Class_kill(bc_Class *self);

#endif //BINDINGC_SORTEDTYPES_H
