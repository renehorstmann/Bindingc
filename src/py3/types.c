

#include "bindingc/utilc/alloc.h"

#include "bindingc/py3/types.h"


BcTypeArray bc_py3_get_default_types() {
    BcTypeArray res;
    res.array = New0(BcType_s, (res.size=7));

    int id = 0;
    res.array[id++] = (BcType_s) {
        "bool",
        "bool",
        "c_bool",
        "c_bool()",
        "c_bool($)",
        "$.value"
    };


    res.array[id++] = (BcType_s) {
        "char",
        "str",
        "c_char($.encode())",
        "$.value.decode()",
        "c_char()"
    };
    res.array[id++] = (BcType_s) {
        "int",
        "int",
        "c_int($)",
        "$.value",
        "c_int()",
    };
    res.array[id++] = (BcType_s) {
        "size_t",
        "int",
        "c_size_t($)",
        "$.value",
        "c_size_t()",
    };
    res.array[id++] = (BcType_s) {
        "float",
        "float",
        "c_float($)",
        "$.value",
        "c_float()",
    };
    res.array[id++] = (BcType_s) {
        "double",
        "float",
        "c_double($)",
        "$.value",
        "c_double()",
    };
    res.array[id++] = (BcType_s) {
        "char *",
        "str",
        "c_char_p($.encode())",
        "$.value.decode()",
        "c_char_p()",
    };

    return res;
}

