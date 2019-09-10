#include "bindingc/parse.h"


#include <stdio.h>
#include <string.h>

/**
 * int
 * @param a inti
 * @param b intii
 * @return intiii
 * @error <=1
 */
int foo(int a, int b);

int main() {

    const char *test = "/**\n"
                       " * Lol title wtf yeah\n"
                       " * @param haha infooo\n"
                       " * @param yo=true\n"
                       "*@param a=10.2: a is great\n"
                       "         *@returns: nothing\n"
                       " * @return a value with smth\n"
                       " * \t @error -1  \t\n"
                       " */\n"
                       "EXPORT void foo(int a, int b);";

    bc_ParsedFunction *functions;
    bc_parse_file(&functions, ToStrViu(test));

    return 0;
}
