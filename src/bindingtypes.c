#include "bindingc/utilc/alloc.h"
#include "bindingc/utilc/assume.h"
#include "bindingc/utilc/strviu.h"

#include "bindingc/bindingtypes.h"

void bc_type_array_kill(BcTypeArray *self) {
    Free0(self->array);
    self->size = 0;
}


void bc_type_array_append(BcTypeArray *self, BcType_s push) {
    self->array = ReNew(BcType_s, self->array, self->size++);
    self->array[self->size-1] = push;
}

BcType_s *bc_type_array_try_get(BcTypeArray self, const char *name){
    BcType_s *res = NULL;
    for(int i=0; i<self.size; i++) {
        if(strcmp(self.array[i].c_name, name) == 0)
            return self.array+i;
    }
    return res;
}

BcType_s *bc_type_array_get(BcTypeArray self, const char *name) {
    BcType_s *res = bc_type_array_try_get(self, name);
    assume(res, "bc_type_array_get failed for: %s", name);
    return res;
}

void bc_apply_template(char *dst, int dst_size, const char *template_text, const char *replacement) {
    sv_replace_into_sv((strviu) {dst, dst+dst_size},
                       ToStrViu(template_text),
                       ToStrViu("$"), ToStrViu(replacement));
}

