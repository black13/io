/*
 * includes.h
 *
 *  Created on: Dec 25, 2013
 *      Author: josburn
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_


/*************************************************************************
* includes.h - Master header file template for star12 projects.
*              This version is for ETec454 Lab1 using the Metrowerks
*              compiler.
*
* Todd Morton 01/05/05
* Modified 01/31/13 by Dan Zumwalt
**************************************************************************
* General type definitions
*************************************************************************/

typedef unsigned char   INT8U;
typedef signed char     INT8S;
typedef unsigned short  INT16U;
typedef signed short    INT16S;
typedef unsigned long   INT32U;
typedef signed long     INT32S;

/*************************************************************************
* General Defined Constants
*************************************************************************/
//#define DEBUG

#define FALSE    0
#define TRUE     1

/*************************************************************************
* General defined macros
*************************************************************************/
#define FOREVER()       while(1)
#define TRAP()          while(1){}

/*************************************************************************
* MCU specific definitions
* Move the comment to use the project MCU, or enter a new MCU header.
*************************************************************************/
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"

/*************************************************************************
* Project Constant and Macro Definitions
*************************************************************************/
#define BIT0            0x01
#define BIT1            0x02
#define BIT2            0x04
#define BIT3            0x08
#define BIT4            0x10
#define BIT5            0x20
#define BIT6            0x40
#define BIT7            0x80
#define GPIO_ALL 		0xff
#define	GPIO_LNIB		0x0f
#define GPIO_HNIB		0xf0

#define CR				0x0A
#define LF				0x0D

#define CRYSTAL_16MHZ (INT32U)16000000

#define SLICE_PER 10
//#define DB_PORT			GPIO_PORTF_BASE
//#define DB_SLICE        GPIO_PIN_1
//#define DB_UART         GPIO_PIN_3
//#define DB_DISPLAY		GPIO_PIN_2

#define BAUD			115200
#define BUFLEN			8

/*************************************************************************
* System Header Files
*************************************************************************/


#endif /* INCLUDES_H_ */
