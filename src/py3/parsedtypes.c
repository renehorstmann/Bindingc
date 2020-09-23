#include "bindingc/utilc/alloc.h"
#include "bindingc/utilc/dynarray.h"

#include "bindingc/py3/types.h"

DynArray(BcType_s, TypeArray, type_array)


BcTypeArray bc_py3_get_default_types() {
    TypeArray types = {0};

    type_array_push(&types, (BcType_s) {
        "bool",
        "bool",
        "ct.c_bool",
        "ct.c_bool()",
        "$",
        "$.value"
    });

    type_array_push(&types, (BcType_s) {
        "int",
        "int",
        "ct.c_int",
        "ct.c_int()",
        "$",
        "$.value"
    });

    type_array_push(&types, (BcType_s) {
        "float",
        "float",
        "ct.c_float",
        "ct.c_float()",
        "$",
        "$.value"
    });

    return (BcTypeArray) {types.array, types.size};
}

