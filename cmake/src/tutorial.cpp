#include <stdio.h>

// See how we can include either "math.h" and "header.h" even
// though they aren't in the same folder.
#include "header.h"
#include "math.h"

int main(void) {
    printf("Hello, this is the CMake tutorial\n");
    printf("The macro value is: %s\n", MACRO);
    printf("Add 5 + 7 = %d\n", operation(5, 7));
}