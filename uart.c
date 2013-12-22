
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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

//#include "uart.h"

#define UART_BUFF_SIZE		256
#define	UART_BUFF_MASK		(UART_BUFF_SIZE-1)
uint8_t m_portNum;
uint32_t m_uartBase;
volatile struct {
	uint16_t	inptr, outptr;			/* in/out index */
	uint8_t		buff[UART_BUFF_SIZE];	/* receive/transmit buffer */
} TxFifo, RxFifo;



// UART peripherals
static const unsigned long g_UARTPeriph[] =
{
	SYSCTL_PERIPH_UART0, SYSCTL_PERIPH_UART1,
	SYSCTL_PERIPH_UART2, SYSCTL_PERIPH_UART3,
	SYSCTL_PERIPH_UART4, SYSCTL_PERIPH_UART5,
	SYSCTL_PERIPH_UART6, SYSCTL_PERIPH_UART7
};

// base addresses for the console UART
static const unsigned long g_UARTBase[] =
{
	UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE,
	UART4_BASE, UART5_BASE, UART6_BASE, UART7_BASE
};

// interrupts for the console UART
static const unsigned long g_UARTInt[] =
{
	INT_UART0, INT_UART1, INT_UART2, INT_UART3,
	INT_UART4, INT_UART5, INT_UART6, INT_UART7
};

// UART gpio configurations
static const unsigned long g_UARTConfig[8][2] =
{
	{GPIO_PA0_U0RX, GPIO_PA1_U0TX}, {GPIO_PB0_U1RX, GPIO_PB1_U1TX},
	{GPIO_PD6_U2RX, GPIO_PD7_U2TX}, {GPIO_PC6_U3RX, GPIO_PC7_U3TX},
	{GPIO_PC4_U4RX, GPIO_PC5_U4TX},	{GPIO_PE4_U5RX, GPIO_PE5_U5TX},
	{GPIO_PD4_U6RX, GPIO_PD5_U6TX},	{GPIO_PE0_U7RX, GPIO_PE1_U7TX}
};
static const unsigned long g_UARTPort[8] =
{
	GPIO_PORTA_BASE, GPIO_PORTB_BASE, GPIO_PORTD_BASE, GPIO_PORTC_BASE,
	GPIO_PORTC_BASE, GPIO_PORTE_BASE, GPIO_PORTD_BASE, GPIO_PORTE_BASE
};
static const unsigned long g_UARTPins[8] =
{
	GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1,
	GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_6 | GPIO_PIN_7,
	GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_4 | GPIO_PIN_5,
	GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_0 | GPIO_PIN_1
};

uint8_t isrx()
{
	// checks if a character is present in the RX buffer
	return (RxFifo.inptr != RxFifo.outptr);
}

uint8_t getc()
{
	uint8_t c;

	if(!m_uartBase) return 0;

	// wait until a character is present
	while (isrx()==0)	continue;

	// get a character from RX buffer
	c = RxFifo.buff[RxFifo.outptr];
	// increment out index
	RxFifo.outptr = (RxFifo.outptr+1)&UART_BUFF_MASK;

	return c;
}
void UARTIntHandler(void)
//void UARTIntHandler(void)
{
	// get and clear the interrupt status
	uint32_t status = ROM_UARTIntStatus( m_uartBase, true );
	ROM_UARTIntClear( m_uartBase, status );

	if( status & (UART_INT_RX | UART_INT_RT) )
	{
        while(ROM_UARTCharsAvail(m_uartBase))
        {	// Get all the available characters from the UART
            long lChar = ROM_UARTCharGetNonBlocking(m_uartBase);
            if(!(((RxFifo.inptr + 1) & UART_BUFF_MASK) == RxFifo.outptr))
            {
                RxFifo.buff[RxFifo.inptr] = (unsigned char)(lChar & 0xFF);
                RxFifo.inptr = (RxFifo.inptr+1)%UART_BUFF_SIZE;
            }
        }
	}
	if( status & UART_INT_TX )
	{
		// disable the UART interrupt
		ROM_IntDisable(g_UARTInt[m_portNum]);
		while(ROM_UARTSpaceAvail(m_uartBase) && !(TxFifo.outptr == TxFifo.inptr))
		{	// place the character to the data register
			ROM_UARTCharPutNonBlocking(m_uartBase, TxFifo.buff[TxFifo.outptr]);
			TxFifo.outptr = (TxFifo.outptr+1)%UART_BUFF_SIZE;
		}
		// reenable the UART interrupt
		ROM_IntEnable(g_UARTInt[m_portNum]);

		if(TxFifo.outptr == TxFifo.inptr) //if(TX_BUFFER_EMPTY)
        {	// If the output buffer is empty, turn off the transmit interrupt.
            ROM_UARTIntDisable(m_uartBase, UART_INT_TX);
        }
	}
}

void begin(void)
{
	m_portNum = 0;
	m_uartBase = 0;

	TxFifo.inptr = TxFifo.outptr = RxFifo.inptr = RxFifo.outptr = 0;

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    ROM_IntMasterEnable();

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //ROM_IntEnable(INT_UART0);
    //ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    m_portNum = 0;
    m_uartBase = g_UARTBase[m_portNum];

	ROM_UARTIntDisable( m_uartBase, 0xFFFFFFFF );
	ROM_UARTIntEnable( m_uartBase, UART_INT_RX | UART_INT_RT );
	ROM_IntEnable( g_UARTInt[m_portNum] );

	ROM_IntMasterEnable();

	// Enable the UART operation
	ROM_UARTEnable( m_uartBase );

}

