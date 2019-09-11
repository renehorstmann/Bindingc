#include <stdio.h>


#include "bindingc/parse.h"

int error(const char *text) {
    fputs(text, stderr);
    return 1;
}

int main() {

    StrViu viu;

    // parameter info
    {
        bc_ParsedParameterInfo res;

        viu = ToStrViu("valves=16: The amount of engine valves per cylinder. ");
        res = bc_parse_parameter_info_text(viu);
        if (strcmp(res.name, "valves") != 0
            || strcmp(res.default_value, "16") != 0
            || strcmp(res.info, "The amount of engine valves per cylinder.") != 0)
            return error("bc_parse_parameter_info_text 1 failed");

        viu = ToStrViu("turbo_available\tSpecifies if a turbo charge is mounted to the engine");
        res = bc_parse_parameter_info_text(viu);
        if (strcmp(res.name, "turbo_available") != 0
            || res.default_value != NULL
            || strcmp(res.info, "Specifies if a turbo charge is mounted to the engine") != 0)
            return error("bc_parse_parameter_info_text 2 failed");

        viu = ToStrViu("out_km: driven kilometers.\n");
        res = bc_parse_parameter_info_text(viu);
        if (strcmp(res.name, "out_km") != 0
            || res.default_value != NULL
            || strcmp(res.info, "driven kilometers.") != 0)
            return error("bc_parse_parameter_info_text 3 failed");

        viu = ToStrViu("use_lights=true\nturn on the car lights");
        res = bc_parse_parameter_info_text(viu);
        if (strcmp(res.name, "use_lights") != 0
            || strcmp(res.default_value, "true") != 0
            || strcmp(res.info, "turn on the car lights") != 0)
            return error("bc_parse_parameter_info_text 4 failed");
    }

    // info
    {
        bc_ParsedInfo res;

        viu = ToStrViu("/** single line comment before the function */");
        res = bc_parse_info_text(viu);

        // todo



        viu = ToStrViu("/**\n"
                       " * Lol title wtf yeah\n"
                       " * @param haha infooo\n"
                       " * @param yo=true\n"
                       "*@param a=10.2: a is great\n"
                       "         *@returns: nothing\n"
                       " * @return a value with smth\n"
                       " * \t @error -1  \t\n"
                       " */\n");
    }

    return 0;
}

