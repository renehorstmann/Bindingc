#ifndef BINDINGC_BASE_H
#define BINDINGC_BASE_H

#define BC_MAX_PARAMETERS 32

typedef char bc_shortstring[128];
typedef char bc_string[256];
typedef char bc_longstring[512];

typedef const char *bc_err;

#define BC_SUCCESS (bc_err) 0

#endif //BINDINGC_BASE_H
