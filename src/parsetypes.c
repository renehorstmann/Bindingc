
#include "bindingc/utilc/alloc.h"

#include "bindingc/parsetypes.h"

void bc_ParsedFunctionArray_kill(bc_ParsedFunctionArray *self) {
    Free0(self->array);
    self->size = 0;
}

//void bc_ParsedTypeDeclarationArray_kill(bc_ParsedTypeDeclarationArray *self) {
//    Free0(self->array);
//    self->size = 0;
//}

//void bc_ParsedStruct_kill(bc_ParsedStruct *self) {
//    bc_ParsedTypeDeclarationArray_kill(&self->members);
//}

