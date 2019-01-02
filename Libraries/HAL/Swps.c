/****************************************************************************
* File name    :	Swps.c
* Function     :	the driver of SWP Slave controller
* Author       : 	hongjz
* Date         :	2016/11/07
* Version      :    v1.0
* Description  :  	this file implements SWP slave MAC layer driver
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
typedef	struct	{
	UINT8 nData[30];
	UINT8 nLen;
	SWPS_ERR nStat;
}SWPS_RX_FRAME;

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static SWPS_RX_FRAME RxFrame = {0};
static UINT8 gMutex = 0; 	// the mutex semaphore for interrupt receive and application process
static SWP_STATE gSwpsState = SWP_DEACTIVATED;
static UINT8 gSofOk = 0;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	SWPS_Init
 * Function:	to initialization SWPS 
 * Input:		None 
 * Output:		SWPS_ERR	
 * Description:	 
 * Date:		2012.08.22
 * ModifyRecord:
 * *************************************************************************/
SWPS_ERR SWPS_Init(void)
{
	UINT32 nSysClk = SCM_GetSysClk()/1000000;
	SCM_EnableModClk(ID_SWPS);
	SCM_KeepModRst(ID_SWPS);
	SCM_ReleaseModRst(ID_SWPS);	

	// Config wait time
	SwpsRegs.SUSPEND_TM = 40*nSysClk;		// >MaxHighTime
	SwpsRegs.DEACTIVATED_TM = 100*nSysClk;	// >P4 (100us)
	SwpsRegs.INHIBIT_TM = 100*1000*nSysClk;		// >P6 (20ms)
	SwpsRegs.S1_PERIOD.bit.MaxHighTime = 20*nSysClk;	// >7.5us
	SwpsRegs.S1_PERIOD.bit.MaxLowTime = 20*nSysClk;		// >7.5us

	SwpsRegs.CTRL.bit.SuspendIdleBit = 7;
	SwpsRegs.CTRL.bit.TxResumeBit = 0;

	SwpsRegs.IE.bit.ClfStart = 1;
	SwpsRegs.IE.bit.Deactive = 1;
	SwpsRegs.IE.bit.Suspend	= 1;
	SwpsRegs.IE.bit.Resume = 1;
	SwpsRegs.IE.bit.RxSof = 1;
	SwpsRegs.IE.bit.RxFifoNotEmpty = 1;
	SwpsRegs.IE.bit.RxDone = 1;

	NVIC_EnableIRQ(SWPS_IRQn);	// enable SWPS interrupt
	SwpsRegs.CTRL.bit.RxEn = 1;		// enable SWPS receive
	gSwpsState = SWP_DEACTIVATED;
	return SWPS_OK;
}

/****************************************************************************
 * Subroutine:	SWPS_SendFrame
 * Function:	to send a frame with SWP 
 * Input:		UINT8 *pData --	to specify the data for sending. 
 *				UINT8 nLen -- to specify the length of data
 *				UINT32 nLoops --  to specify the loops for waiting the frame done
 *								when run in ROM, a loop costs about 22 cycles
 * Output:		SWPS_ERR	
 * Description:	The data for sending is only the payload, excluding the CRC bytes,
 *				SOF and EOF. The length for data in limited to 30 
 * Date:		2012.08.24
 * ModifyRecord:
 * *************************************************************************/
SWPS_ERR SWPS_SendFrame(UINT8 *pData, UINT8 nLen, UINT32 nLoops)
{
	UINT32 cnt = 0;
	if(nLen>30) nLen = 30;	// the payload size is limit to 30 bytes
	if(nLen==0) return SWPS_PARAM_ERR;
	SwpsRegs.LEN.bit.TxLen = nLen;
	
	while((SwpsRegs.INT.bit.TxFifoNotFull) && nLen)
	{
		SwpsRegs.TX_FIFO = *pData++;  	// write transmit FIFO
		nLen -- ;
	}
	SwpsRegs.INT.all = SWPS_INT_TX_DONE;	// clear done flag
	SwpsRegs.CTRL.bit.TxEn = 1; 	// enable transfer
	while(SwpsRegs.INT.bit.TxDone == 0)
	{
		while((SwpsRegs.INT.bit.TxFifoNotFull) && nLen) 	// if TX fifo is not full
		{
			SwpsRegs.TX_FIFO = *pData++;  	// write fifo
			nLen -- ;	
		}
		if(nLoops!=0) 	// timeout enable
		{
			if(cnt++ == nLoops)		// timeout
			{ 
				SwpsRegs.CTRL.bit.TxEn = 0; 	// enable transfer
				SwpsRegs.CTRL.bit.TxFifoClr = 1;	// clear transmit FIFO
				return SWPS_TX_TIMEOUT;
			}
		}
	}

	SwpsRegs.INT.all = SWPS_INT_TX_DONE;	// clear done flag
	if(SwpsRegs.STATUS.bit.TxInhibit)	// unable to send out
	{
		SwpsRegs.CTRL.bit.TxFifoClr = 1;	// clear transmit FIFO
		return SWPS_TX_INHIBIT;
	}
	else if(SwpsRegs.STATUS.bit.TxFifoUnderFlow)	// under run
	{
		SwpsRegs.CTRL.bit.TxFifoClr = 1;	// clear transmit FIFO
		return SWPS_TX_UNDERFLOW;
	}
	else if(SwpsRegs.STATUS.bit.TxFrameErr)
	{
		SwpsRegs.CTRL.bit.TxFifoClr = 1;	// clear transmit FIFO
		return SWPS_TX_FRAME_ERR;
	}
	else
	{
		return SWPS_OK;
	}	
}

/****************************************************************************
 * Subroutine:	SWPS_GetFrame
 * Function:	to get a frame from SWPS receive buffer 
 * Input:		UINT8 *pData --	to specify the a frame data buffer for read. 
 *				UINT8 *pLen -- to return the length of the frame
 *				UINT32 nLoops --  to specify the loops for waiting the frame
 * Output:		SWPS_ERR	
 * Description:	The frame data is only the payload, excluding the CRC bytes,
 *				SOF and EOF. The length for data in limited to 30 
 * Date:		2012.08.27
 * ModifyRecord:
 * *************************************************************************/
SWPS_ERR SWPS_GetFrame(UINT8 *pData, UINT8 *pLen, UINT32 nLoops)
{
	SWPS_ERR mRet;
	UINT32 i = nLoops;

	while((!nLoops) || (i--))
	{
		NVIC_DisableIRQ(SWPS_IRQn);	// disable SWPS interrupt
		if(gMutex)
		{
			if(RxFrame.nStat == SWPS_OK)
			{
				memcpy(pData, RxFrame.nData, RxFrame.nLen);
				*pLen = RxFrame.nLen;
				RxFrame.nLen = 0;	
			}
			mRet = RxFrame.nStat;
			gMutex = 0;
			NVIC_EnableIRQ(SWPS_IRQn);	// enable SWPS interrupt
			return mRet;
		}
		else
		{
			NVIC_EnableIRQ(SWPS_IRQn);	// enable SWPS interrupt
			continue;
		}	
	}
	return SWPS_RX_TIMEOUT;
}

/****************************************************************************
 * Subroutine:	SWPS_Handler
 * Function:	to handle the SWPS interrupt for receive
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.27
 * ModifyRecord:
 * *************************************************************************/
void SWPS_Handler(void)
{
	union SWPS_INT_REG nInt;

	nInt.all = SwpsRegs.INT.all;

	if(nInt.bit.ClfStart)  	// CLF Start, when powered up
	{
		SwpsRegs.IE.bit.ClfStart = 0;
		SwpsRegs.INT.all = SWPS_INT_CLF_START; 

		gSwpsState = SWP_CLF_START;
	}
	if(nInt.bit.Suspend)	// Suspend
	{
		SwpsRegs.INT.all = SWPS_INT_SUSPEND; 

		gSwpsState = SWP_SUSPENDED;		
	}
	if(nInt.bit.Resume)	// Resume
	{
		SwpsRegs.INT.all = SWPS_INT_RESUME; 

		gSwpsState = SWP_ACTIVATED;		
	}
	if(nInt.bit.Deactive)	// Deactive
	{
		SwpsRegs.INT.all = SWPS_INT_DEACTIVE; 

		gSwpsState = SWP_DEACTIVATED;		
	}
	if(nInt.bit.RxSof) 	// SOF
	{
		SwpsRegs.INT.all = SWPS_INT_RX_SOF;
		gMutex = 0;	
		RxFrame.nLen = 0;	// start to receive a new packet
		RxFrame.nStat = SWPS_OK;
		gSofOk = 1;	
	}	
	while(SwpsRegs.INT.bit.RxFifoNotEmpty)		// not empty
	{
		RxFrame.nData[RxFrame.nLen++] = SwpsRegs.RX_FIFO;
	}

	if(nInt.bit.RxDone)	 	// done
	{
		SwpsRegs.INT.all = SWPS_INT_RX_DONE;

		if(SwpsRegs.STATUS.bit.RxFrameErr) 	// Receive frame error
		{
			if(gSofOk)
			{
				RxFrame.nStat = SWPS_RX_FRAME_ERR;
				gSofOk = 0;
			}
			else return;
		}
		else if(SwpsRegs.STATUS.bit.RxCrcErr)	// CRC error
		{
			RxFrame.nStat = SWPS_RX_CRC_ERR;
		}
		else if(SwpsRegs.STATUS.bit.RxEofNotFoundErr) 	// no EOF but data length is 30 bytes
		{
			RxFrame.nStat = SWPS_RX_EOF_ABSENT;
		}
		else if(SwpsRegs.STATUS.bit.RxFifoOverFlow) 	// receive FIFO is overflow
		{
			RxFrame.nStat = SWPS_RX_OVERFLOW;
		}
		else 	// received, but no error
		{
			if(RxFrame.nLen != 	SwpsRegs.LEN.bit.RxLen) RxFrame.nStat = SWPS_RX_LEN_UNMATCH;
		   	else RxFrame.nStat = SWPS_OK;
		}
		if(RxFrame.nStat!=SWPS_OK)SwpsRegs.CTRL.bit.RxFifoClr = 1;	// clear receive FIFO
		gMutex = 1;		// post mutex semaphore 
	}	
}

/****************************************************************************
 * Subroutine:	SWPS_GetState
 * Function:	to get the SWPS current State
 * Input:		None
 * Output:		SWP_STATE --	the state of SWPS interface	
 * Description:	 
 * Date:		2012.08.27
 * ModifyRecord:
 * *************************************************************************/
SWP_STATE SWPS_GetState(void)
{
	return gSwpsState;
}

