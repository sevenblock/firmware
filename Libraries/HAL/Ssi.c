/****************************************************************************
* File name    :	Ssi.c
* Function     :	the driver of SSI
* Author       : 	coins
* Date         :	2012/12/21
* Version      :    v1.0
* Description  :
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
// define the SSI hardware FIFO depth
#define	SSI_FIFO_DEPTH		8

// configuration for SSI driver
// Data Frame Size Select
#define	SSI_DFS_8		0 	// DFS: 4 ~ 8 bit
#define	SSI_DFS_16		1	// DFS: 9 ~ 16 bit
#define SSI_DFS_SEL		SSI_DFS_8	
			

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static UINT8 *gRxBuf;		// the SSI receive buffer pointer
static UINT16 gRxBufSize;	// the SSI receive buffer size
static volatile UINT16 gRxSize;	// the SSI received byte number

#if(SSI_DMA_TX)
static UINT8 gSsiTxDmaChn;
static UINT8 *gTxBuf;
static UINT16 gTxBufSize;
static UINT8 gDmaTxDone = 0;
#endif

#if(SSI_DMA_RX)
static UINT8 gSsiRxDmaChn;
static UINT8 gDmaRxDone = 0;
#endif		

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/
#if(SSI_DMA_TX)
void SSI_DmaTxProc(UINT8 nIntType);
#endif

/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	SSI_PortInit
 * Function:	SSI Port initialization  
 * Input:		SSI_MAP mPortMap --	to specify the SSI port map to. 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012/12/21
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_PortInit(SSI_MAP mPortMap)
{
	switch(mPortMap)
	{
		case SSI_MAP_A3_9:
			// to select Pins for SSI signals
			GpioRegs.GPAUSE0.bit.GPA3 = MUX_SSI;		// SSI_SSN	
			GpioRegs.GPAUSE1.bit.GPA9 = MUX_SSI;		// SSI_RXD	
			GpioRegs.GPAUSE0.bit.GPA7 = MUX_SSI;		// SSI_CLK	
			GpioRegs.GPAUSE1.bit.GPA8 = MUX_SSI;		// SSI_TXD
			break;
		case SSI_MAP_A17_20:
			// to select Pins for SSI signals
			GpioRegs.GPAUSE2.bit.GPA20 = MUX_SSI;		// SSI_SSN	
			GpioRegs.GPAUSE2.bit.GPA19 = MUX_SSI;		// SSI_RXD	
			GpioRegs.GPAUSE2.bit.GPA18 = MUX_SSI;		// SSI_CLK	
			GpioRegs.GPAUSE2.bit.GPA17 = MUX_SSI;		// SSI_TXD
			break;
		default:
			return RT_FAIL;
	}
			
	// to enable SSI module and reset all logic
	SCM_DisableModClk(ID_SSI);// disable SSI clock
	SCM_KeepModRst(ID_SSI);// reset
	SCM_EnableModClk(ID_SSI);// enable SSI clock
	SCM_ReleaseModRst(ID_SSI);// Relinquish from reset
//	SsiRegs.SSIENR = 0;		// disable SSI
	return RT_OK;	
}

/****************************************************************************
 * Subroutine:	SSI_SpiSlvInit
 * Function:	SSI initialization for SPI Slave mode 
 * Input:		SSI_MAP mPortMap --	to specify the SSI port map to.
 *				UINT8 nCPOL --	to specify the clock polarity, value 0 or 1
 *				UINT8 nCPHA --	to specify the clock phase, value 0 or 1
 *				UINT8 nDFS --  to specify the data frame size, value 4 ~ 16 
 *				UINT8 *pRxBuf -- to specify the buffer for receiving
 *				UINT16 nBufSize -- to specify the buffer size for receiving
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_SpiSlvInit(SSI_MAP mPortMap, UINT8 nCPOL, UINT8 nCPHA, UINT8 nDFS, \
						UINT8 *pRxBuf, UINT16 nBufSize)
{	
	if(SSI_PortInit(mPortMap) != RT_OK) return RT_FAIL;

	SsiRegs.CTRLR0.bit.Frf = 0;	// Protocol: Motorola SPI
	SsiRegs.CTRLR0.bit.Scpol = nCPOL;  	// Clock Polarity
	SsiRegs.CTRLR0.bit.Scph = nCPHA; 	// Clock Phase
	SsiRegs.CTRLR0.bit.Dfs = nDFS-1;	// Data Frame Size
	SsiRegs.CTRLR0.bit.SlvOe = 0;
	SsiRegs.IMR.all = 0;	
	SsiRegs.CTRLR0.bit.Tmod = 0x0;	// transmit and receive

	if((pRxBuf != NULL) && (nBufSize != 0))	// Receive interrupt used
	{
		gRxBuf = pRxBuf;
		gRxBufSize = nBufSize;
		gRxSize = 0;
		SsiRegs.RXFTLR = 0;		
	}
	SsiRegs.SSIENR = 1;		// enable SSI
	return RT_OK;
}

#if(SSI_DMA_TX)
/****************************************************************************
 * Subroutine:	SSI_SpiSlvInitWithDMA
 * Function:	SSI initialization for SPI Slave mode with DMA transmit 
 * Input:		SSI_MAP mPortMap --	to specify the SSI port map to.
 *				UINT8 nCPOL --	to specify the clock polarity, value 0 or 1
 *				UINT8 nCPHA --	to specify the clock phase, value 0 or 1
 *				UINT8 nDFS --  to specify the data frame size, value 4 ~ 16 
 *				UINT8 *pRxBuf -- to specify the buffer for receiving
 *				UINT16 nBufSize -- to specify the buffer size for receiving
 *				UINT8 nDmaChn -- t0 specify the DMA channel for Tx
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2015.11.03
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_SpiSlvInitWithDMA(SSI_MAP mPortMap, UINT8 nCPOL, UINT8 nCPHA, UINT8 nDFS, \
						UINT8 *pRxBuf, UINT16 nBufSize, UINT8 nDmaChn)
{
	UINT8 nRet;
	#if(SSI_DFS_SEL==SSI_DFS_8)
		STRUCT_DMA_DEV sSrcDevSend = {0x20000000, INC, BYTE_1, BURST_1, HARD};
		STRUCT_DMA_DEV sDestDevSend = {(UINT32)&SsiRegs.DR, FIX, BYTE_1, BURST_1, HARD};	
	#elif(SSI_DFS_SEL==SSI_DFS_16)
	   	STRUCT_DMA_DEV sSrcDevSend = {0x20000000, INC, BYTE_2, BURST_4, HARD};
		STRUCT_DMA_DEV sDestDevSend = {(UINT32)&SsiRegs.DR, FIX, BYTE_2, BURST_4, HARD};
	#endif
	nRet = SSI_SpiSlvInit(mPortMap, nCPOL, nCPHA, nDFS, pRxBuf, nBufSize);
	if(nRet!=RT_OK) return nRet;
	SsiRegs.DMATDLR = 1;
	SsiRegs.DMACR.bit.Tdmae = 1; 	  	//Transmit DMA Enable
	gSsiTxDmaChn = nDmaChn;				
	nRet = DMA_Init(nDmaChn, nDmaChn, &sSrcDevSend, &sDestDevSend, DMA_INT_TFR, SSI_DmaTxProc);
	return nRet;
}
#endif


/****************************************************************************
 * Subroutine:	SSI_WaitTxIdle
 * Function:	to wait SSI transfer idle 
 * Input:		UINT32 nTimeOut --	to specify the timeout, uint in us.
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_WaitTxIdle(UINT32 nTimeOut)
{
	UINT32 nTimeCnt = 0;
	#if(SSI_DMA_TX)
		while((!SsiRegs.SR.bit.Tfe) || SsiRegs.SR.bit.Busy || (gDmaTxDone==0))	// wait transmit FIFO empty
	#else
		while((!SsiRegs.SR.bit.Tfe) || SsiRegs.SR.bit.Busy)	// wait transmit FIFO empty
	#endif
		{
			if(nTimeOut != 0)	// timeout check
			{
				_delay_us(1);
				if(nTimeCnt++ == nTimeOut) 
				{
					return RT_OVERTIME;
				}
			}
		}
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	SSI_SpiSlvWrite
 * Function:	SSI SPI Slave mode send data  
 * Input:		UINT8 *pData --	to specify the data to be sent
 *				UINT32 nLen --	to specify the size to be sent
 *				UINT32 nTimeOut -- to specify the timeout for each byte  
 * Output:		RT_OK or RT_OVERTIME	
 * Description:	if SSI_DFS_SEL==SSI_DFS_16, pData must to be aligned to 2  
 * Date:		2015.11.03
 * ModifyRecord:
 * *************************************************************************/
UINT8 SSI_SpiSlvWrite(UINT8 *pData, UINT16 nLen, UINT32 nTimeOut)
{
	UINT32 nTimeCnt = 0;
	UINT8 nTxCnt;
	#if(SSI_DFS_SEL==SSI_DFS_8)
		UINT8 *pDat = (UINT8 *)pData;
	#elif(SSI_DFS_SEL==SSI_DFS_16)
		UINT16 *pDat = (UINT16 *)pData;
		if(((UINT32)pData&0x1) || (nLen&0x1)) return RT_PARAM_ERR;
		nLen /= 2;							   	
	#endif
	
	#if(SSI_DMA_TX)	
	if(SsiRegs.DMACR.bit.Tdmae)
	{
		UINT16 nDmaTs;
		nDmaTs = (nLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : nLen;
		gTxBufSize = nLen - nDmaTs;
		gTxBuf = (UINT8 *)pDat;				   	
		gDmaTxDone = 0;
		DMA_Start(gSsiTxDmaChn, (UINT32)pDat, (UINT32)&SsiRegs.DR, nDmaTs);			
	}
	else
	#endif
	{
		while(nLen)
		{
			nTimeCnt = 0;
			while(!SsiRegs.SR.bit.Tfnf)	// TX FIFO Full, wait and check timeout
			{
				if(nTimeOut)	// timeout check
				{
					_delay_us(1);
					if(nTimeCnt++ == nTimeOut) 
					{
						return RT_OVERTIME;
					}
				}
			}
			nTxCnt = SSI_FIFO_DEPTH - SsiRegs.TXFLR; 	// read TX FIFO valid data entries
			while(nTxCnt-- && nLen)		// Fill TX FIFO
			{
				SsiRegs.DR = *pDat++;
				nLen--;		
			}
		}	
	}	
	return RT_OK;			
}

/****************************************************************************
 * Subroutine:	SSI_StopReceive
 * Function:	to stop SSI receive.  
 * Input:		None  
 * Output:		None	
 * Description:	to disable SSI receive interrupt and clear received byte number  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
void SSI_StopReceive(void)
{
	NVIC_DisableIRQ(SSI_IRQn);	// disable SSI interrupt
	SsiRegs.IMR.bit.Rxf = 0;
	gRxSize = 0;
	NVIC_EnableIRQ(SSI_IRQn);	// enable SSI interrupt
}

/****************************************************************************
 * Subroutine:	SSI_StartReceive
 * Function:	to start to receive  
 * Input:		UINT8 nDummyByte -- to specify the dummy byte when receiving  
 * Output:		None	
 * Description:	  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
void SSI_StartReceive(UINT16 nDummyByte)
{
	NVIC_DisableIRQ(SSI_IRQn);	// disable SSI interrupt
//	SsiRegs.SSIENR = 0;		// disable SSI
	SsiRegs.IMR.bit.Rxf = 1;
	gRxSize = 0;			// reset buffer index
	NVIC_EnableIRQ(SSI_IRQn);	// clear Rxfifo
	NVIC_DisableIRQ(SSI_IRQn);
	SsiRegs.IMR.bit.Rxf = 1;
	gRxSize = 0;			// reset buffer index
//	SsiRegs.SSIENR = 1;		// enable SSI
	SsiRegs.DR = nDummyByte;
	NVIC_EnableIRQ(SSI_IRQn);	// enable SSI interrupt
}

/****************************************************************************
 * Subroutine:	SSI_GetRxSize
 * Function:	to get received byte number  
 * Input:		None 
 * Output:		to return the number	
 * Description:	  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
UINT16 SSI_GetRxSize(void)
{
	return gRxSize;
}

/****************************************************************************
 * Subroutine:	SSI_Handler
 * Function:	to handle SSI interrupt 
 * Input:		None 
 * Output:		None	
 * Description:	  
 * Date:		2012.12.21
 * ModifyRecord:
 * *************************************************************************/
void SSI_Handler(void)
{
	UINT8 nRxCnt;
	UINT16 nDat16;
	
	if(SsiRegs.ISR.bit.Rxf)
	{
		// receive processing
		while(SsiRegs.RXFLR && SsiRegs.IMR.bit.Rxf) 	// receive data valid and not soft FIFO not overflow
		{
			nRxCnt = SsiRegs.RXFLR; 	// read RX FIFO valid data entries
			#if(SSI_DFS_SEL==SSI_DFS_8)	
				if((gRxBufSize - gRxSize)<nRxCnt) 	// no enough buffer for store
				{
					nRxCnt = gRxBufSize - gRxSize;
					if(nRxCnt == 0)
					{ 
						SsiRegs.IMR.bit.Rxf = 0;	// overflow and disable receive interrupt
					}
				}				
				while(nRxCnt--)		// Load RX FIFO and Fill FIFO
				{
	                nDat16 = SsiRegs.DR;
					gRxBuf[gRxSize++] = nDat16;				
				}
			 #elif(SSI_DFS_SEL==SSI_DFS_16)	
		 		// load RX FIFO and store two bytes
				if((gRxBufSize - gRxSize)<nRxCnt*2) 	// no enough buffer for store
				{
					nRxCnt = (gRxBufSize - gRxSize)/2;
					if(nRxCnt == 0)
					{ 
						SsiRegs.IMR.bit.Rxf = 0;	// overflow and disable receive interrupt
					}
				}
				while(nRxCnt--)		// Load RX FIFO
				{
					nDat16 = SsiRegs.DR;
					gRxBuf[gRxSize++] = nDat16;		// LSB stored
					gRxBuf[gRxSize++] = nDat16>>8;
				}
			#endif
		}	
	}				
}

#if(SSI_DMA_TX)
/****************************************************************************
 * Subroutine:	SSI_DmaTxProc
 * Function:	to handle SSI DMA interrupt 
 * Input:		None 
 * Output:		None	
 * Description:	  
 * Date:		2015.11.03
 * ModifyRecord:
 * *************************************************************************/
void SSI_DmaTxProc(UINT8 nIntType)
{
	UINT16 nDmaTs;
	if(gTxBufSize)
	{
		nDmaTs = (gTxBufSize>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : gTxBufSize;
		gTxBufSize -= nDmaTs;
		#if(SSI_DFS_SEL==SSI_DFS_8)
			gTxBuf += nDmaTs;
		#elif(SSI_DFS_SEL==SSI_DFS_16)
			gTxBuf += nDmaTs*2;				   	
		#endif 
		DMA_Start(gSsiTxDmaChn, (UINT32)gTxBuf, (UINT32)&SsiRegs.DR, nDmaTs);	
	}
	else
	{
		gDmaTxDone = 1;
	}		
}
#endif
