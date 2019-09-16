#include <stdio.h>
#include <stdlib.h>

#include "bindingc/parse.h"
#include "bindingc/sort.h"
#include "bindingc/filter.h"

int error(const char *text) {
    fputs(text, stderr);
    return 1;
}

char *open_file_as_string(const char *filename) {
    char *text = NULL;
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);
        text = malloc(length);
        if (text)
            fread(text, 1, length, file);
        fclose(file);
    }
    return text;
}

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


