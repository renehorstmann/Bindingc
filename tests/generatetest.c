#include <stdio.h>

#include "bindingc/types.h"
#include "bindingc/py3/types.h"
#include "bindingc/py3/generate.h"

int error(const char *text) {
    fputs(text, stderr);
    return 1;
}


int main() {

    bc_TypeArray types = bc_py3_get_default_types();
    bc_Function function;

    function = (bc_Function){
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

    puts(init);
    puts("");
    puts(code);
    return 0;
}

