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

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "utils/uartstdio.h"


#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

int main(void)
{
    volatile unsigned long ulLoop;
    long delay=0;
    unsigned char c;
    ROM_FPULazyStackingEnable();

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioInit(0);

    UARTprintf("enter text\n");
    
    //i want to get a string and echo it back to terminal.
    while(1)
    {
    	c = UARTgetc();
    	UARTprintf("%d",(int)c);
    	/*
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        SysCtlDelay(SysCtlClockGet() / 10 / 3);

        delay = SysCtlClockGet() / 10 / 3;

        UARTprintf("%d\n",delay);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
        
        SysCtlDelay(SysCtlClockGet() / 10 / 3);
        */
    }
}
