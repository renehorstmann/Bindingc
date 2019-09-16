
#include "bindingc/utilc/alloc.h"

#include "bindingc/parse.h"


static void clear_strings(StrViu viu, char clear) {
    while (viu.begin < viu.end) {
        StrViu comment = sv_eat_until(viu, '\"');
        viu.begin = comment.begin + 1;
        for (;;) {
            int end = sv_find_first(viu, '\"');
            comment.end = viu.begin + end + 1;
            if (viu.begin[end - 1] != '\\')
                break;
            viu.begin += end + 1;
        }
        while (comment.begin < comment.end)
            *comment.begin++ = clear;
        viu.begin = comment.end;
    }
}

static void clear_comments(StrViu viu, char clear) {
    // first block, then line ( /* // */ int a; )
    StrViu back_up = viu;
    while (viu.begin < viu.end) {
        viu = sv_eat_until_cstring(viu, "/*");
        if (sv_empty(viu))
            break;

        int end = sv_find_first_cstring(viu, "*/");
        assert(end >= 2); // min (/**/, not /*/)
        for (int i = 0; i < end + 2; i++) {
            if (*viu.begin != '\n')
                *viu.begin = clear;
            viu.begin++;
        }
    }
    viu = back_up;
    while (viu.begin < viu.end) {
        viu = sv_eat_until_cstring(viu, "//");
        int end = sv_find_first(viu, '\n');
        if (end < 2) // comment on last line
            end = sv_length(viu);
        for (int i = 0; i < end; i++)
            *viu.begin++ = clear;
    }
}

static void clear_macros(StrViu viu, char clear) {
    while (viu.begin < viu.end) {
        viu = sv_lstrip(viu, ' ');
        if (sv_empty(viu))
            return;
        if (*viu.begin == '#') {
            // clear macro code
            StrViu macro = viu;
            for (;;) {
                int line_end = sv_find_first(viu, '\n');
                if (line_end < 0) // macro on last line
                    break;
                StrViu line = {viu.begin, viu.begin + line_end};
                int expand = sv_find_first(line, '\\');
                if (expand < 0) {
                    macro.end = line.end;
                    break;
                }
                viu.begin = line.end;
            }

            while (macro.begin < macro.end) {
                if (*macro.begin != '\n')
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
    for (;;) {
        int next_open = sv_find_first(viu, '{');
        int next_close = sv_find_first(viu, '}');
        if (next_open >= 0 && next_open < next_close) {
            viu.begin += next_open;
            viu = eat_implementation_r_(viu);
        } else {
            assert(next_close >= 0);
            viu.begin += next_close + 1;
            assert(viu.begin[-1] == '}');
            return viu;
        }
    }
}

// viu should be the cleared filetext with clear_strings, _comments, _macros
static StrViuArray get_functions(StrViu viu) {
    StrViuArray functions = {0};

    while (viu.begin < viu.end) {
        viu = sv_lstrip(viu, ' ');
        int decl_end = sv_find_first_multiple(viu, "=;{");
        if (decl_end < 0)
            break;
        if (viu.begin[decl_end] == '=') {
            viu.begin += decl_end;
            viu = sv_eat_until(viu, ';');
            viu.begin++;
            continue;
        }
        StrViu decl = {viu.begin, viu.begin + decl_end};
        bool is_function = false;
        bool is_struct;
        if (sv_count(decl, '(') == 1 && sv_count(decl, ')') == 1) {
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
            if (sv_empty(tmp) || (*tmp.begin != '*' && sv_count(tmp, ' ') > 0))
                is_function = true;
        }
        is_struct = !is_function && viu.begin[decl_end] == '{';

        if (is_function) {
            functions.array = ReNew(StrViu, functions.array, ++functions.size);
            functions.array[functions.size - 1] = decl;
        }

        if (viu.begin[decl_end] == ';') {
            viu.begin += decl_end + 1;
            continue;
        }
        assert(viu.begin[decl_end] == '{');
        viu.begin += decl_end;
        viu = eat_implementation_r_(viu);
        if (is_struct) {
            viu = sv_eat_until(viu, ';');
            viu.begin++;
        }
    }

    return functions;
}

bc_ParsedFunctionArray bc_parse_file(StrViu filetext) {
    bc_ParsedFunctionArray res = {0};

    char *copy_function = sv_heap_cpy(filetext);
    StrViu viu = {copy_function, copy_function + sv_length(filetext)};

    clear_strings(viu, ' ');
    clear_comments(viu, ' ');
    clear_macros(viu, ' ');


    StrViuArray functions = get_functions(viu);
    StrViuArray comments = {New(StrViu, functions.size), functions.size};
    res.size = functions.size;
    if(res.size > 0)
        res.array = New0(bc_ParsedFunction, functions.size);

    for (size_t i = 0; i < functions.size; i++) {
        size_t decl_start_pos = functions.array[i].begin - copy_function;
        StrViu back = {filetext.begin, filetext.begin + decl_start_pos};
        back = sv_eat_back_until(back, '\n');
        back.end--;
        StrViu line = {back.end, back.end};
        comments.array[i] = (StrViu) {back.end, back.end};
        do {
            comments.array[i].begin = back.end;
            back = sv_eat_back_until(back, '\n');
            back.end--;
            line.end = line.begin;
            line.begin = back.end;
            line = sv_strip(line, ' ');
        } while (!sv_empty(line));
    }

    for(size_t i=0; i<functions.size; i++)
        res.array[i] = bc_parse_function(comments.array[i], functions.array[i]);

    free(copy_function);
    StrViuArray_kill(&functions);
    StrViuArray_kill(&comments);
    return res;
}

