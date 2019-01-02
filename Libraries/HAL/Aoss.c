/***************************************************************************************
* File name    :	Aoss.c
* Function     :	the function of Aoss
* Author       : 	Footman
* Date         :	2016/11/09
* Version      :    v1.0
* Description  :   
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
static UINT8 IsLeapYear(UINT16 nyear);
static UINT8 tm_DataWeek(UINT16 nyear,UINT16 nmonth,UINT16 nday);

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
const UINT8 gnDays[12]={31,28,31,30,31,30,31,31,30,31,30,31};
static double dAdcCh0K = 0;
static double dAdcCh0TempB = 0;
static double dAdcCh1Vstep = 0;
static double dAdcCh1TempB = 0;
static double dAdcCh23Vstep = 0;
static double dAdcCh23TempB = 0;
/**************************************************************************
* Local Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/

/***************************************************************************
* Subroutine:	AOSS_BitTake
* Function:		Set Aoss io take bit
* Input:		nPort --- 	GPIOA or GPIOB
				nBitNum ---	bit number 
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitTake(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT8 *pUse;
	UINT8 temp;

	if(nBitNum > 32) return RT_PARAM_ERR;
	if(nPort == GPIOA) pUse = (volatile UINT8 *)&(AossRegs.GPIO_TAKE.all[0]);
	else pUse = (volatile UINT8 *)&(AossRegs.GPIO_TAKE.all[4]);

	pUse += nBitNum/8;
	temp = (nBitNum%8);
	*pUse |= 0x1<<temp;

	if((*pUse>>temp)&0x1) return RT_OK;
	else return RT_FAIL;	
}

/***************************************************************************
* Subroutine:	AOSS_BitUse
* Function:		Set Aoss port use
* Input:		nPort --- 	GPIOA or GPIOB
				nBitNum ---	bit number
				nMode ---	INPUT or OUTPUT
* Output:		RT_OK or RT_FAIL 
* Description:	input function can't read data, just for other use
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitUse(UINT8 nPort, UINT8 nBitNum, UINT8 nMode)
{
	volatile UINT8 *pUse;
	UINT8 temp;

	if(nBitNum > 32) return RT_PARAM_ERR;

	if(nPort == GPIOA) pUse = (volatile UINT8 *)&(AossRegs.GPIO_OEN.all[0]);
	else pUse = (volatile UINT8 *)&(AossRegs.GPIO_OEN.all[4]);

	pUse += nBitNum/8;
	temp = (nBitNum%8);
	if(nMode == INPUT) nMode = 1;
	else nMode = 0;
	*pUse &= ~(0x1<<temp);
	*pUse |= nMode<<temp;

	if(((*pUse>>temp)&0x1) == nMode) return RT_OK;
	else return RT_FAIL;	
}

/***************************************************************************
* Subroutine:	AOSS_BitOut
* Function:		Set Aoss port output value
* Input:		nPort --- 	GPIOA or GPIOB
				nBitNum ---	bit number
				nBitVal ---	bit value, 1 or 0
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitOut(UINT8 nPort, UINT8 nBitNum, UINT8 nBitVal)
{
	volatile UINT8 *pData;
	UINT8 temp;

	if(nPort == GPIOA) pData = (volatile UINT8 *)&(AossRegs.GPIO_DOUT.all[0]);
	else pData = (volatile UINT8 *)&(AossRegs.GPIO_DOUT.all[4]);

	pData += nBitNum/8;
	temp = (nBitNum%8);
	*pData &= ~(1<<temp);
	*pData |= (nBitVal<<temp);

	return RT_OK;	
}

/***************************************************************************
* Subroutine:	AOSS_BitPullEnable
* Function:		enable pull resistor
* Input:		nPort --- 	GPIOA or GPIOB
				nBitNum ---	bit number
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitPullEnable(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT8 *pData;
	UINT8 temp;

	if(nPort == GPIOA) pData = (volatile UINT8 *)&(AossRegs.GPIO_REN.all[0]);
	else pData = (volatile UINT8 *)&(AossRegs.GPIO_REN.all[4]);

	pData += nBitNum/8;
	temp = (nBitNum%8);
	*pData &= ~(1<<temp);

	return RT_OK;
}

/***************************************************************************
* Subroutine:	AOSS_BitPullDisable
* Function:		disable pull resistor
* Input:		nPort --- 	GPIOA or GPIOB
				nBitNum ---	bit number
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitPullDisable(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT8 *pData;
	UINT8 temp;

	if(nPort == GPIOA) pData = (volatile UINT8 *)&(AossRegs.GPIO_REN.all[0]);
	else pData = (volatile UINT8 *)&(AossRegs.GPIO_REN.all[4]);

	pData += nBitNum/8;
	temp = (nBitNum%8);
	*pData |= (1<<temp);

	return RT_OK;
}

/***************************************************************************
* Subroutine:	AOSS_BKRamInit
* Function:		BKRAM: 0x2000_CD00~0x2000_D0FF
* Input:		nAddrDstb ---	address disturb value, 0~0xFFF
				nDataDstb ---	data disturb value
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BKRamInit(UINT16 nAddrDstb, UINT8 nDataDstb)
{
	AossRegs.BKRAM_ADDR_KEY0 = (nAddrDstb&0xff);
	AossRegs.BKRAM_ADDR_KEY1.all = (nAddrDstb>>8);
	AossRegs.BKRAM_DATA_KEY = nDataDstb;

	return RT_OK;
}

/***************************************************************************
* Subroutine:	AOSS_WriteBKRam
* Function:		write BKRAM
* Input:		nAddr ---	write address, 0~1023
				pData ---	data pointer
				nLen ---	data len
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_WriteBKRam(UINT32 nAddr, UINT8 *pData, UINT32 nLen)
{
	UINT8 *pBKRAM = (UINT8 *)BKRAM_START;
	UINT32 i;	

	pBKRAM += nAddr;

	for(i=0;i<nLen;i++)
	{
		*(pBKRAM+i) = *(pData+i);
	}

	for(i=0;i<nLen;i++)
	{
		if(*(pBKRAM+i) != *(pData+i)) return RT_FAIL;
	}

	return RT_OK;
}

/***************************************************************************
* Subroutine:	AOSS_ReadBKRam
* Function:		read BKRAM
* Input:		nAddr ---	read address, 0~1023
				pData ---	data pointer
				nLen ---	data len
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_ReadBKRam(UINT32 nAddr, UINT8 *pData, UINT32 nLen)
{
	UINT8 *pBKRAM = (UINT8 *)BKRAM_START;
	UINT32 i;	

	pBKRAM += nAddr;

	for(i=0;i<nLen;i++)
	{
		*(pData+i) = *(pBKRAM+i);
	}

	for(i=0;i<nLen;i++)
	{
		if(*(pBKRAM+i) != *(pData+i)) return RT_FAIL;
	}

	return RT_OK;	
}

/***************************************************************************
* Subroutine:	AOSS_WriteBKKey
* Function:		BKKey: 0x500400e0~0x500400ff
* Input:		nAddr ---	read address, 0~31
				pData ---	data pointer
				nLen ---	data len
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_WriteBKKey(UINT8 nAddr, UINT8 *pData, UINT8 nLen)
{
	UINT8 *pBKKey = (UINT8 *)BKKEY_START;
	UINT32 i;	

	pBKKey += nAddr;

	for(i=0;i<nLen;i++)
	{
		*(pBKKey+i) = *(pData+i);
	}

	for(i=0;i<nLen;i++)
	{
		if(*(pBKKey+i) != *(pData+i)) return RT_FAIL;
	}

	return RT_OK;	
}

/***************************************************************************
* Subroutine:	AOSS_ReadBKKey
* Function:		BKKey: 0x500400e0~0x500400ff
* Input:		nAddr ---	read address, 0~31
				pData ---	data pointer
				nLen ---	data len
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_ReadBKKey(UINT8 nAddr, UINT8 *pData, UINT8 nLen)
{
	UINT8 *pBKKey = (UINT8 *)BKKEY_START;
	UINT32 i;	

	pBKKey += nAddr;

	for(i=0;i<nLen;i++)
	{
		*(pData+i) = *(pBKKey+i);
	}

	for(i=0;i<nLen;i++)
	{
		if(*(pBKKey+i) != *(pData+i)) return RT_FAIL;
	}

	return RT_OK;	
}

/***************************************************************************
* Subroutine:	AOSS_BitPowerDown
* Function:		Set Aoss power off tigger port
* Input:		nPort ---	GPIOA or GPIOB
				nBitNum ---	bit number
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitPowerDown(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT8 *pUse;
	UINT8 temp;

	AossRegs.RST_MARK = 0x55;	//set mask
	AossRegs.PMU_CFG.bit.ExtSleepEn = 0;

	GPIO_Init(nPort, nBitNum, INPUT);
	AOSS_BitUse(nPort, nBitNum, INPUT);
	AossRegs.PD_FLT = 0xff;

	if(nPort == GPIOA) pUse = (volatile UINT8 *)&(AossRegs.PD_PINS.all[0]);
	else pUse = (volatile UINT8 *)&(AossRegs.PD_PINS.all[4]);

	pUse += nBitNum/8;
	temp = (nBitNum%8);
	*pUse |= 0x1<<temp;
	AossRegs.PMU_CFG.bit.ExtSleepEn = 1;

	if(((*pUse>>temp)&0x1) == 0x1) return RT_OK;
	else return RT_FAIL;	
}

/***************************************************************************
* Subroutine:	AOSS_BitWakeUp
* Function:		Set Aoss wake up tigger port 
* Input:		nPort ---	GPIOA or GPIOB
				nBitNum ---	bit number
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitWakeUp(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT8 *pUse;
	UINT8 temp;

	GPIO_Init(nPort, nBitNum, INPUT);
	AOSS_BitUse(nPort, nBitNum, INPUT);
	AossRegs.WAK_FLT = 0xff;

	if(nPort == GPIOA) pUse = (volatile UINT8 *)&(AossRegs.WAK_PINS.all[0]);
	else pUse = (volatile UINT8 *)&(AossRegs.WAK_PINS.all[4]);

	pUse += nBitNum/8;
	temp = (nBitNum%8);
	*pUse |= 0x1<<temp;

	if(((*pUse>>temp)&0x1) == 0x1) return RT_OK;
	else return RT_FAIL;	
}

/***************************************************************************
* Subroutine:	AOSS_BitPdWuClr
* Function:		Clr Aoss wake up or power off tigger port 
* Input:		nPort ---	GPIOA or GPIOB
				nBitNum ---	bit number
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_BitPdWuClr(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT8 *pWuUse;
	volatile UINT8 *pPdUse;
	UINT8 temp;

	if(nPort == GPIOA) pWuUse = (volatile UINT8 *)&(AossRegs.WAK_PINS.all[0]);
	else pWuUse = (volatile UINT8 *)&(AossRegs.WAK_PINS.all[4]);

	if(nPort == GPIOA) pPdUse = (volatile UINT8 *)&(AossRegs.PD_PINS.all[0]);
	else pPdUse = (volatile UINT8 *)&(AossRegs.PD_PINS.all[4]);

	pWuUse += nBitNum/8;
	pPdUse += nBitNum/8;
	temp = (nBitNum%8);
	*pWuUse &= (~(0x1<<temp));
	*pPdUse &= (~(0x1<<temp));

	if((((*pWuUse>>temp)&0x1) == 0x0)&&(((*pPdUse>>temp)&0x1) == 0x0)) return RT_OK;
	else return RT_FAIL;
}

/***************************************************************************
* Subroutine:	AOSS_PowerOff
* Function:		Aoss software power off
* Input:		 
* Output:		NONE 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
void AOSS_PowerOff(void)
{
	AossRegs.RST_MARK = 0x55;
	AossRegs.PMU_CFG.bit.LdoDis = 1;		//Ldo-H close
	AossRegs.PMU_CFG.bit.VddaIoDis = 1;		//Vdda io close
	AossRegs.PMU_CFG.bit.BkRamRetEn = 1;	//bkram close
	AossRegs.PMU_CFG.bit.LdoStepEn = 1;		//enhance power-down mode 
	AossRegs.PMU_CFG.bit.VrefDis = 1;
	AossRegs.PMU_LDO_STEP_TAR = 7;
	AossRegs.ANMOD_CFG.bit.AvrLpPd = 0;
//	AossRegs.ANMOD_CFG.bit.Osc32kEn=0;
	AossRegs.ANMOD_CFG.bit.UsbPhyPowEn=0;
	AossRegs.PMU_PD_CMD = 1;	//power down	
}

/***************************************************************************
* Subroutine:	AOSS_RtcWakeUpEnable
* Function:		enable rtc wake up function
* Input:		nWakeTime ---	wake up delay, (S)
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_RtcWakeUpEnable(UINT32 nWakeTime)
{
	UINT8 i;

	for(i=0;i<4;i++) AossRegs.RTC_WCVR[i] = (nWakeTime>>(8*i))&0xff;

	AossRegs.PMU_CFG.bit.RtcWakeEn = 1;		
	return RT_OK;			
}

/****************************************************************************
 * Subroutine:	RTC_Init
 * Function:	RTC initialization
 * Input:		nInitVal ---	the initial value for counter
 *				nIntType ---	the interrupt type
 								RTC_INT_NO
								RTC_INT_SEC 
								RTC_INT_HSEC
								RTC_INT_MIN				
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void RTC_Init(UINT32 nInitVal, UINT8 nIntType)
{	
	RTC_WrInitVal(nInitVal);
	RTC_SetIntType(nIntType);		
}

/****************************************************************************
 * Subroutine:	RTC_WrInitVal
 * Function:	write RTC initial value
 * Input:		nLoad -- initial value
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void RTC_WrInitVal(UINT32 nLoad)
{
	UINT8 i, rtcEn =  AossRegs.RTC_CTRL;

	AossRegs.RTC_CTRL = 0;
	for(i=0;i<4;i++)
	{
		AossRegs.RTC_LOAD[i] = (nLoad>>(8*i));
	}

	AossRegs.RTC_CTRL = rtcEn;	
}

/****************************************************************************
 * Subroutine:	RTC_SetIntType
 * Function:	set RTC interrupt type
 * Input:		nIntType -- the interrupt type
 							RTC_INT_NO
							RTC_INT_SEC
							RTC_INT_HSEC
							RTC_INT _MIN
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void RTC_SetIntType(UINT8 nIntType)
{
	AossRegs.RTC_INTE.all = (UINT32)nIntType;
	if(nIntType)
	{
		AossRegs.RTC_INTE.bit.GlobalInt = 1;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();
	}
	else AossRegs.RTC_INTE.bit.GlobalInt = 0; 
}

/****************************************************************************
 * Subroutine:	IsLeapYear
 * Function:	Leap to judge
 * Input:		nyear				
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
static UINT8 IsLeapYear(UINT16 nyear)  
{ 
    if(((nyear%4==0)&&(nyear%100!=0))||(nyear%400==0)) return TRUE; 
    return FALSE; 
}

/****************************************************************************
 * Subroutine:	tm_DataWeek
 * Function:	Known date calculation weeks
 * Input:		UINT16 nyear
 *				UINT16 nmonth
 *				UINT16 nday
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
static UINT8 tm_DataWeek(UINT16 nyear,UINT16 nmonth,UINT16 nday)
{
	if (nmonth==1||nmonth==2)
	{
		nyear -=1;		
		nmonth +=12;	
	}
	return (nday+1+2*nmonth+3*(nmonth+1)/5+nyear+(nyear/4)-nyear/100+nyear/400)%7;
}

/****************************************************************************
 * Subroutine:	RTC_DataToCV
 * Function:	to translate date into the counter value
 * Input:		DATA_FORM sDataForm
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT32 RTC_DataToCV(DATA_FORM sDataForm)
{
    UINT32 nSum;
    UINT16 ni;
    
    nSum=0;
    for(ni=2000;ni<sDataForm.year;ni++)
    {
        nSum+=365;
        if(IsLeapYear(ni))
        {
            nSum+=1;
        }
    }
    
    for(ni=1;ni<sDataForm.month;ni++)
    {
        nSum+=gnDays[ni-1];
    }

    if(sDataForm.month>2)
    {
        if(IsLeapYear(sDataForm.year))
        {
            nSum+=1;
        }
    }

    nSum += sDataForm.day - 1;
    nSum = nSum*24*60;
    nSum += sDataForm.hour*60 + sDataForm.minute;
	nSum <<= 6;
	nSum += sDataForm.second;
    
    return nSum; 
}

/****************************************************************************
 * Subroutine:	RTC_CVToData
 * Function:	Counter value into a date
 * Input:		UINT32 nCCVR -- the current counter value
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
DATA_FORM RTC_CVToData(UINT32 nCCVR)
{
    DATA_FORM sDataForm;
    UINT16  nday,nsum;
	UINT32	nMinute;
	UINT8	nSecond;

	nSecond = (nCCVR & 0x3F);
	nMinute = (nCCVR >> 6);
    
    sDataForm.year=2000;
    sDataForm.month=1;
    sDataForm.day=1;
    sDataForm.hour=0;
    sDataForm.minute=0;
    sDataForm.second=0;    
    nday = nMinute/1440;	
	nMinute -= nday*1440;
    sDataForm.hour = nMinute/60;
    nMinute -= sDataForm.hour*60;
	sDataForm.minute = nMinute;
    sDataForm.second = nSecond;
    
    while(1)
    {
        nsum=365; 
        if( IsLeapYear(sDataForm.year)) 
        {            
            nsum+=1; 
        }
        if(nday<nsum)
        {         
            break;
        }
        sDataForm.year++;
        nday -=nsum;
    }
    
    while(1)
    {
        nsum=gnDays[sDataForm.month-1];
        if(sDataForm.month==2)
        {
            if(IsLeapYear(sDataForm.year)) 
            {     
                nsum+=1; 
            }
        }
        if(nday<nsum)
        {            
            sDataForm.day+=nday;
            break;
        }
        sDataForm.month++;
        nday-=nsum;
    }
    sDataForm.week = tm_DataWeek(sDataForm.year,sDataForm.month,sDataForm.day);

    return sDataForm;
}

/****************************************************************************
 * Subroutine:	AOSS_SldInit
 * Function:	shield init
 * Input:		nMode ---	shield exception mode
 				nInitVal --- init value
 * Output:		RT_OK	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_SldInit(AOSS_MODE nMode, UINT8 nInitVal)
{
	AossRegs.SHIELD_INITVAL = nInitVal;

	if(nMode & AOSS_INT)
	{
		AossRegs.SHIELD_INTEN = 1;
		AossRegs.SHIELD_INT = 1;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();		
	}
	if(nMode & AOSS_SD) AossRegs.SHIELD_CTRL.bit.SdEn = 1;
	if(nMode & AOSS_RESET) AossRegs.SHIELD_CTRL.bit.RstEn = 1;

	AossRegs.SHIELD_CTRL.bit.Active = 1;
	
	return RT_OK;	
}

/****************************************************************************
 * Subroutine:	AOSS_DcpSglInit
 * Function:	init single channel decap function
 * Input:		nChMap ---	channel map, SD_IN0 ~ SD_IN5
 				nMode --- 	decap exception mode
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_DcpSglInit(UINT8 nChMap, AOSS_MODE nMode)
{
	UINT16 modeCtrl = 0, temp = 0;
	UINT8 i;

	for(i=0;i<6;i++) 
	{
		temp |= ((1&(nChMap>>i))<<(i*2)); 
		AossRegs.DECAP_DMODECTRL1.all &= ~((1&(nChMap>>i))<<(i/2));	//disable double channel
	}

	if(nMode & AOSS_INT) 
	{
		AossRegs.DECAP_SMODEINTEN.all = nChMap;
		AossRegs.DECAP_SMODEINT.all = nChMap;
		AossRegs.DECAP_SMODECTRL2.all = nChMap;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();
		
		return RT_OK;
	}

	if(nMode & AOSS_RESET) modeCtrl = temp;
	if(nMode & AOSS_SD) modeCtrl = (temp<<1);		
	AossRegs.DECAP_SMODECTRL0.all = modeCtrl;
	AossRegs.DECAP_SMODECTRL1.all = modeCtrl>>8;
	AossRegs.DECAP_SMODECTRL2.all = nChMap;			
	
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	AOSS_DcpDupInit
 * Function:	init duplicate channel decap function
 * Input:		nChMap --- channel map, SD_IN0 ~ SD_IN2
 				nMode --- 	decap exception mode
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_DcpDupInit(UINT8 nChMap, AOSS_MODE nMode)
{
	UINT8 i, modeCtrl = 0, temp = 0;

	if(nChMap & (SD_IN3|SD_IN4|SD_IN5)) return RT_FAIL;
	for(i=0;i<3;i++) temp |= ((1&(nChMap>>i))<<(i*2));

	AossRegs.DECAP_SMODECTRL2.all &= ~(temp|(temp<<1));//disable single channel

	if(nMode & AOSS_INT) 
	{
		AossRegs.DECAP_DMODEINTEN.all = nChMap;
		AossRegs.DECAP_DMODEINT.all = nChMap;
		AossRegs.SHIELD_CTRL.bit.Active = 1;
		AossRegs.DECAP_DMODECTRL1.all = nChMap;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();
	}

	if(nMode & AOSS_RESET) modeCtrl = temp;
	if(nMode & AOSS_SD) modeCtrl = (temp<<1);	

	AossRegs.DECAP_DMODECTRL0.all = modeCtrl;
	AossRegs.SHIELD_CTRL.bit.Active = 1;
	AossRegs.DECAP_DMODECTRL1.all = nChMap;			

	return RT_OK;		
}

/****************************************************************************
 * Subroutine:	AOSS_DcpClose
 * Function:	close single and double channel decap function
 * Input:		
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void AOSS_DcpClose(void)
{
	AossRegs.DECAP_SMODEINTEN.all = 0;
	AossRegs.DECAP_SMODECTRL0.all = 0;
	AossRegs.DECAP_SMODECTRL0.all = 0;
	AossRegs.DECAP_DMODEINTEN.all = 0;
	AossRegs.DECAP_DMODECTRL0.all = 0;
	AossRegs.SHIELD_CTRL.bit.Active = 0;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoTempInit
 * Function:	auto temerate init
 * Input:		nMin ---	compare min value
				nMax ---	compare max value
				nMode ---	ADC exception mode
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void AOSS_AutoTempInit(UINT8 nMin, UINT8 nMax, AOSS_MODE nMode)
{
	AossRegs.CH0_LOW = nMin;
	AossRegs.CH0_HIGH = nMax;
	if(nMode == AOSS_INT) 
	{
		AossRegs.INTEN.bit.Ch0AutoErr = 1;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();
	}
	if(nMode == AOSS_SD) AossRegs.AUTO_CTRL.bit.Ch0SdEn = 1;
	if(nMode == AOSS_RESET)	AossRegs.AUTO_CTRL.bit.Ch0RstEn = 1;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoBvdInit
 * Function:	auto BVD init
 * Input:		nMin ---	compare min value
 *				nMax ---	compare max value
 *				nMode ---	ADC exception mode
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void AOSS_AutoBvdInit(UINT8 nMin, UINT8 nMax, AOSS_MODE nMode)
{
	AossRegs.CH1_LOW = nMin;
	AossRegs.CH1_HIGH = nMax;
	if(nMode == AOSS_INT) 
	{
		AossRegs.INTEN.bit.Ch1AutoErr = 1;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();
	}
	if(nMode == AOSS_SD) AossRegs.AUTO_CTRL.bit.Ch1SdEn = 1;
	if(nMode == AOSS_RESET)	AossRegs.AUTO_CTRL.bit.Ch1RstEn = 1;
}

/****************************************************************************
 * Subroutine:	AOSS_AutoADCCycleSet
 * Function:	auto ADC conversion cycle set
 * Input:		nType ---	Auto ADC type: ADC_TEMP;ADC_BVD
 *				nCycle ---	conversion cycle	
 * Output:		None	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_AutoADCCycleSet(UINT8 nType, UINT16 nCycle)
{
	if(nType>ADC_BVD) return RT_PARAM_ERR;
	if(nType == ADC_TEMP)
	{
		AossRegs.CH0_INTERVAL_B0 = nCycle&0xFF;
		AossRegs.CH0_INTERVAL_B1 = (nCycle>>8)&0xFF;
	}
	else
	{
		AossRegs.CH1_INTERVAL_B0 = nCycle&0xFF;
		AossRegs.CH1_INTERVAL_B1 = (nCycle>>8)&0xFF;		
	}
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	AOSS_ADCInit
 * Function:	init ADC normal mode
 * Input:		nChNum --- channel number, 0~3
 				bIntEn --- interrupt enable
 * Output:		ADC trans data	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void AOSS_ADCInit(UINT8 nChNum, BOOL bIntEn)
{
	AossRegs.CH_SEL = nChNum;
	if(bIntEn) 
	{
		AossRegs.INTEN.bit.Eoc = 1;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();
	}
//	AossRegs.AUTO_CTRL.all &= ~(0x3);	//disable auto trans mode
	AossRegs.RSTDEL = 0xf;
	AossRegs.SOCDEL = 0x2;
}

/****************************************************************************
 * Subroutine:	AOSS_ADCGetData
 * Function:	adc get trans data
 * Input:		
 * Output:		ADC trans data	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_ADCGetData(void)
{	
	while(!AossRegs.INTFLAG.bit.Eoc);
	AossRegs.INTFLAG.bit.Eoc = 1;
	return AossRegs.DOUT&0x7F; 
}

/****************************************************************************
 * Subroutine:	AOSS_ADCGetData
 * Function:	start ADC once transfer
 * Input:		
 * Output:		ADC trans data	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
void AOSS_ADCTransfer(void)
{
	AossRegs.ADC_CTRL.bit.Resetn = 0;	//start adc	normal mode
	AossRegs.ADC_CTRL.bit.Soc = 1;	
}

/****************************************************************************
 * Subroutine:	AOSS_ADCAd2Tv
 * Function:	Calculate the temperature or voltage value corresponding to the ADC conversion value
 * Input:		nChNum ---	channel number, CH0~3
 *				nAd ---	ADC conversion value	
 * Output:		dTV ---	temperature or voltage value 	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_ADCAd2Tv(UINT8 nChNum, UINT8 nAd, double* dTV)
{
//	double dAdcCh0K;
	UINT8 nData,nData1;
	double dTempTV=0;

	if(nChNum>ADC_CH3) return RT_PARAM_ERR;

	if(nChNum == ADC_TEMP)
	{
//		dAdcCh0K = 1.627136;
	 	if(dAdcCh0K == 0)
		{
			nData = *(UINT8*)ADC_CH0_ADDR;
			nData1 = *(UINT8*)ADC_CH0_ADDR1;
			if((nData <= 0x7F)&&(nData1 <= 0x7F)&&(nData1>nData))
			{
				dAdcCh0K = (double)(80-25)/(nData1-nData);	
			}
			else
				dAdcCh0K = 1.627136;
		}
	 	if(dAdcCh0TempB == 0)
		{
			nData = *(UINT8*)ADC_CH0_ADDR;
			if(nData <= 0x7F)
			{
				dAdcCh0TempB = (double)25-dAdcCh0K*nData;	
			}
			else
				dAdcCh0TempB = -77.189097;
		}
		dTempTV = dAdcCh0K*nAd+dAdcCh0TempB;
	}
	else if(nChNum == ADC_BVD)
	{
		if(dAdcCh1Vstep == 0)
		{
			nData = *(UINT8*)ADC_CH1_ADDR;
			nData1 = *(UINT8*)ADC_CH1_ADDR1;
			if((nData <= 0x7F)&&(nData1 <= 0x7F)&&(nData1<nData))
			{
				dAdcCh1Vstep = (double)(ADC_CH1_VALUE-ADC_CH1_VALUE1)/(nData-nData1);
				dAdcCh1TempB = (double)ADC_CH1_VALUE-dAdcCh1Vstep*nData;
			}
			else if((nData <= 0x7F)&&(nData1 == 0xFF))
			{
				dAdcCh1Vstep = (double)800/(nData+128);
				dAdcCh1TempB = dAdcCh1Vstep*128;	
			}
			else
			{
				dAdcCh1Vstep = 4;
				dAdcCh1TempB = 512;
			}
		}
//		dTempTV = dAdcCh1Vstep*(nAd+128);
		dTempTV = dAdcCh1Vstep*nAd+dAdcCh1TempB;				
	}
	else
	{
		if(dAdcCh23Vstep == 0)
		{
			nData = *(UINT8*)ADC_CH23_ADDR;
			nData1 = *(UINT8*)ADC_CH23_ADDR1;
			if((nData <= 0x7F)&&(nData1 <= 0x7F)&&(nData1<nData))
			{
				dAdcCh23Vstep = (double)(ADC_CH23_VALUE-ADC_CH23_VALUE1)/(nData-nData1);
				dAdcCh23TempB = (double)ADC_CH23_VALUE-dAdcCh23Vstep*nData;
			}
			else if((nData <= 0x7F)&&(nData1 == 0xFF))
			{
				dAdcCh23Vstep = (double)800/nData;
				dAdcCh23TempB = 0;	
			}
			else
			{
				dAdcCh23Vstep = 8;
				dAdcCh23TempB = 0;
			}
		}
//		dTempTV = dAdcCh23Vstep*nAd;
		dTempTV = dAdcCh23Vstep*nAd+dAdcCh23TempB;
	}
	*dTV = dTempTV;
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	AOSS_ADCTv2Ad
 * Function:	Calculate the temperature or voltage value corresponding to the ADC conversion value
 * Input:		nChNum ---	channel number, CH0~3
 *				dTV ---	temperature or voltage value	
 * Output:		nAd ---	ADC conversion value 	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_ADCTv2Ad(UINT8 nChNum, UINT8* nAd, UINT32 dTV)
{
//	double dAdcCh0K;
	UINT8 nData,nData1;
	UINT8 nTempAd=0;

	if(nChNum>ADC_CH3) return RT_PARAM_ERR;

	if(nChNum == ADC_TEMP)
	{
//		dAdcCh0K = 1.627136;
	 	if(dAdcCh0K == 0)
		{
			nData = *(UINT8*)ADC_CH0_ADDR;
			nData1 = *(UINT8*)ADC_CH0_ADDR1;
			if((nData <= 0x7F)&&(nData1 <= 0x7F)&&(nData1>nData))
			{
				dAdcCh0K = (double)(80-25)/(nData1-nData);
			}
			else
				dAdcCh0K = 1.627136;
		}
	 	if(dAdcCh0TempB == 0)
		{
			nData = *(UINT8*)ADC_CH0_ADDR;
			if(nData <= 0x7F)
			{
				dAdcCh0TempB = (double)25-dAdcCh0K*nData;
			}
			else
				dAdcCh0TempB = -77.189097;
		}
		nTempAd = (dTV-dAdcCh0TempB+dAdcCh0K/2)/dAdcCh0K;
	}
	else if(nChNum == ADC_BVD)
	{
		if(dAdcCh1Vstep == 0)
		{
			nData = *(UINT8*)ADC_CH1_ADDR;
			nData1 = *(UINT8*)ADC_CH1_ADDR1;
			if((nData <= 0x7F)&&(nData1 <= 0x7F)&&(nData1<nData))
			{
				dAdcCh1Vstep = (double)(ADC_CH1_VALUE-ADC_CH1_VALUE1)/(nData-nData1);
				dAdcCh1TempB = (double)ADC_CH1_VALUE-dAdcCh1Vstep*nData;
			}
			else if((nData <= 0x7F)&&(nData1 == 0xFF))
			{
				dAdcCh1Vstep = (double)800/(nData+128);
				dAdcCh1TempB = dAdcCh1Vstep*128;	
			}
			else
			{
				dAdcCh1Vstep = 4;
				dAdcCh1TempB = 512;
			}
		}
//		nTempAd	= dTV/dAdcCh1Vstep-128;
		nTempAd	= (dTV-dAdcCh1TempB+dAdcCh1Vstep/2)/dAdcCh1Vstep;	
	}
	else
	{
		if(dAdcCh23Vstep == 0)
		{
			nData = *(UINT8*)ADC_CH23_ADDR;
			nData1 = *(UINT8*)ADC_CH23_ADDR1;
			if((nData <= 0x7F)&&(nData1 <= 0x7F)&&(nData1<nData))
			{
				dAdcCh23Vstep = (double)(ADC_CH23_VALUE-ADC_CH23_VALUE1)/(nData-nData1);
				dAdcCh23TempB = (double)ADC_CH23_VALUE-dAdcCh23Vstep*nData;
			}
			else if((nData <= 0x7F)&&(nData1 == 0xFF))
			{
				dAdcCh23Vstep = (double)800/nData;
				dAdcCh23TempB = 0;	
			}
			else
			{
				dAdcCh23Vstep = 8;
				dAdcCh23TempB = 0;
			}
		}
//		nTempAd	= dTV/dAdcCh23Vstep;
		nTempAd	= (dTV-dAdcCh23TempB+dAdcCh23Vstep/2)/dAdcCh23Vstep;	
	}
	*nAd = nTempAd;
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	AOSS_Fd32kInit
 * Function:	Fd32K init
 * Input:		nMin ---	compare min value
 				nMax ---	compare max value
				nMode ---	exception mode
 * Output:		RT_OK	
 * Description:	 
 * Date:		2012.07.27
 * ModifyRecord:
 * *************************************************************************/
UINT8 AOSS_Fd32kInit(UINT16 nMin, UINT16 nMax, AOSS_MODE nMode)
{
	AossRegs.MAX_B0 = nMax;
	AossRegs.MAX_B1 = (nMax>>8);
	AossRegs.MIN_B0 = nMin;
	AossRegs.MIN_B1 = (nMin>>8);

	if(nMode & AOSS_SD) AossRegs.FD32K_CTRL.bit.SdEn = 1;
	if(nMode & AOSS_RESET) AossRegs.FD32K_CTRL.bit.RstEn = 1;	
	if(nMode & AOSS_INT) 
	{
		AossRegs.FD32K_CTRL.bit.FdIntEn = 1;
		NVIC_EnableIRQ(AOSS_IRQn);
		__enable_irq();
	}
	
	return RT_OK;	
}

/***************************************************************************
* Subroutine:	AOSS_VrefValueSel
* Function:		Select the vref voltage gear  
* Input:		
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2016.5.5
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_VrefValueSel(void)
{
	UINT8 nVref = (*(UINT8*)VREF_VALUE_ADDR);

	if(nVref<8)
	{
		AossRegs.ANMOD_CFG.bit.VrefTrim = nVref;
		_delay_us(100);
		return RT_OK;
	}
	else
		return RT_FAIL;
}

/***************************************************************************
* Subroutine:	AOSS_Ldo33OutVSel
* Function:		Select the output voltage gear of LDO33 
* Input:		UINT16 nSelV	-- 	to specify the voltage gear to be selected;
*									3.3V: 3.3*100=330=0x14A
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2016.5.5
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_Ldo33OutVSel(UINT16 nSelV)
{
	UINT8 nVdata[LDO33OUT_NUM];
	UINT8 nTargetV = nSelV-LDO33OUT_REF_V;
	UINT8 i;
	UINT8 nTargetG=0xff,nTargetTempV = 0xff; 

	memcpy(nVdata,(UINT8*)LDO33OUT_VALUE_ADDR,LDO33OUT_NUM);
	for(i=0;i<LDO33OUT_NUM;i++)
	{
		if(nVdata[i] != 0xff)
		{
			if(nVdata[i]>=nTargetV) nVdata[i]-=nTargetV;
			else nVdata[i]=nTargetV-nVdata[i];

			if((nVdata[i]<=LDO33OUT_RANGE)&&(nVdata[i]<nTargetTempV))
			{
				nTargetTempV = nVdata[i];
				nTargetG = i;
			}
		}
	}

	if(nTargetG<LDO33OUT_NUM)
	{
		AossRegs.ANMOD_CFG.bit.Ldo5033Sel = nTargetG;
		_delay_us(100);
		return RT_OK;
	}
	else
		return RT_FAIL;
}

/***************************************************************************
* Subroutine:	AOSS_Ldo18OutVSel
* Function:		Select the output voltage gear of LDO18 
* Input:		UINT8 nSelV	-- 	to specify the voltage gear to be selected;
*									1.8V: 1.8*100=180=0xB4
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2016.5.5
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_Ldo18OutVSel(UINT8 nSelV)
{
	UINT8 nVdata[LDO18OUT_NUM];
	UINT8 nTargetV = nSelV;
	UINT8 i;
	UINT8 nTargetG=0xff,nTargetTempV = 0xff; 

	memcpy(nVdata,(UINT8*)LDO18OUT_VALUE_ADDR,LDO18OUT_NUM);
	for(i=0;i<LDO18OUT_NUM;i++)
	{
		if(nVdata[i] != 0xff)
		{
			if(nVdata[i]>=nTargetV) nVdata[i]-=nTargetV;
			else nVdata[i]=nTargetV-nVdata[i];

			if((nVdata[i]<=LDO18OUT_RANGE)&&(nVdata[i]<nTargetTempV))
			{
				nTargetTempV = nVdata[i];
				nTargetG = i;
			}
		}
	}

	if(nTargetG<LDO18OUT_NUM)
	{
		AossRegs.ANMOD_CFG.bit.Ldo3318Sel = nTargetG;
		_delay_us(100);
		return RT_OK;
	}
	else
		return RT_FAIL;
}

/***************************************************************************
* Subroutine:	AOSS_Ldo11OutVSel
* Function:		Select the output voltage gear of LDO11 
* Input:		UINT8 nSelV	-- 	to specify the voltage gear to be selected;
*									1.8V: 1.8*100=180=0xB4
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2016.5.5
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_Ldo11OutVSel(UINT8 nSelV)
{
	UINT8 nVdata[LDO11OUT_NUM];
	UINT8 nTargetV = nSelV;
	UINT8 i;
	UINT8 nTargetG=0xff,nTargetTempV = 0xff; 

	memcpy(nVdata,(UINT8*)LDO11OUT_VALUE_ADDR,LDO11OUT_NUM);
	for(i=0;i<LDO11OUT_NUM;i++)
	{
		if(nVdata[i] != 0xff)
		{
			if(nVdata[i]>=nTargetV) nVdata[i]-=nTargetV;
			else nVdata[i]=nTargetV-nVdata[i];

			if((nVdata[i]<=LDO11OUT_RANGE)&&(nVdata[i]<nTargetTempV))
			{
				nTargetTempV = nVdata[i];
				nTargetG = i;
			}
		}
	}

	if(nTargetG<LDO11OUT_NUM)
	{
		AossRegs.ANMOD_CFG.bit.Ldo3311Sel = nTargetG;
		_delay_us(100);
		return RT_OK;
	}
	else
		return RT_FAIL;
}

/***************************************************************************
* Subroutine:	AOSS_Ldo11LpOutVSel
* Function:		Select the output voltage gear of LDO11LP 
* Input:		UINT8 nSelV	-- 	to specify the voltage gear to be selected;
*									1.8V: 1.8*100=180=0xB4
* Output:		RT_OK or RT_FAIL 
* Description:	
* Date:			2016.5.5
* ModifyRecord:
* *************************************************************************/
UINT8 AOSS_Ldo11LpOutVSel(UINT8 nSelV)
{
	UINT8 nVdata[LDO11LPV5OUT_NUM];
	UINT8* pVdata;
	UINT8 nVdataLen;
	UINT8 nTargetV = nSelV;
	UINT8 i;
	UINT8 nTargetG=0xff,nTargetTempV = 0xff; 

	pVdata = nVdata;
	nVdataLen = LDO11LPV5OUT_NUM;	   
	memcpy(pVdata,(UINT8*)LDO11LPV5OUT_VALUE_ADDR,nVdataLen);

	for(i=0;i<nVdataLen;i++)
	{
		if(pVdata[i] != 0xff)
		{
			if(pVdata[i]>=nTargetV) pVdata[i]-=nTargetV;
			else pVdata[i]=nTargetV-pVdata[i];

			if((pVdata[i]<=LDO11LPOUT_RANGE)&&(pVdata[i]<nTargetTempV))
			{
				nTargetTempV = pVdata[i];
				nTargetG = i;
			}
		}
	}

	if(nTargetG<nVdataLen)
	{	
		AossRegs.ANMOD_CFG.bit.Ldo3311LpSel = nTargetG;
		_delay_us(100);
		return RT_OK;
	}
	else
		return RT_FAIL;
}

/***************************************************************************
* Subroutine:	AOSS_LdoInit
* Function:		LDO OUT init 
* Input:		
* Output:		
* Description:	
* Date:			2018.4.5
* ModifyRecord:
* *************************************************************************/
void AOSS_LdoInit(void)
{
	UINT8 nLdoVdata = (*(UINT8*)LDOOUT_VALUE_ADDR);
	UINT8 nTargetG;
	UINT8 nFtVerdata = (*(UINT8*)FT_VER_ADDR);
	UINT8 nMark = 0x0f;
	UINT8 nTempTargetG = 0x08;

	if(nFtVerdata == 0xff)
	{
		nMark = 0x07;
		nTempTargetG = 0x07;
	}
				
	nTargetG = ((nLdoVdata>>4)&nMark);
	if(nTargetG<nTempTargetG)
		AossRegs.ANMOD_CFG.bit.Ldo5033Sel = nTargetG;
	nTargetG = (nLdoVdata&nMark);
	if(nTargetG<nTempTargetG)
		AossRegs.ANMOD_CFG.bit.Ldo3318Sel = nTargetG;

	nLdoVdata = (*(UINT8*)(LDOOUT_VALUE_ADDR+1));
	nTargetG = ((nLdoVdata>>4)&nMark);
	if(nTargetG<nTempTargetG)
		AossRegs.ANMOD_CFG.bit.Ldo3311Sel = nTargetG;
	nTargetG = (nLdoVdata&nMark);
	if(nTargetG<nTempTargetG)
		AossRegs.ANMOD_CFG.bit.Ldo3311LpSel = nTargetG;
}