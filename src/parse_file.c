
#include "bindingc/utilc/alloc.h"

#include "bindingc/parse.h"



static void clear_strings(StrViu viu, char clear) {
    while(viu.begin < viu.end) {
        StrViu comment = sv_eat_until(viu, '\"');
        viu.begin = comment.begin + 1;
        for(;;) {
            int end = sv_find_first(viu, '\"');
            comment.end = viu.begin + end + 1;
            if(viu.begin[end-1] != '\\')
                break;
            viu.begin += end + 1;
        }
        while(comment.begin < comment.end)
            *comment.begin++ = clear;
        viu.begin = comment.end;
    }
}

static void clear_comments(StrViu viu, char clear) {
    // first block, then line ( /* // */ int a; )
    StrViu back_up = viu;
    while(viu.begin < viu.end) {
        viu = sv_eat_until_cstring(viu, "/*");
        if(sv_empty(viu))
            break;

        int end = sv_find_first_cstring(viu, "*/");
        assert(end>=2); // min (/**/, not /*/)
        for(int i=0; i<end+2; i++) {
            if(*viu.begin != '\n')
                *viu.begin = clear;
            viu.begin++;
        }
    }
    viu = back_up;
    while(viu.begin < viu.end) {
        viu = sv_eat_until_cstring(viu, "//");
        int end = sv_find_first(viu, '\n');
        if(end<2) // comment on last line
            end = sv_length(viu);
        for(int i=0; i<end; i++)
            *viu.begin++ = clear;
    }
}

static void clear_macros(StrViu viu, char clear) {
    while(viu.begin < viu.end) {
        viu = sv_lstrip(viu, ' ');
        if(sv_empty(viu))
            return;
        if(*viu.begin == '#') {
            // clear macro code
            StrViu macro = viu;
            for(;;) {
                int line_end = sv_find_first(viu, '\n');
                if(line_end<0) // macro on last line
                    break;
                StrViu line = {viu.begin, viu.begin+line_end};
                int expand = sv_find_first(line, '\\');
                if(expand<0) {
                    macro.end = line.end;
                    break;
                }
                viu.begin = line.end;
            }

            while(macro.begin < macro.end) {
                if(*macro.begin != '\n')
                    *macro.begin = clear;
                macro.begin++;
            }

            viu.begin = macro.end;
        }
        viu = sv_eat_until(viu, '\n');
    }
}

static StrViu eat_implementation_r_(StrViu viu) {
    assert(*viu.begin == '{');
    viu.begin++;
    for(;;) {
        int next_open = sv_find_first(viu, '{');
        int next_close = sv_find_first(viu, '}');
        if (next_open >= 0 && next_open < next_close) {
            viu.begin += next_open;
            viu = eat_implementation_r_(viu);
        } else {
            assert(next_close>=0);
            viu.begin += next_close + 1;
            assert(viu.begin[-1] == '}');
            return viu;
        }
    }
}



#include <stdio.h>
bc_ParsedFunction *bc_parse_file(StrViu filetext) {
    bc_ParsedFunction *res = New0(bc_ParsedFunction, 1);

    char *copy = sv_heap_cpy(filetext);
    StrViu viu = {copy, copy + sv_length(filetext)};

    clear_strings(viu, ' ');
    clear_comments(viu, ' ');
    clear_macros(viu, ' ');

    while(viu.begin < viu.end) {
        viu = sv_lstrip(viu, ' ');
        int decl_end = sv_find_first_multiple(viu, "=;{");
        if(decl_end<0)
            break;
        if(viu.begin[decl_end] == '=') {
            viu.begin += decl_end;
            viu = sv_eat_until(viu, ';');
            viu.begin++;
            continue;
        }
        StrViu decl = {viu.begin, viu.begin+decl_end};
        bool is_function = false;
        if(sv_count(decl, '(')==1 && sv_count(decl, ')') == 1) {
            // could be function ptr decleration
            // void (*fun_ptr);
            // -> starts with a ptr
            // or it could be some sort of int (a);
            // -> single word (no spaces in between)
            StrViu tmp = sv_eat_until(decl, '(');
            tmp.begin++;
            tmp = sv_eat_back_until(tmp, ')');
            tmp.end--;
            tmp = sv_strip(tmp, ' ');
            if(sv_empty(tmp) || (*tmp.begin != '*' && sv_count(tmp, ' ') > 0))
                is_function = true;
        }

        if(is_function) {
            printf("decl: <%s>\n", sv_heap_cpy(decl));
            // todo: find comment
        }

        bool is_struct_declaration = !is_function
                && viu.begin[decl_end] == '{'
                && (sv_find_first_cstring(decl, "struct")>=0 || sv_find_first_cstring(decl, "union"));

        if(viu.begin[decl_end] == ';') {
            viu.begin += decl_end+1;
            continue;
        }
        assert(viu.begin[decl_end] == '{');
        viu.begin += decl_end;
        viu = eat_implementation_r_(viu);
        if(is_struct_declaration) {
            viu = sv_eat_until(viu, ';');
            viu.begin++;
        }
    }


    if(!res->name)
        Free0(res);

    free(copy);
    return res;
}

