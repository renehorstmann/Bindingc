#ifndef BINDINGC_SORTEDTYPES_H
#define BINDINGC_SORTEDTYPES_H

typedef char ShortString[80];
typedef char String[160];
typedef char LongString[320];

typedef struct {
    ShortString name;
    ShortString type;
    ShortString default_value;
    String info;
} bc_Parameter;

typedef struct {
    bc_Parameter *array;
    size_t size;
} bc_ParameterArray;

typedef struct {
    ShortString name;
    ShortString return_type;
    LongString info;
    String return_info;
    String error_info;
    bc_ParameterArray output_parameters;
    bc_ParameterArray input_parameters;
    bc_ParameterArray optional_parameters;
} bc_Function;

typedef struct {
    bc_Function *array;
    size_t size;
} bc_FunctionArray;

typedef struct {
    ShortString name;
    LongString info;
    bc_Function constructor;
    bc_Function destructor;
    bc_FunctionArray methods;
} bc_Class;

void bc_ParameterArray_kill(bc_ParameterArray *self);

void bc_Function_kill(bc_Function *self);

void bc_FunctionArray_kill(bc_FunctionArray *self);

void bc_Class_kill(bc_Class *self);

#endif //BINDINGC_SORTEDTYPES_H
