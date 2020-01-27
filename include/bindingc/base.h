#ifndef BINDINGC_BASE_H
#define BINDINGC_BASE_H

#define MAX_PARAMETERS 16

typedef char bc_shortstring[128];
typedef char bc_string[256];
typedef char bc_longstring[512];

typedef const char *bc_err;

#define BcSuccess (bc_err) 0

#endif //BINDINGC_BASE_H
