
#include <bindingc/utilc/alloc.h>
#include "bindingc/utilc/strviu.h"

#include "bindingc/bindingtypes.h"

void bc_TypeArray_kill(bc_TypeArray *self) {
    Free0(self->array);
    self->size = 0;
}

bc_Type *bc_TypeArray_try_get(bc_TypeArray self, const char *name){
    bc_Type *res = NULL;
    for(size_t i=0; i<self.size; i++) {
        if(strcmp(self.array[i].c_name, name) == 0)
            return self.array+i;
    }
    return res;
}

bc_Type *bc_TypeArray_get(bc_TypeArray self, const char *name) {
    bc_Type *res = bc_TypeArray_try_get(self, name);
    assert(res);
    return res;
}

void bc_apply_template(char *dst, const char *template, const char *replacement) {
    char *rep = sv_replace_to_heap_cstring(ToStrViu(template), "$", replacement);
    strcpy(dst, rep);
    free(rep);
}

