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
//#include "uart.h"
#include "debug_uart.h"

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//typedef unsigned char u_char;
//#define BUFFER_SIZE 16
//u_char g_ucbuffer[BUFFER_SIZE];
//unsigned long g_lindex;
//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
/*
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


        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
}
*/

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

void Init_Sys(void)
{
    ROM_FPUEnable();
    // enable lazy stacking if FPU is to be used in interrupt handlers, or multiple tasks.
//    ROM_FPULazyStackingEnable();

    // 16 MHz clock directly from the crystal
//    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // System clock frequency is 400 MHz/(2*SYSDIV)
    // 50 MHz clock off PLL
//    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Max speed: 80 MHz clock off PLL
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}
int main(void)
{
    Init_Sys();
    ROM_IntMasterEnable();
    DBG_Init();

    dbg_putstr("Ready:\r\n");
    while(1)
    {
    	char cmdbfr[16];
    	if(dbg_getline_nb(cmdbfr,16) > 0)
    	{
    		//dbg_printf("\r\nEntered: %s\r\n", cmdbfr);
    		dbg_putstr_nb(cmdbfr);
    	}
    }
}
