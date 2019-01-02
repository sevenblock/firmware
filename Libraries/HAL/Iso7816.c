/**********************************************************************************
* File name    :	Iso7816.c
* Function     :	The base function for ISO7816 hardware driver
* Author       : 	hong
* Date         :	2013/01/13
* Version      :    v1.0
* Description  :	
* ModifyRecord :
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"

/**********************************************************************************
* Local Macro Definition
**********************************************************************************/

/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/

/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/

/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/
static ISO7816_ERR WaitStatStable(void);
static ISO7816_ERR WaitTxOver(UINT32 nTimeout);
static ISO7816_ERR WaitRxOver(UINT32 nTimeout);

/**********************************************************************************
* Functon
**********************************************************************************/
/****************************************************************************
 * Subroutine:	WaitStatStable
 * Function:	to wait state stable
 * Input:		None
 * Output:		ISO7816_ERR	
 * Description:	 
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
static ISO7816_ERR WaitStatStable(void)
{
	UINT32 i = 0;
	while(Iso7816Regs.STS.bit.WAIT)
	{
		if(i++ == 1000) return ISO7816_TIMEOUT;
	}	
	return ISO7816_OK;
} 

/****************************************************************************
 * Subroutine:	WaitTxOver
 * Function:	to wait transfer a byte complete
 * Input:		UINT32 nTimeout -- the timeout, unit in us
 * Output:		ISO7816_ERR	
 * Description:	 
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
static ISO7816_ERR WaitTxOver(UINT32 nTimeout)
{
	UINT32 i = 0;
	while(!Iso7816Regs.STS.bit.TBE)
	{
		if(Iso7816Regs.STS.bit.PE) 
		{
		    Iso7816Regs.STS.bit.PE = 1;
			return ISO7816_TX_PARITY_Err;	// Parity error with error signal
		}
		if(nTimeout)  	// timeout
		{
			_delay_us(1);
			if(i++ == nTimeout) return ISO7816_TIMEOUT;
		}
	}	
	return ISO7816_OK;
} 

/****************************************************************************
 * Subroutine:	WaitRxOver
 * Function:	to wait receive a byte complete
 * Input:		UINT32 nTimeout -- the timeout, unit in us
 * Output:		ISO7816_ERR	
 * Description:	 
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
static ISO7816_ERR WaitRxOver(UINT32 nTimeout)
{
	UINT32 i = 0;
	while(!Iso7816Regs.STS.bit.RBNE)
	{
		if((Iso7816Regs.CON2.bit.TPS == 0)&&(Iso7816Regs.STS.bit.PE == 1)) // T0
		{
			return ISO7816_RX_PARITY_Err;	// Parity error with error signal
		}
		if(nTimeout)  	// timeout
		{
			_delay_us(1);
			if(i++ == nTimeout) return ISO7816_TIMEOUT;
		}
	}	
	return ISO7816_OK;
} 

/****************************************************************************
 * Subroutine:	ISO7816_Init
 * Function:	ISO7816 module initialization 
 * Input:		None 
 * Output:		None	
 * Description:	to reset ISO7816 module and set Ports. After initialization, ISO7816
 *				work in receive state, T=0, direct coding convention, no EGT, no error retry, 1 etu 
 *				error signal protocol 
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
void ISO7816_Init(void)
{
	SCM_KeepModRst(ID_ISO7816);
	SCM_DisableModClk(ID_ISO7816);	
	SCM_EnableModClk(ID_ISO7816);
	SCM_ReleaseModRst(ID_ISO7816);
	
	GpioRegs.GPBUSE0.bit.GPB5 = MUX_ISO7816;	//IO
	GpioRegs.GPBUSE0.bit.GPB6 = MUX_ISO7816;	//CLK		
}

/****************************************************************************
 * Subroutine:	ISO7816_SetTransProtocol
 * Function:	to set transfer protocol for ISO7816 module work, T=0 or T=1 
 * Input:		UINT8 nTProtocol ---	the transfer protocol, 0 for T=0, 1 for T=1 
 * Output:		ISO7816_ERR	
 * Description:	 
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_SetTransProtocol(UINT8 nTProtocol)
{
	if(nTProtocol>1) return ISO7816_PARAM_ERR;
	Iso7816Regs.CON2.bit.TPS = nTProtocol;
	return ISO7816_OK;
}

/****************************************************************************
 * Subroutine:	ISO7816_SetCodingConvention
 * Function:	to set coding convention, direct or inverse 
 * Input:		UINT8 nConvention ---	the coding convention 
 * Output:		ISO7816_ERR	
 * Description:	 
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_SetCodingConvention(UINT8 nConvention)
{
	if(nConvention>1) return ISO7816_PARAM_ERR;
	Iso7816Regs.CON0.bit.INV = nConvention;
	return ISO7816_OK;
}

/****************************************************************************
 * Subroutine:	ISO7816_SetClkOut
 * Function:	to set ISO7816 clock output when working in master mode 
 * Input:		UINT8 nDiv ---	the divider parameter, when nDiv is 0, the output
 *				ISO7816 clock frequency is FCLK; when nDiv is 1~31, the output
 *				ISO7816 clock frequency is FCLK/(2*nDiv)	 
 * Output:		ISO7816_ERR	
 * Description:	 
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_SetClkOut(UINT8 nDiv)
{
	if(nDiv>31) return ISO7816_PARAM_ERR;
	Iso7816Regs.CON1.bit.DIV = nDiv;
	Iso7816Regs.CON0.bit.CLKOE = 1;
	return ISO7816_OK;
}

/****************************************************************************
 * Subroutine:	ISO7816_SetEGT
 * Function:	to set the extra guard time controlled by ISO7816 module 
 * Input:		UINT8 nEGT ---	the extra guard time uint in etu (0~254)					 
 * Output:		ISO7816_ERR	
 * Description:	note that: as terminal, the EGT is from 0 to 254
 *				
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_SetEGT(UINT8 nEGT)
{
    UINT16 nEGTNum = nEGT;
	if(nEGT>254) return ISO7816_PARAM_ERR;
	else
	{
        if(Iso7816Regs.CON2.bit.TPS == ISO7816_TP_T0)//T0
	    {
	       if((UINT8)(++nEGTNum) == 0)
	            nEGTNum = 1;
	    }       
	    else//T1
	    {
	       ++nEGTNum;
	       if((UINT8)(++nEGTNum) == 1)
	            nEGTNum = 1;        
	    }

		Iso7816Regs.CON1.bit.EGT = nEGTNum;
	}
	return ISO7816_OK;	
}

/****************************************************************************
 * Subroutine:	ISO7816_SetRetry
 * Function:	to set the retry for sending 0r receiving
 * Input:		UINT8 nType --      send (0) or receive (1)
 *				UINT8 nRetry ---	the retry times (0~7)					 
 * Output:		ISO7816_ERR	
 * Description:	only used for T0
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_SetRetry(UINT8 nType ,UINT8 nRetry)
{
	if(nRetry>7) return ISO7816_PARAM_ERR;
	else
	{
		if(nType == 0)
			Iso7816Regs.CON1.bit.TRY = nRetry;
		else if(nType == 1)
			Iso7816Regs.CON1.bit.RRY = nRetry;
		else
			return ISO7816_PARAM_ERR;
	}
	return ISO7816_OK;	
}

/****************************************************************************
 * Subroutine:	ISO7816_SetErrSignalWidth
 * Function:	to set the width of error signal
 * Input:		UINT8 nErrSignalWidth ---	the width of error signal(1~2)					 
 * Output:		ISO7816_ERR	
 * Description:	
 * Date:		2013/10/28
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_SetErrSignalWidth(UINT8 nErrSignalWidth)
{
	if((nErrSignalWidth>2) || (nErrSignalWidth==0)) return ISO7816_PARAM_ERR;
	else
	{
		Iso7816Regs.CON2.bit.WES0 = nErrSignalWidth-1;
	}
	return ISO7816_OK;	
}

/***************************************************************************
 * Subroutine:	ISO7816_Etu0CntDly
 * Function:	to delay time with ETU0 Counter
 * Input:		UINT8 nDlyType --  the delay type, ETU(0) or ISO7816 clock (1)
 *				UINT16 nDly --	the delay number; 
 *				UINT32 nTimeout -- the timeout, unit in us
 * Output:		ISO7816_ERR
 * Description:
 * Date:		2013.10.30
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_Etu0CntDly(UINT8 nDlyType, UINT16 nDly, UINT32 nTimeout)
{
	UINT32 i = 0;
	if(nDly==0) return ISO7816_OK;	// if delay is 0, always return RT_OK
	Iso7816Regs.TDAT0 = nDly;
	Iso7816Regs.TRLD0 = nDly;
	Iso7816Regs.TCON0.all = nDlyType? 0xD : 0xB;	// ETU0 Conter started up
	while(!Iso7816Regs.TCON0.bit.TF)
	{
		if(nTimeout)  	// timeout
		{
			_delay_us(1);
			if(i++ == nTimeout) return ISO7816_TIMEOUT;
		}
	}	
	Iso7816Regs.TCON0.all = 0x8;
	return WaitStatStable();
}

/***************************************************************************
 * Subroutine:	ISO7816_Etu1CntDly
 * Function:	to delay time with ETU1 Counter
 * Input:		UINT8 nDlyType --  the delay type, ETU(0) or ISO7816 clock (1)
 *				UINT16 nDly --	the delay number; 
 *				UINT32 nTimeout -- the timeout, unit in us
 * Output:		ISO7816_ERR
 * Description:
 * Date:		2013.10.30
 * ModifyRecord:
 * *************************************************************************/
ISO7816_ERR ISO7816_Etu1CntDly(UINT8 nDlyType, UINT16 nDly, UINT32 nTimeout)
{
	UINT32 i = 0;
	if(nDly==0) return ISO7816_OK;	// if delay is 0, always return RT_OK
	Iso7816Regs.TDAT1 = nDly;
	Iso7816Regs.TRLD1 = nDly;
	Iso7816Regs.TCON1.all = nDlyType? 0xD : 0xB;	// ETU1 Conter started up
	while(!Iso7816Regs.TCON1.bit.TF)
	{
		if(nTimeout)  	// timeout
		{
			_delay_us(1);
			if(i++ == nTimeout) return ISO7816_TIMEOUT;
		}
	}	
	Iso7816Regs.TCON1.all = 0x8;
	return WaitStatStable();
}

/***************************************************************************
 * Subroutine:	ISO7816_StartupProcByte
 * Function:	to start up procedure byte sending (card mode)
 * Input:		UINT16 nPeriod --  the interval be between two procedure bytes 
 *				UINT8 nProcByte --	the procedure byte, usually 0x60; 
 * Output:		None
 * Description:
 * Date:		2013.10.30
 * ModifyRecord:
 * *************************************************************************/
void ISO7816_StartupProcByte(UINT16 nPeriod, UINT8 nProcByte)
{
	Iso7816Regs.CON0.bit.TR = 1;
	Iso7816Regs.TMSK0.bit.MTF = 1;
	Iso7816Regs.TCON0.bit.TMOD = 0;
	Iso7816Regs.TDAT0 = nPeriod;
	Iso7816Regs.TRLD0 = nPeriod;
	Iso7816Regs.PROC0.all = nProcByte;
	Iso7816Regs.TCON0.all = 0x9;				
}


/***********************************************************************************
 * Subroutine:	ISO7816_Write
 * Function:	to send out bytes by ISO7816	interface
 * Input:		UINT8 *pData --	to specify the data for writing;
 * 				UINT32 nLen -- 	to specify the length;
 *				UINT8 nEGT -- to specify the extra guard time for sending
 *				UINT32 nTimeout -- the timeout, unit in us
 * Output:		ISO7816_ERR
 * Description:
 * Date:		2013.10.31
 * ModifyRecord:
 * ********************************************************************************/
ISO7816_ERR ISO7816_Write(UINT8 *pData, UINT32 nLen, UINT8 nEGT, UINT32 nTimeout)
{
	ISO7816_ERR nRet;
	nRet = WaitTxOver(nTimeout);	// wait transfer completed
	if(nRet != ISO7816_OK) return nRet;

	nRet = ISO7816_SetEGT(nEGT); 	// to set EGT
	if(nRet != ISO7816_OK) return nRet;	

	Iso7816Regs.CON0.bit.TR = 1;	// the direction is switched to transfer			
	while(nLen--)
	{
		Iso7816Regs.BUF.all = *pData++;
		nRet = WaitStatStable();	// wait state register stable
		if(nRet != ISO7816_OK) break;
		nRet = WaitTxOver(nTimeout);	// wait transfer completed
		if(nRet != ISO7816_OK) break;
	}
	Iso7816Regs.CON0.bit.TR = 0;	// the direction is switched to receive
	return nRet;
}

/***********************************************************************************
 * Subroutine:	ISO7816_Read
 * Function:	to receive bytes by ISO7816	interface 
 * Input:		UINT8 *pData --	to specify the data buffer for reading;
 * 				UINT32 nLen -- 	to specify the length to read;
 *				UINT32 *pRetLen -- to return the actual length;
 *				UINT32 nTimeout -- the timeout, unit in us
 * Output:		ISO7816_ERR
 * Description:
 * Date:		20103.10.31
 * ModifyRecord:
 * ********************************************************************************/
ISO7816_ERR ISO7816_Read(UINT8 *pData, UINT32 nLen, UINT32 *pRetLen, UINT32 nTimeout)
{
	ISO7816_ERR nRet;
	*pRetLen = 0;
	while(nLen--)
	{		
		nRet = WaitRxOver(nTimeout);
		if(nRet != ISO7816_OK) break;
		*pData++ = (UINT8)(Iso7816Regs.BUF.all);
		*pRetLen++;
		nRet = WaitStatStable(); 	// wait state register stable
		if(nRet != ISO7816_OK) break;
	}
	if(Iso7816Regs.STS.bit.PE) 
	{
		Iso7816Regs.STS.bit.PE = 1;
		return ISO7816_RX_PARITY_Err;	// Parity error with error signal
	}
	if(Iso7816Regs.STS.bit.OE)
	{
		Iso7816Regs.STS.bit.OE = 1;
		 return ISO7816_RX_OE;	// receive buffer overrun error
	}
	return nRet;
}


