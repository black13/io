#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

typedef unsigned char u_char;
#define BUFFER_SIZE 16
u_char g_ucbuffer[BUFFER_SIZE];
unsigned long g_lindex;

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

void UARTIntHandler(void)
{
    unsigned long ulStatus;
    u_char c;
    ulStatus = ROM_UARTIntStatus(UART0_BASE, true);

    ROM_UARTIntClear(UART0_BASE, ulStatus);

    while(ROM_UARTCharsAvail(UART0_BASE))
    {
    	c = ROM_UARTCharGetNonBlocking(UART0_BASE);

    	ROM_UARTCharPutNonBlocking(UART0_BASE,c);
        g_ucbuffer[g_lindex++ % BUFFER_SIZE] = c;
        
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************

void morse (unsigned char c);

int main(void)
{
	u_char c;
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable the GPIO pins for the LED (PF2).  
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);


    while(1)
    {
    	while(g_lindex > 0)
    	{
    		g_lindex = g_lindex-- % BUFFER_SIZE;
    		c = g_ucbuffer[g_lindex];
    		morse(c);
    	}
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        //SysCtlDelay(SysCtlClockGet() / 10 );

    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
    	//SysCtlDelay(SysCtlClockGet() / 20 / 3);
    }
}


