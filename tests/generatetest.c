#include "bindingc/types.h"
#include "bindingc/py3/types.h"
#include "bindingc/py3/generate.h"

#include "test_helper.h"

int main() {

    BcTypeArray types = bc_py3_get_default_types();
    BcFunction_s function;

    function = (BcFunction_s){
            "ce_load",
            "load",
            "loads the module",
            "",
            {
                    "",
                    "int",
                    "",
                    "handle"
            },
            {
                {
                            {
                                    "size",
                                    "float",
                                    "1.0f",
                                    "width * height",
                                    1,
                                    0
                            }
                    },
                    1
            }
    };


    char *init = bc_py3_generate_interface(&function, types, 0);
    char *code = bc_py3_generate_function(&function, types, 0);

    if(strcmp(init, "_lib.ce_load.argtypes = [float]\n_lib.ce_load.restype = int\n") != 0)
        return error("generate_interface failed");

    puts(init);
    puts("");
    puts(code);
    return 0;
}

