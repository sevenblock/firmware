/***************************************************************************************
* File name    :	Dma.h
* Function     :	The header of Dma.c
* Author       : 	
* Date         :	
* Version      :    
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef DMA_H_
#define DMA_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	LLMBT		1 	// Link List Multi-Block Transfer function supported

// define the number of DMA channels
#define	DMA_NUM_CHANNELS	4		// number of DMA channels
#define	DMA_MAX_TRAN_SIZE	4095   	// DMA Maximum block transfer size

// define interrupt type
#define	DMA_INT_TFR			0x01
#define	DMA_INT_BLOCK		0x02
#define	DMA_INT_SRC_TRAN	0x04
#define	DMA_INT_DST_TRAN	0x08
#define	DMA_INT_ERR			0x10

/**************************************************************************
* Global Type Definition
***************************************************************************/
// Define the address mode type
typedef	enum {
	INC = 0, 	// increment
	DEC = 1,	// decrement
	FIX = 2,	// no change
}ENUM_DMA_ADDR_MODE;

// Define the transfer width type
typedef	enum {
	BYTE_1 = 0,		// 8-bit wide 
	BYTE_2 = 1,	   	// 16-bit wide
	BYTE_4 = 2,		// 32-bit wide
}ENUM_DMA_TRAN_WIDTH;

// Define the burst transaction length type
typedef	enum {
	BURST_1 = 0, 	// 1 data of transfer width for each request	
	BURST_4 = 1,	// 4 data of transfer width for each request
	BURST_8 = 2,	// 8 data of transfer width for each request
}ENUM_DMA_BURST_LEN;

// Define the peripheral handshaking type
typedef	enum {
	HARD = 0, 	// hardware handshaking, request by peripheral request	
	SOFT = 1,	// software handshaking, request by registers	
	NOHANDSHAKE = 2, // no handshaking, request by moudle fifo
}ENUM_DMA_HANDSHAKING;

// 
#ifndef	_STRUCT_DMA_DEV_
#define	_STRUCT_DMA_DEV_ 
typedef struct {
	UINT32 nAddr;
	ENUM_DMA_ADDR_MODE mAddrMode;
	ENUM_DMA_TRAN_WIDTH mTranWidth;
	ENUM_DMA_BURST_LEN mBurstLen;
	ENUM_DMA_HANDSHAKING mHandshaking;
}STRUCT_DMA_DEV, * P_STRUCT_DMA_DEV;
#endif

#ifndef	_FP_DMA_CH_ISR_CB_
#define	_FP_DMA_CH_ISR_CB_
typedef	void (*FP_DMA_CH_ISR_CB)(UINT8 nIntType);
#endif

//--------------------------------------------------
// define for LLP
//-------------------------------------------------
#if(LLMBT)
// the type for LLI (Link List Item)
typedef	struct LLI{
	UINT32 nSAR; 		// source address 
	UINT32 nDAR;	 	// destination address
	struct LLI * pLLP;		// link list pointer, points to next LLI
	UINT32 nCTL_L;	 	// control low
	UINT32 nCTL_H; 		// control high
	UINT32 nSSTAT;		// source state
	UINT32 nDSTAT;	 	// destination state
	UINT32 nCFG;		// save config for CFG register
}DMA_LLI, *P_DMA_LLI;

#endif
//--------------------------------------------------

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/
UINT8 DMA_Init(UINT8 nChn, UINT8 nPrio, STRUCT_DMA_DEV *pSrcDev, STRUCT_DMA_DEV *pDestDev, \
		UINT8 nIntType, FP_DMA_CH_ISR_CB fp);
void DMA_Start(UINT8 nChn, UINT32 nSrcAddr,UINT32 nDstAddr, UINT32 nTranTimes);
UINT8 DMA_CheckDone(UINT8 nChn, UINT32 nTimeout);

#if(LLMBT)
UINT8 DMA_SetupLLI(P_DMA_LLI *pLLI, P_DMA_LLI pNewLLIbuf, STRUCT_DMA_DEV *pSrcDev, STRUCT_DMA_DEV *pDestDev, UINT32 nTranTimes);
UINT8 DMA_InitWithLLI(UINT8 nChn, UINT8 nPrio, P_DMA_LLI pLLI, UINT32 nSSTATAR, UINT32 DSTATAR, UINT8 nIntType, FP_DMA_CH_ISR_CB fp);
void DMA_StartWithLLI(UINT8 nChn, P_DMA_LLI pLLI);
#endif


/****************************************************************************
 * Subroutine:	DMA_ChangeSrcAddrMode
 * Function:	to change source address mode
 * Input:		UINT8  nChn -- DMA channel, values 0 ~ DMA_NUM_CHANNELS-1 
 *				ENUM_DMA_ADDR_MODE mAddrMode -- to specify the address mode
 * Output:		None	
 * Description:	 
 * Date:		2015.11.03
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void DMA_ChangeSrcAddrMode(UINT8 nChn, ENUM_DMA_ADDR_MODE mAddrMode)
{
	DmaRegs.CH[nChn].CTL_L.bit.Sinc = mAddrMode;
}

/****************************************************************************
 * Subroutine:	DMA_ChangeDestAddrMode
 * Function:	to change destination address mode
 * Input:		UINT8  nChn -- DMA channel, values 0 ~ DMA_NUM_CHANNELS-1 
 *				ENUM_DMA_ADDR_MODE mAddrMode -- to specify the address mode
 * Output:		None	
 * Description:	 
 * Date:		2015.11.03
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void DMA_ChangeDestAddrMode(UINT8 nChn, ENUM_DMA_ADDR_MODE mAddrMode)
{
	DmaRegs.CH[nChn].CTL_L.bit.Dinc = mAddrMode;
}
#endif /* DMA_H_ */

