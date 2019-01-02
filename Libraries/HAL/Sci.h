/***************************************************************************************
* File name    :	Sci.h
* Function     :	The header of Sci.c
* Author       : 	coins
* Date         :	2012/01/09
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef SCI_H_
#define SCI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#ifndef	_POWER_TYPE
#define	_POWER_TYPE
#define TYPE_OFF    0x00
#define TYPE_A  	0x04	// 5V
#define TYPE_B  	0x02  	// 3V
#define TYPE_C  	0x01 	// 1.8V
#endif

#define	NO_LE_SEND	0x8000


/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the callback function type for power control when class selection
typedef void (*FP_SCI_POWER_CTRL)(UINT8 type);
typedef void (*FP_SCI_POWER_CTRL_INIT)(void);
// define the return of SCI operation
typedef	enum	{
	SCI_OK 		= 0,	// SCI operation ok
	SCI_FAIL	= 1,   	// SCI error but unspecified

	SCI_PARAM_ERR,
	SCI_TIMEOUT	,   	// SCI operation timeout
	SCI_CWT_OUT,
	SCI_BWT_OUT,
	SCI_PPS_ERR,

	SCI_TX_ERR 	,	// SCI transmit error
	SCI_RX_OV	,	// SCI receive overflow
	SCI_RX_PARITY_ERR ,

	SCI_CARD_ABSENT , 	// the Card is absent
	SCI_NO_ATR	,	// SCI no ATR after active
	SCI_ATR_ERR	,	// ATR ERR
	SCI_ATR_DTOUT,	// ATR duration time out

	SCI_NO_ATR_CHAR,	// no specified ATR character
	SCI_SW_ABSENT,		// no SW1 or SW2 return
	SCI_LEN_UNMATCH,  	// when read, the actual data length is unmatched with expected
	SCI_EDC_ERR,
	SCI_OTHER_ERR,
	SCI_IFSR_ERR,

}SCI_ERR;

typedef	enum	{
	ATR_TS = 0xFE,
	ATR_T0 = 0x00,
	ATR_T1 = 0x01,	// history byte
	ATR_T2 = 0x02,
	ATR_T3 = 0x03,
	ATR_T4 = 0x04,
	ATR_T5 = 0x05,
	ATR_T6 = 0x06,
	ATR_T7 = 0x07,
	ATR_T8 = 0x08,
	ATR_T9 = 0x09,
	ATR_T10 = 0x0A,
	ATR_T11 = 0x0B,
	ATR_T12 = 0x0C,
	ATR_T13 = 0x0D,
	ATR_T14 = 0x0E,
	ATR_T15 = 0x0F,
	ATR_TA = 0x10,	// for seach first TA
	ATR_TB = 0x20,
	ATR_TC = 0x40,
	ATR_TD = 0x80,
	ATR_TA1 = 0x11,
	ATR_TB1 = 0x21,
	ATR_TC1 = 0x41,
	ATR_TD1 = 0x81,
	ATR_TA2 = 0x12,
	ATR_TB2 = 0x22,
	ATR_TC2 = 0x42,
	ATR_TD2 = 0x82,
	ATR_TA3 = 0x13,
	ATR_TB3 = 0x23,
	ATR_TC3 = 0x43,
	ATR_TD3 = 0x83,
	ATR_TA4 = 0x14,
	ATR_TB4 = 0x24,
	ATR_TC4 = 0x44,
	ATR_TD4 = 0x84,
	ATR_TA5 = 0x15,
	ATR_TB5 = 0x25,
	ATR_TC5 = 0x45,
	ATR_TD5 = 0x85,
	ATR_TA6 = 0x16,
	ATR_TB6 = 0x26,
	ATR_TC6 = 0x46,
	ATR_TD6 = 0x86,
	ATR_TA7 = 0x17,
	ATR_TB7 = 0x27,
	ATR_TC7 = 0x47,
	ATR_TD7 = 0x87,
	ATR_TA8 = 0x18,
	ATR_TB8 = 0x28,
	ATR_TC8 = 0x48,
	ATR_TD8 = 0x88,
	ATR_TA9 = 0x19,
	ATR_TB9 = 0x29,
	ATR_TC9 = 0x49,
	ATR_TD9 = 0x89,
	ATR_TCK = 0xFD,
}ATR_CHAR_INDEX;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
// Low Level Driver
SCI_ERR SCI_Init(UINT8 PresPol, FP_SCI_POWER_CTRL_INIT fpSciPowerCtrlInit, 
				FP_SCI_POWER_CTRL fpSciPowerCtrl);

SCI_ERR SCI_ActiveOnly(UINT8 nType, UINT32 nFeq, UINT32 nTimeout);
SCI_ERR SCI_Write(UINT8 *pData, UINT32 nLen, UINT32 nTimeout);
SCI_ERR SCI_Read(UINT8 *pData, UINT32 *pLen, UINT32 nTimeout);
SCI_ERR SCI_GetATR(UINT8 *pATRBuf, UINT8 *pATRLen, UINT32 nTimeout);
SCI_ERR SCI_GetAnATRChar(UINT8 *pATRSting, ATR_CHAR_INDEX mIndex, UINT8 *pChar);
SCI_ERR SCI_Active(UINT8 nType, UINT32 nFeq, UINT8 *pATRBuf, UINT8 *pATRLen, UINT32 nTimeout);
SCI_ERR SCI_Deactive(UINT32 nTimeout);
SCI_ERR SCI_WarmReset(UINT8 *pATRBuf, UINT8 *pATRLen, UINT32 nTimeout);
SCI_ERR SCI_StopClock(UINT8 ClkVal, UINT16 StopDly, UINT32 nTimeout);
SCI_ERR SCI_ResumeClock(UINT16 StartDly, UINT32 nTimeout);
SCI_ERR SCI_SendPPS(UINT8 Protocol, UINT8 ParamFI, UINT8 ParamDI, UINT32 nTimeout);
SCI_ERR SCI_SendIFSRequest(UINT8 nIFSValue, UINT32 nTimeout);
SCI_ERR SCI_ChangeParamWithATR(UINT8 *pATRBuf);

// High Level Driver (T=0 and T=1)
SCI_ERR SCI_SendAPDU(UINT32 Command, UINT8 Lc, UINT8 *pLcField, UINT16 Le, 
			UINT8 *pLeField, UINT16 *pLeFieldLen, UINT16 *pSw1Sw2, UINT32 nTimeout);

/**************************************************************************
* Functon
***************************************************************************/


#endif /* SCI_H_ */

