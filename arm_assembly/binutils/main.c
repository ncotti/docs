#include "main.h"

// This global variable should appear as a symbol in the object file in the ".data" section
// It occupies 4 bytes.
int global_var = 55;

// Comments should be erased
int main (void) {
    int a = MACRO_A;
    int b = MACRO_B;

    #ifdef NOT_DEFINED
    // This will not be copied for the preprocessor
    int c = 123;
    #endif

    add(a, b);
    return 0;
}

int add(int a, int b) {
    return a + b;
}
