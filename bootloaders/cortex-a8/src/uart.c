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

    // Enable TX and RX UART
    *uartcr = 0x0301;
}

uint8_t uart_is_busy(void) {
    volatile uint32_t * uartfr = UARTFR;
    return ((*uartfr) >> 3) & 0b1;
}

uint8_t uart_new_data(void) {
    volatile uint32_t * uartfr = UARTFR;
    return ((*uartfr) >> 4) & 0b1;
}

void uart_write_char(char a) {
    volatile uint32_t * uartdr = UARTDR;

    // Only write on 8 LSB
    *((uint8_t *)uartdr) = a;
}

uint8_t uart_read_char(void) {
    volatile uint32_t * uartdr = UARTDR;
    return (uint8_t) *uartdr & 0xFF;
}