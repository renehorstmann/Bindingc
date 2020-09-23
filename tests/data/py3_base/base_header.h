#ifndef BASE_HEADER_H
#define BASE_HEADER_H

#include <stdbool.h>

void foo();

/**
 * Bar function
 */
int bar(int b);


/**
 * Returns the sum of a + b
 * @param absolute: If true, returns |a| + |b|
 */
float sum(float a, float b, bool absolute);


#endif //BASE_HEADER_H
