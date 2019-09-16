
#include "bindingc/utilc/alloc.h"

#include "bindingc/parsetypes.h"

void bc_ParsedInfo_kill(bc_ParsedInfo *self) {
    Free0(self->parameter_infos);
    self->parameter_infos_len = 0;
}

void bc_ParsedFunction_kill(bc_ParsedFunction *self) {
    bc_ParsedInfo_kill(&self->info);
    Free0(self->parameters);
    self->parameters_len = 0;
}

void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self) {
    for(size_t f=0; f<self->size; f++)
        bc_ParsedFunction_kill(&self->array[f]);
    self->size = 0;
    Free0(self->array);
}
