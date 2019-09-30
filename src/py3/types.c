

#include "bindingc/utilc/alloc.h"

#include "bindingc/py3/types.h"


bc_TypeArray bc_py3_get_default_types() {
    bc_TypeArray res;
    res.array = New0(bc_Type, (res.size=7));

    int id = 0;
    res.array[id++] = (bc_Type) {
        "bool",
        "bool",
        "c_bool",
        "c_bool()",
        "c_bool($)",
        "$.value"
    };


    res.array[id++] = (bc_Type) {
        "char",
        "str",
        "c_char($.encode())",
        "$.value.decode()",
        "c_char()"
    };
    res.array[id++] = (bc_Type) {
        "int",
        "int",
        "c_int($)",
        "$.value",
        "c_int()",
    };
    res.array[id++] = (bc_Type) {
        "size_t",
        "int",
        "c_size_t($)",
        "$.value",
        "c_size_t()",
    };
    res.array[id++] = (bc_Type) {
        "float",
        "float",
        "c_float($)",
        "$.value",
        "c_float()",
    };
    res.array[id++] = (bc_Type) {
        "double",
        "float",
        "c_double($)",
        "$.value",
        "c_double()",
    };
    res.array[id++] = (bc_Type) {
        "char *",
        "str",
        "c_char_p($.encode())",
        "$.value.decode()",
        "c_char_p()",
    };

    return res;
}

