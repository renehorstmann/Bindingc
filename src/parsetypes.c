
#include "bindingc/utilc/alloc.h"

#include "bindingc/parsetypes.h"

void bc_ParsedParameter_kill(bc_ParsedParameter *self) {
    Free0(self->name);
    Free0(self->type);
}

void bc_ParsedParameterInfo_kill(bc_ParsedParameterInfo *self) {
    Free0(self->name);
    Free0(self->default_value);
    Free0(self->info);
}

void bc_ParsedInfo_kill(bc_ParsedInfo *self) {
    for (size_t p = 0; self->parameter_infos_len; p++)
        bc_ParsedParameterInfo_kill(&self->parameter_infos[p]);
    self->parameter_infos_len = 0;
    Free0(self->text);
    Free0(self->return_info);
    Free0(self->parameter_infos);
}

void bc_ParsedFunction_kill(bc_ParsedFunction *self) {
    for (size_t p = 0; self->parameters_len; p++)
        bc_ParsedParameter_kill(&self->parameters[p]);
    self->parameters_len = 0;
    bc_ParsedInfo_kill(&self->info);
    Free0(self->name);
    Free0(self->return_type);
    Free0(self->parameters);
}


void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self) {
    for(size_t f=0; f<self->functions_len; f++)
        bc_ParsedFunction_kill(&self->functions[f]);
    self->functions_len = 0;
    Free0(self->functions);
}
