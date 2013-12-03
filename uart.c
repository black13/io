#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"

#include "uart.h"

RINGB g_ringbuffer;

void initrb(RINGB *rbp)
{
	rbp->rb_hd = rbp->rb_tl = rbp->rb_buf;
}
/*
void UART_Init()
{
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
	(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	UART_CONFIG_PAR_NONE));

	//enable the interrupt.
	ROM_IntEnable(INT_UART0);
	//receive and transmit.
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);


	//initrb(&g_ringbuffer);
}
*/
char rb_putc(char c,RINGB *rbp)
{
	char *nxtp;

	/* ring buffer full? */
	if ( (nxtp = RB_SUCC(rbp, rbp->rb_tl)) == rbp->rb_hd) return (-1);

	/* stuff character */
	*rbp->rb_tl = c;
	rbp->rb_tl = nxtp;
	return(0);
}

typedef unsigned char u_char;
int rb_getc(RINGB *rbp)
{
	u_char c;

	/* ring buffer empty? */
	if (rbp->rb_hd == rbp->rb_tl)
          return(-1);

	/* fetch character, locate next character */
	c = *(u_char *) rbp->rb_hd;
	rbp->rb_hd = RB_SUCC(rbp, rbp->rb_hd);
	return (c);
}

#define PLAIN_BUFFER 1024

/**
 * Simple buffer based.
 */

//u_uchar g_buffer[PLAIN_BUFFER];



/// <summary>
///		Write bufsz characters to the UART from buffer
///		Must make sure all bytes are transmitted before returning
/// </summary>
void UARTWriteBuffer(const unsigned char* buffer, unsigned int bufsz)
{
	const unsigned char* str = buffer;
	while(bufsz)
	{
		while(ROM_UARTBusy(UART0_BASE));
		ROM_UARTCharPut(UART1_BASE, *str++);
		bufsz--;
	}
}
unsigned long g_TimerCounter ;
/// <summary>
///		Write bufsz characters to the UART from buffer
///		Must make sure all bytes are transmitted before returning
/// </summary>
int UARTReadBuffer(unsigned char* buffer, unsigned int bufsz)
{
	ROM_IntDisable(INT_UART0);
	int index = 0;
	unsigned char ch;
	g_TimerCounter = 0;
	while(index < bufsz)
	{
	    if (ROM_UARTCharsAvail(UART1_BASE)) // Loop while there are characters in the receive FIFO.
	    {
	    	ch = ROM_UARTCharGet(UART0_BASE); //  get the character from the serial port
	    	buffer[index] = ch;
	    	index++;
	    	g_TimerCounter = 0; // reset timeout
	    }
	    else if (g_TimerCounter > 200)
	    {
	    	index = -1;
	    	break;
	    }
	}
	ROM_IntEnable(INT_UART1);
	return index;
}
