#include "main.h"

int main(void) {
    int time = 0;
    time++; // Avoid unused variable
    while (1) {
        asm("WFI");
    }
}

