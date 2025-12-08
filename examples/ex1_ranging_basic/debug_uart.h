#ifndef DEBUG_UART_H
#define DEBUG_UART_H

void debug_uart_init(int pio_tx_pin);
void debug_print(const char* fmt, ...);

#endif