/**********************************************************************************
* File name    :	Sci.c
* Function     :	The base function for SCI interface compatible to ISO7816-3
* Author       : 	coins
* Date         :	2012/07/23
* Version      :    v1.0
* Description  :	this is comfort to ISO7816-3/4
* ModifyRecord :
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"

/**********************************************************************************
* Local Macro Definition
**********************************************************************************/
//#define SCI_DBG

/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static FP_SCI_POWER_CTRL  gfpSciPowerCtrl;
static UINT8 gATRCharIndex[33]; 	// save for ATR string index
static UINT8 gLrcOrCrc = 0;		// for T=1 epilogue field LRC or CRC
static UINT8 gIFSC = 0x20;	// for T=1 IFSC
static UINT16 gFi = 372;
static UINT8 gDi = 1;
static UINT8 gNS = 0;	// send sequence number

/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/
const UINT16 FiTab[16] = {372, 372, 558, 744, 1116, 1488, 1860, 0, 0, 512, 768, 1024, 1536, 2048, 0, 0};
const UINT8 DiTab[16] = {0, 1, 2, 4, 8, 16, 32, 64, 12, 20, 0, 0, 0, 0, 0, 0};


/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/

/**********************************************************************************
* Functon
**********************************************************************************/


/***********************************************************************************
 * Subroutine:	SCI_Init
 * Function:	SCI initialization
 * Input:		UINT8 PresPol --	to specify the polarity of PRES signal
 *									0 is low level for present, 1 if is high level for present
 *									Allway set to 1 for SY03
 *				FP_SCI_POWER_CTRL_INIT fpSciPowerCtrlInit -- 
 *									to specify the SCI power control initialization callback function;
 *				FP_SCI_POWER_CTRL fpSciPowerCtrl -- 
 *									to specify the SCI power control callback function
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.07.24
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_Init(UINT8 PresPol, FP_SCI_POWER_CTRL_INIT fpSciPowerCtrlInit, 
				FP_SCI_POWER_CTRL fpSciPowerCtrl)
{
	UINT32 freq_system = SCM_GetSysClk();
	if(fpSciPowerCtrl != NULL) gfpSciPowerCtrl = fpSciPowerCtrl;	
	if(fpSciPowerCtrlInit != NULL) fpSciPowerCtrlInit();
	if(PresPol == 0)return SCI_PARAM_ERR;

	// to select Pins - GPIOA0/1/2/3 for SCI signals
	GpioRegs.GPAUSE3.bit.GPA28 = MUX_SCI;		// SCI_PRES	
	GpioRegs.GPAUSE3.bit.GPA29 = MUX_SCI;		// SCI_IO	
	GpioRegs.GPAUSE3.bit.GPA30 = MUX_SCI;		// SCI_RST	
	GpioRegs.GPAUSE3.bit.GPA31 = MUX_SCI;		// SCI_CLK	

	// to enable SCI module and reset all logic
	SCM_DisableModClk(ID_SCI);
	SCM_KeepModRst(ID_SCI);
	SCM_EnableModClk(ID_SCI);
	SCM_ReleaseModRst(ID_SCI);

	// Direct convention, even parity, receive/transmit handshaking disabled when ATR
	// NOTE: TXNAK is used to determine whether T=0 or T=1 protocol is in operation
	SciRegs.CR0.all = 0;

	// Receive mode, timeout disable, PRES PAD polarity setting
	SciRegs.CR1.all = 0;
	//SciRegs.CR1.bit.CardSocket = PresPol;

	// disable all interrupt, and clear all interrupt flags
	SciRegs.IMSC.all = 0;
	//SciRegs.ICR.all = SciRegs.ICR.all;

	// if no external interface IC,
	// Stable time = system period * 65536 * SCISTABLE, 100ms is proposed
//	if(gfpSciPowerCtrl != NULL) 	// external interface IC for debounce
//	{
//		SciRegs.CR1.bit.ExDbnce = 1;
//	}
//	else 
		SciRegs.STABLE = 10*(freq_system/65536)/1000;

	// to time the three stages of the cold and warm reset
	// between 40000 - 45000 smart card cycles
	SciRegs.ATIME = 0xAFC8;

	// to time the three stages of the deactivation sequence, in terms of smart card cycles
	// within 1ms
	SciRegs.DTIME = 600;	// 

	// the time ART start from reset with in 40000 smart card cycles
	// that results in SCIARTSTOUTINTR
	SciRegs.ATRSTIME = 40000;

	// ATR duration time must be within 19200 etus
	// that results in SCIATRDTOUTINTR
	//SciRegs.ATRDTIME = 19200+960;//PBOC
	SciRegs.ATRDTIME = 19200;

	// the maximum time in etus between the leading edge of two consecutive characters
	// that results in SCICHTOUTINTR
	SciRegs.CHTIMEMS = 0;
	SciRegs.CHTIMELS = 0x2580;
	//SciRegs.CHTIMELS = 0x2580+480//PBOC;

	// This is the receive (bit[7:4]) and transmit (bit[3:0]) FIFO tide level
	// that results in SCIRXTIDEINTR and SCITXTIDEINTR
	SciRegs.TIDE.all = 0x10;	//0x18;

	// Clock for external Smart Card
	// f = (System clock frequency)/((SCICLKICC+1)*2), 
	// so SCICLKICC = (System clock frequency)/f/2-1
	// the initial frequency must be 1-5MHz
	SciRegs.CLKICC = freq_system/4000000/2-1;

	// set the etu, 372 by default, Fd = 372, Dd = 1;
	// etu = (F/f)*(1/D) = (1+SCIBAUD)*SCIVALUE /(System clock frequency)
	// = F*((SCICLKICC+1)*2)/(D*(System clock frequency))
	// so that D*(1+SCIBAUD)*SCIVALUE = F*(SCICLKICC+1)*2, F and D is returned by card
	// SCIBAUD = F/D-1
	SciRegs.BAUD = 372-1;
	// SCIVALUE = (1+SCICLKICC)*2
	SciRegs.VALUE = (1+SciRegs.CLKICC)*2;

	SciRegs.STOPTIME = 1860;
	SciRegs.STARTTIME = 700;
	SciRegs.RETRY.bit.TxRetry = 0;
	SciRegs.RETRY.bit.RxRetry = 0;

//	SciRegs.ICR.all = SciRegs.RIS.all;

	return SCI_OK;
}

/***********************************************************************************
 * Subroutine:	SCI_CheckCardIn
 * Function:	to check card insert or not
 * Input:		UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.13
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR	SCI_CheckCardIn(UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;

	while(!SciRegs.RIS.bit.Insert) 	// wait for card insert
	{
		if(nTimeCnt++ == nTimeout) return SCI_TIMEOUT;
		_delay_us(10);	
	}
	SciRegs.ICR.bit.Insert = 1;
	return SCI_OK;
}


/***********************************************************************************
 * Subroutine:	SCI_ActiveOnly
 * Function:	Active ISO7816 card only by assert power, clock, i/o and reset signal in order
 * Input:		UINT8 nType --	to specify the power type, only A, B or C
 *				UINT32 nFeq -- 	to specify the clock frequency for ISO7816 card, range 32K ~ 20M
 *								is used
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.11
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_ActiveOnly(UINT8 nType, UINT32 nFeq, UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;
	UINT32 freq_system = SCM_GetSysClk();
    UINT8 nRet = SCI_OK;
	if((nType != TYPE_A) && (nType != TYPE_B) && (nType != TYPE_C))	return SCI_PARAM_ERR;
	if((nFeq>20000000) || (nFeq<32000)) return SCI_PARAM_ERR;
	if(nFeq>freq_system) return SCI_PARAM_ERR;
    nRet = SCI_CheckCardIn(nTimeout);
	if(nRet != SCI_OK) return SCI_CARD_ABSENT;

	// Clock for external Smart Card
	// f = (System clock frequency)/((SCICLKICC+1)*2), 
	// so SCICLKICC = (System clock frequency)/f/2-1
	SciRegs.CLKICC = freq_system/nFeq/2-1;
	// SCIVALUE = (1+SCICLKICC)*2/D
	SciRegs.VALUE = (1+SciRegs.CLKICC)*2;
	// to time the three stages of the deactivation sequence, in terms of smart card cycles
	// within 1ms
	SciRegs.DTIME = freq_system/SciRegs.VALUE/5000;	// 0.2ms
	// Clear FIFO
	SciRegs.TXCOUNT = 0;
	SciRegs.RXCOUNT = 0;

	// power on 
	if(gfpSciPowerCtrl!=NULL)gfpSciPowerCtrl(nType);
	// to active ISO7816 card 
	SciRegs.CR2.bit.StartUp = 1;
	nTimeCnt = 0;
//	GpioRegs.GPADATA.bit.GPA0 = 1;		// TEST
	while(!SciRegs.RIS.bit.PowerOn)	// wait for active sequence completing
	{
		if(nTimeout)
		{
			_delay_us(10);
			if(nTimeCnt++ == nTimeout) 
			{
				return SCI_TIMEOUT;
			}
		}
	}
	SciRegs.ICR.bit.PowerOn = 1;	// clear the interrupt flag
	return SCI_OK;
}

/***********************************************************************************
 * Subroutine:	SCI_Write
 * Function:	to write data to ISO7816 interface
 * Input:		UINT8 *pData --	to specify the data for writing;
 * 				UINT32 nLen -- 	to specify the length;
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.11
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_Write(UINT8 *pData, UINT32 nLen, UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;
	UINT8 nTxCnt;

	SciRegs.CR1.bit.Mode = 1;
	while(nLen)
	{
		if(SciRegs.RIS.bit.TxErr) 	// transmit error (after retry)
		{
			SciRegs.ICR.bit.TxErr = 1;
			return SCI_TX_ERR;
		}
		nTimeCnt = 0;
		while(SciRegs.FIFOSTATUS.bit.TxFF)
		{
			if(nTimeout)  	// timeout
			{
				_delay_us(100);
				if(nTimeCnt++ == nTimeout) return SCI_TIMEOUT;
			}
		}
		nTxCnt = 8 - SciRegs.TXCOUNT;
		while(nTxCnt-- && nLen)
		{
			SciRegs.DATA.all = *pData++;
			nLen--;
		}
	}
	SciRegs.CR1.bit.Mode = 0;
	return SCI_OK;
}

/***********************************************************************************
 * Subroutine:	SCI_Read
 * Function:	to read data from interface
 * Input:		UINT8 *pData --	to specify the data for reading;
 * 				UINT32 *pLen -- to specify the expected length and receive the actual;
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out			
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.11
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_Read(UINT8 *pData, UINT32 *pLen, UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;
	UINT32 nLen = *pLen;
	UINT8 nRxCnt;
	#ifdef	SCI_DBG
	UINT8 *p = pData;
	#endif
	*pLen = 0;
	SciRegs.ICR.bit.ChTOut = 1;
	SciRegs.ICR.bit.BlkTout = 1;
	while(nLen)
	{
		while(SciRegs.FIFOSTATUS.bit.RxFE)	// if FIFO is empty, waiting
		{
			if(SciRegs.RIS.bit.ChTOut)	// CWT (character wait timeout), no data
			{
				SciRegs.ICR.bit.ChTOut = 1;
				return SCI_CWT_OUT;
			}
			if(SciRegs.RIS.bit.BlkTout)	// BWT (block wait timeout), no data
			{
				SciRegs.ICR.bit.BlkTout = 1;
				return SCI_BWT_OUT;
			}
			if(nTimeout)  	// timeout
			{
				_delay_us(10);
				if(nTimeCnt++ == nTimeout) return SCI_TIMEOUT;
			}	
		}
		if(SciRegs.RIS.bit.RxOv) 	// Receive overflow
		{
			SciRegs.ICR.bit.RxOv = 1;
			return SCI_RX_OV;
		}
		nRxCnt = SciRegs.RXCOUNT;
		while(nRxCnt-- && nLen)
		{
			*pData++ = SciRegs.DATA.all;
			*pLen += 1;	
			nLen--;
		}
	}
	#ifdef SCI_DBG
	nLen = *pLen;
	printf("R:");
	while(nLen--)printf("%x ", *p++);
   	printf("\n");
	#endif
	return SCI_OK;
}

/***********************************************************************************
 * Subroutine:	SCI_GetATR
 * Function:	to receive Answer-To-Reset
 * Input:		UINT8 *pATRBuf --	to specify ATR data buffer;
 * 				UINT32 *pATRLen -- to return the ATR length;
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.11
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_GetATR(UINT8 *pATRBuf, UINT8 *pATRLen, UINT32 nTimeout)
{
	UINT8 i;
	UINT32 nLen;
	UINT32 nTimeCnt = 0;
	UINT8 nTD, nTDVal;
	UINT16 T = 0;
	UINT8 ATRCharTCK = 0;
	SCI_ERR mRet;
	gNS = 0;
	for(i=0;i<33;i++) gATRCharIndex[i] = 0xFF;
	// to wait TS
	while(SciRegs.FIFOSTATUS.bit.RxFE)	// if FIFO is empty, waiting
	{
		//printf("RIS = %x\n", SciRegs.RIS.all);
		if(SciRegs.RIS.bit.ATRSTOut)	// ATR Start time out
		{
			SciRegs.ICR.bit.ATRSTOut = 1;
			return SCI_NO_ATR;
		}
		if(nTimeout)  	// timeout
		{
			_delay_us(10);
			if(nTimeCnt++ == nTimeout) return SCI_TIMEOUT;
		}		
	}
	pATRBuf[0] = SciRegs.DATA.all;	// read TS
	if(pATRBuf[0] == 0x03)
	{
		SciRegs.CR0.all |= 0x3;
		pATRBuf[0] = 0x3F;
	} 
	pATRBuf += 1; 
	*pATRLen = 1;
	gATRCharIndex[0] = ATR_TS;
	ATRCharTCK = 0;
	//SciRegs.CR1.bit.ATRDEn = 1;
	
	nLen = 1;
	mRet = SCI_Read(pATRBuf, &nLen, nTimeout); 	// read T0
	if(mRet != SCI_OK) return mRet;
	pATRBuf += 1;
	*pATRLen += 1;
	gATRCharIndex[1] = ATR_T0;
	ATRCharTCK ^= *(pATRBuf-1);

	nTDVal = *(pATRBuf-1);
	nTD = 1;
	while(nTD<16)	// ATR at most 32 byte, assumed that TD15 is the maximum
	{
		#ifdef	SCI_DBG
			printf("TD=%d\n", nTD);
		#endif
	
		for(i=0;i<4;i++)
		{
			if(nTDVal&(0x10<<i)) 	// TA1 TB1 TC1 TD1 TA2 ... 
			{
				nLen = 1;
				mRet = SCI_Read(pATRBuf, &nLen, nTimeout);
				if(mRet != SCI_OK) return mRet;
				pATRBuf += 1;
				*pATRLen += 1;
				gATRCharIndex[*pATRLen-1] = (0x10<<i) + nTD;
				ATRCharTCK ^= *(pATRBuf-1);
			}
		}
	
		if(nTDVal&0x80)
		{
			nTDVal = *(pATRBuf-1);
			T |= (1<<(nTDVal&0xF));
			nTD += 1;
		}
		else 
		{
			nTD = 16;
		}

		if(SciRegs.RIS.bit.ATRDTOut)  	// ATR duration time out
		{
			SciRegs.ICR.bit.ATRDTOut = 1;
			SciRegs.CR1.bit.ATRDEn = 0;
			return SCI_ATR_DTOUT;
		}
	}

	nLen = *(pATRBuf-*pATRLen+1) & 0xF;	// read history byte
	mRet = SCI_Read(pATRBuf, &nLen, nTimeout);
	if(mRet != SCI_OK) return mRet;
	for(i=0;i<nLen;i++)
	{
		gATRCharIndex[*pATRLen+i] = i+1;
		ATRCharTCK ^= *(pATRBuf+i);
	}
	pATRBuf += nLen;
	*pATRLen += nLen;

	if(T>1)		// other T protocol but T = 0 supported, need TCK
	{
		nLen = 1;	
		mRet = SCI_Read(pATRBuf, &nLen, nTimeout);
		if(mRet != SCI_OK) return mRet;
		pATRBuf += 1;
		gATRCharIndex[*pATRLen] = ATR_TCK;
		*pATRLen += 1;	
		ATRCharTCK ^= *(pATRBuf-1);
		if(ATRCharTCK)
			return SCI_ATR_ERR;			
	}
	SciRegs.CR1.bit.ATRDEn = 0;

	if(SciRegs.RIS.bit.ATRDTOut)  	// ATR duration time out
	{
		SciRegs.ICR.bit.ATRDTOut = 1;
		return SCI_ATR_DTOUT;
	}
	#ifdef	SCI_DBG
		for(i=0;i<*pATRLen;i++)printf("%2x ", gATRCharIndex[i]);
		printf("\n");
	#endif
   	return SCI_OK;
}

/***********************************************************************************
 * Subroutine:	SCI_CheckATRByPBOC
 * Function:	Check ATR parameters By PBOC3.0 specification
 * Input:		UINT8 *pATRBuf --	to specify ATR data buffer;
 *				UINT8  ATRType --  ATR Type  0  Cold reset ;1 Warm reset;			
 * Output:		SCI_ERR
 * Description:
 * Date:		2014.02.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_CheckATRByPBOC(UINT8 *pATRBuf, UINT8 ATRType)
{
	UINT8 i = 0;
	UINT8 temple = 0;
	UINT8 CardType = 0;
	UINT8 ATRChar = 0;
	UINT8 temp = 0;
	UINT8 FI_Temp,DI_Temp;
	UINT8 N = 0;

	//TS
	if((*(pATRBuf+i)!=0x3F)&&(*(pATRBuf+i)!=0x3B))
		return SCI_ATR_ERR;
	i++;

	//T0
	ATRChar = *(pATRBuf+i);
	temp=ATRChar&0xF0;//高4位表示TA1 TB1 TC1 TD1是否存在
	i++;

	//TA1 默认0x11
	FI_Temp=0x01;//默认值
	DI_Temp=0x01;//默认值
	if(temp&0x10)//判断是否有TA1
	{
		ATRChar = *(pATRBuf+i);
		i++;
		FI_Temp	=	ATRChar>>4;
		DI_Temp	=	ATRChar&0x0F;
	}
	
	if(temp&0x20)//判断是否有TB1
	{
		ATRChar = *(pATRBuf+i);
		i++;		
		if(ATRChar)//Card_TB1不是0,如果是冷复位，需要热复位
			if(ATRType==0)//=0表示冷复位，=1表示热复位
			{
				return SCI_ATR_ERR;//不启动热复位，否则点特性测试不过
			}
	}
	else//没有TB1
	{
		if(ATRType==0)//=0表示冷复位，=1表示热复位
		{
			return SCI_ATR_ERR;//不启动热复位，否则点特性测试不过
		}
	}
	
	//TC1
	if(temp&0x40)//判断是否有TC1
	{
		ATRChar = *(pATRBuf+i);
		i++;
		N = ATRChar;
	}
		
	//TD1
	if(temp&0x80)//判断是否有TD1
	{
		ATRChar = *(pATRBuf+i);
		i++;
		//TD1的低4位只能是0或者1
		if( (ATRChar&0x0F)==0x00 )CardType=0;
		else
		{
			if( (ATRChar&0x0F)==0x01 )CardType=1;
			else return SCI_ATR_ERR;
		}
		temp= ATRChar&0xf0;//高4位表示TA2 TB2 TC2 TD2是否存在
	}
	else temp=0;
	//TA2
	if(temp&0x10)//判断是否有TA2
	{
		ATRChar = *(pATRBuf+i);
		i++;
		//TA2最低位表明的协议类型正是ATR中第一次表明的协议类型
		if( (ATRChar&0x0F)==0 )//低4位是0
		{
			if(CardType)return SCI_ATR_ERR;
		}
		else if( (ATRChar&0x0F)==1 )//低4位是1
		{
			if(CardType==0)return SCI_ATR_ERR;
		}
		else return SCI_ATR_ERR;
		//--	b8表明IC卡是否有能力改变它的操作模式。如果b8置0则表明具有这样的能力，而如果b8置1则表明不具有这样的能力；
		//--	b5表明在复位应答后是按接口字节提供的传输参数进行，还是按终端默认的传输参数进行。如果b5置0，则按照接口字节定义的传输参数进行；如果b5置1，则按照终端默认的传输参数进行。
		if( (ATRChar&0x10)==0 )//如果收到TA2并且TA2.4=0，则立即使用新的TA1
		{
			if(FI_Temp!=0x01)return SCI_ATR_ERR;
			if((DI_Temp!=0x01)&&(DI_Temp!=0x02)&&(DI_Temp!=0x03))return SCI_ATR_ERR;
		}
		else if( (ATRChar&0x10)==0x10 )//如果收到TA2并且TA2.4=1，则返回
		{
			return SCI_ATR_ERR;
		}
	}
	//TB2
	if(temp&0x20)//判断是否有TB2
	{
		ATRChar = *(pATRBuf+i);
		i++;
		return SCI_ATR_ERR;//不允许有TB2
	}
	//TC2默认0x0A
	if(temp&0x40)//判断是否有TC2
	{//TC2专用于T=0协议，传输工作等待时间整数（WI），
     //WI用来确定由IC卡发送的任意一个字符起始位下降沿与IC卡或终端发送的前一个字符起始位下降沿之间的最大时间间隔。
     //工作等待时间为： 960×D×WI。
		ATRChar = *(pATRBuf+i);
		i++;
		if(ATRChar!=0x0A)return SCI_ATR_ERR;//TC2必须是0x0A
	}
	//TD2
	if(temp&0x80)//判断是否有TD2
	{
		ATRChar = *(pATRBuf+i);
		i++;
		temple=ATRChar&0x0F;//取低4位
		if(CardType)
		{
			if(temple!=0x01)return SCI_ATR_ERR;//原来是1,现在不是1,出错
		}
		else//原来是0，现在收到TD2，则必须是1或者14，否则出错
		{//增加这个判断，好像捷德的卡不能过
			if( (temple!=0x01) && (temple!=0x0E) )return SCI_ATR_ERR;//有TD2必须是协议1或者0x0E
		}				
		temp= ATRChar&0xf0;//高4位表示TA3 TB3 TC3 TD3是否存在
	}
	else
	{
		temp=0;
		if(CardType)return SCI_ATR_ERR;//T=1协议必须有TD2
	}
	//TA3默认0x20
	//TA3只的是IC卡能够接收的最大信息域长度
	if(temp&0x10)//判断是否有TA3
	{
		ATRChar = *(pATRBuf+i);
		i++;
		if(CardType)
		{
			if((ATRChar<=0x0F)||(ATRChar==0xFF)) return SCI_ATR_ERR; //TA3不能是0-F,或者0xFF
		}		
	}
	
	//TB3  BWI/CWI
	if(temp&0x20)//判断是否有TB3
	{
		ATRChar = *(pATRBuf+i);
		i++;
		if( (ATRChar & 0x0f)>0x05 )return SCI_ATR_ERR;//低4位0-5 CWI
		if( ((ATRChar>>4)&0x0F)>0x04 )return SCI_ATR_ERR;//高4位0-4 BWI
		if(CardType)
		{
			if(N==0xFF)N=0;
			else N=N+1;
			if((1<<(ATRChar & 0x0f))<=N)return SCI_ATR_ERR;
		}
	}
	else
	{
		if(CardType)return SCI_ATR_ERR;//T=1协议必须有TB3
	}
	
	//TC3默认0x00
	if(temp&0x40)//判断是否有TC3
	{//TC3的最低位表示校验类型
		ATRChar = *(pATRBuf+i);
		i++;
		if(CardType)
		{
			if(ATRChar)return SCI_ATR_ERR;//T=1协议TC3必须是0
		}
	}	
	return SCI_OK;
}
/***********************************************************************************
 * Subroutine:	SCI_GetAnATRChar
 * Function:	to get an ATR character from ATR string
 * Input:		UINT8 *pATRSting --	to specify ATR string;
 * 				ATR_CHAR_INDEX mIndex -- to specify the index for ATR character							
 *				UINT8 *pChar -- to return the character according to pStr				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.14
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_GetAnATRChar(UINT8 *pATRSting, ATR_CHAR_INDEX mIndex, UINT8 *pChar)
{
	UINT8 i, j;
	if(pATRSting==NULL) return SCI_PARAM_ERR;
	if((mIndex==ATR_TA) || (mIndex==ATR_TB) || (mIndex==ATR_TC) || (mIndex==ATR_TD))	// for T=1
	{
		for(j=3;j<16;j++)
		{
			for(i=0;i<33;i++)
			{
				if(gATRCharIndex[i] == (mIndex+j)) 
				{
					*pChar = pATRSting[i];
					return SCI_OK;
				}
			} 
		}	
	}
	else
	{
		for(i=0;i<33;i++)
		{
			if(gATRCharIndex[i] == mIndex) 
			{
				*pChar = pATRSting[i];
				return SCI_OK;
			}
		}
	}
	return SCI_NO_ATR_CHAR;
}

/***********************************************************************************
 * Subroutine:	SCI_ChangeParamWithATR
 * Function:	to change transmit parameters with the ATR characters
 * Input:		UINT8 *pATRBuf --	to specify ATR;				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.18
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_ChangeParamWithATR(UINT8 *pATRBuf)
{
	SCI_ERR mRet;
	UINT8 ch;
	// chage protocol parameters
	mRet = SCI_GetAnATRChar(pATRBuf, ATR_TD1, &ch);
	if((mRet == SCI_OK) && ((ch&0xF) == 1))	// TD1 is present and T = 1
	{
		SciRegs.CR0.bit.TxNak = 0;
		SciRegs.CR0.bit.RxNak = 0;
		SciRegs.RETRY.bit.TxRetry = 0;
		SciRegs.RETRY.bit.RxRetry = 0;		
	}
	else 	// TD1 is absent and T = 0
	{
		SciRegs.CR0.bit.TxNak = 1;
		SciRegs.CR0.bit.RxNak = 1;
		SciRegs.RETRY.bit.TxRetry = 3;
		SciRegs.RETRY.bit.RxRetry = 3;
	}
	gFi = 372;
	gDi = 1;
	mRet = SCI_GetAnATRChar(pATRBuf, ATR_TA2, &ch);
	if((mRet == SCI_OK) && ((ch&0x10) == 0))	//TA2.4=0
	{
		mRet = SCI_GetAnATRChar(pATRBuf, ATR_TA1, &ch);
		if(mRet == SCI_OK)	// TA1 is present 
		{
			gFi = FiTab[(ch>>4)&0xF];
			gDi = DiTab[ch&0xF];
				// SCIBAUD = F/D-1
			SciRegs.BAUD = gFi/gDi-1;
		}
	}
	if(SciRegs.CR0.bit.TxNak == 1)	// T = 0
	{ 	// to change protocol for T = 0
		// CGT
		mRet = SCI_GetAnATRChar(pATRBuf, ATR_TC1, &ch);
		if(mRet != SCI_OK)	// TC1 is absent
		{
			SciRegs.CHGUARD = 0;
		}
		else
		{
			if(ch==255) SciRegs.CHGUARD = 0;
			else SciRegs.CHGUARD = ch ;
		}
		// BGT
		SciRegs.BLKGUARD = 16-12;
		//SciRegs.BLKGUARD = 16-1-12;  //PBOC

		//WWT
		mRet = SCI_GetAnATRChar(pATRBuf, ATR_TC2, &ch);
		if(mRet != SCI_OK)	// TC2 is absent
			ch = 10;

		//WWT = 960×D×WI个etu。（D和W1分别在TA1和TC2中返回。）
		//终端必须能够正确解释最大间隔为WWT etu的字符。失败，WWT+（D×9600）etu内启动下电时序。
		SciRegs.CHTIMEMS = (960*gDi*ch)>>16;
		SciRegs.CHTIMELS = (960*gDi*ch)&0xFFFF;
		SciRegs.BLKTIMEMS = (960*gDi*ch)>>16;
		SciRegs.BLKTIMELS = (960*gDi*ch)&0xFFFF;

//		/////PBOC
//		//WWT = 960×D×WI个etu。（D和W1分别在TA1和TC2中返回。）
//		//终端必须能够正确解释最大间隔为WWT+（D×480）etu的字符。失败，WWT+（D×9600）etu内启动下电时序。
//		SciRegs.CHTIMEMS = (960*gDi*ch+480*gDi)>>16;
//		SciRegs.CHTIMELS = (960*gDi*ch+480*gDi)&0xFFFF;
//		SciRegs.BLKTIMEMS = (960*gDi*ch+480*gDi)>>16;
//		SciRegs.BLKTIMELS = (960*gDi*ch+480*gDi)&0xFFFF;
	}
	else	// T = 1
	{	// to change protocol for T = 1
		mRet = SCI_GetAnATRChar(pATRBuf, ATR_TA, &ch);
		if(mRet != SCI_OK)	// TA is absent
		{
			gIFSC = 0x20;
		}
		else gIFSC = ch;
		// CGT
		mRet = SCI_GetAnATRChar(pATRBuf, ATR_TC1, &ch);
		if(mRet != SCI_OK)	// TC1 is absent
		{
			SciRegs.CHGUARD = 1;
		}
		else
		{
			if(ch==255) SciRegs.CHGUARD = 0;
			else SciRegs.CHGUARD = ch + 1;
		}
		// BGT
		SciRegs.BLKGUARD = 22-11;
		//SciRegs.BLKGUARD = 22-1-11;//PBOC
		//
		mRet = SCI_GetAnATRChar(pATRBuf, ATR_TC, &ch);
		if(mRet != SCI_OK)	// TC is absent
		{
			gLrcOrCrc = 0;
		}
		else gLrcOrCrc = ch & 0x1;

		// CWT
		mRet = SCI_GetAnATRChar(pATRBuf, ATR_TB, &ch);
		if(mRet != SCI_OK)	// TB is absent
		{
			SciRegs.CHTIMEMS = 0;
			SciRegs.CHTIMELS = 1<<13;
			SciRegs.BLKTIMEMS = 120*372*gDi/gFi;
			SciRegs.BLKTIMELS = ((UINT64)8192*960*372*gDi/gFi)&0xFFFF;
		}
		else
		{
			SciRegs.CHTIMEMS = (1<<(ch&0xF))>>16;
			SciRegs.CHTIMELS = (1<<(ch&0xF))&0xFFFF;
			SciRegs.BLKTIMEMS =  ((1<<(ch>>4))*960*372*gDi/gFi)>>16;
			SciRegs.BLKTIMELS =  ((1<<(ch>>4))*960*372*gDi/gFi)&0xFFFF;	
			
			//pboc
			////CWT = 1<<CWI+11个etu。接收方CWT+4 etu的字符。
			////CWT的取值范围是12到43个etu
			//SciRegs.CHTIMEMS = ((1<<(ch&0xF))+11+4)>>16;
			//SciRegs.CHTIMELS = ((1<<(ch&0xF))+11+4)&0xFFFF;
			////BWT = ((1<<BWI)×960）+11}个etu。终端必须能够正确解释BWT+（Dx960）个etu
			////BWT = {[(1<<BWI)×960×372D/F]+11}etu
			////BWT的取值范围是971到15,371个etu。
			//SciRegs.BLKTIMEMS = (((1<<(ch>>4))*960*372*gDi/gFi)+11+960*gDi)>>16;
			//SciRegs.BLKTIMELS = (((1<<(ch>>4))*960*372*gDi/gFi)+11+960*gDi)&0xFFFF;	
		}
		SciRegs.CR1.bit.BlkEn = 1;
		SciRegs.CR1.bit.BgtEn = 1;	
	}
	return SCI_OK;	
}


/***********************************************************************************
 * Subroutine:	SCI_Active
 * Function:	Active ISO7816 card and get ATR. if sucess to get ATR, changing 
 *				the protocol parameters according to the ATR characters.
 * Input:		UINT8 nType --	to specify the power type, only A, B or C
 *				UINT32 nFeq -- 	to specify the clock frequency for ISO7816 card, range 32K ~ 20M
 *								is used, but if nFeq = 0, orignal clock frequency is used
 *				UINT8 *pATRBuf --	to specify ATR data buffer;
 * 				UINT32 *pATRLen -- to return the ATR length;
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.14
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_Active(UINT8 nType, UINT32 nFeq, UINT8 *pATRBuf, UINT8 *pATRLen, UINT32 nTimeout)
{
	SCI_ERR mRet;
	mRet = SCI_ActiveOnly(nType, nFeq, nTimeout);
	if(mRet != SCI_OK) return mRet;
	mRet = SCI_GetATR(pATRBuf, pATRLen, nTimeout);
	if(mRet != SCI_OK) return mRet;			
	//mRet = SCI_CheckATRByPBOC(pATRBuf, 0);
	//if(mRet != SCI_OK) return mRet;			
	return SCI_ChangeParamWithATR(pATRBuf);	
}

/***********************************************************************************
 * Subroutine:	SCI_Deactive
 * Function:	Deactive ISO7816 card and power off 
 * Input:		UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.18
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_Deactive(UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;
	SciRegs.ICR.bit.PowerOff = 1;
  	SciRegs.CR2.bit.Finish = 1;
   	while(!SciRegs.RIS.bit.PowerOff) 	// wait for power down
	{
		if(nTimeout)
		{
			_delay_us(10);
			if(nTimeCnt++ == nTimeout) 
			{
				if(gfpSciPowerCtrl!=NULL)gfpSciPowerCtrl(TYPE_OFF);
				return SCI_TIMEOUT;
			}
		}
	}
	if(gfpSciPowerCtrl!=NULL)gfpSciPowerCtrl(TYPE_OFF);
	SciRegs.ICR.bit.PowerOff = 1;
	return SCI_OK; 
}

/***********************************************************************************
 * Subroutine:	SCI_WarmReset
 * Function:	to warm reset ISO7816 card and get ATR. if sucess to get ATR, changing 
 *				the protocol parameters according to the ATR characters.
 * Input:		UINT8 *pATRBuf --	to specify ATR data buffer;
 * 				UINT32 *pATRLen -- to return the ATR length;
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.18
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_WarmReset(UINT8 *pATRBuf, UINT8 *pATRLen, UINT32 nTimeout)
{
	SCI_ERR mRet;
	SciRegs.CR2.bit.WrReset = 1;
	mRet = SCI_GetATR(pATRBuf, pATRLen, nTimeout);
	if(mRet != SCI_OK) return mRet;			
	//mRet = SCI_CheckATRByPBOC(pATRBuf, 1);
	//if(mRet != SCI_OK) return mRet;			
	return SCI_ChangeParamWithATR(pATRBuf);			
}

/***********************************************************************************
 * Subroutine:	SCI_StopClock
 * Function:	to stop smart card clock output 
 * Input:		UINT8 ClkVal --	to specify the level of CLK signal when clock stops;
 *								0 is for low level, 1 is for high level
 * 				UINT16 StopDly -- to specify the clock delay before clock stops unit in smart card clock;
 *								the value should be less than 4096
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.18
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_StopClock(UINT8 ClkVal, UINT16 StopDly, UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;
	SciRegs.CR0.bit.ClkVal = ClkVal;
	SciRegs.STOPTIME = (StopDly>1860)? StopDly : 1860;

	SciRegs.ICR.bit.ClkStp = 1;
  	SciRegs.CR0.bit.ClkDis = 1;	 	// stop clock 
   	while(!SciRegs.RIS.bit.ClkStp) 	// wait for clock stop
	{
		if(nTimeout)
		{
			_delay_us(10);
			if(nTimeCnt++ == nTimeout) 
			{
				return SCI_TIMEOUT;
			}
		}
	}
	
	SciRegs.ICR.bit.ClkStp = 1;
	return SCI_OK; 	
}

/***********************************************************************************
 * Subroutine:	SCI_ResumeClock
 * Function:	to resume smart card clock output 
 * Input:	 	UINT16 StartDly -- to specify the clock delay after clock start unit in smart card clock;
 *								the value should be less than 4096
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.18
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_ResumeClock(UINT16 StartDly, UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;
	SciRegs.STARTTIME = (StartDly>700)? StartDly : 700;

	SciRegs.ICR.bit.ClkAct = 1;
  	SciRegs.CR0.bit.ClkDis = 0;	 	// start clock 
   	while(!SciRegs.RIS.bit.ClkAct) 	// wait for clock start
	{
		if(nTimeout)
		{
			_delay_us(10);
			if(nTimeCnt++ == nTimeout) 
			{
				return SCI_TIMEOUT;
			}
		}
	}
	
	SciRegs.ICR.bit.ClkAct = 1;
	return SCI_OK; 	
}

/***********************************************************************************
 * Subroutine:	SCI_SendPPS
 * Function:	to send PPS request and get response 
 * Input:		UINT8 Protocol --	to select protocol T = 0, or T = 1
 *				UINT8 ParamFI -- 	to specify the FI, 0<=FI<=15
 *				UINT8 ParamDI --	to specify the DI, 0<=DI<=15
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_SendPPS(UINT8 Protocol, UINT8 ParamFI, UINT8 ParamDI, UINT32 nTimeout)
{
	SCI_ERR mRet;
	UINT8 cmd[4];
	UINT8 buf[4];
	UINT32 nSize;
	cmd[0] = 0xFF;		// PPSS
	cmd[1] = 0x10 | Protocol;	// PPS0
	cmd[2] = ParamFI<<4 | ParamDI;	// PPS1
	cmd[3] = cmd[0] ^ cmd[1] ^ cmd[2];	// PCK
   	mRet = SCI_Write(cmd, 4, nTimeout);
	if(mRet != SCI_OK) return mRet;
	nSize = 4;
	mRet = SCI_Read(buf, &nSize, nTimeout);
	if(mRet != SCI_OK) return mRet;
	if(nSize < 3) return SCI_LEN_UNMATCH;
	if(buf[1]== cmd[1])
	{
		if(Protocol == 0) 	// T = 0
		{
			SciRegs.CR0.bit.TxNak = 1;
			SciRegs.CR0.bit.RxNak = 1;
			SciRegs.RETRY.bit.TxRetry = 3;
			SciRegs.RETRY.bit.RxRetry = 3;
		}
		else
		{
			SciRegs.CR0.bit.TxNak = 0;
			SciRegs.CR0.bit.RxNak = 0;
			SciRegs.RETRY.bit.TxRetry = 0;
			SciRegs.RETRY.bit.RxRetry = 0;
		}
		gFi = FiTab[ParamFI];
		gDi = DiTab[ParamDI];
		// SCIBAUD = F/D-1
		SciRegs.BAUD = gFi/gDi-1;		
		return SCI_OK;
	} 
	else return SCI_PPS_ERR;
}

/***********************************************************************************
*		Protocol T = 0
************************************************************************************/

/***********************************************************************************
 * Subroutine:	SCI_SendCmdOnly
 * Function:	to send 5-byte command only with protocol T=0 
 * Input:		UINT32 Command --	to specify the command, defined as following:
 *									bit31 - 24: CLA
 *									bit23 - 16: INS
 *									bit15 - 8: P1
 *									bit7 - 0: P2
 * 				UINT8 nLen -- to specify the number to be sent (Lc or Le)(255 max)
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.13
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_SendCmdOnly(UINT32 Command, UINT8 nLen, UINT32 nTimeout)
{
	UINT8 buf[5];
	buf[0] = (UINT8)(Command>>24);
	buf[1] = (UINT8)(Command>>16);
	buf[2] = (UINT8)(Command>>8);
	buf[3] = (UINT8)(Command);
	buf[4] = nLen;
	return SCI_Write(buf, 5, nTimeout);
}

/***********************************************************************************
 * Subroutine:	SCI_InCmm
 * Function:	to carry out a command for in comming data with T=0 
 * Input:		UINT32 Command --	to specify the command, defined as following:
 *									bit31 - 24: CLA
 *									bit23 - 16: INS
 *									bit15 - 8: P1
 *									bit7 - 0: P2
 * 				UINT8 nLen -- to specify the number to be sent (Lc)(0 ~ 255)
 *				UINT8 *pData -- to specify the buffer to be sent
 *				UINT16 *pSw1Sw2 -- to specify the word for SW1 and SW2 stored
 *									bit15 - 8: SW1		bit7 - 0 : SW2
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.13
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_InCmm(UINT32 Command, UINT8 nLen, UINT8 *pData, UINT16 *pSw1Sw2, UINT32 nTimeout)
{
	SCI_ERR mRet;
	UINT8 nProcByte, nSw1, nSw2;
	UINT32 nSize;

	mRet = SCI_SendCmdOnly(Command, nLen, nTimeout);	
	if(mRet != SCI_OK) return mRet;
	// read procedure byte
	do{
		nSize = 1;
		mRet = SCI_Read(&nProcByte, &nSize, nTimeout);
		if(mRet != SCI_OK) return mRet;
		if(nSize != 1) return SCI_LEN_UNMATCH;
	}while(nProcByte == 0x60);

	#ifdef	SCI_DBG
	printf("Procedure Byte %2x\n", nProcByte);
	#endif
		
	if((nProcByte==(UINT8)(Command>>16)) || ((nProcByte^0xFF)==(UINT8)(Command>>16)))
	{
		mRet = SCI_Write(pData, nLen, nTimeout);	  	// write data
		if(mRet != SCI_OK) return mRet;
		nSize = 1;
		mRet = SCI_Read(&nSw1, &nSize, nTimeout);		// to read SW1
		if(mRet != SCI_OK) return mRet;
		if(nSize != 1) return SCI_LEN_UNMATCH;
	}
	else if(((nProcByte&0xF0)==0x60) || ((nProcByte&0xF0)==0x90))	// SW1
	{
		nSw1 = nProcByte;		
	}
	else
	{
		return SCI_SW_ABSENT;
	}
	// to read SW2
	nSize = 1;
	mRet = SCI_Read(&nSw2, &nSize, nTimeout);	  	// read SW2
	if(mRet != SCI_OK) return mRet;
	if(nSize != 1) return SCI_LEN_UNMATCH;
	*pSw1Sw2 = nSw1<<8 | nSw2;
	return SCI_OK;
}

/***********************************************************************************
 * Subroutine:	SCI_OutCmm
 * Function:	to carry out a command for in outgoing data with T=0
 * Input:		UINT32 Command --	to specify the command, defined as following:
 *									bit31 - 24: CLA
 *									bit23 - 16: INS
 *									bit15 - 8: P1
 *									bit7 - 0: P2
 * 				UINT8 *pLen -- to specify the number of data required, and return
 *								the actual number (Le)(0~255)
 *								the card returns 256 bytes if *pLen = 0
 *				UINT8 *pData -- to specify the buffer to be received
 *				UINT16 *pSw1Sw2 -- to specify the word for SW1 and SW2 stored
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.13
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_OutCmm(UINT32 Command, UINT8 *pLen, UINT8 *pData, UINT16 *pSw1Sw2, UINT32 nTimeout)
{
	SCI_ERR mRet;
	UINT8 nProcByte, nSw1, nSw2;
	UINT32 nSize, nLen;

	nLen = *pLen;
	mRet = SCI_SendCmdOnly(Command, nLen, nTimeout);	
	if(mRet != SCI_OK) return mRet;
	// read procedure byte
	do{
		nSize = 1;
		mRet = SCI_Read(&nProcByte, &nSize, nTimeout);
		if(mRet != SCI_OK) return mRet;
		if(nSize != 1) return SCI_LEN_UNMATCH;
	}while(nProcByte == 0x60);

	#ifdef	SCI_DBG
	printf("Procedure Byte %2x\n", nProcByte);
	#endif
		
	if((nProcByte==(UINT8)(Command>>16)) || ((nProcByte^0xFF)==(UINT8)(Command>>16)))
	{
		nSize = (*pLen)? *pLen : 256; 	// if *pLen = 0, 256 bytes need to read
		nLen = nSize;
		mRet = SCI_Read(pData, &nSize, nTimeout);	  	// read data
		if(mRet != SCI_OK) return mRet;
		if(nSize != nLen) return SCI_LEN_UNMATCH;
		nSize = 1;
		mRet = SCI_Read(&nSw1, &nSize, nTimeout);		// to read SW1
		if(mRet != SCI_OK) return mRet;
		if(nSize != 1) return SCI_LEN_UNMATCH;
	}
	else if(((nProcByte&0xF0)==0x60) || ((nProcByte&0xF0)==0x90))	// SW1
	{
		nSw1 = nProcByte;		
	}
	else
	{
		return SCI_SW_ABSENT;
	}
		// to read SW2
	nSize = 1;
	mRet = SCI_Read(&nSw2, &nSize, nTimeout);	  	// read SW2
	if(mRet != SCI_OK) return mRet;
	if(nSize != 1) return SCI_LEN_UNMATCH;
	*pSw1Sw2 = nSw1<<8 | nSw2;
	return SCI_OK;	
}


/***********************************************************************************
*		Protocol T = 1
************************************************************************************/

/***********************************************************************************
 * Subroutine:	SCI_SendBlock
 * Function:	to send block with T=1 protocol 
 * Input:		UINT8 nNAD --	to specify the node address, usually set to 0:									
 * 				UINT8 nPCB -- to specify the protocol control byte
 *				UINT8 nLEN -- to specify the length byte
 *				UINT8 *pINFO -- to specify the infomation field
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_SendBlock(UINT8 nNAD, UINT8 nPCB, UINT8 nLEN, UINT8 *pINFO, UINT32 nTimeout)
{
	SCI_ERR mRet;
	UINT16 edc = 0;
	UINT8 buf[2];
	UINT8 i;
	if(gLrcOrCrc) // CRC
	{
	  	// here for crc calculate
	
	}
	else  	// LRC
	{
		for(i=0;i<nLEN;i++)edc ^= pINFO[i];
		edc ^= nNAD;
		edc ^= nPCB;
		edc ^= nLEN;
		buf[0] = (UINT8)edc;
	}
	
	mRet = SCI_Write(&nNAD, 1, nTimeout);		// send prologue field NAD
   	if(mRet != SCI_OK) return mRet;
	mRet = SCI_Write(&nPCB, 1, nTimeout);		// send prologue field PCB
   	if(mRet != SCI_OK) return mRet;
	mRet = SCI_Write(&nLEN, 1, nTimeout);		// send prologue field LEN
   	if(mRet != SCI_OK) return mRet;
	mRet = SCI_Write(pINFO, nLEN, nTimeout);	// send info field
   	if(mRet != SCI_OK) return mRet;
	
	if(gLrcOrCrc)
	{
		mRet = SCI_Write(buf, 2, nTimeout);		// send prologue field
   		if(mRet != SCI_OK) return mRet;
	}
	else
	{
		mRet = SCI_Write(buf, 1, nTimeout);		// send prologue field
   		if(mRet != SCI_OK) return mRet;
	}
	return SCI_OK;	
}

/***********************************************************************************
 * Subroutine:	SCI_RecvBlock
 * Function:	to receive block with T=1 protocol 
 * Input:		UINT8 *pNAD --	to return the node address									
 * 				UINT8 *pPCB -- to return the protocol control byte
 *				UINT8 *pLEN -- to return the length byte
 *				UINT8 *pINFO -- to return the infomation field
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_RecvBlock(UINT8 *pNAD, UINT8 *pPCB, UINT8 *pLEN, UINT8 *pINFO, UINT32 nTimeout)
{
	UINT8 buf[2], i;
	UINT16 edc = 0;
	UINT32 nSize;
	SCI_ERR mRet;

	nSize = 1;
	mRet = SCI_Read(pNAD, &nSize, nTimeout);	 	// read prologue field NAD
	if(mRet != SCI_OK) return mRet;
	if(nSize != 1) return SCI_LEN_UNMATCH;

	nSize = 1;
	mRet = SCI_Read(pPCB, &nSize, nTimeout);	 	// read prologue field PCB
	if(mRet != SCI_OK) return mRet;
	if(nSize != 1) return SCI_LEN_UNMATCH;

	nSize = 1;
	mRet = SCI_Read(pLEN, &nSize, nTimeout);	 	// read prologue field LEN
	if(mRet != SCI_OK) return mRet;
	if(nSize != 1) return SCI_LEN_UNMATCH;

	nSize = *pLEN; 	// to read info
	if(nSize)
	{
		mRet = SCI_Read(pINFO, &nSize, nTimeout);
		if(mRet != SCI_OK) return mRet;
		if(nSize != *pLEN) return SCI_LEN_UNMATCH;
	}

	if(gLrcOrCrc) // CRC
	{
	  	// here for crc calculate
		return SCI_OK;	
	}
	else  	// LRC
	{
		for(i=0;i<*pLEN;i++)edc ^= pINFO[i];
		edc ^= *pNAD;
		edc ^= *pPCB;
		edc ^= *pLEN;
		nSize = 1;
		mRet = SCI_Read(buf, &nSize, nTimeout);		// read prologue field
   		if(mRet != SCI_OK) return mRet;	
		if(nSize != 1) return SCI_LEN_UNMATCH;	
		if((UINT8)edc != buf[0]) return SCI_EDC_ERR;
		return SCI_OK;
	}	
}

/***********************************************************************************
 * Subroutine:	SCI_SendIFSRequest
 * Function:	to send IFS request and get response 
 * Input:		UINT8 nIFSValue --	to specify the IFS value, it sets the maximum size sent by the card									
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:	the function set IFSD size, but here IFSD is always 254
 * Date:		2012.08.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_SendIFSRequest(UINT8 nIFSValue, UINT32 nTimeout)
{
	SCI_ERR mRet;
	UINT8 nPCB = 0xC1, nLEN = 1;
	UINT8 buf[4];
	if((nIFSValue==0) || (nIFSValue>254)) return SCI_PARAM_ERR;

	mRet = SCI_SendBlock(0, nPCB, nLEN, &nIFSValue, nTimeout);
	if(mRet != SCI_OK) return mRet;	
	mRet =  SCI_RecvBlock(buf, buf+1, buf+2, buf+3, nTimeout);  		
	if(mRet != SCI_OK) return mRet;
	if((buf[1] == 0xE1) && (buf[3] == nIFSValue)) return SCI_OK;
	else return SCI_IFSR_ERR;
}

/***********************************************************************************
 * Subroutine:	SCI_SendCmdBlock
 * Function:	to send command with T=1 protocol 
 * Input:		UINT32 Command --	to specify the command, defined as following:
 *									bit31 - 24: CLA
 *									bit23 - 16: INS
 *									bit15 - 8: P1
 *									bit7 - 0: P2
 * 				UINT8 Lc -- to specify the number to be sent (Lc)(0 ~ 255)
 *				UINT8 *pLcField -- to specify the buffer to be sent for LC data field
 *				UINT16 Le -- to specify the number to receive, if Le = 0, no LE send (0 ~ 256 used) 		
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_SendCmdBlock(UINT32 Command, UINT8 Lc, UINT8 *pLcField, UINT16 Le, UINT32 nTimeout) 
{
	UINT8 info[261], i;
	UINT8 buf[3];
	UINT16 nLen;
	UINT8 nPCB = 0, nLEN;
	UINT8 *p = info;
	SCI_ERR mRet;
	*p++ = (UINT8)(Command>>24);
	*p++ = (UINT8)(Command>>16);
	*p++ = (UINT8)(Command>>8);
	*p++ = (UINT8)(Command);
	if(Lc!=0) 	
	{
		*p++ = Lc;
		for(i=0;i<Lc;i++) *p++ = pLcField[i];
	}
	if(Le!=0)
	{
		if(Le>=256)*p++ = 0;
		else *p++ = Le;
	}

	nLen = p - info;
	p = info;
	while(nLen)
	{
		nLEN = (nLen>gIFSC)? gIFSC : nLen;
		nLen -= nLEN;
		nPCB = gNS;
		if(nLen) nPCB |= (1<<5);	// more data bit
		mRet = SCI_SendBlock(0, nPCB, nLEN, p, nTimeout);
		if(mRet != SCI_OK) return mRet;
		if(nLen)
		{
			mRet =  SCI_RecvBlock(buf, buf+1, buf+2, NULL, nTimeout);  		// read R(N(R))
			if(mRet != SCI_OK) return mRet;
			if(((buf[1]&(1<<4))<<2) == (nPCB&(1<<6)))  	// re-send
			{
				nLen += nLEN;
			}
			else  	// send next block
			{
				p += nLEN;
				gNS ^= (1<<6);
			}	
		}	
	}
	gNS ^= (1<<6);
	return SCI_OK;	
}

/***********************************************************************************
 * Subroutine:	SCI_ReceRespBlock
 * Function:	to receive response with T=1 protocol 
 * Input:	 	UINT16 Le -- to specify the number to receive, if Le = 0, no data receive (0 ~ 256 used) 
 *				UINT8 *pLeField -- to return the LE data field 
 *				UINT16 *pLeFieldLen -- to return the LE data field size 	
 *				UINT16 *pSw1Sw2 -- to specify the word for SW1 and SW2 stored	
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_ReceRespBlock(UINT16 Le, UINT8 *pLeField, UINT16 *pLeFieldLen, UINT16 *pSw1Sw2, UINT32 nTimeout)
{
	UINT16 nLen, i;
	UINT8 nNAD, nPCB, nLEN, Info[258];
	UINT8 *p = Info;
	SCI_ERR mRet;
	nLen = Le+2;
	*pLeFieldLen = 0;
	while(nLen)
	{
		mRet = SCI_RecvBlock(&nNAD, &nPCB, &nLEN, p, nTimeout);
		if(mRet != SCI_OK) return mRet;
		nLen -= nLEN;
		p += nLEN;
		*pLeFieldLen += nLEN;
		if(nPCB & (1<<5))  	// more data bit
		{
			if(nPCB&(1<<6)) nPCB = 0x80; 	// R(0)
			else nPCB = 0x90; 	// R(1)
			nLEN = 0;
			mRet = SCI_SendBlock(0, nPCB, nLEN, NULL, nTimeout);	// send R(N)
			if(mRet != SCI_OK) return mRet;
		}
		else break;	// no more bit		
	}
	if(*pLeFieldLen>=2)
	{
		*pLeFieldLen -= 2;
		for(i=0;i<*pLeFieldLen;i++)*pLeField++ = Info[i];
		*pSw1Sw2 = (UINT16)Info[i]<<8 | Info[i+1]; 
		return SCI_OK;	
	}
	else 	// R-block
	{
		*pLeFieldLen = 0;
		switch(nPCB&0x3)
		{
			case 0: return SCI_OK;
			case 1: return SCI_EDC_ERR;
			case 2: return SCI_OTHER_ERR;
			default: return SCI_OK;
		}
	}
}



/***********************************************************************************
*		Both Protocol T = 0 and T = 1
************************************************************************************/ 

/***********************************************************************************
 * Subroutine:	SCI_SendAPDU
 * Function:	to send APDU with both T = 0 and T = 1 
 * Input:	 	UINT32 Command --	to specify the command, defined as following:
 *									bit31 - 24: CLA
 *									bit23 - 16: INS
 *									bit15 - 8: P1
 *									bit7 - 0: P2
 * 				UINT8 Lc -- to specify the number to be sent (Lc)(0 ~ 255)
 *				UINT8 *pLcField -- to specify the buffer to be sent for LC data field
 *				UINT16 Le -- to specify the number to receive, 0 ~ 255 is used;
 *							for T = 1, NO_LE_SEND can be used  
 *				UINT8 *pLeField -- to return the LE data field 
 *				UINT16 *pLeFieldLen -- to return the LE data field size 	
 *				UINT16 *pSw1Sw2 -- to return the word for SW1 and SW2
 									bit15 - 8: SW1		bit7 - 0 : SW2	
 *				UINT32 nTimeout -- to sepcify the timeout for opeation, unit in 10us, 0 is no time out				
 * Output:		SCI_ERR
 * Description:
 * Date:		2012.08.17
 * ModifyRecord:
 * ********************************************************************************/
SCI_ERR SCI_SendAPDU(UINT32 Command, UINT8 Lc, UINT8 *pLcField, UINT16 Le, UINT8 *pLeField, UINT16 *pLeFieldLen, UINT16 *pSw1Sw2, UINT32 nTimeout)
{
	SCI_ERR mRet;
	UINT16 nLeSize;
	if(SciRegs.CR0.bit.TxNak == 1)	// T = 0
	{ 	
		if(Lc==0)
		{
			mRet = SCI_OutCmm(Command, (UINT8 *)&Le, pLeField, pSw1Sw2, nTimeout);
			if(mRet != SCI_OK) return mRet;
			if(Le == 0) *pLeFieldLen = 256;
			else *pLeFieldLen = Le;
			return SCI_OK;
		}
		else 
		{
			mRet = SCI_InCmm(Command, Lc, pLcField, pSw1Sw2, nTimeout);
			return mRet;
		}
	}
	else	// T = 1
	{
		if(Le & NO_LE_SEND)	nLeSize = 0;
		else if(Le == 0) nLeSize = 256;
		else  nLeSize = Le;
		mRet = SCI_SendCmdBlock(Command, Lc, pLcField, nLeSize, nTimeout);
		if(mRet != SCI_OK) return mRet;
		mRet = SCI_ReceRespBlock(nLeSize, pLeField, pLeFieldLen, pSw1Sw2, nTimeout);
		return mRet;
	}	
}

