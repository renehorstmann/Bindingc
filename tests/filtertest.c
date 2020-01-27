#include "bindingc/parse.h"
#include "bindingc/sort.h"
#include "bindingc/filter.h"

#include "test_helper.h"

int main() {

    char *file = open_file_as_string("filetest1.txt");
    if(!file)
        return error("couldnt open file");

    bc_ParsedFunctionArray parsed_functions = bc_parse_file(ToStrViu(file));
    if(parsed_functions.size != 8)
        return error("bc_parse_file failed");

    bc_FunctionArray functions = bc_get_function_array_without_paramaters(&parsed_functions);

    functions = bc_filter_function_non_static(functions, true);
    if(functions.size != 7)
        return error("bc_filter_function_non_static failed");

    functions = bc_filter_function_name_prefix(functions, "bc_", true);
    if(functions.size != 6)
        return error("bc_filter_function_name_prefix failed");



    return 0;
}


