#include "main.h"
#include <stdlib.h>
#include <stdio.h>

char buf[BUFSIZ]; // BUFSIZ is an implementation-defined size


int main(void) {
    int time = 0;
    time++; // Avoid unused variable
    uart_init();
    //time = rand();
    char * dyn_mem;
    dyn_mem = malloc(20);
    dyn_mem[0] = 'a';
    dyn_mem[1] = 'b';
    dyn_mem[2] = 'c';

    uart_write_char(dyn_mem[0]);
    uart_write_char(dyn_mem[1]);
    uart_write_char(dyn_mem[2]);

    //setbuf(stdout, 0)
    setvbuf(stdout, buf, _IOFBF, BUFSIZ);

    while (1) {
        time++;
        printf("Hola\n");
        fflush(stdout);
        //uart_write_char('x');
    }
}

