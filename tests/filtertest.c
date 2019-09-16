#include <stdio.h>
#include <stdlib.h>

#include "bindingc/parse.h"
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

    bc_ParsedFunctionArray functions = bc_parse_file(ToStrViu(file));
    if(functions.functions_len != 8)
        return error("bc_parse_file failed");

    bc_filter_non_static(&functions);
    if(functions.functions_len != 7)
        return error("bc_filter_non_static failed");

    bc_filter_name_prefix(&functions, "bc_");
    if(functions.functions_len != 6)
        return error("bc_filter_name_prefix failed");


    return 0;
}


