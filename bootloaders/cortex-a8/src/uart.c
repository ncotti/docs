#include "uart.h"

void uart_init(void) {
    volatile uint32_t * uartibrd = UARTIBRD;
    volatile uint32_t * uartfbrd = UARTFBRD;
    volatile uint32_t * uartlcr_h = UARTLCR_H;
    volatile uint32_t * uartcr = UARTCR;

    // Disable UART
    *uartcr = 0x0000;

    // Baudrate 11520
    *uartfbrd = 0U;
    *uartibrd = 0x4U;

    // 8 bit, no fifo, no parity, 1 stop
    *uartlcr_h = 0b01100000;

    // Enable TX UART
    *uartcr = 0x0101;
}

uint8_t uart_is_busy(void) {
    volatile uint32_t * uartfr = UARTFR;
    return ((*uartfr) >> 3) & 0b1;
}

void uart_write_char(char a) {
    volatile uint32_t * uartdr = UARTDR;

    // Only write on 8 LSB
    *((uint8_t *)uartdr) = a;
}