
#include <bindingc/utilc/alloc.h>

#include "bindingc/types.h"

void bc_function_array_kill(BcFunctionArray *self) {
    Free0(self->array);
    self->size = 0;
}
