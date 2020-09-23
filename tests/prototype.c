#include <reflectionc/parse.h>
#include "bindingc/reflectionc.h"
#include "bindingc/py3.h"


int main() {
    HrFunctionArray hr_functions = hr_parse_file("base_example.h");
    BcFunctionArray functions = bc_function_array_from_reflectionc(hr_functions, true);





}

