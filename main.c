//*****************************************************************************
//
// io.c - Simple io world example.
//
// This is best run with the Teraterm terminal program. This can be found at
// http://ttssh2.sourceforge.jp/index.html.en.
// The icid dirvers need to be installed ww.ti.com/tool/stellaris_icdi_drivers.
// The aims of this demo:
// is there an equivalent of the following : ctypes stdlib and stdio.
//*****************************************************************************

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
#include "uart.h"

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
//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void UARTIntHandler(void)
{
    unsigned long ulStatus;
    u_char c;
    ulStatus = ROM_UARTIntStatus(UART0_BASE, true);
    ROM_UARTIntClear(UART0_BASE, ulStatus);

    while(ROM_UARTCharsAvail(UART0_BASE))
    {
    	c = ROM_UARTCharGetNonBlocking(UART0_BASE);
    	g_ucbuffer[g_lindex++ % BUFFER_SIZE] = c;
        //ROM_UARTCharPutNonBlocking(UART0_BASE,ROM_UARTCharGetNonBlocking(UART0_BASE));
    	ROM_UARTCharPutNonBlocking(UART0_BASE,c);

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
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
    }
}

int main(void)
{
	//set the possition of the buffer;
	g_lindex = 0;

    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    ROM_IntMasterEnable();

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /*
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
	*/
    UART_Init();

    while(1)
    {
    	//lets see if we stop
    }
}
