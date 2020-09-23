#include <stdio.h>
#include <math.h>
#include "base_header.h"


void foo() {
    puts("c(foo)");
}

/**
 * Bar function
 */
int bar(int b) {
    printf("c(bar(%d) return b+1)\n", b);
    return b+1;
}


/**
 * Returns the sum of a + b
 * @param absolute: If true, returns |a| + |b|
 */
float sum(float a, float b, bool absolute) {
    if(absolute) {
        return fabsf(a) + fabsf(b);
    }
    return a + b;
}
