/***************************************************************************
* File name    :	Uart.c
* Function     :	The functions for UART
* Author       : 	Leixj
* Date         :	2012.07.11
* Version      :    v1.0
* Description  :    UART Hardware Abstract Layer compatible with AS569
* ModifyRecord :
****************************************************************************/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "AS569.h"

/**************************************************************************
* Local Macro Definition
***************************************************************************/


/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static STRUCT_UART_FIFO gUartTxFIFO[UART_NUM];	// the global for UART transmitting
static STRUCT_UART_FIFO gUartRxFIFO[UART_NUM];	// the global for UART receiving
static const IRQn_Type gUartxIRQn[UART_NUM] = {UART0_IRQn, UART1_IRQn, UART2_IRQn};
static union UART_FCR_REG gUART_FCR[UART_NUM];
static union UART_IER_REG gUART_IER[UART_NUM];

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
#if(UART_DMA)
static UINT8 gUartTxDmaChn[UART_NUM] = {UART0_IRQn, UART1_IRQn, UART2_IRQn};
static UINT8 gUartRxDmaChn[UART_NUM] = {UART0_IRQn, UART1_IRQn, UART2_IRQn};
#endif

/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/

/***************************************************************************
 * Subroutine:	UART_GetRegs
 * Function:	Get UART register file address
 * Input:		UINT8 UARTx -- 	UART number, should be less than UART_NUM;
 * 								if it is out of range, dead loops.
 * Output:		volatile struct UART_REGS * --	Return UARTx register file
 * 												address
 * Description:	Get UART register file address.
 * Date:		2012.07.11
 * ModifyRecord:
 * *************************************************************************/
volatile struct UART_REGS * UART_GetRegs(UINT8 UARTx)
{
    switch(UARTx)
	{
		case UART0:
			return &Uart0Regs;
		case UART1:
			return &Uart1Regs;
		case UART2:
			return &Uart2Regs;
		default:
			return NULL;
	}
}

/****************************************************************************
 * Subroutine:	UART_PortInit
 * Function:	UART_Port initialization  
 * Input:       UART_MAP mPortMap-- Select which group of GPIO port as UART
 *								UART0_MAP_A3_A4,	//A3--RXD, A4--TXD, A1--CTS, A2--RTS 
 *								UART1_MAP_A1_A2,	//A1--RXD, A2--TXD, A5--CTS, A6--RTS 
 *								UART1_MAP_A22_A23,	//A22--RXD, A23--TXD, A26--CTS, A27--RTS 
 *								UART2_MAP_A26_A27,	//A26--RXD, A27--TXD, A24--CTS, A25--RTS
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2015/08/28
 * ModifyRecord:
***************************************************************************/
UINT8 UART_PortInit(UART_MAP mPortMap)
{
    switch(mPortMap)
	{
		case UART0_MAP_A3_A4:
			GpioRegs.GPAUSE0.bit.GPA3 = MUX_UART0;	 //Select RxD pin for UART0
			GpioRegs.GPAUSE0.bit.GPA4 = MUX_UART0;	 //Select TxD pin for UART0	
	        #if(UART_AFCE)
			GpioRegs.GPAUSE0.bit.GPA1 = MUX_UART0;	 //Select CTS pin for UART0
			GpioRegs.GPAUSE0.bit.GPA2 = MUX_UART0;	 //Select RTS pin for UART0	
			#endif
			break;				
		case UART1_MAP_A1_A2:
			GpioRegs.GPAUSE0.bit.GPA1 = MUX_UART1;	  //Select RxD pin for UART1
			GpioRegs.GPAUSE0.bit.GPA2 = MUX_UART1;	  //Select TxD pin for UART1
			#if(UART_AFCE)			       	
			GpioRegs.GPAUSE0.bit.GPA5 = MUX_UART1;	  //Select CTS pin for UART1
			GpioRegs.GPAUSE0.bit.GPA6 = MUX_UART1;	  //Select RTS pin for UART1
			#endif
			break;
		case UART1_MAP_A22_A23:
			GpioRegs.GPAUSE2.bit.GPA22 = MUX_UART1;	  //Select RxD pin for UART1
			GpioRegs.GPAUSE2.bit.GPA23 = MUX_UART1;	  //Select TxD pin for UART1			
			#if(UART_AFCE)
			GpioRegs.GPAUSE3.bit.GPA26 = MUX_UART1;	  //Select CTS pin for UART1
			GpioRegs.GPAUSE3.bit.GPA27 = MUX_UART1;	  //Select RTS pin for UART1
			#endif
		case UART2_MAP_A26_A27:		
	       	GpioRegs.GPAUSE3.bit.GPA26 = MUX_UART2;	 //Select RxD pin for UART2
			GpioRegs.GPAUSE3.bit.GPA27 = MUX_UART2;	 //Select TxD pin for UART2	
			#if(UART_AFCE)	
	        GpioRegs.GPAUSE3.bit.GPA24 = MUX_UART2;	 //Select CTS pin for UART2
			GpioRegs.GPAUSE3.bit.GPA25 = MUX_UART2;	 //Select RTS pin for UART2	
        	#endif
			break;
		default:
			return RT_FAIL;
	}
	return RT_OK;	
}

/***************************************************************************
 * Subroutine:	UART_Init
 * Function:	UART initialization
 * Input:		UINT8 UARTx -- 		UART number, UART0/1/2
 * 				UART_CLK_SRC mSrc--	UART clk source,
 *									UART_CLK_SRC_FCLK, UART_CLK_SRC_XTAL, UART_CLK_SRC_HFROSC_DIV1,
 *									UART_CLK_SRC_HFROSC_DIV2, UART_CLK_SRC_HFROSC_DIV4, UART_CLK_SRC_HFROSC_DIV8	  	
 *				UART_MAP mPortMap-- Select which group of GPIO port as UART
 *									UART0_MAP_A3_A4,	//A3--RXD, A4--TXD, A1--CTS, A2--RTS 
 *									UART1_MAP_A1_A2,	//A1--RXD, A2--TXD, A5--CTS, A6--RTS 
 *									UART1_MAP_A22_A23,	//A22--RXD, A23--TXD, A26--CTS, A27--RTS 
 *									UART2_MAP_A26_A27,	//A26--RXD, A27--TXD, A24--CTS, A25--RTS
 *				STRUCT_UART_FORMAT sFrame --
 * 									UART frame format;
 * 				STRUCT_DATA_BUF *pRxBuf --
 * 									UART receive buffer, to specify the buffer and size for receive,
 * 									if pRxBuf is NULL, receiver will be disabled and its pin will be used
 * 									as default (GPIO)
 * 				STRUCT_DATA_BUF *pTxBuf --
 * 									UART transmit buffer, to specify the buffer and size for transmit,
 *									if pRxBuf is NULL and sFrame.TxMode == UART_TX_INT, receiver will be disabled and its pin will be used
 * 									as default (GPIO)
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 UART_Init(UINT8 UARTx, UART_CLK_SRC mSrc, UART_MAP mPortMap, STRUCT_UART_FORMAT sFrame, \
	STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf)
{
	volatile struct UART_REGS * pUart;
	UINT32 nBaudRate;
	if(UART_PortInit(mPortMap) != RT_OK) return RT_FAIL;
	switch(UARTx)
	{
		case UART0:
			SCM_DisableModClk(ID_UART0);// disable UART0 clock
			SCM_KeepModRst(ID_UART0);// reset
			SCM_EnableModClk(ID_UART0);// enable UART0 clock
			SCM_ReleaseModRst(ID_UART0);// Relinquish from reset
			break;
		case UART1:
   			SCM_DisableModClk(ID_UART1);// disable UART1 clock
			SCM_KeepModRst(ID_UART1);// reset
			SCM_EnableModClk(ID_UART1);// enable UART1 clock
			SCM_ReleaseModRst(ID_UART1);// Relinquish from reset
			break;
		case UART2:
   			SCM_DisableModClk(ID_UART2);// disable UART2 clock
			SCM_KeepModRst(ID_UART2);// reset
			SCM_EnableModClk(ID_UART2);// enable UART2 clock
			SCM_ReleaseModRst(ID_UART2);// Relinquish from reset
			break;
		default:return RT_FAIL;
	}
	gUART_FCR[UARTx].all=0;
	gUART_IER[UARTx].all=0;
	// 2. select UART register file
	pUart = UART_GetRegs(UARTx);
	SCM_SetUartClk(UARTx, mSrc);	
	nBaudRate = (SCM_GetUartClk(UARTx))/(16*sFrame.BaudRate);

	pUart->LCR.bit.DLAB	= 0x1;	
	pUart->RBR_THR_DLL	= nBaudRate;
	pUart->IER_DLH		= (nBaudRate>>8);
	pUart->LCR.bit.DLAB	= 0x0;

	switch(sFrame.CharBit)
	{
		case CHAR_5BIT:
			pUart->LCR.bit.DLS	= 0x0;
			break;
		case CHAR_6BIT:
			pUart->LCR.bit.DLS	= 0x1;
			break;
		case CHAR_7BIT:
			pUart->LCR.bit.DLS	= 0x2;
			break;
		case CHAR_8BIT:
			pUart->LCR.bit.DLS	= 0x3;
			break;
		default:
			return RT_FAIL;
	}
	switch(sFrame.StopBit)
	{
		case ONE_STOP:
			pUart->LCR.bit.STOP = 0;	
			break;
		case TWO_STOP://if DLS = 0,1.5 stop bits,else 2stop 
			pUart->LCR.bit.STOP = 1;
			break;
		default:
			return RT_FAIL;
	}
	switch(sFrame.Parity)
	{
		case NONE:
			pUart->LCR.bit.PEN = 0;
			break;
		case ODD:
			pUart->LCR.bit.EPS = 0;
			pUart->LCR.bit.PEN = 1;			
			break;
		case EVEN:	
			pUart->LCR.bit.EPS = 1;
			pUart->LCR.bit.PEN = 1;		
			break;
		case MARK:
			pUart->LCR.bit.SP = 1;
			pUart->LCR.bit.EPS = 0;
			pUart->LCR.bit.PEN = 1;			
			break;
		case SPACE:
			pUart->LCR.bit.SP = 1;	
			pUart->LCR.bit.EPS = 1;
			pUart->LCR.bit.PEN = 1;		
			break;
		default:
			return RT_FAIL;
	}
	if(sFrame.ShadowEn)
	{
		pUart->SFE = 1;	//Shadow  FIFO Enable
		pUart->SRT = 0;	//Shadow  Recieve FIFO Tigger
		pUart->STET = 0;	//Shadow  Transmit FIFO Tigger
		pUart->SDMAM = 1;	//Shadow  DMA Mode
		pUart->SRTS = 1;	//Shadow  RTS
			
	}
	// FIFO Enable	, Transmit and receive FIFO depths of 32Bytes
//	pUart->IIR_FCR |= (1<<0);
	gUART_FCR[UARTx].all |= (1<<0);
	pUart->IIR_FCR=gUART_FCR[UARTx].all;
	//if FCR[3]=0, DMA Mode 0: Signle-byte transfer
	//if FCR[3]=1, DMA Mode 1: Multi-byte  transfer
//	pUart->IIR_FCR |= (1<<3);
    gUART_FCR[UARTx].all |= (1<<3);
	pUart->IIR_FCR=gUART_FCR[UARTx].all;  	
//	pUart->IIR_FCR &= (1<<3);
	switch(sFrame.Mode)
	{
		case UART_RX_INT:
			gUartRxFIFO[UARTx].pFFData = pRxBuf->pBuf;		// to initialize receive FIFO
			gUartRxFIFO[UARTx].FFDepth = pRxBuf->Size;
			gUartRxFIFO[UARTx].FFValidSize = 0;
			gUartRxFIFO[UARTx].FFInOffset = 0;
			gUartRxFIFO[UARTx].FFOutOffset = 0;	
			gUartRxFIFO[UARTx].gUartxIntMode = 1;
	
//			pUart->IER_DLH |= (1<<0);//Enable Recv Data Available Interrupt
			gUART_IER[UARTx].all |= (1<<0);
			pUart->IER_DLH=gUART_IER[UARTx].all;

			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			break;

		case UART_TX_INT:
			gUartTxFIFO[UARTx].pFFData = pTxBuf->pBuf;		// to initialize transmit FIFO
			gUartTxFIFO[UARTx].FFDepth = pTxBuf->Size;
			gUartTxFIFO[UARTx].FFValidSize = 0;
			gUartTxFIFO[UARTx].FFInOffset = 0;
			gUartTxFIFO[UARTx].FFOutOffset = 0;	
			gUartRxFIFO[UARTx].gUartxIntMode = 2;
			
//			pUart->IER_DLH |= (1<<7);//THRE Intterupt Enable
//			pUart->IER_DLH |= (1<<1);//Enable Transmit Data Available Interrupt
			gUART_IER[UARTx].all |= (1<<7);
			gUART_IER[UARTx].all |= (1<<1);
			pUart->IER_DLH=gUART_IER[UARTx].all;
			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			break;

		case UART_BOTH_INT:
			gUartRxFIFO[UARTx].pFFData = pRxBuf->pBuf;		// to initialize receive FIFO
			gUartRxFIFO[UARTx].FFDepth = pRxBuf->Size;
			gUartRxFIFO[UARTx].FFValidSize = 0;
			gUartRxFIFO[UARTx].FFInOffset = 0;
			gUartRxFIFO[UARTx].FFOutOffset = 0;
	
			gUartTxFIFO[UARTx].pFFData = pTxBuf->pBuf;		// to initialize transmit FIFO
			gUartTxFIFO[UARTx].FFDepth = pTxBuf->Size;
			gUartTxFIFO[UARTx].FFValidSize = 0;
			gUartTxFIFO[UARTx].FFInOffset = 0;
			gUartTxFIFO[UARTx].FFOutOffset = 0;
			gUartRxFIFO[UARTx].gUartxIntMode = 3;
			
//			pUart->IER_DLH |= (1<<7);//THRE Intterupt Enable
//			pUart->IER_DLH |= (1<<0);//Enable Recv Data Available Interrupt	
//			pUart->IER_DLH |= (1<<1);//Enable Transmit Data Available Interrupt
			gUART_IER[UARTx].all |= (1<<7);
			gUART_IER[UARTx].all |= (1<<0);
			gUART_IER[UARTx].all |= (1<<1);
			pUart->IER_DLH=gUART_IER[UARTx].all;
			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			break;
		
		default: break;
	}
	#if(UART_AFCE)
		pUart->MCR.bit.AFCE = 1;	//Auto Flow Control Enable
		pUart->MCR.bit.RTS = 1;	//set RTS High	(inactive)
		pUart->MCR.bit.SIRE = 0;	//Disable SIR mode
//		pUart->IIR_FCR |= (3<<6);//to set RECV FIFO Level : 2Bytes less than full
	    gUART_FCR[UARTx].all |= (3<<6);
		pUart->IIR_FCR=gUART_FCR[UARTx].all; 
	#endif

	return RT_OK;
}

/***************************************************************************
 * Subroutine:	UART_GetRxFFValidSize
 * Function:	to get UART receive buffer valid data number
 * Input:		UINT8 UARTx -- 	UART number, UARTx for SY09;
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT16 UART_GetRxFFValidSize(UINT8 UARTx)
{
	UINT16 nLen;
	NVIC_DisableIRQ(gUartxIRQn[UARTx]);
	nLen=gUartRxFIFO[UARTx].FFValidSize;
	NVIC_EnableIRQ(gUartxIRQn[UARTx]);
	return nLen;
} 

/***************************************************************************
 * Subroutine:	UART_SendByte
 * Function:	UART send byte
 * Input:		UINT8 UARTx -- 		UART number, UART0/1/2;
 * 				UINT8 bData --		The value to be transfered
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2010.05.26
 * ModifyRecord:
 * *************************************************************************/
UINT8 UART_SendByte(UINT8 UARTx, UINT8 bData)
{
	volatile struct UART_REGS * pUart;
	UINT32  lWaitTime = 10000;
	UINT32  i = 0;

	if(UARTx >= UART_NUM) return RT_FAIL;
	pUart = UART_GetRegs(UARTx);
		
	if((gUartRxFIFO[UARTx].gUartxIntMode==2)||(gUartRxFIFO[UARTx].gUartxIntMode==3))//if transmit interrupt enabled	
	{		
		NVIC_DisableIRQ(gUartxIRQn[UARTx]);   // disable UART0 interrupt

//		while(gUartTxFIFO[UARTx].FFValidSize >= gUartTxFIFO[UARTx].FFDepth)
//		{
////			pUart->IER_DLH |= (1<<1);//Enable Transmit Data Available Interrupt
//			gUART_IER[UARTx].all |= (1<<1);
//			pUart->IER_DLH=gUART_IER[UARTx].all;
//
//			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
//		}
		// write to TX FIFO buffer
		if(gUartTxFIFO[UARTx].FFValidSize < gUartTxFIFO[UARTx].FFDepth)		// buffer is not full
		{
			gUartTxFIFO[UARTx].pFFData[gUartTxFIFO[UARTx].FFInOffset++] = bData;
			if(gUartTxFIFO[UARTx].FFInOffset >= gUartTxFIFO[UARTx].FFDepth)
				gUartTxFIFO[UARTx].FFInOffset = 0;
			gUartTxFIFO[UARTx].FFValidSize++;	// valid size + 1
//			pUart->IER_DLH |= (1<<1);//Enable Transmit Data Available Interrupt
			gUART_IER[UARTx].all |= (1<<1);
			pUart->IER_DLH=gUART_IER[UARTx].all;		     
			NVIC_EnableIRQ(gUartxIRQn[UARTx]);		
			return RT_OK;
		}
		else
		{
			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			return RT_FAIL;
		}				
	}
	else
	{
		while(pUart->USR.bit.TFNF == 0);//wait transmit is not full
		pUart->RBR_THR_DLL = bData;
		while(pUart->LSR.bit.TEMT==0)
		{
			if(++i == lWaitTime) return RT_FAIL;
		}	
	}
	return RT_OK;	
}

/**************************************************************************
 * Subroutine:	UART_SendBuf
 * Function:	UART send buffer
 * Input:		UINT8 UARTx --		UART number, UART0/1/2;
 * 				UINT8 *pBuf --		The buffer to be sent
 * 				UINT16 iLen --		The length of buffer
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2009.07.31
 * ModifyRecord:
 * **********************************************************************/
UINT8 UART_SendBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen)
{
	UINT8 nRet = RT_OK;

	while(iLen--)
	{
		nRet = UART_SendByte(UARTx, *pBuf++);
		if(nRet != RT_OK) break;	
	}
	return nRet;
}

/***************************************************************************
 * Subroutine:	UART_ReceByte
 * Function:	UART receive byte
 * Input:		UINT8 UARTx -- 		UART number, UART0/1/2 ;
 * 				UINT8 *pData --		The value to be received (pointer)
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description: the received data is from receive buffer
 * Date:		2009.04.03
 * ModifyRecord:
 * ************************************************************************/
UINT8 UART_ReceByte(UINT8 UARTx, UINT8* bData)
{
	volatile struct UART_REGS * pUart;
	if((gUartRxFIFO[UARTx].gUartxIntMode==1) || (gUartRxFIFO[UARTx].gUartxIntMode==3))//2016/5/16							
	{		
		if(gUartRxFIFO[UARTx].FFValidSize != 0)	// have data in FIFO
		{
			*(bData) = gUartRxFIFO[UARTx].pFFData[gUartRxFIFO[UARTx].FFOutOffset++];
			if(gUartRxFIFO[UARTx].FFOutOffset == gUartRxFIFO[UARTx].FFDepth)
				gUartRxFIFO[UARTx].FFOutOffset = 0;
			NVIC_DisableIRQ(gUartxIRQn[UARTx]);
			gUartRxFIFO[UARTx].FFValidSize--;	// valid size - 1
			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			return RT_OK;
		}
		else
		{
			return RT_FAIL;
		}			
	}
	else
	{
		pUart = UART_GetRegs(UARTx);
		if(pUart->USR.bit.RFF)
		__nop();
		while(pUart->LSR.bit.DR==0);
		*bData = pUart->RBR_THR_DLL;
		return RT_OK;	
	}
}

/**************************************************************************
 * Subroutine:	UART_ReceBuf
 * Function:	UART receive buffer
 * Input:		UINT8 UARTx -- 		UART number, only UART0/1/2;
 * 				UINT8 *pBuf --		The buffer used for storing received data
 * 				UINT16 iLen --		The length will be receive
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL(no enough data)
 * Description:
 * Date:		2009.07.31
 * ModifyRecord:
 * ************************************************************************/
UINT8 UART_ReceBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen)
{
	if((gUartRxFIFO[UARTx].gUartxIntMode==1) || (gUartRxFIFO[UARTx].gUartxIntMode==3))//2016/5/16	
	{	
		if(gUartRxFIFO[UARTx].FFValidSize >= iLen)	// have data in FIFO
		{
			while(iLen--)
			{
				*pBuf++ = gUartRxFIFO[UARTx].pFFData[gUartRxFIFO[UARTx].FFOutOffset++];
				if(gUartRxFIFO[UARTx].FFOutOffset == gUartRxFIFO[UARTx].FFDepth)
					gUartRxFIFO[UARTx].FFOutOffset = 0;
				NVIC_DisableIRQ(gUartxIRQn[UARTx]);
				gUartRxFIFO[UARTx].FFValidSize--;	// valid size - 1
				NVIC_EnableIRQ(gUartxIRQn[UARTx]);		
			}
			return RT_OK;
		}
		else
		{
			return RT_FAIL;
		}			
	}
	else
	{
		while(iLen--)
		{
			if(RT_OK != UART_ReceByte(UARTx,pBuf++))
				return RT_FAIL;
		}	
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	UART_DmaInit
 * Function:	UART initialization	with DMA
 * Input:		UINT8 UARTx -- 		UART number, UART0/1/2 
 *				UART_CLK_SRC mSrc--	UART clk source,
 *									UART_CLK_SRC_FCLK, UART_CLK_SRC_XTAL, UART_CLK_SRC_HFROSC_DIV1,
 *									UART_CLK_SRC_HFROSC_DIV2, UART_CLK_SRC_HFROSC_DIV4, UART_CLK_SRC_HFROSC_DIV8	 
 *				UART_MAP mPortMap-- Select which group of GPIO port as UART
 *									UART0_MAP_A3_A4,	//A3--RXD, A4--TXD, A1--CTS, A2--RTS 
 *									UART1_MAP_A1_A2,	//A1--RXD, A2--TXD, A5--CTS, A6--RTS 
 *									UART1_MAP_A22_A23,	//A22--RXD, A23--TXD, A26--CTS, A27--RTS 
 *									UART2_MAP_A26_A27,	//A26--RXD, A27--TXD, A24--CTS, A25--RTS
 * 				STRUCT_UART_FORMAT sFrame --
 * 									UART frame format; 
 * 				STRUCT_DATA_BUF *pRxBuf --
 * 									UART receive buffer, to specify the buffer and size for receive,
 * 									if pRxBuf is NULL, receiver will be disabled and its pin will be used
 * 									as default (GPIO)
 * 				STRUCT_DATA_BUF *pTxBuf --
 * 									UART transmit buffer, to specify the buffer and size for transmit,
 *									if pRxBuf is NULL and sFrame.TxMode == UART_TX_INT, receiver will be disabled and its pin will be used
 * 									as default (GPIO)
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 UART_DmaInit(UINT8 UARTx, UART_CLK_SRC mSrc, UART_MAP mPortMap, STRUCT_UART_FORMAT sFrame, \
	UINT8 nTxDmaChn, UINT8 nRxDmaChn)
{
	UINT8 nRet;
	volatile struct UART_REGS * pUart;
	
	STRUCT_DMA_DEV sSrcDevTx = {0x20000000, INC, BYTE_1, BURST_1, HARD};
	STRUCT_DMA_DEV sDestDevTx = {0, FIX, BYTE_1, BURST_1, HARD};
	STRUCT_DMA_DEV sSrcDevRx = {0, FIX, BYTE_1, BURST_1, HARD};
	STRUCT_DMA_DEV sDestDevRx = {0x20000000, INC, BYTE_1, BURST_1, HARD};
	
	nRet = UART_Init(UARTx, mSrc, mPortMap, sFrame, NULL, NULL);

	if(nRet!=RT_OK) return nRet;

	pUart = UART_GetRegs(UARTx);

	sDestDevTx.nAddr = 	(UINT32)&(pUart->RBR_THR_DLL);
	sSrcDevRx.nAddr = 	(UINT32)(&(pUart->RBR_THR_DLL))+0x4;	

	gUartTxDmaChn[UARTx] = nTxDmaChn;
	gUartRxDmaChn[UARTx] = nRxDmaChn;

	nRet = DMA_Init(gUartTxDmaChn[UARTx], gUartTxDmaChn[UARTx], &sSrcDevTx, &sDestDevTx, 0, NULL);	 //轮询模式
	if(nRet!=RT_OK) return nRet;
	nRet = DMA_Init(gUartRxDmaChn[UARTx], gUartRxDmaChn[UARTx], &sSrcDevRx, &sDestDevRx, 0, NULL);
	if(nRet!=RT_OK) return nRet;
	return RT_OK;
}

/**************************************************************************
 * Subroutine:	UART_DmaSend
 * Function:	UART send buffer  with DMA
 * Input:		UINT8 UARTx -- 		UART number, only UART0/1/2;
 * 				UINT8 *pData --		The buffer to be sent
 * 				UINT32 nLen --		The length of buffer
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2009.07.31
 * ModifyRecord:
 * **********************************************************************/
UINT8 UART_DmaSend(UINT8 UARTx, UINT8 *pData, UINT32 nLen)
{
	UINT8 nRet;
	UINT32 nSrcAddr;
	UINT32 nDmaTs;
	volatile struct UART_REGS * pUart;

	pUart = UART_GetRegs(UARTx);

	nSrcAddr = (UINT32)pData;
	while(nLen)
	{
		nDmaTs = (nLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : nLen;

		DMA_Start(gUartTxDmaChn[UARTx], nSrcAddr, (UINT32)&(pUart->RBR_THR_DLL), nDmaTs);
		nRet = DMA_CheckDone(gUartTxDmaChn[UARTx], 0xFFFFFFFF);
		if(nRet!=RT_OK) return nRet;

		if(pData!=NULL) nSrcAddr+=nDmaTs;
		nLen -= nDmaTs;

	}
	return RT_OK;
}

/**************************************************************************
 * Subroutine:	UART_DmaRece
 * Function:	UART receive buffer with DMA
 * Input:			UINT8 bUsartNum -- 	USART number
 * 				UINT8 *pData --		The buffer used for storing received data
 * 				UINT32  nLen --		The length will be receive
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL(no enough data)
 * Description:
 * Date:		2009.07.31
 * ModifyRecord:
 * ************************************************************************/
UINT8 UART_DmaRece(UINT8 UARTx, UINT8 *pData, UINT32 nLen)
{
	UINT8 nRet;
	UINT32 nDstAddr;
	UINT32 nDmaTs;
	volatile struct UART_REGS * pUart;

	pUart = UART_GetRegs(UARTx);

	nDstAddr = (UINT32)pData;
	while(nLen)
	{
		while(pUart->LSR.bit.DR==0);	//data ready ,at least one character in the PBR or the receiver FIFO 
		nDmaTs = (nLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : nLen;
		DMA_Start(gUartRxDmaChn[UARTx], (UINT32)&(pUart->RBR_THR_DLL), nDstAddr, nDmaTs);
		nRet = DMA_CheckDone(gUartRxDmaChn[UARTx], 0xFFFFFFFF);
		if(nRet!=RT_OK) return nRet;
		if(pData!=NULL) nDstAddr+=nDmaTs;
		nLen -= nDmaTs;
	}
	return RT_OK;
} 
			  
/***************************************************************************
 * Subroutine:	UARTx_IntProc
 * Function:	UARTx interrupt process
 * Input:		UINT8 UARTx --		UART number, UART0/1/2 ;
 * Output:		none
 * Description: to process all UART interrupt
 * Date:		2010.05.27
 * ModifyRecord:
 * ************************************************************************/
void UARTx_IntProc(UINT8 UARTx)
{
	union UART_LSR_REG IntFlag;
	volatile struct UART_REGS * pUart;
	pUart = UART_GetRegs(UARTx);
	
	IntFlag = pUart->LSR;
	if(IntFlag.bit.OE || IntFlag.bit.PE)//Overrun error or Framing Error 
	{
		return  ;
	}
		
	//if((pUart->IIR_FCR&0x0F) == 0x4)// 接收到有效数据
	if(IntFlag.bit.DR)
	{
		if(gUartRxFIFO[UARTx].FFValidSize < gUartRxFIFO[UARTx].FFDepth)	// not full
		{
			gUartRxFIFO[UARTx].pFFData[gUartRxFIFO[UARTx].FFInOffset++] = pUart->RBR_THR_DLL;
			if(gUartRxFIFO[UARTx].FFInOffset == gUartRxFIFO[UARTx].FFDepth)
				gUartRxFIFO[UARTx].FFInOffset = 0;
			gUartRxFIFO[UARTx].FFValidSize++;
		}
			
	}
	if((gUartRxFIFO[UARTx].gUartxIntMode==2)||(gUartRxFIFO[UARTx].gUartxIntMode==3))	
	{
		while(pUart->USR.bit.TFNF)	// till FIFO is full
		{
			if(gUartTxFIFO[UARTx].FFValidSize != 0)	// not empty
			{
				pUart->RBR_THR_DLL = gUartTxFIFO[UARTx].pFFData[gUartTxFIFO[UARTx].FFOutOffset++];
				if(gUartTxFIFO[UARTx].FFOutOffset == gUartTxFIFO[UARTx].FFDepth) gUartTxFIFO[UARTx].FFOutOffset = 0;
				gUartTxFIFO[UARTx].FFValidSize--;
			}
			else 
			{
//				pUart->IER_DLH &= ~(1<<1);
				gUART_IER[UARTx].all &= ~(1<<1);
				pUart->IER_DLH=gUART_IER[UARTx].all;
				break;
			}
		}
	}	
}

/***************************************************************************
 * Subroutine:	UART0_Handler
 * Function:	UART0 Interrupt Service Routine
 * Input:		None
 * Output:		none
 * Description: UART0 interrupt processing
 * Date:		2012.10.26
 * ModifyRecord:
 * ************************************************************************/
void UART0_Handler(void)
{
	UARTx_IntProc(UART0);	
}

/***************************************************************************
 * Subroutine:	UART1_Handler
 * Function:	UART1 Interrupt Service Routine
 * Input:		None
 * Output:		none
 * Description: UART1 interrupt processing
 * Date:		2012.10.26
 * ModifyRecord:
 * ************************************************************************/
void UART1_Handler(void)
{
	UARTx_IntProc(UART1);
}

/***************************************************************************
 * Subroutine:	UART2_Handler
 * Function:	UART2 Interrupt Service Routine
 * Input:		None
 * Output:		none
 * Description: UART2 interrupt processing
 * Date:		2012.10.26
 * ModifyRecord:
 * ************************************************************************/
void UART2_Handler(void)
{
	UARTx_IntProc(UART2);
}


