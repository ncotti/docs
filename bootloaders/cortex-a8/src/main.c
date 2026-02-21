#include "main.h"

char buf;

int main(void) {
    char value_read;
    char * dyn_mem;

    uart_init();

    dyn_mem = malloc(20);
    dyn_mem[0] = 'a';
    dyn_mem[1] = 'b';
    dyn_mem[2] = 'c';

    uart_write_char(dyn_mem[0]);
    uart_write_char(dyn_mem[1]);
    uart_write_char(dyn_mem[2]);

    free(dyn_mem);

    // This is required for newlib.
    // "1" is the amount of chars that "_read()" will handle at the same time.
    // By default BUFF_SIZE = 1024, and read would be called with a "len" of
    // 1024 for "getchar()".
    setvbuf(stdin, &buf, _IOFBF, 1);

    while (1) {
        printf("Reading a single character ...\n");
        value_read = getchar();
        printf("Character read: \"%c\"\n", value_read);
    }
}

