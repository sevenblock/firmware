/**********************************************************************************
* File name    :	Efc.c
* Function     :	The base function for embedded flash controller
* Author       : 	coins
* Date         :	2015/10/14
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
//#define	EFC_ROM_FUNC_USED

// The configurations for EFC operation
#define		SMP_MAX_TIMES	12	// Smart Program Maximum retry times
#define		SME_MAX_TIMES	46	// Smart Erase Maximum retry times

#define		SMP_TYPE	HW_SMP	// Smart Program type selection
#define		SME_TYPE	HW_SME 	// Smart Erase type	selection

#define		PROG_BUF_LEN	16	// the programming data buffer size


/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
UINT32 nKeyPermission[7] = {	AREA_KEY0_WE, \
								AREA_KEY1_WE, \
								AREA_KEY2_WE, \
								AREA_KEY3_WE, \
								AREA_KEY4_WE, \
								AREA_KEY5_WE, \
								AREA_KEY6_WE \
								};

/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/


/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/


/**********************************************************************************
* Functon
**********************************************************************************/
//
///***************************************************************************
//* Subroutine:	EFC_ConfigTiming
//* Function:		to config EFC timing.
//* Input:		UINT8 nReadMode -- 	to set read mode.
//*						 			0/3 -- for Txa Timing;
//*									1 -- for one cycle read;
//*									2 -- for two cycle read;		
//*				UINT32 nTC0	-- 	to specify the TC0 timing register value;
//* 				UINT32 nTC1 --	to specify the TC1 timing register value;
//* Output:		None;
//* Description:	if EFC_ROM_FUNC_USED is defined, ROM function will be called
//* Date:			2012.09.21
//* ModifyRecord:
//* *************************************************************************/
//UINT8 EFC_CalcParameters(UINT32	nFreq, UINT32 *pTC0, UINT32 *pTC1, UINT32 *pTC2, UINT32 *pTC3)
//{
//	
//	unsigned int par;
//	nFreq /= 10000;
//	// TC0
//	par = ((unsigned long long int)nFreq * T_NVS/16+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC0 = par;
//
//	par = ((unsigned long long int)nFreq * T_NVH/16+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC0 <<= 8;
//	*pTC0 |= par;
//
//	par = ((unsigned long long int)nFreq * T_NVH1/256+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC0 <<= 8;
//	*pTC0 |= par;
//
//	par = ((unsigned long long int)nFreq * T_PGS/16+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC0 <<= 8;
//	*pTC0 |= par;
//
//	// TC1
//	par = ((unsigned long long int)nFreq * T_PROG/4+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC1 = par;
//
//	par = ((unsigned long long int)nFreq * T_RCV/16+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC1 <<= 8;
//	*pTC1 |= par;
//
//	par = ((unsigned long long int)nFreq * T_ERASE/1024+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC1 <<= 8;
//	*pTC1 |= par;
//
//	par = ((unsigned long long int)nFreq * T_ME/16384+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC1 <<= 8;
//	*pTC1 |= par;
//
//	// TC2
//	par = ((unsigned long long int)nFreq * T_ADH+50000)/100000;
//	if(par>15) return RT_FAIL;
//	*pTC2 = par;
//
//	par = ((unsigned long long int)nFreq * T_PGH+50000)/100000;
//	if(par>15) return RT_FAIL;
//	*pTC2 <<= 4;
//	*pTC2 |= par;
//
//	par = ((unsigned long long int)nFreq * T_RS+50000)/100000;
//	if(par>15) return RT_FAIL;
//	*pTC2 <<= 4;
//	*pTC2 |= par;
//
//	par = ((unsigned long long int)nFreq * T_REC/4+50000)/100000;
//	if(par>15) return RT_FAIL;
//	*pTC2 <<= 12;
//	*pTC2 |= par;
//
//	// TC3
//	par = ((unsigned long long int)nFreq * T_ADS+50000)/100000;
//	if(par>15) return RT_FAIL;
//	*pTC3 = par;
//
//	par = ((unsigned long long int)nFreq * T_ACCV+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC3 <<= 12;
//	*pTC3 |= par;
//
//	par = ((unsigned long long int)nFreq * T_ACC+50000)/100000;
//	if(par>255) return RT_FAIL;
//	*pTC3 <<= 8;
//	*pTC3 |= par;
//	
//	return RT_OK;
//}
//


/***************************************************************************
* Subroutine:	EFC_ConfigTiming
* Function:		to config EFC timing.
* Input:		UINT8 nReadMode -- 	to set read mode.
*						 			0/3 -- for Txa Timing;
*									1 -- for one cycle read;
*									2 -- for two cycle read;		
*				UINT32 nTC0	-- 	to specify the TC0 timing register value;
* 				UINT32 nTC1 --	to specify the TC1 timing register value;
* Output:		None;
* Description:	if EFC_ROM_FUNC_USED is defined, ROM function will be called
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_ConfigTiming(UINT32 nTC0, UINT32 nTC1, UINT32 nTC2, UINT32 nTC3)
{
	#ifndef	EFC_ROM_FUNC_USED
	EfcRegs.TC0.all = nTC0;
	EfcRegs.TC1.all = nTC1;
	EfcRegs.TC2.all = nTC2;
	EfcRegs.TC3.all = nTC3;
	#else
	_EFC_ConfigTiming(nTC0, nTC1, nTC2, nTC3);
	#endif
}

/***************************************************************************
* Subroutine:	EFC_Program
* Function:		flash program word-by-word.
* Input:		UINT32 *pData	-- 	the data to be programmed, word aligned;
* 				UINT32 nAddr --	the offset address word-aligned excluded 
*								embedded flash base address;
* 				UINT32 nLen --		the length of data, size in word.
* Output:		RT_OK or RT_FAIL;
* Description:	Program nLen word into flash address addr_align. Before the 
*				Function is called, CHECK register should write corret value.
* Date:			2016.11.20
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_Program(UINT32 *pData, UINT32 nAddr, UINT32 nLen)
{
	UINT8 nRet = RT_OK;
    UINT32 lRegStatus;
	VUINT32 * pSysAddr;
	
	nAddr &= 0xFFFFFFFC;	// align to 4 
	pSysAddr = (VUINT32 *)(FLASH_BASE_ADDR+nAddr);	// start address to program
    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();	// disable interrupt

	// set SMP parameters
	EfcRegs.CTRL.bit.SMW_en = 1;   	// enable Smart Program	
	EfcRegs.CTRL.bit.SMW_EVPV = 1;	// when SMW, PV=1 
	EfcRegs.CTRL.bit.WIPGM_th = 19;	
	EfcRegs.SMW.bit.Use_sep_WHV = 1;	// using step WHV&WMV
	EfcRegs.WHV_ARRAY = 0x55555554;
	EfcRegs.WMV_ARRAY = 0x66666666;
	EfcRegs.SMW.bit.N_smp = SMP_MAX_TIMES;	// smart program times
	while(nLen--)
	{
		EfcRegs.WEN.all = nAddr | 0x80000000;	// page write enable
		*pSysAddr++ = *pData++;	// program a word (32 bits)
		EFC_PROG_CMD();		// program command and verify
		nAddr += 4;	   	// next address
		while(EfcRegs.STS.bit.Busy); 	// wait for completing program 
		if(EfcRegs.SMW.bit.SMP_Fail)
		{	/* fail */
			nRet = RT_FAIL;
			break;		
		}
	}

	EfcRegs.WEN.all = 0;
	__set_PRIMASK(lRegStatus); 			// recover interrupt
	return nRet;
}

/***************************************************************************
* Subroutine:	EFC_PageProg
* Function:		Embedded flash program a page.
* Input:		UINT32 addr --	the flash offset address of byte;
* 				UINT32 *pData	-- 	the data to be programmed;
* Output:		RT_OK or RT_FAIL;
* Description:	to program a page that the addr is included.
* Date:			2016.11.20
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_PageProg(UINT32 nAddr, UINT32 *pData)
{
	UINT8 i, j, nRet = RT_OK;
	VUINT32 *pSysAddr;
	UINT32 lRegStatus;

	nAddr &= (UINT32)(~(FLASH_PAGE_SIZE-1));	// align to FLASH_PAGE_SIZE
	pSysAddr = (VUINT32 *)(FLASH_BASE_ADDR+nAddr);	// start address to program
	lRegStatus = __get_PRIMASK();	// save global interrput
   	__disable_irq();	// disable interrupt
	EfcRegs.WEN.all = nAddr | 0x80000000;

	// set SMP parameters
	EfcRegs.CTRL.bit.SMW_en = 1;   	// enable Smart Program	
	EfcRegs.CTRL.bit.SMW_EVPV = 1;	// when SMW, PV=1 
	EfcRegs.CTRL.bit.WIPGM_th = 19;	
	EfcRegs.SMW.bit.Use_sep_WHV = 1;	// using step WHV&WMV
	EfcRegs.WHV_ARRAY = 0x55555554;
	EfcRegs.WMV_ARRAY = 0x66666666;
	EfcRegs.SMW.bit.N_smp = SMP_MAX_TIMES;	// smart program times

	for(i=0;i<(FLASH_PAGE_SIZE/4/PROG_BUF_LEN);i++)
	{
		for(j=0;j<PROG_BUF_LEN;j++)	// write to flash buffer PROG_BUF_LEN word (32 bits)
		{
			*pSysAddr++ = *pData++;	
		}
		EFC_PROG_CMD();	// program command and verify
		while(EfcRegs.STS.bit.Busy); 	// wait for completing program 
		if(EfcRegs.SMW.bit.SMP_Fail)
		{	/* fail */
			nRet = RT_FAIL;	
			break;	
		}
	}

	EfcRegs.WEN.all = 0;
	__set_PRIMASK(lRegStatus); 		// recover interrupt
	return nRet;	
}

/***************************************************************************
* Subroutine:	EFC_IsEraseOK
* Function:		Check flash erase is successful.
* Input:		UINT32 nAddr	-- 	the offset address word-aligned;
* 				UINT32 nLen --		the length of data, size in word.
*				UINT8 nMode --		the verify mode, 1 for margin, 0 for normal£»must be 0
* Output:		RT_OK or RT_FAIL 
* Description:	check the address including from nAddr to (nAddr+nLen*4-1)
*				NOTE: If the margin read mode is used, the RT_FAIL may be also returned
*				after erase successful.
* Date:			2015.10.23
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_IsEraseOK(UINT32 nAddr, UINT32 nLen, UINT8 nMode)
{
	UINT8 nRet = RT_OK;
	UINT32 nLen1;
    UINT32 lRegStatus;

	nAddr &= 0xFFFFFFFC;	// align to 4 
	nLen *= 4;	// byte length
    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();	// disable interrupt
	// disable Smart Write 
	// as the erase check command should be done only in non-SMW mode
	EfcRegs.CTRL.bit.SMW_en = 0;   	
	EfcRegs.CTRL.bit.HW_MR = 1;
	EfcRegs.CTRL.bit.EV = nMode;	// margin or normal read verify
	while(nLen)
	{
		EfcRegs.PEA.all = nAddr;	// start address
		nLen1 = FLASH_PAGE_SIZE-(nAddr%FLASH_PAGE_SIZE);
		if(nLen1>nLen) nLen1 = nLen;
		EfcRegs.EVRF = ((nAddr%FLASH_PAGE_SIZE)+nLen1)-4;	   	// end address	
		EFC_ERASE_CHK_CMD();
		while(EfcRegs.STS.bit.Busy);
		if(EfcRegs.STS.bit.Err_num!=0)
		{
			nRet = RT_FAIL;
			break;				
		}
		nAddr += nLen1;
		nLen -= nLen1;
	}
	EfcRegs.CTRL.bit.EV = 0;	
	__set_PRIMASK(lRegStatus); 	// recover interrupt
	return nRet;	
}

/***************************************************************************
* Subroutine:	EFC_Erase
* Function:		Embedded flash page erase. A page is 512-byte aligned.
* Input:		UINT32 nAddr	-- 	the flash offset address of byte;
* 				UINT32 nPageNum --	page number n;
* Output:		RT_OK or RT_FAIL
* Description:	to erase n pages from the page that the nAddr included.				
* Date:			2016.11.20
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_Erase(UINT32 nAddr, UINT32 nPageNum)
{
	UINT8 nRet = RT_OK;
	UINT32 page;
	UINT32 lRegStatus;
	
	nAddr &= (UINT32)(~(FLASH_PAGE_SIZE-1));	// align to FLASH_PAGE_SIZE 
	// set SME parameters
	EfcRegs.CTRL.bit.SMW_en = 1;   	// enable Smart Erase	
	EfcRegs.CTRL.bit.SMW_EVPV = 1;	// when SMW, EV=1 
	EfcRegs.CTRL.bit.WIPGM_th = 19;	
	EfcRegs.SMW.bit.Use_sep_WHV = 1;	// using step WHV&WMV
	EfcRegs.WHV_ARRAY = 0xDDDB9542;
	EfcRegs.WMV_ARRAY = 0x55555555;
	EfcRegs.SMW.bit.N_sme = SME_MAX_TIMES;	// smart erase times

	for(page=0;page<nPageNum;page++)
	{
		lRegStatus = __get_PRIMASK();	// save global interrput
   		__disable_irq();	// disable interrupt

		EfcRegs.WEN.all = nAddr | 0x80000000;	// write enable
		EfcRegs.PEA.all = nAddr; 	// erase page address and verify start address
		EfcRegs.EVRF = 0x1FC;	   	// verify end address
		EFC_ERASE_CMD();   	// erase and verify
		while(EfcRegs.STS.bit.Busy);	// wait to complete erase
		__set_PRIMASK(lRegStatus); 	// recover interrupt
		if(EfcRegs.SMW.bit.SME_Fail)
		{
			nRet = RT_FAIL;
			break;	
		}
		else
		{
			// next page
			nAddr += FLASH_PAGE_SIZE;	
		}
	} 
	EfcRegs.WEN.all = 0;
	return nRet;
}

/***************************************************************************
* Subroutine:	EFC_ChipErase
* Function:		to erase the EFC flash chip which the specified
* 				byte address is in.
* Input:		None;
* Output:		None;
* Description:	to erase n pages from the page that the addr_offset included.
* 				if EFC_ROM_FUNC_USED is defined, ROM function will be called
* Date:			2014.09.21
* ModifyRecord:
* *************************************************************************/
#pragma arm section code = ".ram_code"
void EFC_ChipErase(void)
{
	UINT32 lRegStatus;
	lRegStatus = __get_PRIMASK();	// save global interrput
   	__disable_irq();	// disable interrupt
	EFC_WrProtect(AREA_PARA_WE);
	EfcRegs.CTRL.bit.SMW_en = 0;
	EfcRegs.WEN.all = 0x80000000|ASA_PARA;
	EfcRegs.PEA.all = ASA_PARA;	
	EfcRegs.CTRL.bit.WMV = 0x5;	// 0b101
	EfcRegs.CTRL.bit.WHV = 0xD;	// 0b1101
	EfcRegs.CTRL.bit.MAS1 = 1;
	EFC_ERASE_CMD();
	while(EfcRegs.STS.bit.Busy);
	EfcRegs.CTRL.bit.MAS1 = 0;
	EFC_WrProtect(AREA_PROT_DI);
	__set_PRIMASK(lRegStatus); 	// recover interrupt
}

/***************************************************************************
* Subroutine:	EFC_PreRead
* Function:		to pre-read flash
* Input:		UINT32 nPreReadMaxTimes -- to specify the maximum times for pre-read;
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2016.01.09
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_PreRead(UINT32 nPreReadMaxTimes)
{
	while(nPreReadMaxTimes--)
	{
		EfcRegs.CTRL.bit.Pre_Read = 1;
		while(EfcRegs.STS.bit.Busy);
		if(EfcRegs.PREAD==*(UINT32 *)0x10080000) break;
	}
	if(EfcRegs.PREAD==*(UINT32 *)0x10080000) return RT_OK;
	else return RT_FAIL;
}

#pragma arm section code

/***************************************************************************
* Subroutine:	EFC_MpuWrEnable
* Function:		to enable MPU write according the address
* Input:		UINT32 nAddr -- the address will be written
* Output:		None
* Description:	
* Date:			2015.10.27
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_MpuWrEnable(UINT32 nAddr)
{
	if(nAddr<(EfcRegs.DATA&0x0FFFFFFF))		// EO or PROG area
	{
		EfcRegs.CHK0 = AREA_PROG_WE;	
	}
	else if(nAddr<ASA_PARA)		// DATA or KEY area
	{
		EfcRegs.CHK0 = AREA_DATAPRO_WE;
		if(EfcRegs.STS.bit.Key_en && (nAddr>=ASA_KEY)) 	// KEY area enable and address in
		{
			EfcRegs.CHK1 = nKeyPermission[(nAddr-ASA_KEY)/AREA_KEY_BANK_SIZE];
		}
	}
	else if(nAddr<ASA_OTP) 	// PARA area
	{
		EfcRegs.CHK0 = AREA_PARA_WE;
	}
	else if(nAddr<(FLASH_TOTAL_PAGES*FLASH_PAGE_SIZE)) 	// OTP area
	{
		EfcRegs.CHK0 = AREA_OTP_WE;
	}
	else	// undefined area
	{
		EfcRegs.CHK0 = AREA_PROT_DI;
	}
	return RT_OK;
}

/***************************************************************************
* Subroutine:	FlashWriteBlock
* Function:		to write a Flash Block(page), Block(page) is 512-byte aligned.
* Input:		UINT8 *pBuf	-- 	to specify the data to be written;
* 				UINT32 lBlock -- to specify the block (page) to be written;
* 				UINT32 lOffset -- to specify the offset in block for the data
* 								  to be written start;
* 				UINT32 lLen --	to specify the length data to be written
* Output:		UINT8 --	return the result;
* Description:
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/
#ifndef	EFC_ROM_FUNC_USED
static UINT8 FlashWriteBlock(UINT8 *pBuf, UINT32 lBlock, UINT32 lOffset, UINT32 lLen)
{
	UINT8 *pTmp;
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	UINT8 bMustErase;
	UINT32 lStartAddr;

 	if((lOffset+lLen)>FLASH_PAGE_SIZE)
	    return RT_PARAM_ERR;

 	lStartAddr=lBlock*FLASH_PAGE_SIZE;
 	// first read back the data
 	pTmp=(UINT8 *)Buf;
 	memcpy(pTmp,(void *)(FLASH_BASE_ADDR+lStartAddr),FLASH_PAGE_SIZE);
	bMustErase=0;
//	if(EFC_IsEraseOK(lStartAddr, FLASH_PAGE_SIZE/4, 0) != RT_OK)
		bMustErase=1;
	if(bMustErase)//	//must erase;
	{
	    //erase the block
	   if(EFC_Erase(lStartAddr,1) != RT_OK)
	   	  return RT_FAIL;
	}
	memcpy((pTmp+lOffset), pBuf, lLen);
    //write to flash
    return EFC_PageProg(lStartAddr, (UINT32 *)Buf);
}
#endif

/***************************************************************************
* Subroutine:	EFC_WriteInfo
* Function:		to write Embedded Flash data 
* Input:		UINT8 *pDataBuf	-- 	to specify the data to be written;
* 				UINT32 lAddress -- to specify the Flash address;
* 				UINT32 lLen --	to specify the length data to be written
* Output:		UINT8 --	return the result;
* Description:	
* 				
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{
	#ifndef	EFC_ROM_FUNC_USED
	UINT32 nStart;
	UINT16 nNum;
	UINT16 nStartBlock,nStartOffset;
	UINT8 *pBuf;
	UINT8 nRet;	   

	//check the data range;
	if(lLen<=0)
		return RT_PARAM_ERR;
	nStart=lAddress;
	nStartBlock=(UINT16)(nStart/FLASH_PAGE_SIZE);
	pBuf=pDataBuf;
	nNum=lLen;
	//loop to write data to flash
	do{
		nStartOffset=(UINT16)(nStart%FLASH_PAGE_SIZE);
		if(nStartOffset)
		{			
			if(FLASH_PAGE_SIZE-nStartOffset>lLen)
			{		 
				return FlashWriteBlock(pBuf,nStartBlock,nStartOffset,lLen);
			}
			nRet=FlashWriteBlock(pBuf,nStartBlock,nStartOffset,FLASH_PAGE_SIZE-nStartOffset);
			   
			if(nRet!=RT_OK) return nRet;
			    
			pBuf+=FLASH_PAGE_SIZE-nStartOffset;
			nStart+=FLASH_PAGE_SIZE-nStartOffset;
			nNum-=FLASH_PAGE_SIZE-nStartOffset;
			nStartBlock++;
		}
		else if(nNum>=FLASH_PAGE_SIZE)
		{
			nRet=FlashWriteBlock(pBuf,nStartBlock,0,FLASH_PAGE_SIZE);
				
			if(nRet!=RT_OK) return nRet;
						    
		    pBuf+=FLASH_PAGE_SIZE;
		    nStart+=FLASH_PAGE_SIZE;
		    nNum-=FLASH_PAGE_SIZE;
			nStartBlock++;			  	
		}			 
		else
		{
			nRet=FlashWriteBlock(pBuf,nStartBlock,0,nNum);			 
	   	    if(nRet!=RT_OK) return nRet;
			nNum=0;
		}
	}while(nNum>0);
			    
	return RT_OK;
	#else
	return _EFC_WriteInfo(pDataBuf, lAddress, lLen);
	#endif
}

/***************************************************************************
* Subroutine:	EFC_ReadInfo
* Function:		to Read Embedded Flash data 
* Input:		UINT8 *pDataBuf	-- 	to specify the data to be Read;
* 				UINT32 lAddress -- to specify the Flash address;
* 				UINT32 lLen --	to specify the length data to be read
* Output:		UINT8 --	return the result;
* Description:	
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_ReadInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{ 
	VUINT8* pAddr;
	pAddr=(VUINT8*)(lAddress+FLASH_BASE_ADDR);
	memcpy(pDataBuf,(void*)pAddr,lLen);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_KEYWrite
* Function:		to write Embedded Flash key area
* Input:		EFC_AREA_KEY_ID nKeyAreaID	-- 	to specify the number of the KEY AREA ID
*				UINT8 *pDataBuf	-- 	to specify the data to be written;
* 				UINT32 lAddress -- to specify the offset address in a key area(0~4095);
* 				UINT32 lLen --	to specify the length data to be written(1~4096)
* Output:		UINT8 --	return the result;
* Description:	
* 				If the address out of the range, RT_PARAM_ERR will be returned.
* Date:			2016.11.04
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_KEYWrite(EFC_AREA_KEY_ID nKeyAreaID, UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{
	UINT32 nStartOffset;
	UINT8 nRet;
 
	/* the program must be in EO, and EO and KEY area enable */
	if(EfcRegs.STS.bit.Key_en && EfcRegs.STS.bit.Exe_en && IS_ADDR_IN_EO((UINT32)EFC_KEYWrite))
	{
		if(nKeyAreaID>EFC_AREA_KEY_6) return RT_PARAM_ERR;
		if(lAddress>0x6FFF) return RT_PARAM_ERR;
		if((lAddress+lLen)>0x1000) return RT_PARAM_ERR;
	
		nStartOffset = ASA_KEY + nKeyAreaID*0x1000;	
		EFC_WrProtect(AREA_KEY_WE);
		EFC_KEYWrProtect(nKeyPermission[nKeyAreaID]);
		nRet = EFC_WriteInfo(pDataBuf,nStartOffset + lAddress, lLen);
		EFC_WrProtect(AREA_PROT_DI);
		return nRet;
	}
	else return RT_FAIL;
}

/***************************************************************************
* Subroutine:	EFC_KEYRead
* Function:		to read Embedded Flash key area
* Input:		EFC_AREA_KEY_ID nKeyAreaID	-- 	to specify the number of the KEY AREA ID
*				UINT8 *pDataBuf	-- 	to specify the data buffer for read;
* 				UINT32 lAddress -- to specify the offset address in a key area(0~4095);
* 				UINT32 lLen --	to specify the length data to be read(1~4096)
* Output:		UINT8 --	return the result;
* Description:	
* 				If the address out of the range, RT_PARAM_ERR will be returned.
* Date:			2016.11.04
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_KEYRead(EFC_AREA_KEY_ID nKeyAreaID, UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{
	UINT32 nStartOffset;
	UINT8 nRet;
 
	/* the program must be in EO, and EO and KEY area enable */
	if(EfcRegs.STS.bit.Key_en && EfcRegs.STS.bit.Exe_en && IS_ADDR_IN_EO((UINT32)EFC_KEYWrite))
	{
		if(nKeyAreaID>EFC_AREA_KEY_6) return RT_PARAM_ERR;
		if(lAddress>0x6FFF) return RT_PARAM_ERR;
		if((lAddress+lLen)>0x1000) return RT_PARAM_ERR;
	
		nStartOffset = ASA_KEY + nKeyAreaID*0x1000;	
		EFC_WrProtect(AREA_KEY_WE);
		EFC_KEYWrProtect(nKeyPermission[nKeyAreaID]);
		nRet = EFC_ReadInfo(pDataBuf,nStartOffset + lAddress, lLen);
		EFC_WrProtect(AREA_PROT_DI);
		return nRet;
	}
	else return RT_FAIL;
}

/***************************************************************************
* Subroutine:	EFC_OtpWrite
* Function:		to write OTP user field.
* Input:		EFC_OTP_UESR_FIELD_ID nUserFieldID -- 	to specify the number of the OTP user field ID(1~2)
*				UINT32 nAddress	-- 	to specify the start address of the OTP user field (1~31)
* 				UINT32 nNum --	the number of data to be written, size of word (1~31)
* 				UINT32* pBuf -- to specify the data 
* Output:		UINT8 --	return the result;
* Description:  
* Date:			2016.11.04
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpWrite(EFC_OTP_UESR_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32* pBuf)
{
	UINT32 nStartOffset;

	if((nUserFieldID<1)||(nUserFieldID>2)) return RT_PARAM_ERR;
	if((nAddress<1)||(nAddress>31)) return RT_PARAM_ERR;
	if((nAddress+nNum)>32) return RT_PARAM_ERR;

	nStartOffset = ASA_OTP + nUserFieldID*0x80;	
	if(*((UINT32 *)(FLASH_BASE_ADDR+nStartOffset)) != 0xFFFFFFFF)
		return RT_FAIL;
	EFC_WrProtect(AREA_OTP_WE);
	EFC_Program(pBuf, nAddress*4+nStartOffset, nNum);
	EFC_WrProtect(AREA_PROT_DI);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_OtpDisWrite
* Function:		to disable Write otp function.
* Input:		EFC_OTP_UESR_FIELD_ID nUserFieldID -- 	to specify the number of the OTP user field ID(1~3)
* 				UINT32* pBuf -- to specify the data 
* Output:		UINT8 --	return the result;
* Description:
* Date:			2016.11.04
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpDisWrite(EFC_OTP_UESR_FIELD_ID nUserFieldID)
{
	UINT32 nAddr, data = 0;

	if((nUserFieldID<1)||(nUserFieldID>3)) return RT_PARAM_ERR;

	nAddr = ASA_OTP + nUserFieldID*0x80;
	if(*((UINT32 *)(FLASH_BASE_ADDR+nAddr)) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_WE);
		EfcRegs.WEN.all = 0x80000000 | nAddr;
		EFC_Program(&data, nAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(data == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_OtpRead
* Function:		to read OTP user field.
* Input:		EFC_OTP_UESR_FIELD_ID nUserFieldID -- 	to specify the number of the OTP user field ID(1~3)
*				UINT32 nAddress	-- 	to specify the start address of the OTP user field (0~31)
* 				UINT32 nNum --	the number of data to be read (1~32);
* 				UINT8* pBuf -- to return the data
* Output:		UINT8 --	return the result;
* Description:	when nUserFieldID is 3, the maximum size is 30
* Date:			2016.11.04
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpRead(EFC_OTP_UESR_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32 *pBuf)
{
	if((nUserFieldID<1)||(nUserFieldID>2)) return RT_PARAM_ERR;
	if(nAddress>31) return RT_PARAM_ERR;
	if((nAddress+nNum)>32) return RT_PARAM_ERR;

	memcpy((void *)pBuf, (void *)(nAddress*4+FLASH_BASE_ADDR + ASA_OTP + nUserFieldID*0x80), nNum*4);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_ReadChipSN
* Function:		to read OTP chip Serial Number.
* Input:		UINT8 *pSN	-- 	to return SN data;
* 				UINT16* pLen -- to return the length of SN, unit in byte
* Output:		None
* Description:	if pSN is NULL, only the length of SN will return by pLen
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
void EFC_ReadChipSN(UINT8 *pSN, UINT16* pLen)
{
	if(pSN == NULL)
	{
		*pLen = 0x20;
		return;
	}
	memcpy((void *)pSN, (void *)(FLASH_BASE_ADDR + ASA_OTP), 0x20);
	*pLen = 0x20;
}

/***************************************************************************
* Subroutine:	EFC_LockJtag
* Function:		to lock JTAG
* Input:		None
* Output:		UINT8 --	return the result;
* Description:	
* Date:			2016.11.04
* ModifyRecord: 
* *************************************************************************/
UINT8 EFC_LockJtag(void)
{
	UINT32 nAddr = AREA_LOCK_JTAG;
	UINT32 lock = 0;
	
    if(*(UINT32 *)(FLASH_BASE_ADDR+nAddr) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_WE);
		EfcRegs.WEN.all = 0x80000000 | nAddr;
		EFC_Program((UINT32 *)&lock, nAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(lock == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_SetEoSt
* Function:		to set execute only area start page
* Input:		UINT32 nPage  -- Page number;
* Output:		UINT8 --	return the result;
* Description:
* Date:			2016.11.04
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_SetEoSt(UINT32 nPage)
{
	UINT32 nEoStConfigAddr = AREA_EO_EN;

	if(EfcRegs.STS.bit.Exe_en)
		return RT_FAIL;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr) == 0xFFFFFFFF)
	{
		nPage *= FLASH_PAGE_SIZE;
		nPage += FLASH_BASE_ADDR;
		EFC_WrProtect(AREA_PARA_WE); 
		EfcRegs.WEN.all = 0x80000000 | nEoStConfigAddr;
		EFC_Program((UINT32 *)&nPage, nEoStConfigAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(nPage == *(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}
/***************************************************************************
* Subroutine:	EFC_SetKeySt
* Function:		to set KEY area start page
* Input:		UINT32 nPage  -- Page number;
* Output:		UINT8 --	return the result;
* Description:
* Date:			2012.10.17
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_SetKeySt(UINT32 nPage)
{
	UINT32 nEoStConfigAddr = AREA_KEY_EN;

	if((EfcRegs.STS.bit.Key_en)||(EfcRegs.STS.bit.Exe_en))
		return RT_FAIL;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr) == 0xFFFFFFFF)
	{
		nPage *= FLASH_PAGE_SIZE;
		nPage += FLASH_BASE_ADDR;
		EFC_WrProtect(AREA_PARA_WE); //AREA_OTP_WE
		EfcRegs.WEN.all = 0x80000000 | nEoStConfigAddr;
		EFC_Program((UINT32 *)&nPage, nEoStConfigAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(nPage == *(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}
/***********************************************************************************
 * Subroutine:	EFC_ClrProgFlag
 * Function:	Clear SY09 code programmed flag and parameter page
 * Input:		UINT32 Key -- 	AREA_PARA_WE(0x4444_5678);
 * Output:		None
 * Description: It is recommended the parameter Key is get from USB, UART or
 * 				other communication interface for parameter security.
 * Date:		2012.11.05
 * ModifyRecord:
 * ********************************************************************************/
void EFC_ClrProgFlag(UINT32 Key)
{
	EFC_WrProtect(Key); 
	EFC_Erase(ASA_PARA, 4);
	EFC_WrProtect(AREA_PROT_DI);
}

/***************************************************************************
* Subroutine:	EFC_LockTest
* Function:		to lock test
* Input:		None
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.18
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_LockTest(void)
{
	UINT32 nAddr = AREA_LOCK_TEST;
	UINT32 nVal = 0x23581321;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nAddr) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_WE);
		EfcRegs.WEN.all = 0x80000000 | nAddr;
		EFC_Program((UINT32 *)&nVal, nAddr, 1);
		if(nVal == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}

/***************************************************************************
* Subroutine:	EFC_WrProtect
* Function:		Embedded flash write and erase protect enable or disable with proper
* 				check code.
* Input:		UINT32 permission	-- 	the permission for an operation;
* Output:		None
* Description:	the permission value defined as following:
* 				0x1111_5678 -- 	program area writable and erasable;
* 				0x1111_5678 -- 	E0 area writable and erasable;
* 				0x2222_5678 -- 	protected data area writable and erasable;
* 				0x2222_5678 -- 	KEY area writable and erasable;
* 				0x4444_5678 -- 	Parameter area writable and erasable;
*				0x5555_5678 -- 	OTP area un-writable;
* Date:		2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_WrProtect(UINT32 permission)
{
	EfcRegs.CHK0 = permission;
}
/***************************************************************************
* Subroutine:	EFC_KEYWrProtect
* Function:		Embedded flash KEY area write and erase protect enable or disable with proper
* 				check code.
* Input:		UINT32 permission	-- 	the permission for an operation;
* Output:		None
* Description:	the permission value defined as following:
*				0x0123_4567 -- 	KEY AREA 0 area writable and erasable;
* 				0x1234_5678 -- 	KEY AREA 1 area writable and erasable;
* 				0x2345_6789 -- 	KEY AREA 2 area writable and erasable;
* 				0x3456_789A -- 	KEY AREA 3 area writable and erasable;
* Date:		2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_KEYWrProtect(UINT32 permission)
{
	EfcRegs.CHK1 = permission;
}
