/***************************************************************************************
* File name    :	Iso7816.h
* Function     :	The header of Iso7816.c
* Author       : 	Coins
* Date         :	2013/10/28
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef ISO7816_H_
#define ISO7816_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/ 


/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the transfer protocol
#define ISO7816_TP_T0	0 		// T = 0
#define ISO7816_TP_T1	1		// T = 1
// define the coding convention
#define ISO7816_CC_DIRECT	0 	// direct coding convention
#define ISO7816_CC_INVERSE	1	// inverse coding convention
// define the error signal width 
#define ISO7816_WES_1ETU	0 	// 1ETU
#define ISO7816_WES_2ETU	1	// 2ETU
#define ISO7816_WES_1_5ETU	2	// 1.5ETU

// define the start etu of the retx by T0
#define ISO7816_RTX_13ETU	0 	// 13ETU
#define ISO7816_RTX_14ETU	1	// 14ETU


// define the return of ISO7816 operation
typedef	enum	{
	ISO7816_OK 		= 0,	// ISO7816 operation ok
	ISO7816_FAIL	= 1,   	// ISO7816 error but unspecified

	ISO7816_PARAM_ERR,
	ISO7816_TIMEOUT	,   	// ISO7816 operation timeout

	ISO7816_TX_ERR 	,	// ISO7816 transmit error
	ISO7816_TX_PARITY_Err ,

	ISO7816_RX_ERR	,	// ISO7816 receive overflow
	ISO7816_RX_OE	,	// ISO7816 receive overflow
	ISO7816_RX_PARITY_Err ,

}ISO7816_ERR;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void ISO7816_Init(void);
ISO7816_ERR ISO7816_SetTransProtocol(UINT8 nTProtocol);
ISO7816_ERR ISO7816_SetCodingConvention(UINT8 nConvention);

ISO7816_ERR ISO7816_SetClkOut(UINT8 nDiv);

ISO7816_ERR ISO7816_SetEGT(UINT8 nEGT);
ISO7816_ERR ISO7816_SetRetry(UINT8 nType ,UINT8 nRetry);
ISO7816_ERR ISO7816_SetErrSignalWidth(UINT8 nErrSignalWidth);
ISO7816_ERR ISO7816_Etu0CntDly(UINT8 nDlyType, UINT16 nDly, UINT32 nTimeout);
ISO7816_ERR ISO7816_Etu1CntDly(UINT8 nDlyType, UINT16 nDly, UINT32 nTimeout);
void ISO7816_StartupProcByte(UINT16 nPeriod, UINT8 nProcByte);

ISO7816_ERR ISO7816_Write(UINT8 *pData, UINT32 nLen, UINT8 nEGT, UINT32 nTimeout);
ISO7816_ERR ISO7816_Read(UINT8 *pData, UINT32 nLen, UINT32 *pRetLen, UINT32 nTimeout);


/****************************************************************************
 * Subroutine:	ISO7816_ClrRxBuf
 * Function:	to clear receive buffer 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.10.29
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void ISO7816_ClrRxBuf(void)
{
	Iso7816Regs.CON0.bit.RBRST = 1;
	Iso7816Regs.CON0.bit.RBRST = 0;
}

/****************************************************************************
 * Subroutine:	ISO7816_StopClkOut
 * Function:	to stop clk out 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.10.29
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void ISO7816_StopClkOut(void)
{
	Iso7816Regs.CON0.bit.CLKOE = 0;
}

/****************************************************************************
 * Subroutine:	ISO7816_StopProcByte
 * Function:	to stop proc byte 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.10.29
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void ISO7816_StopProcByte(void)
{
	Iso7816Regs.TCON0.all = 0x8;	
}

#endif
