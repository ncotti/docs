#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

#define UART_BASE_ADDR 0x10009000U

#define UARTDR      ((uint32_t*) (UART_BASE_ADDR + 0x00))
#define UARTFR      ((uint32_t*) (UART_BASE_ADDR + 0x18))
#define UARTIBRD    ((uint32_t*) (UART_BASE_ADDR + 0x24))
#define UARTFBRD    ((uint32_t*) (UART_BASE_ADDR + 0x28))
#define UARTLCR_H   ((uint32_t*) (UART_BASE_ADDR + 0x2C))
#define UARTCR      ((uint32_t*) (UART_BASE_ADDR + 0x30))


void uart_init(void);
uint8_t uart_is_busy(void);
void uart_write_char(char a);

#endif // _UART_H_
