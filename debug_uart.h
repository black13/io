/*
 * debug_uart.h
 *
 *  Created on: Dec 1, 2013
 *      Author: Administrator
 */

#ifndef DEBUG_UART_H_
#define DEBUG_UART_H_


#define DBG_BAUD  (115200)
//#define DBG_BAUD  (230400)
// #define DBG_BAUD  (460800)

// Sizes are strongly recommended to be powers of 2
#define DBG_OUT_BUFFER_SIZE  (1024)
#define DBG_IN_BUFFER_SIZE  (128)

void DBG_Init(void);

// Write a constant string by pointer
void dbg_putstr(const char * str);
void dbg_putstr_nb(const char * str);

// Write an arbitrary formatted string
void dbg_printf(const char * str, ...);
void dbg_printf_nb(const char * str, ...);

// Get up to len characters into a linear buffer.
// String is null terminated, newline is not included.
// Return value is number of characters in line.
// If no newline has been received, 0 is returned immediately.
int dbg_getline_nb(char * bfr, int len);

#endif /* DEBUG_UART_H_ */
