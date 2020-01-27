#ifndef BINDINGC_BINDINGTYPES_H
#define BINDINGC_BINDINGTYPES_H

typedef char bc_shortstring[128];
typedef char bc_string[256];
typedef char bc_longstring[512];

typedef struct {
    bc_shortstring c_name;         // char *
    bc_shortstring out_name;       // str
    bc_shortstring call_name;      // c_char_p
    bc_string creation;            // c_char_p()
    bc_string to_c_template;       // $.encode()
    bc_string from_c_template;     // $.decode()
} bc_type;

typedef struct {
    bc_type *array;
    size_t size;
} bc_TypeArray;

void bc_TypeArray_kill(bc_TypeArray *self);

bc_type *bc_TypeArray_try_get(bc_TypeArray self, const char *name);
bc_type *bc_TypeArray_get(bc_TypeArray self, const char *name);

void bc_apply_template(char *dst, const char *template, const char *replacement);

#endif //BINDINGC_BINDINGTYPES_H
