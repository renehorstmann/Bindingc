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
    char name[BC_T_SHORT_STR];
    char return_type[BC_T_SHORT_STR];
    char info[BC_T_LONG_STR];
    char return_info[BC_T_MEDIUM_STR];
    char error_info[BC_T_MEDIUM_STR];
    bc_Parameter *output_parameters;
    size_t output_parameters_len;
    bc_Parameter *input_parameters;
    size_t input_parameters_len;
    bc_Parameter *optional_parameters;
    size_t optional_parameters_len;
} bc_Function;

typedef struct {
    char name[BC_T_SHORT_STR];
    char info[BC_T_LONG_STR];
    bc_Function constructor;
    bc_Function destructor;
    bc_Function *methods;
    size_t methods_len;
} bc_Class;

void bc_Function_kill(bc_Function *self);

void bc_Class_kill(bc_Class *self);

#endif //BINDINGC_SORTEDTYPES_H
