/***************************************************************************
* File name    :	Ssi.h
* Function     :	Header of SSI driver
* Author       : 	coins
* Date         :	2012/12/21
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SSI_H_
#define SSI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/



/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	SSI_DMA_TX		1	// DMA transmit supported

/**************************************************************************
* Global Type Definition
***************************************************************************/
// Define the SSI Port Map to GPIO Port
typedef	enum {
	SSI_MAP_A3_9 = 0,
			//GPA3 -- SSI_SSN	
			//GPA9 -- SSI_RXD	
			//GPA7 -- SSI_CLK	
			//GPA8 -- SSI_TXD
	SSI_MAP_A17_20 = 1,
			//GPA20 -- SSI_SSN	
			//GPA19 -- SSI_RXD	
			//GPA18 -- SSI_CLK	
			//GPA17 -- SSI_TXD
}SSI_MAP;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 SSI_SpiSlvInit(SSI_MAP mPortMap, UINT8 nCPOL, UINT8 nCPHA, UINT8 nDFS, \
						UINT8 *pRxBuf, UINT16 nBufSize);
UINT8 SSI_WaitTxIdle(UINT32 nTimeOut);
UINT8 SSI_SpiSlvWrite(UINT8 *pData, UINT16 nLen, UINT32 nTimeOut);
void SSI_StopReceive(void);
void SSI_StartReceive(UINT16 nDummyByte);
UINT16 SSI_GetRxSize(void);

#if(SSI_DMA_TX)
UINT8 SSI_SpiSlvInitWithDMA(SSI_MAP mPortMap, UINT8 nCPOL, UINT8 nCPHA, UINT8 nDFS, \
						UINT8 *pRxBuf, UINT16 nBufSize, UINT8 nDmaChn);
#endif

/****************************************************************************
 * Subroutine:	SSI_TransOnly
 * Function:	to set SSI Transmit Only mode 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.12.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SSI_TransOnly(void)
{
	SsiRegs.CTRLR0.bit.Tmod = 0x1;	// transmit only
}

/****************************************************************************
 * Subroutine:	SSI_RecevOnly
 * Function:	to set SSI Receive Only mode 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.12.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SSI_RecevOnly(void)
{
	SsiRegs.CTRLR0.bit.Tmod = 0x2;	// receive only
}

/****************************************************************************
 * Subroutine:	SSI_TransceiveBoth
 * Function:	to set SSI both Transmit and Receive  mode 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.12.28
 * ModifyRecord:
 * *************************************************************************/
__INLINE void SSI_TransceiveBoth(void)
{
	SsiRegs.CTRLR0.bit.Tmod = 0x0;	// transmit and receive
}

#endif

