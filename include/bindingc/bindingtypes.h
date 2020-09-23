#ifndef BINDINGC_BINDINGTYPES_H
#define BINDINGC_BINDINGTYPES_H

#include "base.h"

typedef struct {
    bc_shortstring c_name;         // char *
    bc_shortstring out_name;       // str
    bc_shortstring binding_name;   // c_char_p
    bc_string creation;            // c_char_p()
    bc_string to_c_template;       // $.encode()
    bc_string from_c_template;     // $.decode()
} BcType_s;

typedef struct {
    BcType_s *array;
    int size;
} BcTypeArray;

void bc_type_array_kill(BcTypeArray *self);

void bc_type_array_append(BcTypeArray *self, BcType_s push);

BcType_s *bc_type_array_try_get(BcTypeArray self, const char *name);
BcType_s *bc_type_array_get(BcTypeArray self, const char *name);

void bc_apply_template(char *dst, int dst_size, const char *template_text, const char *replacement);

#endif //BINDINGC_BINDINGTYPES_H
