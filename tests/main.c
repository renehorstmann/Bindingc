#include <stdio.h>
#include <reflectionc/parse.h>
#include "bindingc/reflectionc.h"
#include "bindingc/py3/types.h"
#include "bindingc/py3/generate.h"


int main() {
    HrFunctionArray hr_functions = hr_parse_file("base_example.h");
    BcFunctionArray functions = bc_function_array_from_reflectionc(hr_functions, true);

    BcTypeArray types = bc_py3_get_default_types();

    char *binding = bc_py3_generate_functions(functions, types, 0);
    puts(binding);

}

