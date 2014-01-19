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

#include <stdint.h>

#include "uart.h"
#include "fifo.h"

fifo_t g_fifo;
fifo_t *pg_fifo = &g_fifo;
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

static const unsigned long g_ulUARTInt[8] =
{
    INT_UART0, INT_UART1, INT_UART2, INT_UART3,
	INT_UART4, INT_UART5, INT_UART6, INT_UART7
};

void begin(void)
{

	init_fifo(&g_fifo);

    // Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Enable the GPIO pins for the LED (PF2).
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Enable processor interrupts.
    ROM_IntMasterEnable();
    // Set GPIO A0 and A1 as UART pins.
    //GPIOPinConfigure(GPIO_PA0_U0RX);
    //GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                           (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                           UART_CONFIG_PAR_NONE));

    //my additions.
    ROM_UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
	ROM_UARTIntDisable(UART0_BASE, 0xFFFFFFFF);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    ROM_IntEnable(g_ulUARTInt[0]);

    //
    // Enable the UART operation.
    //
    ROM_UARTEnable(UART0_BASE);
}


void UARTIntHandler(void)
{
    unsigned long ulStatus;
    uint8_t c;
    ulStatus = ROM_UARTIntStatus(UART0_BASE, true);

    ROM_UARTIntClear(UART0_BASE, ulStatus);

    while(ROM_UARTCharsAvail(UART0_BASE))
    {
    	c = ROM_UARTCharGetNonBlocking(UART0_BASE);

    	add_tail(&g_fifo,c);
    	//ROM_UARTCharPutNonBlocking(UART0_BASE,c);


        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        //SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
}
