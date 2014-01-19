/*
 * Based upon code written by Michael M. Dodd.
*/
/*
**   <<< Morse Code Functions >>>
**
** Written by Michael M. Dodd, N4CF, and placed in the public domain.
**
** The morse() function transmits a string in Morse code on the IBM PC's
** speaker.  The speed is set by a program constant (UNIT_TIME).
**
** There are several other functions in this file, all used by morse(),
** and defined ahead of morse() for convenience.
**
** The main() function at the end of the file is a test program to send
** the command-line argument string in morse code.  Enclose multiple
** words in quotes.  Example:  morse "hello, world"
**
** These functions have been compiled and tested in the Small and Large
** memory models using Microsoft C 6.00a.
**
** Modified for ZTC++, TC++, & BC++ by Bob Stout
*/
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

#include <ctype.h>
#define SPACE_MASK (1 << 15)
#define BIT_MASK (0xfe)
#define UNIT_TIME (18)
#define FREQUENCY (1500)

void note_on (int freq)        /* Turn on the tone.  */
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

void note_off (void)           /* Turn off the tone.  */
{
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	  /*
      int pio_word ;

      pio_word = inp (PIO) ;
      outp (PIO, pio_word & TONE_OFF) ;
      */
}

void pause (unsigned int amount)
{
	/*
      static unsigned int fudge_factor = 0 ;
      unsigned long ul ;
	*/
    SysCtlDelay(SysCtlClockGet() / (1000 * 3));
}

void send_dot (void)           /* Send a dot and a space.  */
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
	//SysCtlDelay(SysCtlClockGet() / (1000 * 3));
	SysCtlDelay(900000);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	SysCtlDelay(500000);
	//note_on (FREQUENCY) ;
	//pause (UNIT_TIME) ;
	//note_off () ;
	ROM_UARTCharPutNonBlocking(UART0_BASE,'.');
	ROM_UARTCharPutNonBlocking(UART0_BASE,' ');
	pause (UNIT_TIME) ;
}

void send_dash (void)          /* Send a dash and a space.  */
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
	//SysCtlDelay(SysCtlClockGet() / (1000 * 3));
	SysCtlDelay(1000000);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
	SysCtlDelay(500000);
	//note_on (FREQUENCY) ;
	//pause (UNIT_TIME * 3) ;
	//note_off () ;
	ROM_UARTCharPutNonBlocking(UART0_BASE,'_');
	ROM_UARTCharPutNonBlocking(UART0_BASE,' ');
	pause (UNIT_TIME) ;
}

void ltr_space (void)          /* Produce a letter space.  */
{
	ROM_UARTCharPutNonBlocking(UART0_BASE,0x0A);
	ROM_UARTCharPutNonBlocking(UART0_BASE,0x0D);
    pause (UNIT_TIME * 2) ;
}

void word_space (void)         /* Produce a word space.  */
{
      pause (UNIT_TIME * 4) ;
}

void morse (unsigned char ch)
{  /*--- MORSE CODE FUNCTION ---*/

      unsigned int c ;
      static unsigned int codes [64] = {
            SPACE_MASK,                      /* Entry 0 = space (0x20)  */
            0, 0, 0, 0, 0, 0, 0, 0,          /* ! " # $  % & " (  */
            0, 0, 0, 115, 49, 106, 41,       /* ) * + , - . /     */
            63, 62, 60, 56, 48, 32, 33, 35,  /* 0 1 2 3 4 5 6 7   */
            39, 47, 0, 0, 0, 0, 0, 76,       /* 8 9 : ; < = > ?   */
            0, 6, 17, 21, 9, 2, 20, 11,      /* @ A B C D E F G   */
            16, 4, 30, 13, 18, 7, 5, 15,     /* H I J K L M N O   */
            22, 27, 10, 8, 3, 12, 24, 14,    /* P Q R S T U V W   */
            25, 29, 19                       /* X Y Z  */
            } ;

      pause (0) ;                  /* Calibrate pause() function.  */

      c = toupper (ch) ;          /* No lower-case Morse characters.  */
      ROM_UARTCharPutNonBlocking(UART0_BASE,c);
      c -= ' ' ;                 /* Adjust for zero-based table.  */

      if (c > 58)       /* If out of range, ignore it.  */
         return;

      c = codes[c] ;             /* Look up Morse pattern from table. */

      if (c & SPACE_MASK)        /* If the space bit is set..  */
      {                          /* ..send a word space and go on.  */
    	  word_space () ;
    	  	  return;
      }

      while (c & BIT_MASK)       /* Transmit one character.  */
      {  /*--- TRANSMIT EACH BIT ---*/
    	  if (c & 1)
    		  send_dash () ;
          else
        	  send_dot () ;

                  c >>= 1 ;
      }  /*--- TRANSMIT EACH BIT ---*/

      ltr_space () ;             /* Send a space following character. */

     /*--- TRANSMIT COMPLETE STRING ---*/

}  /*--- MORSE CODE FUNCTION ---*/
