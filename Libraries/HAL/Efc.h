/***************************************************************************************
* File name    :	Efc.h
* Function     :	The header of Efc.c
* Author       : 	coins
* Date         :	2012/09/13
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef EFC_H_
#define EFC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define 	EFC_ERASE_CMD() 	{*(VUINT32 *)0x11005678 = 0x0000FEDC;}
#define 	EFC_PROG_CMD()		{*(VUINT32 *)0x11001234 = 0x0000CDEF;}
#define		EFC_ERASE_CHK_CMD()	{*(VUINT32 *)0x11009ABC = 0x0000ABCD;}
#define 	EFC_PROG_CHK_CMD()	{*(VUINT32 *)0x1100DEF0 = 0x0000DCBA;}

#define		FLASH_BASE_ADDR		0x10000000
#define		FLASH_PAGE_SIZE		512
#define		FLASH_TOTAL_PAGES	1025
#define		FLASH_BUFF_LEN		16

//------ Define Check Code for Write/Erase Protect --------------------------
#define		AREA_EO_WE			0x11115678
#define		AREA_PROG_WE		0x11115678
#define		AREA_DATAPRO_WE		0x22225678
#define		AREA_KEY_WE			0x22225678
#define		AREA_PARA_WE		0x44445678
#define		AREA_OTP_WE			0x55555678
#define		AREA_CHIP_ERASE_WE	0x11115678
#define		AREA_PROT_DI		0x0

#define		AREA_KEY0_WE		0x01234567
#define		AREA_KEY1_WE		0x12345678
#define		AREA_KEY2_WE		0x23456789
#define		AREA_KEY3_WE		0x3456789A
#define		AREA_KEY4_WE		0x456789AB
#define		AREA_KEY5_WE		0x56789ABC
#define		AREA_KEY6_WE		0x6789ABCD
//------ Define the Different Area Start Address ---------------------------
#define		ASA_EO				0x00000000				// size 96 pages, disabled by default
#define		ASA_PROG			0x00018000				// size 320 pages by default, alterable
#define		ASA_DATAPRO			0x00040000				// size 448 pages by default, alterable
#define		ASA_KEY				0x00078000				// size 56 pages, disabled by default
#define		ASA_PARA			0x0007F000				// size 8 page, fixed
#define		ASA_OTP				0x00080000				// size 1 page, fixed

#define 	AREA_END_PARA_PAGE 	 1016
#define 	AREA_END_PARA_ADD	(FLASH_BASE_ADDR+ASA_PARA)
#define		AREA_EO_ST_ADD		(FLASH_BASE_ADDR+ASA_EO)		
#define		AREA_EO_END_ADD		(FLASH_BASE_ADDR+ASA_PROG)	
#define		AREA_EO_EN			0x0007FFF8						
#define		AREA_EO_EN_ADD		(FLASH_BASE_ADDR+AREA_EO_EN)	
#define		AREA_KEY_EN			0x0007FFFC						
#define		AREA_KEY_EN_ADD		(FLASH_BASE_ADDR+AREA_KEY_EN)
#define		AREA_KEY_BANK_SIZE	(4096)
#define		AREA_LOCK_JTAG		0x801F8						
#define		AREA_LOCK_JTAG_ADD	(FLASH_BASE_ADDR+AREA_LOCK_JTAG)
#define		AREA_LOCK_TEST		0x801FC						
#define		AREA_LOCK_TEST_ADD	(FLASH_BASE_ADDR+AREA_LOCK_TEST)	
#define		AREA_SN_ADD			(FLASH_BASE_ADDR+ASA_OTP)

#define		IS_ADDR_IN_EO(nAddr) ((nAddr>=AREA_EO_ST_ADD) && (nAddr<AREA_EO_END_ADD))	

// --------- Define the ECC enable check range mask ----------------
#define		ECC_EN_INFO			(1L<<31)
#define		ECC_EN_MAIN_TOP		(1L<<30)
#define		ECC_EN_MAIN_LOW		(1L<<29)

// ------ Define the time for flash -------------------
//#define		T_NVS		6400L	// 5~10 us
//#define		T_NVH		6400L	// 5~10 us
//#define		T_NVH1		120000L	// >=100us
//#define		T_PGS		6400L	// >=5us
//
//#define		T_PROG		2000L	// 1.3~2.7us
//#define		T_RCV		25000L	// >=20us
//#define		T_ERASE		100000L	// 66~133us	
//#define		T_ME		15000000L	// 10~20ms
//
//#define		T_ADH		50L		// >=20ns
//#define		T_PGH		50L		// >=20ns
//#define		T_RS		110L		// >=20ns
//#define		T_REC		330L	// >=100ns
//
//#define		T_ADS		50L		// >=20ns
//#define		T_ACCV		500L	// >=200ns
//#define		T_ACC		200L		// >=40ns

#define		TC0_DEFAULT		0x28283028
#define		TC1_DEFAULT		0x32900A5A
#define		TC2_DEFAULT		0x0044A008
#define		TC3_DEFAULT		0x00404020

#define		TC0_144M		0x3A3A443A
#define		TC1_144M		0x48CF0F82
#define		TC2_144M		0x00444006
#define		TC3_144M		0x00402B08

#define		TC0_96M		0x27272D27
#define		TC1_96M		0x308A0A57
#define		TC2_96M		0x00222004
#define		TC3_96M		0x00201C05

#define		TC0_72M		0x1D1D221D
#define		TC1_72M		0x24680841
#define		TC2_72M		0x00222003
#define		TC3_72M		0x00201504

#define		TC0_48M		0x14141714
#define		TC1_48M		0x1845052C
#define		TC2_48M		0x00111002
#define		TC3_48M		0x00100E02

#define		TC0_36M		0x0F0F110F
#define		TC1_36M		0x12340421
#define		TC2_36M		0x00111002
#define		TC3_36M		0x00100A02

#define		TC0_24M		0x0A0A0C0A
#define		TC1_24M		0x0C230316
#define		TC2_24M		0x00000001
#define		TC3_24M		0x00000701

#define		TC0_18M		0x08080908
#define		TC1_18M		0x091A0211
#define		TC2_18M		0x00000001
#define		TC3_18M		0x00000501

#define		TC0_12M		0x05050605
#define		TC1_12M		0x0612020B
#define		TC2_12M		0x00000001
#define		TC3_12M		0x00000300

#define		TC0_6M		0x03030303
#define		TC1_6M		0x03090106
#define		TC2_6M		0x00000000
#define		TC3_6M		0x00000100

#define		TC0_4M		0x02020202
#define		TC1_4M		0x02060104
#define		TC2_4M		0x00000000
#define		TC3_4M		0x00000100

#define		TC0_2M		0x01010101
#define		TC1_2M		0x01030102
#define		TC2_2M		0x00000000
#define		TC3_2M		0x00000000

#define		TC0_1M		0x01010101
#define		TC1_1M		0x01020101
#define		TC2_1M		0x00000000
#define		TC3_1M		0x00000000


// define the OTP user field ID and	user set field ID
typedef	enum	{
	EFC_OTP_UESR_1		= 0x01,
	EFC_OTP_UESR_2		= 0x02,
	EFC_OTP_UESR_SET	= 0x03,
}EFC_OTP_UESR_FIELD_ID;

// define the KEY area ID 
typedef	enum	{
	EFC_AREA_KEY_0		= 0x00,
	EFC_AREA_KEY_1		= 0x01,
	EFC_AREA_KEY_2		= 0x02,
	EFC_AREA_KEY_3		= 0x03,
	EFC_AREA_KEY_4		= 0x04,
	EFC_AREA_KEY_5		= 0x05,
	EFC_AREA_KEY_6		= 0x06,
}EFC_AREA_KEY_ID;

// define the function name for old firmware library revision
#define	Flash_TimConfig(TC0, TC1, TC2, TC3)	EFC_ConfigTiming(TC0, TC1, TC2, TC3)	
#define	Flash_Program		EFC_Program
#define	Flash_Erase			EFC_Erase
#define	Flash_WriteInfo		EFC_WriteInfo
#define	Flash_ReadInfo		EFC_ReadInfo
#define	OTPWrite			EFC_OtpWrite
#define	OTPRead				EFC_OtpRead
#define	ReadChipSN			EFC_ReadChipSN
#define	JtagLock			EFC_LockJtag
#define	SetCodeSt			EFC_SetEoSt
#define	ClrProgFlag			EFC_ClrProgFlag
#define	Flash_PageProg		EFC_PageProg		

/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
//UINT8 EFC_CalcParameters(UINT32	nFreq, UINT32 *pTC0, UINT32 *pTC1, UINT32 *pTC2, UINT32 *pTC3);
void EFC_ConfigTiming(UINT32 nTC0, UINT32 nTC1, UINT32 nTC2, UINT32 nTC3);
UINT8 EFC_Program(UINT32 *pData, UINT32 addr_align, UINT32 nLen);
UINT8 EFC_Erase(UINT32 addr_offset, UINT32 page_num);
void EFC_ChipErase(void);
UINT8 EFC_PreRead(UINT32 nPreReadMaxTimes);
UINT8 EFC_IsEraseOK(UINT32 nAddr, UINT32 nLen, UINT8 nMode);
UINT8 EFC_PageProg(UINT32 addr_offset, UINT32 *pData);
UINT8 EFC_MpuWrEnable(UINT32 nAddr);
UINT8 EFC_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 EFC_ReadInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);

UINT8 EFC_KEYWrite(EFC_AREA_KEY_ID nKeyAreaID, UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 EFC_KEYRead(EFC_AREA_KEY_ID nKeyAreaID, UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 EFC_OtpWrite(EFC_OTP_UESR_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32* pBuf);
UINT8 EFC_OtpDisWrite(EFC_OTP_UESR_FIELD_ID nUserFieldID);
UINT8 EFC_OtpRead(EFC_OTP_UESR_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32 *pBuf);
void EFC_ReadChipSN(UINT8 *pSN, UINT16* pLen);
UINT8 EFC_LockJtag(void);
UINT8 EFC_SetEoSt(UINT32 nPage);
UINT8 EFC_SetKeySt(UINT32 nPage);
void EFC_ClrProgFlag(UINT32 Key);
UINT8 EFC_LockTest(void);
void EFC_WrProtect(UINT32 permission);
void EFC_KEYWrProtect(UINT32 permission);
__INLINE void EFC_DisEncrypt(void);
__INLINE void EFC_ConfigCodeSz(UINT32 nPage);
__INLINE void EFC_EccEnable(UINT32 nEccEnMask);
__INLINE void EFC_EccDisable(UINT32 nEccEnMask);


/**************************************************************************
* Functon
***************************************************************************/

/**************************************************************************
* Subroutine:	EFC_DisEncrypt
* Function:		to disable flash encrypt operation
* Input:		None
* Output:		None
* Description:
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_DisEncrypt(void)
{
	EfcRegs.CTRL.all &= ~(UINT32)EFC_CTRL_ENCRYPT;
}

/**************************************************************************
* Subroutine:	EFC_EnEncrypt
* Function:		to enable flash encrypt operation
* Input:		None
* Output:		None
* Description:
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_EnEncrypt(void)
{
	EfcRegs.CTRL.all |= (UINT32)EFC_CTRL_ENCRYPT;
}

/**************************************************************************
* Subroutine:	EFC_ConfigCodeSz
* Function:		to config code area size
* Input:		UINT32 nPage --	to specify the page numbers for code area
* Output:		None
* Description:
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_ConfigCodeSz(UINT32 nPage)
{
	EfcRegs.DATA = FLASH_BASE_ADDR + nPage*FLASH_PAGE_SIZE;
}

/**************************************************************************
* Subroutine:	EFC_EccEnable
* Function:		to enable ECC 
* Input:		None
* Output:		None
* Description:
* Date:			2015.10.22
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_EccEnable(UINT32 nEccEnMask)
{
	EfcRegs.ECC3.all |= nEccEnMask;		
}
/**************************************************************************
* Subroutine:	EFC_EccDisable
* Function:		to disable ECC 
* Input:		None
* Output:		None
* Description:
* Date:			2015.10.22
* ModifyRecord:
* *************************************************************************/
__INLINE void EFC_EccDisable(UINT32 nEccEnMask)
{
	EfcRegs.ECC3.all &= ~(nEccEnMask);		
}



#endif /* EFC_H_ */
