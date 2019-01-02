/***************************************************************************************
* File name    :	Aoss.h
* Function     :	Header of Aoss.c
* Author       : 	Footman
* Date         :	2016/04/22
* Version      :    v1.0
* Description  :    Header of Vpwm Hardware Abstract Layer
* ModifyRecord :
*****************************************************************************************/
#ifndef AOSS_H_
#define AOSS_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define BKRAM_START		0x50040400
#define BKRAM_END		0x500407FF
#define BKRAM_SIZE		1024

//BKKey Address
#define BKKEY_START		0x500400E0
#define BKKEY_SIZE		32
//AOSS clock type
#define AOSS_LFROSC	0
#define AOSS_RTCCLK	1	

//rtc interrupt type
#define RTC_INT_NO		0		//Not open RTC interrupt
#define RTC_INT_SEC		1		// each second interupt
#define RTC_INT_HSEC	2		// each half second interrupt
#define RTC_INT_MIN		4	 	// each minute interrupt

//shield/decap channel type
#define SD_IN0	(1<<0)
#define SD_IN1	(1<<1)
#define SD_IN2	(1<<2)
#define SD_IN3	(1<<3)
#define SD_IN4	(1<<4)
#define SD_IN5	(1<<5)

//ADC channel type
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_TEMP	ADC_CH0
#define ADC_BVD		ADC_CH1

//ADC channel adjustment 
#define ADC_CH0_ADDR  				0x10080029
#define ADC_CH0_ADDR1  				0x1008019C
#define ADC_CH1_ADDR  				0x1008002A
#define ADC_CH1_ADDR1  				0x1008019D
#define ADC_CH23_ADDR  				0x1008002B
#define ADC_CH23_ADDR1  			0x1008019E

#define ADC_CH1_VALUE  				900
#define ADC_CH1_VALUE1  			600
#define ADC_CH23_VALUE  			900
#define ADC_CH23_VALUE1  			100

//LDO select
#define LDO33OUT_REF_V						(180)

#define LDO33OUT_NUM  						(0x8)
#define LDO18OUT_NUM  						(0x8)
#define LDO11OUT_NUM  						(0x8)
#define LDO11LPV5OUT_NUM  					(0x8)

#define	LDO33OUT_RANGE						(30)//¡À0.3V
#define	LDO18OUT_RANGE						(20)//¡À0.2V
#define	LDO11OUT_RANGE						(10)//¡À0.1V
#define	LDO11LPOUT_RANGE					(10)//¡À0.1V

#define VREF_VALUE_ADDR  					0x10080028
#define LDO33OUT_VALUE_ADDR  				0x10080044
#define LDO18OUT_VALUE_ADDR  				0x1008004C
#define LDO11OUT_VALUE_ADDR  				0x10080054
#define LDO11LPV5OUT_VALUE_ADDR  			0x10080184
#define LDOOUT_VALUE_ADDR  					0x100801CA

#define FT_VER_ADDR  						0x1008007F
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef struct{ 
    UINT16 year;
    UINT8 month;
    UINT8 day;
    UINT8 week;
    UINT8 hour;
    UINT8 minute;
    UINT8 second;
}DATA_FORM; 

//aoss exception mode
typedef enum
{
	AOSS_SD = 1<<0,
	AOSS_RESET = 1<<1,
	AOSS_INT = 1<<2,
}AOSS_MODE;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
//AOSS io function
UINT8 AOSS_BitTake(UINT8 nPort, UINT8 nBitNum);
UINT8 AOSS_BitUse(UINT8 nPort, UINT8 nBitNum, UINT8 nMode);
UINT8 AOSS_BitOut(UINT8 nPort, UINT8 nBitNum, UINT8 nBitVal);
UINT8 AOSS_BitPullEnable(UINT8 nPort, UINT8 nBitNum);
UINT8 AOSS_BitPullDisable(UINT8 nPort, UINT8 nBitNum);

//PMU function
void AOSS_ClkSel(UINT8 nClkSel);
UINT8 AOSS_BitPowerDown(UINT8 nPort, UINT8 nBitNum);
UINT8 AOSS_BitWakeUp(UINT8 nPort, UINT8 nBitNum);
UINT8 AOSS_BitPdWuClr(UINT8 nPort, UINT8 nBitNum);
UINT8 AOSS_RtcWakeUpEnable(UINT32 nWakeTime);
void AOSS_PowerOff(void);
BOOL AOSS_ResetOrWakeUp(void);

//BACKUP RAM function
UINT8 AOSS_BKRamInit(UINT16 nAddrDstb, UINT8 nDataDstb);
UINT8 AOSS_WriteBKRam(UINT32 nAddr, UINT8 *pData, UINT32 nLen);
UINT8 AOSS_ReadBKRam(UINT32 nAddr, UINT8 *pData, UINT32 nLen);
//BKKey
UINT8 AOSS_WriteBKKey(UINT8 nAddr, UINT8 *pData, UINT8 nLen);
UINT8 AOSS_ReadBKKey(UINT8 nAddr, UINT8 *pData, UINT8 nLen);

//RTC function
void RTC_Init(UINT32 nInitVal, UINT8 nIntType);
void RTC_Start(void);
void RTC_Stop(void);
void RTC_WrInitVal(UINT32 nLoad);
void RTC_SetIntType(UINT8 nIntType);
UINT8 RTC_GetIntFlag(void);
void RTC_ClrIntFlag(UINT8 nIntType);
UINT32 RTC_ReadData(void);
UINT32 RTC_DataToCV(DATA_FORM sDataForm);
DATA_FORM RTC_CVToData(UINT32 nCCVR);

//DECAP function
UINT8 AOSS_DcpSglInit(UINT8 nChMap, AOSS_MODE nMode);
UINT8 AOSS_DcpSglGetIntFlag(UINT8 nChMap);
void AOSS_DcpSglClrIntFlag(UINT8 nChMap);
UINT8 AOSS_DcpDupInit(UINT8 nChMap, AOSS_MODE nMode);
UINT8 AOSS_DcpDupGetIntFlag(UINT8 nChMap);
void AOSS_DcpDupClrIntFlag(UINT8 nChMap);
void AOSS_DcpClose(void);

//SHIELD function
UINT8 AOSS_SldInit(AOSS_MODE nMode, UINT8 nInitVal);
BOOL AOSS_SldGetIntFlag(void);
void AOSS_SldClrIntFlag(void);
void AOSS_SldClose(void);

//ADC function
void AOSS_ADCInit(UINT8 nChNum, BOOL bIntEn);
BOOL AOSS_ADCGetIntFlag(void);
void AOSS_ADCClrIntFlag(void);
void AOSS_ADCTransfer(void);
UINT8 AOSS_ADCGetData(void);
UINT8 AOSS_ADCAd2Tv(UINT8 nChNum, UINT8 nAd, double* dTV);
UINT8 AOSS_ADCTv2Ad(UINT8 nChNum, UINT8* nAd, UINT32 dTV);

UINT8 AOSS_AutoADCCycleSet(UINT8 nType, UINT16 nCycle);

void AOSS_AutoTempInit(UINT8 nMin, UINT8 nMax, AOSS_MODE nMode);
void AOSS_AutoTempStart(void);
void AOSS_AutoTempStop(void);
BOOL AOSS_AutoTempGetIntFlag(void);
void AOSS_AutoTempClrIntFlag(void);

void AOSS_AutoBvdInit(UINT8 nMin, UINT8 nMax, AOSS_MODE nMode);
void AOSS_AutoBvdStart(void);
void AOSS_AutoBvdStop(void);
BOOL AOSS_AutoBvdGetIntFlag(void);
void AOSS_AutoBvdClrIntFlag(void);

//FD32K function
UINT8 AOSS_Fd32kInit(UINT16 nMin, UINT16 nMax, AOSS_MODE nMode);
void AOSS_Fd32kStart(void);
void AOSS_Fd32kStop(void);
BOOL AOSS_Fd32kGetIntFlag(void);
void AOSS_Fd32kClrIntFlag(void);
UINT16 AOSS_Fd32kGetData(void);

//LDO select function
UINT8 AOSS_VrefValueSel(void);
UINT8 AOSS_Ldo33OutVSel(UINT16 nSelV);
UINT8 AOSS_Ldo18OutVSel(UINT8 nSelV);
UINT8 AOSS_Ldo11OutVSel(UINT8 nSelV);
UINT8 AOSS_Ldo11LpOutVSel(UINT8 nSelV);
void AOSS_LdoInit(void);
/***************************************************************************
* Subroutine:	AOSS_ClkSel
* Function:		select aoss clock
* Input:		nClkSel --- AOSS_LFROSC or AOSS_RTCCLK
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE void AOSS_ClkSel(UINT8 nClkSel)
{
	if(nClkSel == AOSS_RTCCLK) AossRegs.ANMOD_CFG.bit.Osc32kEn = 1;
	AossRegs.PMU_CFG.bit.AossClkSel = nClkSel;
}

/***************************************************************************
* Subroutine:	AOSS_ResetOrWakeUp
* Function:		judge system is reset or wake up
* Input:		None
* Output:		reset return true, wake up return false 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE BOOL AOSS_ResetOrWakeUp(void)
{
	if(AossRegs.RST_MARK == 0xA9) return TRUE;		
	else return FALSE;
}

/***************************************************************************
* Subroutine:	RTC_Start
* Function:		
* Input:		None
* Output:		None 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE void RTC_Start(void)
{
	AossRegs.RTC_CTRL = 1;
}

/***************************************************************************
* Subroutine:	Rtc_Stop
* Function:		
* Input:		None
* Output:		None 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE void RTC_Stop(void)
{
	AossRegs.RTC_CTRL = 0;
}

/****************************************************************************
 * Subroutine:	RTC_GetIntFlag
 * Function:	get RTC interrupt flag
 * Input:		 
 * Output:		rtc interrupt flag	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT8 RTC_GetIntFlag(void)
{
	return (AossRegs.RTC_INTE.all>>3)&0x7;
}

/****************************************************************************
 * Subroutine:	RTC_ClrIntFlag
 * Function:	clr RTC interrupt flag
 * Input:		nIntType -- the interrupt type
							RTC_INT_NO
							RTC_INT_SEC 
							RTC_INT_HSEC
							RTC_INT_MIN	 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void RTC_ClrIntFlag(UINT8 nIntType)
{
	AossRegs.RTC_INTE.all |= (UINT32)(nIntType<<3);
}

/****************************************************************************
 * Subroutine:	RTC_ReadData
 * Function:	read RTC current value
 * Input:		
 * Output:		RTC current count	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT32 RTC_ReadData(void)
{
	return AossRegs.RTC_CCVR[0]+(AossRegs.RTC_CCVR[1]<<8)+
		(AossRegs.RTC_CCVR[2]<<16)+(AossRegs.RTC_CCVR[3]<<24);
}

/****************************************************************************
 * Subroutine:	AOSS_SldGetIntFlag
 * Function:
 * Input:	
 * Output:		shield interrupt flag	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE BOOL AOSS_SldGetIntFlag(void)
{
	return AossRegs.SHIELD_INT;
}

/****************************************************************************
 * Subroutine:	AOSS_SldClrIntFlag
 * Function:
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_SldClrIntFlag(void)
{
	AossRegs.SHIELD_INT = 1;
}

/****************************************************************************
 * Subroutine:	AOSS_SldClose
 * Function:	close shield function
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_SldClose(void)
{
	AossRegs.SHIELD_CTRL.bit.Active = 0;	
}

/****************************************************************************
 * Subroutine:	AOSS_ADCGetIntFlag
 * Function:	adc normal mode get interrupt flag
 * Input:		
 * Output:		ADC trans data	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE BOOL AOSS_ADCGetIntFlag(void)
{
	return AossRegs.INTFLAG.bit.Eoc;	
}

/****************************************************************************
 * Subroutine:	AOSS_ADCClrIntFlag
 * Function:	adc normal mode clear interrupt flag
 * Input:		
 * Output:		ADC trans data	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_ADCClrIntFlag(void)
{
	AossRegs.INTFLAG.all = (1<<0);
}

/****************************************************************************
 * Subroutine:	AOSS_AutoTempStart
 * Function:	auto temperate start
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_AutoTempStart(void)
{
	AossRegs.AUTO_CTRL.bit.Ch0AutoEn = 1;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoTempStop
 * Function:	auto temperate stop
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_AutoTempStop(void)
{
	AossRegs.AUTO_CTRL.bit.Ch0AutoEn = 0;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoTempGetIntFlag
 * Function:	auto temperate get interrupt flag
 * Input:	
 * Output:		interrupt flag	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE BOOL AOSS_AutoTempGetIntFlag(void)
{
	return AossRegs.INTFLAG.bit.Ch0AutoErr;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoTempClrIntFlag
 * Function:	auto temperate clear interrupt flag
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_AutoTempClrIntFlag(void)
{
	AossRegs.INTFLAG.all = (1<<1);	
}

/****************************************************************************
 * Subroutine:	AOSS_AutoBvdStart
 * Function:	auto BVD start
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_AutoBvdStart(void)
{
	AossRegs.AUTO_CTRL.bit.Ch1AutoEn = 1;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoBvdStop
 * Function:	auto BVD stop
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_AutoBvdStop(void)
{
	AossRegs.AUTO_CTRL.bit.Ch1AutoEn = 0;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoBvdGetIntFlag
 * Function:	auto BVD get interrupt flag
 * Input:	
 * Output:		interrupt flag	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE BOOL AOSS_AutoBvdGetIntFlag(void)
{
	return AossRegs.INTFLAG.bit.Ch1AutoErr;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoBvdClrIntFlag
 * Function:	auto BVD clear interrupt flag
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_AutoBvdClrIntFlag(void)
{
	AossRegs.INTFLAG.all = (1<<2);	
}

/****************************************************************************
 * Subroutine:	AOSS_DcpSglGetIntFlag
 * Function:	get decap single channel interrupt flag
 * Input:		nChMap --- channel map, SD_IN0 ~ SD_IN5
 * Output:		single decap interrupt flag	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT8 AOSS_DcpSglGetIntFlag(UINT8 nChMap)
{
	return (AossRegs.DECAP_SMODEINT.all & nChMap);
} 

/****************************************************************************
 * Subroutine:	AOSS_DcpSglClrIntFlag
 * Function:	clear decap single channel interrupt flag
 * Input:		nChMap --- channel map, SD_IN0 ~ SD_IN5
 * Output:		NONE	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_DcpSglClrIntFlag(UINT8 nChMap)
{
	AossRegs.DECAP_SMODEINT.all |= nChMap;
}

/****************************************************************************
 * Subroutine:	AOSS_DcpDupGetIntFlag
 * Function:	get decap dup channel interrupt flag
 * Input:		nChMap --- channel map, SD_IN0 ~ SD_IN2
 * Output:		dup decap interrupt flag		
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT8 AOSS_DcpDupGetIntFlag(UINT8 nChMap)
{
	return (AossRegs.DECAP_DMODEINT.all & nChMap);	
}

/****************************************************************************
 * Subroutine:	AOSS_DcpDupClrIntFlag
 * Function:	clear decap dup channel interrupt flag
 * Input:		nChMap --- channel map, SD_IN0 ~ SD_IN2
 * Output:		NONE	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_DcpDupClrIntFlag(UINT8 nChMap)
{
	AossRegs.DECAP_DMODEINT.all |= nChMap;	
}

/****************************************************************************
 * Subroutine:	AOSS_ADCStart
 * Function:	ADC start transform
 * Input:		
 * Output:		NONE	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_ADCStart(void)
{
	AossRegs.ADC_CTRL.bit.Soc = 1;	
}

/****************************************************************************
 * Subroutine:	AOSS_Fd32kStart
 * Function:
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_Fd32kStart(void)
{
	AossRegs.FD32K_CTRL.bit.FdEn = 1;	
}

/****************************************************************************
 * Subroutine:	AOSS_Fd32kStop
 * Function:
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_Fd32kStop(void)
{
	AossRegs.FD32K_CTRL.bit.FdEn = 0;	
}

/****************************************************************************
 * Subroutine:	AOSS_Fd32kGetIntFlag
 * Function:
 * Input:	
 * Output:		Fd32k interrupt flag	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE BOOL AOSS_Fd32kGetIntFlag(void)
{
	return AossRegs.FD32K_CTRL.bit.FdInt;
}

/****************************************************************************
 * Subroutine:	AOSS_Fd32kClrIntFlag
 * Function:
 * Input:	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE void AOSS_Fd32kClrIntFlag(void)
{
	AossRegs.FD32K_CTRL.bit.FdInt = 1;		
}

/****************************************************************************
 * Subroutine:	AOSS_Fd32k_GetData
 * Function:
 * Input:	
 * Output:		Fd32k output	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
__INLINE UINT16 AOSS_Fd32kGetData(void)
{
	return (AossRegs.FOUT_B1<<8)+AossRegs.FOUT_B0;	
}

#endif /*AOSS_H_*/
