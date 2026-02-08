// Function defined in assembly
extern int add6_from_asm(int a, char b, char c, int d, int e, char f);

// Variables defined in assembly
extern int var_from_asm;
extern int vector_from_asm[6];

// Variable declared in C used from assembly
int var_from_c = 25;

// Function declared in C used in assembly
int add9(int a, short b, char c, int d, int e, int f, unsigned int g,
        unsigned char h, int i) {
    return a + 2*b + 3*c + 4*d + 5*e + 6*f + 7*g + 8*h + 9*i;
}

int main(void) {
    int a = 0; a++;
    int* b;
    var_from_asm++;     // Should be 6 now
    b = &var_from_asm;  // You can use the address of the label with the "&" operator.
    a = *b;             // Should be 6
    a = add6_from_asm(3, 5, 7, 9, 11, 13);  // Result: r0 = 48
    a = vector_from_asm[4]; // Should give the "4", the fifth element in the array
    asm volatile("ldr r0, =var_from_c");
    asm volatile("ldr r0, [r0]");
}