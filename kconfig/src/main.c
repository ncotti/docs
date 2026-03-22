#include "main.h"

int main(void) {
    printf("%s\n", CONFIG_FIRST_PRINTF_TEXT);

    #ifdef CONFIG_SECOND_PRINTF_TEXT
    printf("%s\n", CONFIG_SECOND_PRINTF_TEXT);
    #endif

    int value = 0;

    #ifdef CONFIG_MATH_OPERATION_ADD
    value = CONFIG_MATH_FIRST_OPERAND + CONFIG_MATH_SECOND_OPERAND;
    #elif CONFIG_MATH_OPERATION_SUB
    value = CONFIG_MATH_FIRST_OPERAND - CONFIG_MATH_SECOND_OPERAND;
    #elif CONFIG_MATH_OPERATION_MUL
    value = CONFIG_MATH_FIRST_OPERAND * CONFIG_MATH_SECOND_OPERAND;
    #endif

    printf("Math operation result: %d\n", value);

    return 0;
}