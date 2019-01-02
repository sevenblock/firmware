/***************************************************************************************
File name    :			Sqi.h
Function     :			header of Sqi.c
Author       : 			coins
Date         :			2013/01/16
Version      :          v1.1
Description  :
ModifyRecord :
*	v1.1	2013/01/16	coins
*		1. To add define and function for MPU protected.
*****************************************************************************************/
#ifndef _SQI_H_
#define _SQI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
//------ Define Flash Read Mode ----------------------------------------------------
#ifndef		_SFLASH_RD_MODE_
#define 	_SFLASH_RD_MODE_
#define		SISOR		0			// Single I/O Read
#define		SISOFR		1			// Single I/O Fast Read
#define		SIDOFR		2			// Single Input Dual Output Fast Read
#define		DIDOFR		3			// Dual I/O Fast Read
#define		SIQOFR		4			// Single Input Quad Output Fast Read
#define		QIQOFR		5			// Quad I/O Fast Read
#define		QIQWOFR		6			// Quad Input Quad Word Output Fast Read
#endif

#define     DEFAULT_RD_MODE     DIDOFR    // define read mode by default 
#define     DEFAULT_CLK_DIV     2       // define clock divide by default 
#define		DEFAULT_MAP			SQI_MAP_A16_21

//------ Define SQI Flash Space Base Address -----------------------------
#define     SQI_BASE_ADDR     	0x18000000

//------ Define SQI Flash Space Page Size ---------------------------------------------
#define    SQI_PAGE_SIZE  		256

//------ Define SQI Flash Space Sector Size -------------------------------------------
#define    SQI_SECTOR_SIZE 		4096


//------ Define Check Code for Write/Erase Protect --------------------------
#define		SQI_PROG_WE			0x11115678
#define		SQI_DATAPRO_WE		0x33335678
#define		SQI_PARA_WE			0x44445678
#define		SQI_CHIP_ERASE_EE	0x56784444		
#define		SQI_PROT_DI			0x0

//------ Define the Different Area Start Address Offset ---------------------------
#define		SQI_AREA_PARA		0x00000000		// size 1 sector, fixed
#define		SQI_AREA_PROG		0x00001000		// size 63 sectors by default, alterable
#define		SQI_AREA_DATAPRO	0x00040000		// size 256 sectors by default, alterable
#define		SQI_AREA_DATAOPEN	0x00140000		// size 192 sectors by default, alterable
	

/**************************************************************************
* Global Type Definition
***************************************************************************/
//------ Define Flash Device ID struct --------------------------------------
#ifndef		_SFLASH_ID_STRUCT_
#define		_SFLASH_ID_STRUCT_
typedef struct
{
	UINT8 Capacity;
	UINT8 MemoryType;
	UINT8 Maufactor;

}T_FLASH_ID;
#endif

// Define the SSI Port Map to GPIO Port
typedef	enum {
	SQI_MAP_A10_15 = 0,
			//GPA3 -- SSI_SSN	
			//GPA9 -- SSI_RXD	
			//GPA7 -- SSI_CLK	
			//GPA8 -- SSI_TXD
	SQI_MAP_A16_21 = 1,
			//GPA20 -- SSI_SSN	
			//GPA19 -- SSI_RXD	
			//GPA18 -- SSI_CLK	
			//GPA17 -- SSI_TXD
}SQI_MAP;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern UINT8 gbInitSBF;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
//----- Declare Functions ----------------------------------------------------

void SQI_ReadID(T_FLASH_ID *flashID);
UINT8 SQI_Init(SQI_MAP mPortMap, UINT8 clk_div, UINT8 read_mode);
UINT8 SQI_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 SQI_SectErase(UINT32 sect);
UINT8 SQI_PageProg(UINT32 byte_addr, UINT32 *pData);
UINT8 SQI_ReadData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
UINT8 SQI_WriteData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
UINT8 SQI_ReadUniqueID(UINT8 *pID,UINT32 UniqueIDAddr);
UINT8 SQI_SecRegProg(UINT32 nSecRegId, UINT32 nAddress, UINT32 *pData, UINT32 nNum);
UINT8 SQI_SecRegErase(UINT32 nSecRegId);
UINT8 SQI_SecRegRead(UINT32 nSecRegId, UINT32 nAddress, UINT8 *pData, UINT32 nNum);
UINT8 SQI_SecRegDisProg(UINT32 nSecRegId);
/***************************************************************************
* Subroutine:	SQI_WrProtect
* Function:		SQI flash write and erase protect enable or disable with proper
* 				check code.
* Input:		UINT32 permission	-- 	the permission for an operation;
* Output:		None
* Description:	the permission value defined as following:
* 				0x1111_5678 -- 	program area writable and erasable;
* 				0x3333_5678 -- 	protected data area writable and erasable;
* 				0x4444_5678 -- 	Parameter area writable and erasable;
*				0x5678_4444 --	chip erasable enable.
* Date:		2013.01.16
* ModifyRecord:
* *************************************************************************/
__INLINE void SQI_WrProtect(UINT32 permission)
{
	SqiRegs.CHECK = permission;
}

#endif   /*SQI.H*/
