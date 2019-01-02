/***************************************************************************************
* File name    :	Timer.c
* Function     :	the function of Timer
* Author       : 	Footman
* Date         :	2015/12/18
* Version      :    v1.0
* Description  :    Timer Hardware Abstract Layer for AS569
* ModifyRecord :
*****************************************************************************************/

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

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Local Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/

/***************************************************************************
 * Subroutine:	TIMER_GetRegs
 * Function:	Get Timer register file base address 
 * Input:		UINT8 nTimerNum -- 	to specify the timer ID
 * Output:		TMR_REGS * --	Return Tx register file address;
 * Description:	Get timer register file base address pointer.
 * Date:		2015.12.31
 * ModifyRecord:
 * *************************************************************************/
static volatile struct TMR_REGS * TMR_GetRegs(UINT8 nTimerNum)
{
	switch(nTimerNum)
	{
		case T0:
			return &T0Regs;
		case T1:
			return &T1Regs;
		case T2:
			return &T2Regs;
		case T3:
			return &T3Regs;
		case T4:
			return &T4Regs;
		case T5:
			return &T5Regs;
		case T6:
			return &T6Regs;
		case T7:
			return &T7Regs;
		default: 
			return NULL;  
	}
}

/***********************************************************************************
 * Subroutine:	TIMER_Init
 * Function:	set timer0~7 work mode
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * 				UINT32 nLimit -- 	limit of counter
 * 				UINT8 nClkSrc --    to specify the timer clock for counting
 * 				UINT8  nMode  --    configure timer work mode
 *                                  can be configured as TMR_CNT ,TMR_INT, TMR_WDT
 * Output:		UINT8 --			RT_OK or RT_FAIL
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_Init(UINT8 nTimerNum, UINT32 nLimit, TCLK_SRC nClkSrc, TMR_MODE nMode)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	SCM_ReleaseModRst(nTimerNum);
	SCM_EnableModClk(nTimerNum);

	pTimer->CONTROL.all = (CONTROL_WKEY | CONTROL_WUFLAG);
	pTimer->LIMIT = nLimit;
	pTimer->CONTROL.all = (CONTROL_WKEY | CONTROL_WUFLAG);
	pTimer->COUNT = 0x00000000;

	switch(nMode)
	{
		case TMR_CNT:
			pTimer->CONTROL.all = CONTROL_WKEY | (UINT32)nClkSrc<<CONTROL_CLKSEL;
			break;
		case TMR_INT:
			pTimer->CONTROL.all = CONTROL_WKEY | (UINT32)nClkSrc<<CONTROL_CLKSEL | (1<<CONTROL_IE);
			break;
		case TMR_WDT:
			pTimer->CONTROL.all = CONTROL_WKEY | (UINT32)nClkSrc<<CONTROL_CLKSEL | (1<<CONTROL_WDT);
			break;
		case TMR_CAP:
			pTimer->CONTROL.all = CONTROL_WKEY | (UINT32)nClkSrc<<CONTROL_CLKSEL;
			pTimer->CAPCTRL.bit.CapFallEdge = 1;		//fall edge capture
			pTimer->CAPCTRL.bit.CapRiseEdge = 1;		//rise edge capture
			pTimer->CAPCTRL.bit.CapEn = 1;				//enable capture
			pTimer->CAPCTRL.bit.CapIntEn = 1;			//enable capture intrrupt
			break;
		default :
			return RT_FAIL;
	}

	if((nClkSrc == TCLK_SRC_TMRX) || (nMode == TMR_CAP))
	{
		if(nTimerNum == T0)		 GpioRegs.GPAUSE0.bit.GPA0 = MUX_TMR;
		else if(nTimerNum == T1) GpioRegs.GPAUSE0.bit.GPA1 = MUX_TMR;
		else if(nTimerNum == T2) GpioRegs.GPAUSE0.bit.GPA2 = MUX_TMR;
		else if(nTimerNum == T3) GpioRegs.GPAUSE0.bit.GPA5 = MUX_TMR;
		else if(nTimerNum == T4) GpioRegs.GPAUSE0.bit.GPA6 = MUX_TMR;
		else if(nTimerNum == T5) GpioRegs.GPAUSE3.bit.GPA24 = MUX_TMR;
		else if(nTimerNum == T6) GpioRegs.GPAUSE3.bit.GPA25 = MUX_TMR;
		else if(nTimerNum == T7) GpioRegs.GPBUSE0.bit.GPB7 = MUX_TMR;	
	}
	
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_Start
 * Function:	start timer to count
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * Output:		UINT8 --			RT_OK or RT_FAIL
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_Start(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	pTimer->CONTROL.all = CONTROL_WKEY | (1<<CONTROL_EN) | (pTimer->CONTROL.all); 
	
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_Stop
 * Function:	stop timer counting
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * Output:		UINT8 --			  RT_OK or RT_FAIL
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_Stop(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	pTimer->CONTROL.all = (CONTROL_WKEY|(pTimer->CONTROL.all)) & ~(UINT32)(1<<CONTROL_EN);	

	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_ClrCnt
 * Function:	clear counter
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * Output:		UINT8 --		RT_OK or RT_FAIL
 * Description:
 * Date:		2015.12.31			  
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_ClrCnt(UINT8 nTimerNum)
{	
	UINT32 RegControlVal;
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;
		
	RegControlVal = pTimer->CONTROL.all;
	pTimer->CONTROL.all = (CONTROL_WKEY|CONTROL_WUFLAG|RegControlVal) & ~(UINT32)(1<<CONTROL_EN);
	pTimer->COUNT = 0x00000000;
	pTimer->CONTROL.all = CONTROL_WKEY | RegControlVal;

	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_GetCnt
 * Function:	to get counter
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 *				UINT32	*pCnt --	to return current value	of counter
 * Output:		UINT32 Count --		the value of timer
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_GetCnt(UINT8 nTimerNum, UINT32 *pCnt)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	*pCnt = pTimer->COUNT;

	return	RT_OK;
}

/***********************************************************************************
 * Subroutine:	TMR_GetIntFlag
 * Function:	get timer interrupt flag
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * Output:		UINT8 --		int flag
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
BOOL TMR_GetIntFlag(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	return pTimer->CONTROL.bit.Ip;		
}

/***********************************************************************************
 * Subroutine:	TIMER_ClrIntFlag
 * Function:	to clear timer interrupt flag
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * Output:		UINT8 --		RT_OK or RT_FAIL
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_ClrIntFlag(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	pTimer->CONTROL.all = CONTROL_WKEY | (pTimer->CONTROL.all) | (1<<CONTROL_IP);
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TMR_GetCapIntFlag
 * Function:	get timer capture interrupt flag
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * Output:		UINT8 --		int flag
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
BOOL TMR_GetCapIntFlag(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	return pTimer->CAPCTRL.bit.CapInt;		
}

/***********************************************************************************
 * Subroutine:	TMR_GetCapData
 * Function:	get timer capture data
 * Input:		UINT8 nTimerNum --  timer ID, T0~T7
 * Output:		UINT32 --	capture data
 * Description:
 * Date:		2015.12.31
 * ModifyRecord:
 * ********************************************************************************/
UINT32 TMR_GetCapData(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	return pTimer->CAPVAL;
}


