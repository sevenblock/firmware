/****************************************************************************
* File name    :	Apsi.c
* Function     :	the driver of APSI
* Author       : 	coins
* Date         :	2016/11/04
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
typedef struct	{
	UINT16 nMessage;
	UINT16 nPara0;
	UINT16 nPara1;
	UINT8 nType;
	UINT8 nMutex;
}APSI_MESSAGE;

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static APSI_MESSAGE gApsiMessage = {0, 0, 0, 0, 0};


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
 * Subroutine:	APSI_Init
 * Function:	to initial APSI, the ready to receive command
 * Input:		APSI_REG_CHK_MODE mRegAccMod --	to specify the register access mode 
 *				APSI_MEM_CHK_MODE mMemAccMod -- to specify the memory access mode
 *				UINT32 nIntType --  to specify the interrupt type
 * Output:		None	
 * Description:	 
 * Date:		2016.11.04
 * ModifyRecord:
 * *************************************************************************/
UINT8 APSI_Init(APSI_REG_CHK_MODE mRegAccMod, APSI_MEM_CHK_MODE mMemAccMod, UINT32 nIntType)
{
	SCM_EnableModClk(ID_APSI);
	SCM_ReleaseModRst(ID_APSI);
	SCM_EnableModClk(ID_SEA);
	SCM_ReleaseModRst(ID_SEA);
	GpioRegs.GPAUSE0.all = 0x55555555;
	GpioRegs.GPAUSE1.all = 0x55555555;
	GpioRegs.GPAUSE2.all = 0x55555555;
	GpioRegs.GPAUSE3.all = 0x55555555;
	GpioRegs.GPBUSE0.all &= 0xF00000FF;
	GpioRegs.GPBUSE0.all |= 0x05555500;
	if(mRegAccMod==APSI_REG_CHK_NONE) ApsiRegs.CFG.bit.RegEdcEn = 0;
	else ApsiRegs.CFG.bit.RegEdcEn = 1;

	ApsiRegs.CFG.bit.MemEdcEn = 0;
	ApsiRegs.CFG.bit.MemEdcMode = 0;
	ApsiRegs.CFG.bit.CrcEndian = 0;	
	switch(mMemAccMod)
	{
		case APSI_MEM_CHK_CRC_BIG:
			ApsiRegs.CFG.bit.CrcEndian = 1;	
		case APSI_MEM_CHK_CRC_LIT:
			ApsiRegs.CFG.bit.MemEdcMode = 1;
		case APSI_MEM_CHK_LRC:
			ApsiRegs.CFG.bit.MemEdcEn = 1;	
		default:
			break;
	}
	ApsiRegs.INTEN.all = nIntType;
	if(nIntType)NVIC_EnableIRQ(APSI_IRQn);

	ApsiRegs.CTRL.bit.ClearBusy = 1;
	return RT_OK;	
}

/****************************************************************************
 * Subroutine:	APSI_GetMessage
 * Function:	to get APSI message
 * Input:		UINT8 *pMessageType --	to specify the type of message, MESSAGE_RST or MESSAGE_SEM 
 *				UINT16 *pMessage -- to specify the message, define by user
 *				UINT16 *pPara0 --  to specify the parameter 0
 *				UINT16 *pPara1 --  to specify the parameter 1
 *				UINT32 nTimeout --  to specify the time out, unit in us, 0 for no time out
 * Output:		RT_OK, RT_FAIL or RT_OVERTIME 	
 * Description:	 
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 APSI_GetMessage(UINT8 *pMessageType, UINT16 *pMessage, UINT16 *pPara0, UINT16 *pPara1, UINT32 nTimeout)
{
	UINT32 nTime = 0;
	UINT32 nPriMask;
	while(1)
	{
		if(gApsiMessage.nMutex)
		{
			nPriMask = __get_PRIMASK();
			__disable_irq();
			gApsiMessage.nMutex = 0;
			*pMessageType = gApsiMessage.nType;
			*pMessage = gApsiMessage.nMessage;
			*pPara0 = gApsiMessage.nPara0;
			*pPara1 = gApsiMessage.nPara1;
			__set_PRIMASK(nPriMask);
			return RT_OK;
		}
		if(nTimeout)  	// timeout
		{
			_delay_us(1);
			if(nTime++ == nTimeout) return RT_OVERTIME;
		}
		else
		{
			return RT_FAIL;
		}		
	}
		
}

/****************************************************************************
 * Subroutine:	APSI_ReturnMessage
 * Function:	to return message to external host
 * Input:		UINT8 MessageType --	to specify the type of message, MESSAGE_RST or MESSAGE_SEM 
 *				UINT16 nMessage -- to specify the message, define by user
 *				UINT16 *nPara0 --  to specify the parameter 0
 *				UINT16 nPara1 --  to specify the parameter 1
 * Output:		RT_OK	
 * Description:	 
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 APSI_ReturnMessage(UINT8 nMessageType, UINT16 nMessage, UINT16 nPara0, UINT16 nPara1)
{
	if(nMessageType == MESSAGE_SEM)
	{
		ApsiRegs.SEM_OUT = nMessage;
		ApsiRegs.GPR2 = nPara0;
		ApsiRegs.GPR3 = nPara1;
	}
	ApsiRegs.CTRL.bit.ClearBusy = 1;
	return RT_OK;	
}


/****************************************************************************
 * Subroutine:	SEA_Handler
 * Function:	to handle APSI interrupt (the APSI interrupt vector is shared with SEA)
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
 void SEA_Handler(void)
{
	if(ApsiRegs.STA.bit.Reset)
	{
		ApsiRegs.STA.all = 1<<0;
		gApsiMessage.nType = MESSAGE_RST;
		gApsiMessage.nMutex = 1;
					
	}
	if(ApsiRegs.STA.bit.SemRcv)
	{
		ApsiRegs.STA.all = 1<<2;
		gApsiMessage.nMessage = ApsiRegs.SEM_OUT;
		gApsiMessage.nPara0 = ApsiRegs.GPR0;
		gApsiMessage.nPara1 = ApsiRegs.GPR1;
		gApsiMessage.nType = MESSAGE_SEM;
		gApsiMessage.nMutex = 1;
	}	
}









