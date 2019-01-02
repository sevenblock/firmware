/***************************************************************************
* File name    :	AS569.h
* Function     :	Header of AS569 
* Author       : 	coins
* Date         :	2014/06/25
* Version      :    v1.0
* Description  :	to include all header files about AS569, such as memory-map-
*					register header, drive
* ModifyRecord :
****************************************************************************/
#ifndef AS569_H_
#define AS569_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "AS569_Device.h"
#include "core_cm0.h"
#include "stdio.h"
#include "string.h"

#include "Scm.h"
#include "Efc.h"
#include "KFlash.h"
#include "SFlash.h"
#include "XFlash.h"
#include "Sci.h"
#include "Sqi.h"
#include "Ssi.h"
//#include "Swps.h"
#include "Gpio.h"
#include "Timer.h"
#include "Spi.h"
#include "Uart.h"
#include "I2c.h"
#include "Iso7816.h"
#include "Vpwm.h"
#include "Crc.h"
#include "Sdsi.h"
#include "Sdi.h" 
#include "Aoss.h"
#include "Fd12m.h"
#include "Usb.h"
#include "Alg.h"
#include "Dma.h"
#include "SFlash.h"
#include "Apsi.h"
#include "Bpss.h"

/**************************************************************************
* Global Macro Definition
***************************************************************************/

#define	FWLIB_VERSION		0x12040112	// Version, BCD code, nn.mm	


// define the function name for old firmware library revision
#define print printf
#define _nop  __nop


/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
/****************************************************************************
 * Subroutine:	FWLIB_GetVersion
 * Function:	to get firmware lib version 
 * Input:		None 
 * Output:		unsigned short	
 * Description:	the version is BCD code, such as 0x0010, is version 0.10 
 * Date:		2014.06.25
 * ModifyRecord:
 * *************************************************************************/
__INLINE unsigned long FWLIB_GetVersion(void)
{
	return (FWLIB_VERSION);
}

#endif /*AS569_H_*/

