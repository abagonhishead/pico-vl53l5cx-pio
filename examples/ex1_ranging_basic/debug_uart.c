#include "hardware/pio.h"
#include "debug_uart.pio.h"
#include <ctype.h>

#include <stdio.h>
#include <stdarg.h> 

#include "string.h"

#include "debug_uart.h"

PIO db_pio;
uint db_sm;
uint db_offset;


// This is the same as the default UART baud rate on Pico
const uint SERIAL_BAUD = 115200;

void debug_uart_init(int pio_tx_pin){
    // This will find a free pio and state machine for our program and load it for us
    // We use pio_claim_free_sm_and_add_program_for_gpio_range (for_gpio_range variant)
    // so we will get a PIO instance suitable for addressing gpios >= 32 if needed and supported by the hardware
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&debug_uart_program, &db_pio, &db_sm, &db_offset, pio_tx_pin, 1, true);
    hard_assert(success);

    debug_uart_program_init(db_pio, db_sm, db_offset, pio_tx_pin, SERIAL_BAUD);
}

void debug_print(const char* fmt, ...){
      char buffer[512];
      memset(buffer, 0, sizeof(buffer));

      va_list list;
      va_start(list, fmt);

      vsprintf(buffer, fmt, list);
          
      va_end(list);
          
      debug_uart_program_puts(db_pio, db_sm, buffer);

}