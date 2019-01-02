/*******************************************************************************
* File name    :	Serial.c
* Function     :	The low level routines for retarget printf function
* Author       : 	coins
* Date         :	2015/05/28
* Version      :    v1.0
* Description  :	
* ModifyRecord :
*******************************************************************************/

/*******************************************************************************
* Include Header Files
*******************************************************************************/
#include <stdint.h>
#include "AS569.h"


/*******************************************************************************
* Local Macro Definition
*******************************************************************************/
//#define	PRT_BAUD_RATE	19200
#define	PRT_BAUD_RATE	57600

#define	PRT_UART0_PORT	0
#define	PRT_UART1_PORT	1
#define	PRT_UART2_PORT	2

#ifndef	PRT_UART_PORT_SEL
#define	PRT_UART_PORT_SEL	PRT_UART0_PORT
#endif

#define	PRT_UART1_PORT_GPA1A2	0
#define	PRT_UART1_PORT_GPA22A23	1
#define	PRT_UART1_PORT_IO_SEL	PRT_UART1_PORT_GPA1A2


#if(PRT_UART_PORT_SEL == PRT_UART0_PORT)	   
#define	PRT_UART	Uart0Regs		
#elif(PRT_UART_PORT_SEL == PRT_UART1_PORT)
#define	PRT_UART	Uart1Regs
#elif(PRT_UART_PORT_SEL == PRT_UART2_PORT)
#define	PRT_UART	Uart2Regs
#else
#error "Undefined UART port!"
#endif
/*******************************************************************************
* Local Type Definition
*******************************************************************************/

/*******************************************************************************
* Local static Variable Declaration
*******************************************************************************/

/*******************************************************************************
* Global Variable Declaration
*******************************************************************************/
extern uint32_t	SystemFrequency;
volatile unsigned char SerInit = 0;


/*******************************************************************************
* Local Functon Declaration
*******************************************************************************/

/*******************************************************************************
* Functon
*******************************************************************************/

/***************************************************************************
* Subroutine:	SER_init
* Function:		to initialize serial port for printf.
* Input:		None
* Output:		None;
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
void SER_init(void)
{
	unsigned int nBaud; 
	UINT8 nPrtUARTx;

	SerInit = 1;

	#if(PRT_UART_PORT_SEL == PRT_UART0_PORT)		   
		ScmRegs.MODCLKCTRL0 |= (1<<ID_UART0);
		ScmRegs.MODRSTCTRL0 &= ~(1<<ID_UART0);	
		ScmRegs.MODRSTCTRL0 |= (1<<ID_UART0);
		GpioRegs.GPAUSE0.bit.GPA3 = MUX_UART0;
		GpioRegs.GPAUSE0.bit.GPA4 = MUX_UART0;
		nPrtUARTx = PRT_UART0_PORT;		
	#elif(PRT_UART_PORT_SEL == PRT_UART1_PORT)
		ScmRegs.MODCLKCTRL0 |= (1<<ID_UART1);
		ScmRegs.MODRSTCTRL0 &= ~(1<<ID_UART1);	
		ScmRegs.MODRSTCTRL0 |= (1<<ID_UART1);
		#if(PRT_UART1_PORT_IO_SEL == PRT_UART1_PORT_GPA1A2)
			GpioRegs.GPAUSE0.bit.GPA1 = MUX_UART1;
			GpioRegs.GPAUSE0.bit.GPA2 = MUX_UART1;
		#elif(PRT_UART1_PORT_IO_SEL == PRT_UART1_PORT_GPA22A23)
			GpioRegs.GPAUSE2.bit.GPA22 = MUX_UART1;
			GpioRegs.GPAUSE2.bit.GPA23 = MUX_UART1;
		#else
			#error "Undefined UART1 port!"
		#endif
		nPrtUARTx = PRT_UART1_PORT;
	#elif(PRT_UART_PORT_SEL == PRT_UART2_PORT)
		ScmRegs.MODCLKCTRL0 |= (1<<ID_UART2);
		ScmRegs.MODRSTCTRL0 &= ~(1<<ID_UART2);	
		ScmRegs.MODRSTCTRL0 |= (1<<ID_UART2);
		GpioRegs.GPAUSE3.bit.GPA26 = MUX_UART2;
		GpioRegs.GPAUSE3.bit.GPA27 = MUX_UART2;
		nPrtUARTx = PRT_UART2_PORT;
	#else
	   	#error "Undefined UART port!"
	#endif
	nBaud = (SCM_GetUartClk(nPrtUARTx)+(8*PRT_BAUD_RATE))/(16*PRT_BAUD_RATE); 	
	PRT_UART.LCR.all = 0x83;	// 8bit/1stop/no parity
	PRT_UART.RBR_THR_DLL = (unsigned char)nBaud;
	PRT_UART.IER_DLH = (unsigned char)(nBaud>>8);
	PRT_UART.LCR.all = 0x03;	// 8bit/1stop/no parity
  	return;
}

/***************************************************************************
* Subroutine:	sendchar
* Function:		Write character to Serial Port
* Input:		int c -- the character to be written
* Output:		return the character;
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
int sendchar(int c)
{
	if(SerInit==0)
	{
		SER_init();
	}

	PRT_UART.RBR_THR_DLL = c;
	while(PRT_UART.LSR.bit.TEMT==0);

  	return (c);
}

/***************************************************************************
* Subroutine:	getkey
* Function:		Read character from Serial Port   (blocking read)
* Input:		None
* Output:		return the character received;
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
int getkey (void) 
{
	int c;
	if(SerInit==0)
	{
		SER_init();
	}
	while(PRT_UART.LSR.bit.DR==0);
	c = PRT_UART.RBR_THR_DLL;

   	return c;
}

/***************************************************************************
* Subroutine:	ExitSimulate
* Function:		Exit Simulate for IC Design
* Input:		None
* Output:		None
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
void ExitSimulate(void)
{
	#if(SIMULATE_FOR_IC)
	unsigned int i = 1000;
	while(i--);
	#endif	
}


