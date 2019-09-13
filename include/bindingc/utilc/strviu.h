#ifndef UTILC_STRVIU_H
#define UTILC_STRVIU_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/** Macro to create a StrViu from a cstring */
#define ToStrViu(string) ((StrViu){(char*)(string), (char*)(string)+strlen((string))})

/** Data type for a string view instead of null termination, it uses an pointer to end (e. g. pointing to the 0) */
typedef struct {
    char *begin;
    char *end;
} StrViu;

/** Data type to store multiple StrViu's */
typedef struct {
    StrViu *array;
    size_t size;
} StrViuArray;

/** @returns: true if the StrViu is empty (length==0) */
static bool sv_empty(StrViu viu) {
    return viu.begin >= viu.end;
}

/** @returns: the length of the StrViu */
static size_t sv_length(StrViu viu) {
    return viu.end - viu.begin;
}

/** @returns: A new StrViu, based on viu, but without the leading chars of strip (space -> isspace()) */
static StrViu sv_lstrip(StrViu viu, char strip) {
    if (strip == ' ') {
        while (viu.begin < viu.end && isspace(*viu.begin))
            viu.begin++;
    } else {
        while (viu.begin < viu.end && *viu.begin == strip)
            viu.begin++;
    }
    return viu;
}

/** @returns: A new StrViu, based on viu, but without the least chars of strip (space -> isspace()) */
static StrViu sv_rstrip(StrViu viu, char strip) {
    if (strip == ' ') {
        while (viu.begin < viu.end && isspace(*(viu.end - 1)))
            viu.end--;
    } else {
        while (viu.begin < viu.end && *(viu.end - 1) == strip)
            viu.end--;
    }
    return viu;
}

/** @returns: A new StrViu, based on viu, but without the leading and least chars of strip (space -> isspace()) */
static StrViu sv_strip(StrViu viu, char strip) {
    return sv_lstrip(sv_rstrip(viu, strip), strip);
}

/** @returns: A new StrViu, based on viu, from the beginning until the first split, or the end */
static StrViu sv_next_split(StrViu viu, char split) {
    StrViu res = {viu.begin, viu.begin + 1};
    if (split == ' ') {
        while (res.end < viu.end && !isspace(*res.end))
            res.end++;
    } else {
        while (res.end < viu.end && *res.end != split)
            res.end++;
    }
    return res;
}

/** @returns: An StrViuArray, based on viu, containing all non empty splits between each split */
static StrViuArray sv_split(StrViu viu, char split) {
    StrViuArray res = {0};
    for (;;) {
        viu = sv_lstrip(viu, split);
        if (sv_empty(viu))
            return res;
        StrViu item = sv_next_split(viu, split);
        viu.begin = item.end;
        res.array = (StrViu *) realloc(res.array, ++res.size * sizeof(StrViu));
        res.array[res.size - 1] = item;
    }
}

/** @returns: The index of the first found char find in viu, or -1 if nothing found */
static int sv_find_first(StrViu viu, char find) {
    int n = 0;
    if (find == ' ') {
        while (viu.begin < viu.end) {
            if (isspace(*viu.begin++))
                return n;
            n++;
        }
    } else {
        while (viu.begin < viu.end) {
            if (*viu.begin++ == find)
                return n;
            n++;
        }
    }
    return -1;
}

/** @returns: The index of the last found char find in viu, or -1 if nothing found */
static int sv_find_last(StrViu viu, char find) {
    int n = (int) sv_length(viu) - 1;
    if (find == ' ') {
        while (--viu.end >= viu.begin) {
            if (isspace(*viu.end))
                return n;
            n--;
        }
    } else {
        while (--viu.end >= viu.begin) {
            if (*viu.end == find)
                return n;
            n--;
        }
    }
    return -1;
}

/** @returns: The index of the first found StrViu find in viu, or -1 if nothing found */
static int sv_find_first_sv(StrViu viu, StrViu find) {
    int n = 0;
    size_t find_len = sv_length(find);
    while (viu.begin <= viu.end - find_len) {
        if (strncmp(viu.begin++, find.begin, find_len) == 0)
            return n;
        n++;
    }
    return -1;
}

/** @returns: The index of the last found StrViu find in viu, or -1 if nothing found */
static int sv_find_last_sv(StrViu viu, StrViu find) {
    size_t find_len = sv_length(find);
    viu.end-=find_len;
    int n = (int) sv_length(viu);
    while (viu.end >= viu.begin) {
        if (strncmp(viu.end--, find.begin, find_len) == 0)
            return n;
        n--;
    }
    return -1;
}

/** @returns: The index of the first found cstring find in viu, or -1 if nothing found */
static int sv_find_first_cstring(StrViu viu, const char *find) {
    return sv_find_first_sv(viu, ToStrViu(find));
}

/** @returns: The index of the last found cstring find in viu, or -1 if nothing found */
static int sv_find_last_cstring(StrViu viu, const char *find) {
    return sv_find_last_sv(viu, ToStrViu(find));
}

/** @returns: The index of the first found char of multiple_chars in viu, or -1 if nothing found */
static int sv_find_first_multiple(StrViu viu, const char *multiple_chars) {
    int n = 0;
    while (viu.begin < viu.end) {
        for (const char *c = multiple_chars; *c != 0; c++) {
            if ((*c == ' ' && isspace(*viu.begin)) || *c == *viu.begin)
                return n;
        }
        viu.begin++;
        n++;
    }
    return -1;
}

/** @returns: The index of the last found char of multiple_chars in viu, or -1 if nothing found */
static int sv_find_last_multiple(StrViu viu, const char *multiple_chars) {
    int n = (int) sv_length(viu)-1;
    while (--viu.end >= viu.begin) {
        for (const char *c = multiple_chars; *c != 0; c++) {
            if ((*c == ' ' && isspace(*viu.end)) || *c == *viu.end)
                return n;
        }
        n--;
    }
    return -1;
}

/** @returns: A new StrViu, based on viu, without every leading char until the char until */
static StrViu sv_eat_until(StrViu viu, char until) {
    int pos = sv_find_first(viu, until);
    if (pos >= 0)
        viu.begin += pos;
    else
        viu.begin = viu.end;
    return viu;
}

/** @returns: A new StrViu, based on viu, without every tailing char until the char until */
static StrViu sv_eat_back_until(StrViu viu, char until) {
    int pos = sv_find_last(viu, until);
    if (pos >= 0)
        viu.end = viu.begin+pos+1;
    else
        viu.end = viu.begin;
    return viu;
}

/** @returns: A new StrViu, based on viu, without every leading char until the StrViu until */
static StrViu sv_eat_until_sv(StrViu viu, StrViu until) {
    int pos = sv_find_first_sv(viu, until);
    if (pos >= 0)
        viu.begin += pos;
    else
        viu.begin = viu.end;
    return viu;
}

/** @returns: A new StrViu, based on viu, without every tailing char until the StrViu until */
static StrViu sv_eat_back_until_sv(StrViu viu, StrViu until) {
    int pos = sv_find_last_sv(viu, until);
    if (pos >= 0)
        viu.end = viu.begin+pos+sv_length(until);
    else
        viu.end = viu.begin;
    return viu;
}

/** @returns: A new StrViu, based on viu, without every leading char until the cstring until */
static StrViu sv_eat_until_cstring(StrViu viu, const char *until) {
    return sv_eat_until_sv(viu, ToStrViu(until));
}

/** @returns: A new StrViu, based on viu, without every tailing char until the cstring until */
static StrViu sv_eat_back_until_cstring(StrViu viu, const char *until) {
    return sv_eat_back_until_sv(viu, ToStrViu(until));
}

/** @returns: A new StrViu, based on viu, without every leading char until one of the chars in multiple_chars */
static StrViu sv_eat_until_multiple(StrViu viu, const char *multiple_chars) {
    int pos = sv_find_first_multiple(viu, multiple_chars);
    if (pos >= 0)
        viu.begin += pos;
    else
        viu.begin = viu.end;
    return viu;
}

/** @returns: A new StrViu, based on viu, without every tailing char until one of the chars in multiple_chars */
static StrViu sv_eat_back_until_multiple(StrViu viu, const char *multiple_chars) {
    int pos = sv_find_last_multiple(viu, multiple_chars);
    if (pos >= 0)
        viu.end = viu.begin+pos+1;
    else
        viu.end = viu.begin;
    return viu;
}

/** @returns: The number of the char search, found in viu */
static int sv_count(StrViu viu, char search) {
    int cnt = 0;
    if(search == ' ') {
        while (viu.begin < viu.end) {
            if (isspace(*viu.begin++))
                cnt++;
        }
    } else {
        while (viu.begin < viu.end) {
            if (*viu.begin++ == search)
                cnt++;
        }
    }
    return cnt;
}

/** @returns: The number of the StrViu search, found in viu */
static int sv_count_sv(StrViu viu, StrViu search) {
    int cnt = 0;
    size_t search_len = sv_length(search);
    while (viu.begin <= viu.end - search_len) {
        if (strncmp(viu.begin++, search.begin, search_len) == 0)
            cnt++;
    }
    return cnt;
}

/** @returns: The number of the cstring search, found in viu */
static int sv_count_cstring(StrViu viu, const char *search) {
    return sv_count_sv(viu, ToStrViu(search));
}

/** @returns: The number of each char in multiple_chars, found in viu */
static int sv_count_multiple(StrViu viu, const char *multiple_chars) {
    int cnt = 0;
    while(*multiple_chars)
        cnt += sv_count(viu, *multiple_chars++);
    return cnt;
}

/** Replaces each char old with replacement in the StrViu viu */
static void sv_replace(StrViu viu, char old, char replacement) {
    while (viu.begin < viu.end) {
        if (*viu.begin == old)
            *viu.begin = replacement;
    }
}

/** Copies the StrViu viu to dst, with each StrViu old replaced to replacement (until the end of one is reached) */
static void sv_replace_into_sv(StrViu dst, StrViu viu, StrViu old, StrViu replacement) {
    size_t old_len = sv_length(old);
    size_t repl_len = sv_length(replacement);
    while (dst.begin < dst.end && viu.begin < viu.end) {
        if (strncmp(viu.begin, old.begin, old_len) == 0) {
            size_t n = sv_length(dst) < repl_len ? sv_length(dst) : repl_len;
            strncpy(dst.begin, replacement.begin, n);
            dst.begin += n;
            viu.begin += old_len;
        } else
            *dst.begin++ = *viu.begin++;
    }
    while (dst.begin < dst.end)
        *dst.begin++ = 0;
}

/** Copies the StrViu viu to dst, with each cstring old replaced to replacement (until the end of one is reached) */
static void sv_replace_into_cstring(StrViu dst, StrViu viu, const char *old, const char *replacement) {
    sv_replace_into_sv(dst, viu, ToStrViu(old), ToStrViu(replacement));
}

/**
 * @returns: A new cstring on the heap, copied from viu to dst,
 * with each StrViu old replaced to replacement
 */
static char *sv_replace_to_heap_sv(StrViu viu, StrViu old, StrViu replacement) {
    size_t old_len = sv_length(old);
    size_t repl_len = sv_length(replacement);
    size_t size = sv_length(viu) + 1;
    size_t add_size = repl_len > 128 ? repl_len : 128;
    char *res = (char *) malloc(size);
    if (!res)
        return NULL;
    int n = 0;
    while (viu.begin < viu.end) {
        if (strncmp(viu.begin, old.begin, old_len) == 0) {
            if (size <= n + repl_len) {
                size += add_size;
                char *r = (char *) realloc(res, size);
                if (!r) {
                    free(res);
                    return NULL;
                }
                res = r;
            }
            strncpy(res + n, replacement.begin, repl_len);
            viu.begin += old_len;
            n += repl_len;
        } else {
            res[n] = *viu.begin++;
            if (++n >= size) {
                size += add_size;
                char *r = (char *) realloc(res, size);
                if (!r) {
                    free(res);
                    return NULL;
                }
                res = r;
            }
        }
    }
    res[n] = 0; // ?
    return res;
}

/**
 * @returns: A new cstring on the heap, copied from viu to dst,
 * with each cstring old replaced to replacement
 */
static char *sv_replace_to_heap_cstring(StrViu viu, const char *old, const char *replacement) {
    return sv_replace_to_heap_sv(viu, ToStrViu(old), ToStrViu(replacement));
}


/** Copies the StrViu viu into the cstring dst */
static void sv_cpy(char *dst, StrViu viu) {
    size_t len = sv_length(viu);
    strncpy(dst, viu.begin, len);
    dst[len] = 0;
}

/** @return: The StrViu viu into a cstring on the heap */
static char *sv_heap_cpy(StrViu viu) {
    char *str = (char *) malloc(sv_length(viu) + 1);
    sv_cpy(str, viu);
    return str;
}


#endif //UTILC_STRVIU_H
