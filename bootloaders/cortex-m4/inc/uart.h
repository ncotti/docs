#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

#define USART_BASE_ADDR 0x40011000U

#define USART_SR        ((uint32_t*) (USART_BASE_ADDR + 0x00))
#define USART_DR        ((uint32_t*) (USART_BASE_ADDR + 0x04))
#define USART_BRR       ((uint32_t*) (USART_BASE_ADDR + 0x08))
#define USART_CR1       ((uint32_t*) (USART_BASE_ADDR + 0x0C))
#define USART_CR2       ((uint32_t*) (USART_BASE_ADDR + 0x10))
#define USART_CR3       ((uint32_t*) (USART_BASE_ADDR + 0x14))
#define USART_GTPR      ((uint32_t*) (USART_BASE_ADDR + 0x18))

void uart_init(void);
uint8_t uart_is_busy(void);
void uart_write_char(char a);
uint8_t uart_new_data(void);
uint8_t uart_read_char(void);

#endif // _UART_H_