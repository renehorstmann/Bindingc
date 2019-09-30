#ifndef BINDINGC_BINDINGTYPES_H
#define BINDINGC_BINDINGTYPES_H

typedef char ShortString[128];
typedef char String[256];
typedef char LongString[512];

typedef struct {
    ShortString c_name;         // char *
    ShortString out_name;       // str
    ShortString call_name;      // c_char_p
    String creation;            // c_char_p()
    String to_c_template;       // $.encode()
    String from_c_template;     // $.decode()
} bc_Type;

typedef struct {
    bc_Type *array;
    size_t size;
} bc_TypeArray;

void bc_TypeArray_kill(bc_TypeArray *self);

bc_Type *bc_TypeArray_try_get(bc_TypeArray self, const char *name);
bc_Type *bc_TypeArray_get(bc_TypeArray self, const char *name);

void bc_apply_template(char *dst, const char *template, const char *replacement);

#endif //BINDINGC_BINDINGTYPES_H
