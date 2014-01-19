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

#include "uart.h"
#include "fifo.h"

extern fifo_t *pg_fifo;
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

void morse (unsigned char c);

int main(void)
{
	uint8_t c;
	ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    begin();

    while(1)
    {
    	if(!empty_fifo(pg_fifo))
    	{
    		remove_head(pg_fifo,&c);
    		morse(c);
    	}
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        //SysCtlDelay(SysCtlClockGet() / 10 );

    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
    	//SysCtlDelay(SysCtlClockGet() / 20 / 3);
    }
}


