/****************************************************************************
* File name    :	Scm.c
* Function     :	the driver of System Control Module
* Author       : 	coins
* Date         :	2012/07/23
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "AS569.h"

extern volatile uint32_t		SystemFrequency; 	// defined in system_AS569.c
extern volatile unsigned char 	SerInit;	 		// defined in Serial.c 

/**************************************************************************
* Local Macro Definition
***************************************************************************/
//#define	SCM_ROM_FUNC_USED

// define HFROSC Auto Trimming range and precision (part per thousand)
#define	HFROSC_AUTO_TRIM_RANGE		200
#define	HFROSC_AUTO_TRIM_PRECISION 	1		

/**************************************************************************
* Local Type Definition
***************************************************************************/

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static UINT16 gCntMs = 12000;
static UINT8 gCntUs = 1;
static UINT32 gHFRoscFreq = F_HFROSC;	// the HFROSC output clock frequency
static UINT32 gAutoTrimRefFreq;

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
 *************************** Miscellaneous API *****************************
 **************************************************************************/
 
/****************************************************************************
 * Subroutine:	_delay_ms
 * Function:	to delay ms
 * Input:		UINT32 ms --	to specify the time of ms;
 * Output:		None	
 * Description:	this delay loops about [(4 * gCntMs + 7) * ms + 18] cycles 
 * Date:		2012.07.24
 * ModifyRecord:
 * *************************************************************************/
#pragma arm section code = ".align_code1"
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
#pragma push
#pragma O3
#pragma Ospace
void _delay_ms(register UINT32 ms)
{
	UINT32 i;	
	while(ms--)
	{
		i =  gCntMs;
		while(i--)
		{
			__NOP();
		}
	}
}
#pragma pop

#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
#pragma optimize = speed high
void _delay_ms(register UINT32 ms)
{
	UINT32 i;	
	while(ms--)
	{
		i =  gCntMs;
		while(i--)
		{
			__NOP();
		}
	}
}
#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif
#pragma arm section code

/****************************************************************************
 * Subroutine:	_delay_us
 * Function:	to delay us
 * Input:		UINT32 us --	to specify the time of us;
 * Output:		None	
 * Description:	the delay loops about [(5 * gCntUs + 12) * us + 2]
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
#pragma arm section code = ".align_code2"
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
#pragma push
#pragma O3
#pragma Ospace
void _delay_us(register UINT32 us)
{
	register UINT32 i, j;
	j = gCntUs;	
	while(j--)
	{
		i =  us;
		while(i--)
		{
            __NOP();
		}
	}
}
#pragma pop

#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
#pragma optimize = speed high
void _delay_us(register UINT32 us)
{
	register UINT32 i, j;
	j = gCntUs;	
	while(j--)
	{
		i =  us;
		while(i--)
		{
            __NOP();
		}
	}
}

#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif
#pragma arm section code

/***************************************************************************
 *************************** Clock Control API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_SetSysClk
 * Function:	to set system clock 
 * Input:		UINT8 Clk --	to specify the system clock. 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_SetSysClk(SYS_CLK Clk)
{
	UINT32 nUsbClkSel;
	UINT32 nTrimStat;
	UINT32 nRegStatus;
//	UINT32 nTC0, nTC1, nTC2, nTC3;

	/* configuration must be changed */
	//if(((ScmRegs.SYSCLKCTRL.all&0x3F)|((ScmRegs.HFROSCCTRL.all>>12)&0xC0))!=Clk)
	{
		nRegStatus = __get_PRIMASK();	// save global interrput
    	__disable_irq();	// disable interrupt

		// when switch system, the auto trimming must be disabled 
		nTrimStat = ScmRegs.TRIM_CTRL.bit.TrimStart; 
		ScmRegs.TRIM_CTRL.bit.TrimStart = 0;

		nUsbClkSel = 0;
		SerInit = 0;	/* print port need to re-initialize */
		if(!(Clk&0x1))		/* higher frequency	*/
		{
			// flash timing parameter to slow
			EFC_ConfigTiming(TC0_144M, TC1_144M, TC2_144M, TC3_144M);

			if(Clk&(1<<5))	/* external */ 
			{
				if(!ScmRegs.AMODENCTRL.bit.Xtal)
				{	
					/* enable XTAL */
					ScmRegs.REGWP0 = 0x55;
					ScmRegs.REGWP0 = 0xAA;
					ScmRegs.REGWP0 = 0x67;
					ScmRegs.AMODENCTRL.bit.Xtal = 1;
					ScmRegs.REGWP0 = 0x00;	
				}
				/* set USB clock to non-divided clock */
				nUsbClkSel = 2;	
				/* set system clock frequency variable */	
				SystemFrequency = F_XTAL;
				/* set delay function parameters */
				gCntMs = 3000;
				gCntUs = 1;		
			}
			else	/* internal */
			{
				if(Clk&(1<<4))	// MFROSC
				{
					if(!ScmRegs.AMODENCTRL.bit.Mfrosc)
					{	
						/* enable MFROSC */
						ScmRegs.REGWP0 = 0x55;
						ScmRegs.REGWP0 = 0xAA;
						ScmRegs.REGWP0 = 0x67;
						ScmRegs.AMODENCTRL.bit.Mfrosc = 1;
						ScmRegs.REGWP0 = 0x00;	
					}
					/* set USB clock unchanged */
					nUsbClkSel = ScmRegs.SYSCLKCTRL.bit.UsbClk48mSel;
					/* set system clock frequency variable */	
					SystemFrequency = F_MFROSC;
					/* set delay function parameters */
					gCntMs = 1000;
					gCntUs = 1;		
				}
				else 	// HFROSC
				{
					if(!ScmRegs.AMODENCTRL.bit.Hfrosc)
					{	
						/* enable HFROSC */
						ScmRegs.REGWP0 = 0x55;
						ScmRegs.REGWP0 = 0xAA;
						ScmRegs.REGWP0 = 0x67;
						ScmRegs.AMODENCTRL.bit.Hfrosc = 1;
						ScmRegs.REGWP0 = 0x00;	
					}
					/* check whether the HFROSC grade need to be modified */
					if(ScmRegs.HFROSCCTRL.bit.HFROscGrade != ((Clk>>6)&0x3))	
					{
						/* to change HFROSC grade */
						/* run in HFROSC/4, because HFROSC frequency may be too large before trimming */
						ScmRegs.SYSCLKCTRL.all = (ScmRegs.SYSCLKCTRL.all & (~(3<<2))) | (1<<2);	
						while(ScmRegs.HFROSCCTRL.bit.HFROscGrade != ((Clk>>6)&0x3)) ScmRegs.HFROSCCTRL.bit.HFROscGrade = (Clk>>6)&0x3;
						ScmRegs.HFROSCCTRL.bit.HFROscTrim = 0x200;	// recover to default
						/* reload trimming value */	
						SCM_LoadHFRoscTrim();	
						/* set USB clock according to HFROSC frequency */
						/* set system clock frequency variable */
						/* set delay function parameters */	
					}
					switch((Clk>>6)&0x3)
					{
						case 3:	/* 144MHz */
							nUsbClkSel = 1;	
							gHFRoscFreq = F_HFROSC_3;
							gCntMs = 36000;
							gCntUs = 20;	
							break;
						case 2:	/* 96MHz */
							nUsbClkSel = 0;
							gHFRoscFreq = F_HFROSC_2;
							gCntMs = 24000;
							gCntUs = 13;		
							break;
						case 1:	/* 48MHz */
							nUsbClkSel = 2;
							gHFRoscFreq = F_HFROSC_1;
							gCntMs = 12000;
							gCntUs = 6;		
							break;
						case 0:	/* 12MHz */
							nUsbClkSel = ScmRegs.SYSCLKCTRL.bit.UsbClk48mSel;
							gHFRoscFreq = F_HFROSC_0;
							gCntMs = 3000;
							gCntUs = 1;		
						default:
							break;
					}
					SystemFrequency=gHFRoscFreq;
				}
			}
			ScmRegs.SYSCLKCTRL.all = (ScmRegs.SYSCLKCTRL.all & 0xF9FFFFC0) | (nUsbClkSel<<25) | Clk;
			__NOP();__NOP();__NOP();__NOP();__NOP();
			/* set system clock frequency variable according to divider */	
			SystemFrequency = SystemFrequency*(1L<<((Clk>>2)&0x3))/8;
//			EFC_CalcParameters(SystemFrequency, &nTC0, &nTC1, &nTC2, &nTC3);
//			EFC_ConfigTiming(nTC0, nTC1, nTC2, nTC3);
			if(SystemFrequency<=1000000) EFC_ConfigTiming(TC0_1M, TC1_1M, TC2_1M, TC3_1M);
			else if(SystemFrequency<=2000000) EFC_ConfigTiming(TC0_2M, TC1_2M, TC2_2M, TC3_2M);
			else if(SystemFrequency<=4000000) EFC_ConfigTiming(TC0_4M, TC1_4M, TC2_4M, TC3_4M);
			else if(SystemFrequency<=6000000) EFC_ConfigTiming(TC0_6M, TC1_6M, TC2_6M, TC3_6M);
			else if(SystemFrequency<=12000000) EFC_ConfigTiming(TC0_12M, TC1_12M, TC2_12M, TC3_12M);
			else if(SystemFrequency<=18000000) EFC_ConfigTiming(TC0_18M, TC1_18M, TC2_18M, TC3_18M);
			else if(SystemFrequency<=24000000) EFC_ConfigTiming(TC0_24M, TC1_24M, TC2_24M, TC3_24M);
			else if(SystemFrequency<=36000000) EFC_ConfigTiming(TC0_36M, TC1_36M, TC2_36M, TC3_36M);
			else if(SystemFrequency<=48000000) EFC_ConfigTiming(TC0_48M, TC1_48M, TC2_48M, TC3_48M);
			else if(SystemFrequency<=72000000) EFC_ConfigTiming(TC0_72M, TC1_72M, TC2_72M, TC3_72M);
			else if(SystemFrequency<=96000000) EFC_ConfigTiming(TC0_96M, TC1_96M, TC2_96M, TC3_96M);
			else  EFC_ConfigTiming(TC0_144M, TC1_144M, TC2_144M, TC3_144M);
			if(nTrimStat)SCM_HFRoscAutoTrim((AUTO_TRIM_REF_CLK)ScmRegs.TRIM_CTRL.bit.TrimRefClkSel, gAutoTrimRefFreq, SCM_GetHFRoscClk());
			/* set delay function parameters according to divider */
			gCntMs = (gCntMs*(1L<<((Clk>>2)&0x3))+4)/8;		/* added 4 for rounding */
			if(gCntMs==0) gCntMs=1;	/* 0 is not used */
			gCntUs = (gCntUs*(1L<<((Clk>>2)&0x3))+4)/8;	
			if(gCntUs==0) gCntUs=1;			
		}
		else	// lower frequency
		{
			if(Clk&(1<<1))	/* RTC clock */	
			{
				/* check RTC clock enable */
				if(AossRegs.ANMOD_CFG.bit.Osc32kEn==0) AossRegs.ANMOD_CFG.bit.Osc32kEn = 1;
				/* set system clock frequency variable */	
				SystemFrequency = F_EXT_RTC;
				/* set delay function parameters */
				gCntMs = 8;	
				gCntUs = 1;		
			}
			else 	/* LFROSC */
			{
				/* check LFROSC clock enable */	
				if(AossRegs.ANMOD_CFG.bit.LFRoscEn==0) AossRegs.ANMOD_CFG.bit.LFRoscEn = 1;
				/* set system clock frequency variable */	
				SystemFrequency = F_LFROSC;
				/* set delay function parameters */
				gCntMs = 8;	
				gCntUs = 1;	
			}
			/* switch to lower frequency clock source */
			ScmRegs.SYSCLKCTRL.all = (ScmRegs.SYSCLKCTRL.all & 0xFFFFFFFC) | 1L | (Clk&(1<<1));
			__NOP();__NOP();__NOP();__NOP();__NOP();
			EFC_ConfigTiming(TC0_1M, TC1_1M, TC2_1M, TC3_1M);
		}

		ScmRegs.TRIM_CTRL.bit.TrimStart = nTrimStat;
		__set_PRIMASK(nRegStatus); 			// recover interrupt
	}
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	SCM_SetSysClkLowFreq
 * Function:	to set system clock to LFROSC 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.01.05
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetSysClkLowFreq(void)
{
	ScmRegs.SYSCLKCTRL.bit.FclkSrc = 0x1;
	__NOP();__NOP();__NOP();__NOP();__NOP();
}
/****************************************************************************
 * Subroutine:	SCM_GetSysClk
 * Function:	to get system clock 
 * Input:		None 
 * Output:		return the system clock frequency in Hz.	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
UINT32 SCM_GetSysClk(void)
{
	return SystemFrequency;
}

/****************************************************************************
 * Subroutine:	SCM_DisableModClk
 * Function:	to disable Module clock
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
 void SCM_DisableModClk(UINT8 ModId)
{
	if(ModId<32)ScmRegs.MODCLKCTRL0 &= ~(1<<ModId);
	else ScmRegs.MODCLKCTRL1 &= ~(1<<(ModId-32));
}

/****************************************************************************
 * Subroutine:	SCM_EnableModClk
 * Function:	to enable Module clock
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
 void SCM_EnableModClk(UINT8 ModId)
{
	if(ModId<32)ScmRegs.MODCLKCTRL0 |= (1<<ModId);
	else ScmRegs.MODCLKCTRL1 |= (1<<(ModId-32));
}

/****************************************************************************
 * Subroutine:	SCM_SetClkout
 * Function:	to set clock out
 * Input:		CLKOUT_SRC mSrc -- to specify the clock out source
 *				CLKOUT_PIN mPin -- to select clock out pin
 *				UINT8 nDiv -- to specify the divider of clock out (0~255) 
 * Output:		None	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetClkout(CLKOUT_SRC mSrc, CLKOUT_PIN mPin, UINT8 nDiv)
{
	if(mPin==CLKOUT_PIN_GPA0) GpioRegs.GPAUSE0.bit.GPA0 = MUX_CLKOUT; 
	else if(mPin==CLKOUT_PIN_GPB7) GpioRegs.GPBUSE0.bit.GPB7 = MUX_CLKOUT;
	ScmRegs.CLKOUTCTRL.all = ((UINT32)mSrc<<9) | nDiv | 0x100; 
}

/****************************************************************************
 * Subroutine:	SCM_SetUartClk
 * Function:	to set UART clock, this clock for baudrate generator
 * Input:		UINT8 UARTx -- to specify the UART number
 *				UART_CLK_SRC mSrc -- to specify the UART clock source
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_SetUartClk(UINT8 UARTx, UART_CLK_SRC mSrc)
{
	if(UARTx<3)
	{
		SerInit = 0;	/* print port need to re-initialize */
		ScmRegs.UARTCLKSEL.all = (ScmRegs.UARTCLKSEL.all & (~(7<<(8*UARTx)))) |	\
							((UINT32)mSrc)<<(8*UARTx);	
		return RT_OK;
	}
	else return RT_FAIL;					
}

/****************************************************************************
 * Subroutine:	SCM_GetUartClk
 * Function:	to get UART clock frequency, uint in Hz
 * Input:		UINT8 UARTx -- to specify the UART number
 * Output:		frequency	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
UINT32 SCM_GetUartClk(UINT8 UARTx)
{
	UART_CLK_SRC mUartSrc = (UART_CLK_SRC)((ScmRegs.UARTCLKSEL.all>>(8*UARTx))&0x7);
	UINT32 nUartFreq = 0;
	switch(mUartSrc)	
	{
		case UART_CLK_SRC_FCLK:
			nUartFreq = SystemFrequency;
			break;
		case UART_CLK_SRC_XTAL:
			nUartFreq = F_XTAL;
			break;	
		case UART_CLK_SRC_HFROSC_DIV1:
			nUartFreq = gHFRoscFreq;
			break;
		case UART_CLK_SRC_HFROSC_DIV2:
			nUartFreq = gHFRoscFreq/2;
			break;
		case UART_CLK_SRC_HFROSC_DIV4:
			nUartFreq = gHFRoscFreq/4;
			break;
		case UART_CLK_SRC_HFROSC_DIV8:
			nUartFreq = gHFRoscFreq/8;
			break;
		default:
			break;
	}
	return 	nUartFreq;
}

/****************************************************************************
 * Subroutine:	SCM_SetI2cClk
 * Function:	to set I2C clock, this clock for baudrate generator
 * Input:		UINT8 I2Cx -- to specify the I2C number
 *				I2C_CLK_SRC mSrc -- to specify the I2C clock source
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_SetI2cClk(UINT8 I2Cx, I2C_CLK_SRC mSrc)
{
	if(I2Cx<1)
	{
		ScmRegs.I2CCLKSEL.all = (ScmRegs.I2CCLKSEL.all & (~(7<<(8*I2Cx)))) |	\
							((UINT32)mSrc)<<(8*I2Cx);
		return RT_OK;
	}
	else
	{
		return RT_FAIL;
	}						
}

/****************************************************************************
 * Subroutine:	SCM_GetI2cClk
 * Function:	to get I2C clock frequency, uint in Hz
 * Input:		UINT8 I2Cx -- to specify the I2C number
 * Output:		frequency	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
UINT32 SCM_GetI2cClk(UINT8 I2Cx)
{
	I2C_CLK_SRC mI2cSrc = (I2C_CLK_SRC)((ScmRegs.I2CCLKSEL.all>>(8*I2Cx))&0x3);
	UINT32 nI2cFreq = 0;
	switch(mI2cSrc)	
	{
		case I2C_CLK_SRC_FCLK:
			nI2cFreq = SystemFrequency;
			break;
		case I2C_CLK_SRC_HFROSC:
			nI2cFreq = gHFRoscFreq;
			break;	
		case I2C_CLK_SRC_MFROSC:
			nI2cFreq = F_MFROSC;
			break;
		case I2C_CLK_SRC_XTAL:
			nI2cFreq = F_XTAL;
			break;
		default:
			break;
	}
	return 	nI2cFreq;
}

/****************************************************************************
 * Subroutine:	SCM_GetHFRoscClk
 * Function:	to get HFROSC clock frequency, uint in Hz
 * Input:		None
 * Output:		frequency	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
UINT32 SCM_GetHFRoscClk(void)
{
	return gHFRoscFreq;
}

/****************************************************************************
 * Subroutine:	SCM_LoadHFRoscTrim
 * Function:	to load default HF ROSC Trimming value from OTP and update to register 
 * Input:		None 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.10.25
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_LoadHFRoscTrim(void)
{
	UINT8 nOffset = ScmRegs.HFROSCCTRL.bit.HFROscGrade;
	UINT16 *pDefaultTrim;  	// trimming field address in OTP
	UINT16 DefaultTrim; 
	union SCM_HFROSCCTRL_REG  uHfOscCtrl;
	UINT8 nHFROscCRE;
	UINT8 nTrimStart;

	pDefaultTrim = (UINT16 *)(0x1008018C);
	DefaultTrim = pDefaultTrim[nOffset]&0xFFF;
	if(DefaultTrim == 0xFFF)
	{	
		pDefaultTrim = (UINT16 *)(0x10080020);
		DefaultTrim = pDefaultTrim[nOffset]&0xFFF;
	}
	if(DefaultTrim != 0xFFF)
	{
		uHfOscCtrl.all = ScmRegs.HFROSCCTRL.all;
		uHfOscCtrl.bit.HFROscGrade = DefaultTrim>>10;
		uHfOscCtrl.bit.HFROscTrim = DefaultTrim&0x3FF;
		nHFROscCRE = ScmRegs.HFROSCCTRL.bit.HFROscCRE;
		nTrimStart = ScmRegs.TRIM_CTRL.bit.TrimStart;
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = 0;
		ScmRegs.TRIM_CTRL.bit.TrimStart = 0;
		ScmRegs.HFROSCCTRL.all = uHfOscCtrl.all;//change HFRosc Trim must disable HFROscCRE.
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = nHFROscCRE;
		ScmRegs.TRIM_CTRL.bit.TrimStart = nTrimStart;
		return RT_OK;
	}
	else
	{
		return RT_FAIL;
	}	
}

/****************************************************************************
 * Subroutine:	SCM_RoscCRE1
 * Function:	to enable HF ROSC clock recover 
 * Input:		UINT8 nType 0 HFROscCRE;1 TrimStart;
 * Output:		None	
 * Description:	 
 * Date:		2012.10.25
 * ModifyRecord:
 * *************************************************************************/
 void SCM_HFRoscCRE1(UINT8 nType)
{
	if(nType == 0)
	{
		ScmRegs.TRIM_CTRL.bit.TrimStart = 0;
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = 1;
	}
	else
	{
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = 0;
		ScmRegs.TRIM_CTRL.bit.TrimStart = 1;
	}
}

/****************************************************************************
 * Subroutine:	SCM_HFRoscAutoTrim
 * Function:	to set HFROSC auto trimming 
 * Input:		AUTO_TRIM_REF_CLK mRef -- to specify the reference clock
 *				UINT32 nRefFreq -- to specify the reference clock frequency
 *									If the reference clock is UART, this field is
 *									the baudrate of UART; the others, this field is
 *									ignored.
 *				UINT32 nTargetFreq -- to specify the HFROSC auto trimming target freq
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_HFRoscAutoTrim(AUTO_TRIM_REF_CLK mRefSrc, UINT32 nRefFreq, UINT32 nTargetFreq)
{
	UINT32 nTargetCnt;
	UINT32 nRange, nPrecision;
	
	if(ScmRegs.SYSCLKCTRL.all & 0x00000031) return RT_FAIL;
	switch(mRefSrc)
	{
		case AUTO_TRIM_REF_CLK_USB:
			nTargetCnt = nTargetFreq/1000;
			break;
		case AUTO_TRIM_REF_CLK_XTAL:
			nTargetCnt = nTargetFreq/(F_XTAL/2048);
			if(!ScmRegs.AMODENCTRL.bit.Xtal)
			{	
				/* enable XTAL */
				ScmRegs.REGWP0 = 0x55;
				ScmRegs.REGWP0 = 0xAA;
				ScmRegs.REGWP0 = 0x67;
				ScmRegs.AMODENCTRL.bit.Xtal = 1;
				ScmRegs.REGWP0 = 0x00;	
			}
			break;
		case AUTO_TRIM_REF_CLK_RTC:
			nTargetCnt = nTargetFreq/(F_EXT_RTC/16);
			if(AossRegs.ANMOD_CFG.bit.Osc32kEn==0) AossRegs.ANMOD_CFG.bit.Osc32kEn = 1;
			break;
		case AUTO_TRIM_REF_CLK_UART0:
		case AUTO_TRIM_REF_CLK_UART1:
		case AUTO_TRIM_REF_CLK_UART2:
			if(nRefFreq == 0) return RT_PARAM_ERR;
			gAutoTrimRefFreq = nRefFreq;
			nTargetCnt = nTargetFreq/nRefFreq;
			break;	
		default:
			return RT_FAIL;	
	}

	nRange = nTargetCnt*HFROSC_AUTO_TRIM_RANGE/1000;
	nPrecision = nTargetCnt*HFROSC_AUTO_TRIM_PRECISION/1000;
	ScmRegs.TRIM_CTRL.bit.TrimStart = 0;
	ScmRegs.TRIM_THRESHOLD_MIN = nTargetCnt-nRange;
	ScmRegs.TRIM_THRESHOLD_MAX = nTargetCnt+nRange;
	ScmRegs.TRIM_STD_MIN = nTargetCnt-nPrecision;
	ScmRegs.TRIM_STD_MAX = nTargetCnt+nPrecision;
	ScmRegs.TRIM_CTRL.bit.TrimRefClkSel = mRefSrc;
	ScmRegs.TRIM_CTRL.bit.TrimStart = 1;
	return RT_OK;			
}

/****************************************************************************
 * Subroutine:	SCM_SetHFRoscTrimLimit
 * Function:	to set HFROSC trimming limit
 * Input:		UINT16 nLLimit -- to specify the low limit
 *				UINT16 nHLimit -- to specify the high limit
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2016.01.16
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_SetHFRoscTrimLimit(UINT16 nLLimit, UINT16 nHLimit)
{
	if((nLLimit<nHLimit)&&(nHLimit<=0x3FF))
	{
		ScmRegs.REGWP1 = 0x32107654;
		ScmRegs.HFROSCTRIMVAL.bit.TrimHLimit = nHLimit;
		ScmRegs.HFROSCTRIMVAL.bit.TrimLLimit = nLLimit;
		return RT_OK;
	}
	else return RT_FAIL;	
}

/***************************************************************************
 *************************** Reset Control API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_KeepModRst
 * Function:	to keep Module in reset state
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
void SCM_KeepModRst(UINT8 ModId)
{
	if(ModId<32)ScmRegs.MODRSTCTRL0 &= ~(1<<ModId);
	else ScmRegs.MODRSTCTRL1 &= ~(1<<(ModId-32));
}

/****************************************************************************
 * Subroutine:	SCM_ReleaseModRst
 * Function:	to release Module from reset state
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
void SCM_ReleaseModRst(UINT8 ModId)
{
	if(ModId<32)ScmRegs.MODRSTCTRL0 |= (1<<ModId);
	else ScmRegs.MODRSTCTRL1 |= (1<<(ModId-32));
}

/***************************************************************************
 *************************** Analog Control API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_AnModPowerEnable
 * Function:	to enable analog mode power 
 * Input:		UINT32 nAnMods --	to specify the analog modules. 
 * Output:		None	
 * Description:	 
 * Date:		2013.12.18
 * ModifyRecord:
 * *************************************************************************/
void SCM_AnModPowerEnable(UINT32 nAnMods)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.all |= (nAnMods);
	ScmRegs.REGWP0 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_AnModPowerDisable
 * Function:	to disable analog mode power 
 * Input:		UINT32 nAnMods --	to specify the analog modules. 
 * Output:		None	
 * Description:	 
 * Date:		2013.12.18
 * ModifyRecord:
 * *************************************************************************/
void SCM_AnModPowerDisable(UINT32 nAnMods)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.all &= ~(nAnMods);
	ScmRegs.REGWP0 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_SetPdr33Th
 * Function:	to set PDR33 threshold 
 * Input:		PDR33_TH mTh --	to specify the threshold. 
 * Output:		None	
 * Description:	 
 * Date:		2016.01.25
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetPdr33Th(PDR33_TH mTh)
{
	ScmRegs.REGWP1 = 0x32107654; 
	ScmRegs.ANALOGTRIM.bit.Pdr33ThSel = mTh;	
}

/****************************************************************************
 * Subroutine:	SCM_SetPdr18Th
 * Function:	to set PDR18 threshold 
 * Input:		PDR18_TH mTh --	to specify the threshold. 
 * Output:		None	
 * Description:	 
 * Date:		2016.01.25
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetPdr18Th(PDR18_TH mTh)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.ANALOGTRIM.bit.Pdr18ThSel = mTh;	
}

/****************************************************************************
 * Subroutine:	SCM_SetArvTh
 * Function:	to set AVR threshold 
 * Input:		AVR_HD18_TH mHD18Th --	to specify the threshold.
 *				AVR_LD18_TH mLD18Th --  to specify the threshold.
 *				AVR_HD11_TH mHD11Th --	to specify the threshold.
 *				AVR_LD11_TH mLD1LTh --	to specify the threshold.
 *				AVR_HDHV_TH mHDHVTh --  to specify the threshold.
 * Output:		None	
 * Description:	 
 * Date:		2016.01.25
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetArvTh(AVR_HD18_TH mHD18Th, AVR_LD18_TH mLD18Th, AVR_HD11_TH mHD11Th,\
				 AVR_LD11_TH mLD1LTh, AVR_HDHV_TH mHDHVTh)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.ANALOGTRIM.bit.AvrHd18ThSel = mHD18Th;
	ScmRegs.ANALOGTRIM.bit.AvrLd18ThSel = mLD18Th;
	ScmRegs.ANALOGTRIM.bit.AvrHd11ThSel = mHD11Th;
	ScmRegs.ANALOGTRIM.bit.AvrLd11ThSel = mLD1LTh;
	ScmRegs.ANALOGTRIM.bit.AvrHdHvThSel = mHDHVTh; 	
}

/***************************************************************************
 *************************** MPU Control API *****************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_RegWrEn
 * Function:	to enable register MODPDCTRL and PWRTOPCTRL writeable 
 * Input:		None
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_RegWrEn(void)
{
	ScmRegs.REGWP0 = 0x55;
	ScmRegs.REGWP0 = 0xAA;
	ScmRegs.REGWP0 = 0x67;
	if(ScmRegs.REGWP0&0x100) return RT_OK;
	else return RT_FAIL;
}

/***************************************************************************
 *************************** System Tick API *******************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SYST_Config
 * Function:	to set system tick  
 * Input:		UINT8 nClkSource --	to specify the clock source for SysTick;
 *									0 -- STCLK; 1 -- SCLK
 *				UINT32 nTicks -- to specify the initialization counter value;
 *				UINT8 nMode -- to specify the work mode for SysTick
 *								0 -- counter mode; 1 -- interrupt mode		
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SYST_Config(UINT8 nClkSource, UINT32 nTicks, UINT8 nMode)
{
	if ((nTicks > SYSTICK_MAXCOUNT) || (nClkSource > SYST_SCLK) || (nMode > SYST_INT)) 
		return RT_FAIL;                                               

  	SysTick->LOAD  =  (nTicks & SYSTICK_MAXCOUNT) - 1;   	/* set reload register */
	/* set Priority for Cortex-M0 System Interrupts */
  	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
  	SysTick->VAL   =  (0x00);  /* Load the SysTick Counter Value */
	/* Enable SysTick Timer */
  	SysTick->CTRL = (nClkSource << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (nMode<<SYSTICK_TICKINT);    
  	return RT_OK;                                                    
}

/***************************************************************************
 ****************************** PMU API **********************************
 **************************************************************************/

/****************************************************************************
 * Subroutine:	SCM_SetDeepSleepMode
 * Function:	to set system state when enter into deep sleep mode  
 * Input:		DSM_EF_TYPE mEfType --	to specify the mode that flash enter into;
 *				UINT8 nPd -- to specify the power module;
 *				UINT8 nSlowClkSw -- to specify whether switch to lower frequency clock;	
 * Output:		None	
 * Description:	 
 * Date:		2016.04.12
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetDeepSleepMode(DSM_EF_TYPE mEfType, UINT32 nPd, UINT8 nSlowClkSw)
{
	switch(mEfType)
	{
		case DSM_EF_STANDBY:
			ScmRegs.DSMCTRL.bit.EfPdm = 0;
			ScmRegs.DSMCTRL.bit.EfSlm = 0;
			EfcRegs.CTRL.bit.PDM = 0;
			EfcRegs.CTRL.bit.SLM = 0;
			break;
		case DSM_EF_SLEEP:
			ScmRegs.DSMCTRL.bit.EfPdm = 0;
			ScmRegs.DSMCTRL.bit.EfSlm = 1;	
			EfcRegs.CTRL.bit.PDM = 0;
			EfcRegs.CTRL.bit.SLM = 1;
			break;
		case DSM_EF_PWRDN:
			ScmRegs.DSMCTRL.bit.EfPdm = 1;
			ScmRegs.DSMCTRL.bit.EfSlm = 0;
			EfcRegs.CTRL.bit.PDM = 1;
			EfcRegs.CTRL.bit.SLM = 0;	
			break;
		default:;
	}

	ScmRegs.DSMCTRL.bit.AutoXtalPowSw = (nPd&DSM_PD_XTAL)? 1: 0;
	ScmRegs.DSMCTRL.bit.AutoMFRoscPowSw = (nPd&DSM_PD_MFROSC)? 1: 0;
	ScmRegs.DSMCTRL.bit.AutoHFRoscPowSw = (nPd&DSM_PD_HFROSC)? 1: 0;
	ScmRegs.DSMCTRL.bit.AutoFclkSwitch = (nSlowClkSw)? 1: 0;
}

