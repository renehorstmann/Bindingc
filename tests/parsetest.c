#include <stdio.h>


#include "bindingc/parse.h"

int error(const char *text) {
    fputs(text, stderr);
    return 1;
}


bool str_not_equal(const char *a, const char *b) {
    // same pointer or both NULL
    if (a == b)
        return false;
    // only one is NULL
    if (!a || !b)
        return true;
    return strcmp(a, b) != 0;
}

int main() {

    StrViu viu;

    // parameter info
    {
        bc_ParsedParameterInfo res;

        viu = ToStrViu("valves=16: The amount of engine valves per cylinder. ");
        res = bc_parse_parameter_info_text(viu);
        if (str_not_equal(res.name, "valves")
            || str_not_equal(res.default_value, "16")
            || str_not_equal(res.info, "The amount of engine valves per cylinder."))
            return error("bc_parse_parameter_info_text 1 failed");

        viu = ToStrViu("turbo_available\tSpecifies if a turbo charge is mounted to the engine");
        res = bc_parse_parameter_info_text(viu);
        if (str_not_equal(res.name, "turbo_available")
            || str_not_equal(res.default_value, NULL)
            || str_not_equal(res.info, "Specifies if a turbo charge is mounted to the engine"))
            return error("bc_parse_parameter_info_text 2 failed");

        viu = ToStrViu("out_km: driven kilometers.\n");
        res = bc_parse_parameter_info_text(viu);
        if (str_not_equal(res.name, "out_km")
            || str_not_equal(res.default_value, NULL)
            || str_not_equal(res.info, "driven kilometers."))
            return error("bc_parse_parameter_info_text 3 failed");

        viu = ToStrViu("use_lights=true\nturn on the car lights");
        res = bc_parse_parameter_info_text(viu);
        if (str_not_equal(res.name, "use_lights")
            || str_not_equal(res.default_value, "true")
            || str_not_equal(res.info, "turn on the car lights"))
            return error("bc_parse_parameter_info_text 4 failed");
    }

    // info
    {
        bc_ParsedInfo res;

        viu = ToStrViu("/** single line comment before the function */");
        res = bc_parse_info_text(viu);
        if (str_not_equal(res.text, "single line comment before the function")
            || str_not_equal(res.return_info, NULL)
            || str_not_equal(res.error_info, NULL)
            || !res.parameter_infos || str_not_equal(res.parameter_infos[0].name, NULL))
            return error("bc_parse_info_text 1 failed");

        viu = ToStrViu("//\tanother single line comment.\t\n");
        res = bc_parse_info_text(viu);
        if (str_not_equal(res.text, "another single line comment.")
            || str_not_equal(res.return_info, NULL)
            || str_not_equal(res.error_info, NULL)
            || !res.parameter_infos || str_not_equal(res.parameter_infos[0].name, NULL))
            return error("bc_parse_info_text 2 failed");

        viu = ToStrViu("/**\n"
                       " * Title text\n"
                       " * @param val range[0-10)\n"
                       " * @param center=true\n"
                       "*@param a=10.2: alpha factor\n"
                       "         *@returns: the result\n"
                       " * @return the real result\n"
                       " @param multi_test={0} Multi\n"
                       " * line\n"
                       " * param explenation a*2=3\n"
                       " * \t @error -1  \t\n"
                       " */\n");
        res = bc_parse_info_text(viu);
        if (str_not_equal(res.text, "Title text")
            || str_not_equal(res.return_info, "the real result")
            || str_not_equal(res.error_info, "-1"))
            return error("bc_parse_info_text 3.1 failed");
        int params = 0;
        while(res.parameter_infos[params].name)
            params++;
        if(params != 4)
            return error("bc_parse_info_text 3.2 failed");
        if (str_not_equal(res.parameter_infos[0].name, "val")
            || str_not_equal(res.parameter_infos[0].default_value, NULL)
            || str_not_equal(res.parameter_infos[0].info, "range[0-10)"))
            return error("bc_parse_info_text 3.3 failed");
        if (str_not_equal(res.parameter_infos[1].name, "center")
            || str_not_equal(res.parameter_infos[1].default_value, "true")
            || str_not_equal(res.parameter_infos[1].info, NULL))
            return error("bc_parse_info_text 3.4 failed");
        if (str_not_equal(res.parameter_infos[2].name, "a")
            || str_not_equal(res.parameter_infos[2].default_value, "10.2")
            || str_not_equal(res.parameter_infos[2].info, "alpha factor"))
            return error("bc_parse_info_text 3.5 failed");
        if (str_not_equal(res.parameter_infos[3].name, "multi_test")
            || str_not_equal(res.parameter_infos[3].default_value, "{0}")
            || str_not_equal(res.parameter_infos[3].info, "Multi line param explenation a*2=3"))
            return error("bc_parse_info_text 3.6 failed");
    }

    return 0;
}

