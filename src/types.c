
#include <bindingc/utilc/alloc.h>

#include "bindingc/types.h"

void bc_FunctionArray_kill(bc_FunctionArray *self) {
    Free0(self->array);
    self->size = 0;
}
