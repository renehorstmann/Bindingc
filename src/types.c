
#include "bindingc/utilc/alloc.h"

#include "bindingc/types.h"


void bc_ParameterArray_kill(bc_ParameterArray *self) {
    Free0(self->array);
    self->size = 0;
}

void bc_Function_kill(bc_Function *self) {
    bc_ParameterArray_kill(&self->output_parameters);
    bc_ParameterArray_kill(&self->input_parameters);
    bc_ParameterArray_kill(&self->optional_parameters);
}

void bc_FunctionArray_kill(bc_FunctionArray *self) {
    for(size_t i=0; i<self->size; i++)
        bc_Function_kill(&self->array[i]);
    Free0(self->array);
    self->size = 0;
}

void bc_Class_kill(bc_Class *self) {
    bc_Function_kill(&self->constructor);
    bc_Function_kill(&self->destructor);
    bc_FunctionArray_kill(&self->methods);
}

