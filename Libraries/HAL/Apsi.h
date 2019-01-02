/***************************************************************************
* File name    :	Apsi.h
* Function     :	Header of APSI
* Author       : 	coins
* Date         :	2016/11/04
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef APSI_H_
#define APSI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/

// define the interrupt type
#define		APSI_INT_RCV	0x4
#define		APSI_INT_RST	0x1

// define the message type
#define		MESSAGE_RST		0
#define		MESSAGE_SEM		1

/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the APSI register access check mode
typedef	enum {
	APSI_REG_CHK_NONE = 0,
	APSI_REG_CHK_EDC = 1,	
}APSI_REG_CHK_MODE;

// define the APSI memory access check mode
typedef	enum {
	APSI_MEM_CHK_NONE = 0,
	APSI_MEM_CHK_LRC = 1,
	APSI_MEM_CHK_CRC_LIT = 2,
	APSI_MEM_CHK_CRC_BIG = 3,	
}APSI_MEM_CHK_MODE;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/



/**************************************************************************
* Functon
***************************************************************************/
UINT8 APSI_Init(APSI_REG_CHK_MODE mRegAccMod, APSI_MEM_CHK_MODE mMemAccMod, UINT32 nIntType);
UINT8 APSI_GetMessage(UINT8 *pMessageType, UINT16 *pMessage, UINT16 *pPara0, UINT16 *pPara1, UINT32 nTimeout);
UINT8 APSI_ReturnMessage(UINT8 nMessageType, UINT16 nMessage, UINT16 nPara0, UINT16 nPara1);






#endif /*APSI_H_*/
