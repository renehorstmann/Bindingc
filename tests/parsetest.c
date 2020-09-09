#include "bindingc/parse.h"

#include "test_helper.h"

int main() {

    strviu viu;

    // parameter info
    {
        BcParsedParameterInfo_s res;

        viu = ToStrViu("");
        res = bc_parse_parameter_info_text(viu);
        if (*res.name != 0 || *res.default_value != 0 || *res.info != 0)
            return error("bc_parse_parameter_info_text 0 failed");

        viu = ToStrViu("valves=16: The amount of engine valves per cylinder. ");
        res = bc_parse_parameter_info_text(viu);
        if (str_not_equal(res.name, "valves")
            || str_not_equal(res.default_value, "16")
            || str_not_equal(res.info, "The amount of engine valves per cylinder."))
            return error("bc_parse_parameter_info_text 1 failed");

        viu = ToStrViu("turbo_available\tSpecifies if a turbo charge is mounted to the engine");
        res = bc_parse_parameter_info_text(viu);
        if (str_not_equal(res.name, "turbo_available")
            || str_not_equal(res.default_value, "")
            || str_not_equal(res.info, "Specifies if a turbo charge is mounted to the engine"))
            return error("bc_parse_parameter_info_text 2 failed");

        viu = ToStrViu("out_km: driven kilometers.\n");
        res = bc_parse_parameter_info_text(viu);
        if (str_not_equal(res.name, "out_km")
            || str_not_equal(res.default_value, "")
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
        BcParsedInfo_s res;

        viu = ToStrViu("");
        res = bc_parse_info_text(viu);
        if (*res.text != 0 || *res.return_info != 0 || *res.error_info != 0 || res.parameter_infos_len > 0)
            return error("bc_parse_parameter_info_text 0 failed");

        viu = ToStrViu("/** single line comment before the function */");
        res = bc_parse_info_text(viu);
        if (str_not_equal(res.text, "single line comment before the function")
            || str_not_equal(res.return_info, "")
            || str_not_equal(res.error_info, "")
            || res.parameter_infos_len != 0)
            return error("bc_parse_info_text 1 failed");

        viu = ToStrViu("//\tanother single line comment.\t\n");
        res = bc_parse_info_text(viu);
        if (str_not_equal(res.text, "another single line comment.")
            || str_not_equal(res.return_info, "")
            || str_not_equal(res.error_info, "")
            || res.parameter_infos_len != 0)
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
        if (res.parameter_infos_len != 4)
            return error("bc_parse_info_text 3.2 failed");
        if (str_not_equal(res.parameter_infos[0].name, "val")
            || str_not_equal(res.parameter_infos[0].default_value, "")
            || str_not_equal(res.parameter_infos[0].info, "range[0-10)"))
            return error("bc_parse_info_text 3.3 failed");
        if (str_not_equal(res.parameter_infos[1].name, "center")
            || str_not_equal(res.parameter_infos[1].default_value, "true")
            || str_not_equal(res.parameter_infos[1].info, ""))
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


    // parameter
    {
        BcParsedParameter_s res;

        viu = ToStrViu("");
        res = bc_parse_parameter(viu);
        if (*res.name != 0 || *res.type != 0)
            return error("bc_parse_parameter 0 failed");

        viu = ToStrViu("int a");
        res = bc_parse_parameter(viu);
        if (str_not_equal(res.name, "a")
            || str_not_equal(res.type, "int"))
            return error("bc_parse_parameter 1 failed");

        viu = ToStrViu("\t char  * str_ing\n");
        res = bc_parse_parameter(viu);
        if (str_not_equal(res.name, "str_ing")
            || str_not_equal(res.type, "char *"))
            return error("bc_parse_parameter 2 failed");

        viu = ToStrViu("\t \nchar \n \t * \n * str_array\n");
        res = bc_parse_parameter(viu);
        if (str_not_equal(res.name, "str_array")
            || str_not_equal(res.type, "char * *"))
            return error("bc_parse_parameter 3 failed");

        viu = ToStrViu("const int **const** const ptr_fun");
        res = bc_parse_parameter(viu);
        if (str_not_equal(res.name, "ptr_fun")
            || str_not_equal(res.type, "const int * * const * * const"))
            return error("bc_parse_parameter 4 failed");

        viu = ToStrViu("OUT Cloud *cloud");
        res = bc_parse_parameter(viu);
        if (str_not_equal(res.name, "cloud")
            || str_not_equal(res.type, "OUT Cloud *"))
            return error("bc_parse_parameter 5 failed");
    }

    // function
    {
        BcParsedFunction_s res;
        strviu info = ToStrViu("");

        viu = ToStrViu("");
        res = bc_parse_function(info, viu);
        if (*res.name != 0 || *res.return_type != 0 || res.parameters_len != 0)
            return error("bc_parse_function 0 failed");

        viu = ToStrViu("void foo()");
        res = bc_parse_function(info, viu);
        if (str_not_equal(res.name, "foo")
            || str_not_equal(res.return_type, "void")
            || res.parameters_len != 0)
            return error("bc_parse_function 1 failed");

        viu = ToStrViu("EXPORT \n\t int \t *bar()");
        res = bc_parse_function(info, viu);
        if (str_not_equal(res.name, "bar")
            || str_not_equal(res.return_type, "EXPORT int *")
            || res.parameters_len != 0)
            return error("bc_parse_function 2 failed");

        viu = ToStrViu("struct P get_p(int a, const char*b)");
        res = bc_parse_function(info, viu);
        if (str_not_equal(res.name, "get_p")
            || str_not_equal(res.return_type, "struct P")
            || res.parameters == NULL)
            return error("bc_parse_function 3.1 failed");
        if (res.parameters_len != 2)
            return error("bc_parse_function 3.2 failed");
        if (str_not_equal(res.parameters[0].name, "a")
            || str_not_equal(res.parameters[0].type, "int"))
            return error("bc_parse_function 3.3 failed");
        if (str_not_equal(res.parameters[1].name, "b")
            || str_not_equal(res.parameters[1].type, "const char *"))
            return error("bc_parse_function 3.4 failed");

    }

//    // type declerations
//    {
//        bc_ParsedTypeDeclaration res;
//
//        viu = ToStrViu("");
//        res = bc_parse_type_declaration(viu);
//        if (*res.name != 0 || *res.type != 0)
//            return error("bc_parse_type_declaration 0 failed");
//
//        viu = ToStrViu("char *string");
//        res = bc_parse_type_declaration(viu);
//        if (str_not_equal(res.name, "string")
//            || str_not_equal(res.type, "char *"))
//            return error("bc_parse_type_declaration 1 failed");
//
//        viu = ToStrViu(" const  char \t* Error ");
//        res = bc_parse_type_declaration(viu);
//        if (str_not_equal(res.name, "Error")
//            || str_not_equal(res.type, "const char *"))
//            return error("bc_parse_type_declaration 2 failed");
//
//        viu = ToStrViu("float Vec3[3]");
//        res = bc_parse_type_declaration(viu);
//        if (str_not_equal(res.name, "Vec3")
//            || str_not_equal(res.type, "float [3]"))
//            return error("bc_parse_type_declaration 3 failed");
//
//        viu = ToStrViu("int Mat4i[4][4]");
//        res = bc_parse_type_declaration(viu);
//        if (str_not_equal(res.name, "Mat4i")
//            || str_not_equal(res.type, "int [4][4]"))
//            return error("bc_parse_type_declaration 4 failed");
//
//        viu = ToStrViu("void (*fun_ptr)(int a, char c)");
//        res = bc_parse_type_declaration(viu);
//        if (str_not_equal(res.name, "fun_ptr")
//            || str_not_equal(res.type, "void (*)"))
//            return error("bc_parse_type_declaration 5 failed");
//    }

//    // struct
//    {
//        bc_ParsedStruct res;
//        strviu info = ToStrViu("");
//
//        viu = ToStrViu("");
//        res = bc_parse_struct(info, viu);
//        if (*res.name != 0 || res.members.size != 0)
//            return error("bc_parse_struct 0 failed");
//
//        viu = ToStrViu("struct {int a;} A;");
//        res = bc_parse_struct(info, viu);
//        if (str_not_equal(res.name, "A")
//            || res.members.size != 1)
//            return error("bc_parse_struct 1.0 failed");
//        if (str_not_equal(res.members.array[0].name, "a")
//            || str_not_equal(res.members.array[0].type, "int"))
//            return error("bc_parse_struct 1.1 failed");
//
//        viu = ToStrViu("struct {void *impl_;} B;");
//        res = bc_parse_struct(info, viu);
//        if (str_not_equal(res.name, "B")
//            || res.members.size != 1)
//            return error("bc_parse_struct 2.0 failed");
//        if (str_not_equal(res.members.array[0].name, "impl_")
//            || str_not_equal(res.members.array[0].type, "void *"))
//            return error("bc_parse_struct 2.1 failed");
//    }

    {
        // file
        BcParsedFunctionArray res;

        char *filetext = open_file_as_string("filetest1.txt");
        if (filetext) {
            res = bc_parse_file(ToStrViu(filetext));
//            printf("%s\n", res.array[0].name);
//            puts("test");
        }
    }

    return 0;
}

