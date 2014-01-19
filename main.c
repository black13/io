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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cbuffer.h"
#include "uart.h"
CircularBuffer g_cb;

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif


#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

void UARTIntHandler(void)
{
    unsigned long ulStatus;
    uint8_t c;
    ulStatus = ROM_UARTIntStatus(UART0_BASE, true);

    ROM_UARTIntClear(UART0_BASE, ulStatus);

    while(ROM_UARTCharsAvail(UART0_BASE))
    {
    	c = ROM_UARTCharGetNonBlocking(UART0_BASE);

    	circular_buffer_push(&g_cb,c);
    	//ROM_UARTCharPutNonBlocking(UART0_BASE,c);

        
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        
        //SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
}

void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

void morse (unsigned char c);


int main(void)
{
	uint8_t c;
	circular_buffer_init(&g_cb);
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    /*
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

    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

	*/
    begin();
    while(1)
    {
    	while((c=circular_buffer_pop(&g_cb)) != 0)
    	{
    		//g_lindex = g_lindex-- % BUFFER_SIZE;
    		//c = g_ucbuffer[g_lindex];
    		morse(c);
    	}
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        //SysCtlDelay(SysCtlClockGet() / 10 );

    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
    	//SysCtlDelay(SysCtlClockGet() / 20 / 3);
    }
}


