
#include <bindingc/utilc/alloc.h>
#include "bindingc/utilc/strviu.h"

#include "bindingc/bindingtypes.h"

void bc_type_array_kill(BcTypeArray *self) {
    Free0(self->array);
    self->size = 0;
}

BcType_s *bc_type_array_try_get(BcTypeArray self, const char *name){
    BcType_s *res = NULL;
    for(size_t i=0; i<self.size; i++) {
        if(strcmp(self.array[i].c_name, name) == 0)
            return self.array+i;
    }
    return res;
}

BcType_s *bc_type_array_get(BcTypeArray self, const char *name) {
    BcType_s *res = bc_type_array_try_get(self, name);
    assert(res);
    return res;
}

void bc_apply_template(char *dst, const char *template, const char *replacement) {
    char *rep = sv_replace_to_heap_cstring(ToStrViu(template), "$", replacement);
    strcpy(dst, rep);
    free(rep);
}

