#include "uart.h"

void uart_init(void) {
    volatile uint32_t * usart_cr1 = USART_CR1;
    volatile uint32_t * usart_brr = USART_BRR;

    // Disable UART
    *usart_cr1 = 0x0000;

    // Frequency: 8MHz. Baudrate 115200. USARTDIV = 4.34028
    *usart_brr = ((4 & 0xFFF) << 4) | (5 & 0xF) ;

    // Enable TX and RX UART, 8bit, 1 start, 1 stop
    *usart_cr1 = 0x000C;
}

uint8_t uart_is_busy(void) {
    volatile uint32_t * usart_sr = USART_SR;
    return !((*usart_sr) >> 7) & 0b1;
}

uint8_t uart_new_data(void) {
    volatile uint32_t * usart_sr = USART_SR;
    return !(((*usart_sr) >> 5) & 0b1);
}

void uart_write_char(char a) {
    volatile uint32_t * usart_dr = USART_DR;
    *((uint8_t *) usart_dr) = a;
}

uint8_t uart_read_char(void) {
    volatile uint32_t * usart_dr = USART_DR;
    return (uint8_t) *usart_dr & 0xFF;
}