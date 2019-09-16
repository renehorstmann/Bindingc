
#include "bindingc/utilc/alloc.h"

#include "bindingc/types.h"



void bc_Function_kill(bc_Function *self) {
    Free0(self->output_parameters);
    self->output_parameters_len = 0;
    Free0(self->input_parameters);
    self->input_parameters_len = 0;
    Free0(self->optional_parameters);
    self->optional_parameters_len = 0;
}

void bc_Class_kill(bc_Class *self) {
    bc_Function_kill(&self->constructor);
    bc_Function_kill(&self->destructor);
    for(size_t i=0; i<self->methods_len; i++) {
        bc_Function_kill(&self->methods[i]);
    }
    Free0(self->methods);
    self->methods_len = 0;
}

