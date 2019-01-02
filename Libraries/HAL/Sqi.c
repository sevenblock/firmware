/***************************************************************************************
* File name    :	Sqi.c
* Function     :	external SQI flash operation
* Author       : 	coins
* Date         :	2013/01/16
* Version      :    v1.1
* Description  :
* ModifyRecord :
*	v1.1	2013/01/16	coins
*		1. To add function SQI_MpuWrEnable and modify function SQI_SectErase, SQI_ChipErase, 
*		SQI_PageProg and SQI_QIPageProg for MPU protected.
*****************************************************************************************/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "AS569.h"


/**************************************************************************
* Local Macro Definition
***************************************************************************/

//#define		SQI_ROM_FUNC_USED

//----------------------------------------------------------------------------
//----- Define the Commands for SQI Flash --------
#define		SFC_WREN		0x06		// Write Enable
#define		SFC_WRDI		0x04		// Write Disable
#define		SFC_RDSR		0x05		// Read Status Register
#define		SFC_RDSR1		0x35		// Read Status Register-1
#define		SFC_WRSR		0x01		// Write Status Register
#define		SFC_RD			0x03		// Read Data
#define		SFC_FRD			0x0B		// Fast Read
#define		SFC_DOFRD		0x3B		// Dual Output Fast Read
#define		SFC_DIOFRD		0xBB		// Dual I/O Fast Read
#define		SFC_QOFRD		0x6B		// Quad Output Fast Read
#define		SFC_QIOFRD		0xEB		// Quad I/O Fast Read
#define		SFC_QIOWFRD		0xE7		// Quad I/O Word Fast Read
#define		SFC_PP			0x02		// Page Program
#define		SFC_SE			0x20		// Sector Erase
#define		SFC_B32E		0x52		// Block 32K Erase
#define		SFC_B64E		0xD8		// Block 64K Erase
#define		SFC_RDID		0x9F		// Read ID
#define		SFC_HPM			0xA3		// High Performance Mode
#define 	SFC_ESR			0x44		//Erase Security Register
#define 	SFC_PSR		    0x42		//Program Security Register
#define 	SFC_RSR			0x48		//Read Security Register

#define		SFC_QIPP		0x32		// Quad Input Page Program
#define		SFC_CE			0x60//0xC7		// Chip Erase

//----- Define the bits of Flash Status Register -----------------------------
#define		SFSR_WIP		0			// The bit Write In Progress
#define		SFSR_WEL		1			// The bit Write Enable Latch
#define		SFSR_BP			2			// The bits Block Protect (BP4~BP0)
#define		SFSR_SPR		7			// The bits Status Register Protect (SPR1~SPR0)
#define		SFSR_QE			9			// The bit Quad Enable

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static UINT8 gSqiClkDiv = 1;	// SQI clock divider
static UINT8 gPortMap = DEFAULT_MAP;	// SQI Port Map
/**************************************************************************
* Global Variable Declaration
***************************************************************************/
UINT8 gbInitSBF=0;

#pragma arm section code = ".ram_code"
/**************************************************************************
* Local Functon Declaration
***************************************************************************/
void SQI_MpuWrEnable(UINT32 nOffsetAddr);

/**************************************************************************
* Functon
***************************************************************************/


/****************************************************************************
 * Subroutine:	WaitSqiCycles
 * Function:	to wait SQIB cycles
 * Input:		UINT16 n --		specifies the wait SQI cycles
 * Output:		None
 * Description:
 * Date:		2010.09.07
 * ModifyRecord:
 * *************************************************************************/
void WaitSqiCycles(UINT16 n)
{
	UINT32 cycles;
	cycles = gSqiClkDiv * n;
	while(cycles--);
}

/**************************************************************************
* Subroutine:	SQI_ReadID
* Function:		to read SQI flash device ID
* Input:		T_FLASH_ID *flashID --	to specify the space to store flash
* 										device ID;
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_ReadID(T_FLASH_ID *flashID)
{
	UINT8 Maufactor;
	UINT8 MemoryType;
	UINT8 Capacity;

	// 1. Set INST (Read ID)
	SqiRegs.INST.all = (1<<NUM_INST) | ((UINT32)SFC_RDID<<INST_DATA);
	// 2. Force CS to low
	// why here using "|=", not "="? to prevent modifying Encrypt
	SqiRegs.INST_SET.all |= (1<<CS_LOW);
    // 3. Channel enabled, for read data from DATA_ADDR register
	SqiRegs.INST_SET.all |= (1<<EN_CHANNEL);
	// 4. Read flash ID
	Maufactor = SqiRegs.DATA_ADDR.all;
	MemoryType = SqiRegs.DATA_ADDR.all;
	Capacity = SqiRegs.DATA_ADDR.all;
	// 5. Wait for SQI pipeline
	WaitSqiCycles(8);
	//rom_func.delay_ms(1, rom_func.GetSysClkInMHz());
	// 6. Channel disabled, CS to release and clear INST
	SqiRegs.INST_SET.all &= ~(UINT32)((1<<CS_LOW) | (1<<EN_CHANNEL));
	SqiRegs.INST.all = 0;

	flashID->Capacity = Capacity;
	flashID->Maufactor = Maufactor;
	flashID->MemoryType = MemoryType;
}

/**************************************************************************
* Subroutine:	SQI_WREN
* Function:		to assert SQI flash write enable
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_WREN(void)
{
	// 1. Set INST (Write Enable)
	SqiRegs.INST.all = (1<<NUM_INST) | ((UINT32)SFC_WREN<<INST_DATA);
	// 2. Start
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(8);
 	// 3. Clear INST
	SqiRegs.INST.all=0;
}

/**************************************************************************
* Subroutine:	SQI_WRDI
* Function:		to assert SQI flash write disable
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_WRDI(void)
{
	// 1. Set INST (Write Enable)
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_WRDI<<INST_DATA);
	// 2. Start
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(8);
 	// 3. Clear INST
	SqiRegs.INST.all=0;
}

/**************************************************************************
* Subroutine:	SQI_HPM
* Function:		to assert SQI flash into High Performance Mode for higher speed.
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_HPM(void)
{
	// 1. Set INST (Write Enable)
	SqiRegs.INST.all = (1<<NUM_INST) | ((UINT32)SFC_HPM<<INST_DATA) | (3<<NUM_DUMMY);
	// 2. Start
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(32);
 	// 3. Clear INST
	SqiRegs.INST.all=0;
}

/**************************************************************************
* Subroutine:	SQI_RDSR
* Function:		to read SQI flash Status Register
* Input:		None
* Output:		UINT16 --	to return the value of Status Register
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
UINT16 SQI_RDSR(void)
{
	UINT16 status_reg;
	// 1. Set INST (Read Status Register-1)
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_RDSR1<<INST_DATA);
	// 2. Channel enabled, for read data from DATA_ADDR register
	SqiRegs.INST_SET.all |= (1<<EN_CHANNEL);
	// 3. Read value of Status Register S15~S8 from DATA_ADDR
	status_reg = SqiRegs.DATA_ADDR.all;
	WaitSqiCycles(16);
	status_reg <<= 8;
	// 4. Channel disabled
	SqiRegs.INST_SET.all &= ~(UINT32)(1<<EN_CHANNEL);
	// 5. Set INST (Read Status Register)
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_RDSR<<INST_DATA);
	// 6. Channel enabled, for read data from DATA_ADDR register
	SqiRegs.INST_SET.all |= (1<<EN_CHANNEL);
	// 7. Read value of Status Register S7~S0 from DATA_ADDR
	status_reg |= SqiRegs.DATA_ADDR.all;
	WaitSqiCycles(16);
	// 8. Channel disabled, and INST cleared
	SqiRegs.INST_SET.all &= ~(UINT32)(1<<EN_CHANNEL);
	SqiRegs.INST.all = 0;

	return status_reg;
}

/**************************************************************************
* Subroutine:	SQI_WaitBusy
* Function:		to wait SQI flash progress
* Input:		UINT32 n_100us --	to specify the timeout unit in 100 us
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_WaitBusy(UINT32 n_100us)
{
	while((SQI_RDSR()&(1<<SFSR_WIP)) && n_100us--) _delay_us(100);
	if(SQI_RDSR()&(1<<SFSR_WIP)) return RT_FAIL;
	else return RT_OK;
}

/**************************************************************************
* Subroutine:	SQI_WRSR
* Function:		to write SQI Status Registers
* Input:		UINT16 status_reg --	to specify the value of Status
* 										Register. the status_reg[15:8] is
* 										S15~S8; while status_reg[7:0] is
* 										S7~S0.
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_WRSR(UINT16 status_reg)
{
	// 1. Enable write
	SQI_WREN();
	// 2. Set INST (Write Status Register, 2 parameter)
	SqiRegs.INST.all = (SFC_WRSR<<INST_DATA) | (1<<NUM_INST) | (2<<NUM_ADDR);
	// 3. Write parameter
	SqiRegs.SPEC.all = ((status_reg&0x00ff)<<REG1) | ((status_reg&0xff00)>>8);
	// 4. Start with parameter in SPEC register
	SqiRegs.INST_SET.all |= (1<<START) | (1<<EN_SPEC);
	WaitSqiCycles(24);
	// 5. Clear SPEC and INST
	SqiRegs.SPEC.all = 0;
	SqiRegs.INST.all = 0;
	// 6. Wait Write Status Register Cycle Time (tw) 15ms
	return SQI_WaitBusy(500);
}

/**************************************************************************
* Subroutine:	SQI_SectErase
* Function:		to erase the SQI flash sector(4K bytes) which the specified
* 				byte address is in.
* Input:		UINT32 byte_addr --	to specify the byte address
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:	Note that the address is not the system address, only the
* 				offset of flash, which is start from 0, not the base address
* 				of flash memory space.
* Date:			2010.09.07
* ModifyRecord:
* 1. mpu protected code added.	2013/01/16	coins
* *************************************************************************/
UINT8 SQI_SectErase(UINT32 byte_addr)
{
	SQI_MpuWrEnable(byte_addr);		// mpu protected code added, 2013.01.16
	// 1. Enable write
	SQI_WREN();
	// 2. Set INST (Sector Erase, 3 addresses)
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_SE<<INST_DATA) | (3<<NUM_ADDR);
	// 3. Set addresses
	SqiRegs.SPEC.all = byte_addr;
	// 4. Start with addresses in SPEC register
	SqiRegs.INST_SET.all |= (1<<START) | (1<<EN_SPEC);
	WaitSqiCycles(32);
	// 5. Clear SPEC and INST
	SqiRegs.SPEC.all = 0;
	SqiRegs.INST.all = 0;
	// 6. Wait Sector Erase Time (tse) 400ms
	return SQI_WaitBusy(8000);
}

/**************************************************************************
* Subroutine:	SQI_BlockErase
* Function:		to erase the SQI flash block(4K,32K,64K bytes) which the specified
* 				byte address is in.
* Input:		UINT8 Command --(0xd8->64k;0x52->32k;0x20->4k)
*				UINT32 Address --	to specify the block address
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:	Note that the address is not the system address, only the
* 				offset of flash, which is start from 0, not the base address
* 				of flash memory space.
* Date:			2016.11.24
* ModifyRecord:
* 
* *************************************************************************/
UINT8 SQI_BlockErase(UINT8 Command,UINT32 Address)
{
	UINT32 status32;
	UINT8 nRet;
	UINT32 n100us; 
	UINT32 nTempAddress = (Address&0x00FFFFFF); 

	//Sector/BLOCK Erase Time (tse) 400,1600,2000ms
	if(Command == SFC_SE)	n100us = 8000;
	else if(Command == SFC_B32E)	n100us = 30000;
	else if(Command == SFC_B64E)	n100us = 40000;
	else return RT_PARAM_ERR;

	status32 = __get_PRIMASK();
	__disable_irq();

	SQI_MpuWrEnable(Address);		
	// 1. Enable write
	SQI_WREN();
	// 2. Force CS to low
	if(gPortMap == SQI_MAP_A10_15)
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xffffdfff)|0x00002000;
		GpioRegs.GPAUSE1.all=(GpioRegs.GPAUSE1.all&0xff0fffff)|0x00800000;
		WaitSqiCycles(10);
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xffffdfff)|0x00000000;
	}
	else
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xfff7ffff)|0x00080000;
		GpioRegs.GPAUSE2.all=(GpioRegs.GPAUSE2.all&0xffff0fff)|0x00008000;
		WaitSqiCycles(10);
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xfff7ffff)|0x00000000;
	}
	// 3. Set INST (BlockErase)
	SqiRegs.INST.all = (1<<NUM_INST) | (Command<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	// 3. Set addresses
	SqiRegs.INST.all = (1<<NUM_INST) | (((nTempAddress>>16)&0x000000FF)<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	SqiRegs.INST.all = (1<<NUM_INST) | (((nTempAddress>>8)&0x000000FF)<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	SqiRegs.INST.all = (1<<NUM_INST) | (((nTempAddress>>0)&0x000000FF)<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	// 4. Force CS to High
	if(gPortMap == SQI_MAP_A10_15)
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xffffdfff)|0x00002000;
		GpioRegs.GPAUSE1.all=(GpioRegs.GPAUSE1.all&0xff0000ff)|0x00111100;
	}
	else
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xfff7ffff)|0x00080000;
		GpioRegs.GPAUSE2.all=(GpioRegs.GPAUSE2.all&0xfff0000f)|0x00011110;
	}
	WaitSqiCycles(32);
	// 5. Clear INST
	SqiRegs.INST.all = 0;
	// 6. Wait Sector/BLOCK Erase Time (tse)
	nRet = SQI_WaitBusy(n100us);
	__set_PRIMASK(status32);
	return nRet;
}

/**************************************************************************
* Subroutine:	SQI_ChipErase
* Function:		to erase the SQI flash chip which the specified
* 				byte address is in.
* Input:		none
* Output:		none
* Description:	
* Date:			2012.12.17
* ModifyRecord:
* 1. mpu protected code added.	2013/01/16	coins
* *************************************************************************/
void SQI_ChipErase(void)
{	
	UINT16 status;
	SQI_WrProtect(SQI_CHIP_ERASE_EE);	// mpu protected code added, 2013.01.16
	// 1. Enable write
	SQI_WREN();	
	// 2. Set INST (Chip Erase )
	SqiRegs.INST.all = (1<<NUM_INST) | ((UINT32)SFC_CE<<INST_DATA);
	// 3. Start 	
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(100);
	// 4. Clear INST
	SqiRegs.INST.all=0;
	// 5. Wait Chip Erase completed
	status=SQI_RDSR();
	while(status & 0x0001)
	{
		status = SQI_RDSR();
	}	
	SqiRegs.INST.all=0;	 
}

/**************************************************************************
* Subroutine:	SQI_PageProg
* Function:		to program the SQI flash one page(256 bytes) which the
* 				specified byte address is in.
* Input:		UINT32 byte_addr --	to specify the byte address;
* 				UINT32 *pData --	to specify the 256-byte-size 4-byte
* 									alignment data buffer for program;
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:	Note that the address is not the system address, only the
* 				offset of flash, which is start from 0, not the base address
* 				of flash memory space.
* Date:			2010.09.07
* ModifyRecord:
* 1. mpu protected code added.	2013/01/16	coins
* *************************************************************************/
UINT8 SQI_PageProg(UINT32 byte_addr, UINT32 *pData)
{
	#ifndef	SQI_ROM_FUNC_USED  
	UINT8 i;
	UINT32 *pTempData = pData;
	UINT32 *pTempp;
	UINT8 nRet = RT_FAIL;
	volatile UINT32 *p = (volatile UINT32 *)(SQI_BASE_ADDR+(byte_addr&0xFFFFFF00));
//	SQI_MpuWrEnable(byte_addr);		// mpu protected code added, 2013.01.16
	__nop();
	pTempp = p; 
	// 1. Enable write
	SQI_WREN();
	// 2. Set INST (Page Program, 3 addresses)
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_PP<<INST_DATA) | (3<<NUM_ADDR);
	// 3. Force CS to low
//	SqiRegs.INST_SET.all |= (1<<EN_ENCRYPT);
	SqiRegs.INST_SET.all |= (1<<CS_LOW);
	// 4. Word Program (only supportted word)
	for(i=0;i<64;i++)	// 64 word = 256 byte = 1 page
	{
		*p++ = *pData++;
	}
	WaitSqiCycles(32);
	// 5. Release CS, and clear INST
//	SqiRegs.INST_SET.all &= ~(1<<EN_ENCRYPT);
	SqiRegs.INST_SET.all &= ~(UINT32)(1<<CS_LOW);
	SqiRegs.INST.all = 0;
	// 6. Wait Page Programming Time (tpp) 3.0ms
	nRet = SQI_WaitBusy(500);
	if(nRet != RT_OK) return nRet;
	// 7. check
	if(memcmp((UINT8*)pTempData,(UINT8*)pTempp,SQI_PAGE_SIZE) != 0) 
		nRet = RT_FAIL;
	return nRet; 
	#else 
	return _SQI_PageProg(byte_addr,pData); 
	#endif 
}
/**************************************************************************
* Subroutine:	SQI_QIPageProg
* Function:		to program the SQI flash one page(256 bytes) which the
* 				specified byte address is in. (by Quad Input Page Program)
* Input:		UINT32 byte_addr --	to specify the byte address;
* 				UINT32 *pData --	to specify the 256-byte-size 4-byte
* 									alignment data buffer for program;
* Output:		none
* Description:	Note that the address is not the system address, only the
* 				offset of flash, which is start from 0, not the base address
* 				of flash memory space.
* Date:			2012.12.21
* ModifyRecord:
* 1. mpu protected code added.	2013/01/16	coins
* *************************************************************************/
void SQI_QIPageProg(UINT32 byte_addr, UINT32 *pData)
{
	UINT8 i;
	UINT16 status_reg;

	volatile UINT32 *p=(UINT32*)(SQI_BASE_ADDR+(byte_addr&0xFFFFFF00));
	SQI_MpuWrEnable(byte_addr);		// mpu protected code added, 2013.01.16
	// 1. Enable write	
	SQI_WREN();
	// 2. Check QE and to set it to 1
	status_reg = SQI_RDSR();
	if((status_reg&(1<<SFSR_QE))==0) SQI_WRSR(status_reg|(1<<SFSR_QE));
	WaitSqiCycles(100);
	// 3. Assert HPM
	SQI_HPM();
	// 4. Set INST (Quad Input Page Program, 3 addresses, quad data mode)
	SQI_WREN();
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_QIPP<<INST_DATA) | (3<<NUM_ADDR) \
					| (2<<MODE_DATA);
	// 5. Force CS to low
	SqiRegs.INST_SET.all |= (1<<CS_LOW);
	// 6. Word Program 	
	for(i=0;i<64;i++)	  //256 Byte= 64*4
	{
		*p=*pData++;
		p++;
	}
	WaitSqiCycles(100);
	// 7. Release CS, and clear INST
	SqiRegs.INST_SET.all &= ~(UINT32)(1<<CS_LOW);
	SqiRegs.INST.all=0;
	// 8. Wait Quad Input Page Program completed
	status_reg = SQI_RDSR();
	while(status_reg & 0x0001)
	{
		status_reg = SQI_RDSR();
	}	
	SqiRegs.INST.all=0;	
}
					   
/**************************************************************************
* Subroutine:	SQI_SetRead
* Function:		to set SQI flash read in normal mode(single low-speed read).
* Input:		None
* Output:		None
* Description:	Flash read in this mode when power on
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_SetRead(void)
{
	// Read Data, 3 addresses
	SqiRegs.INST_READ.all = (1<<NUM_INST) | (SFC_RD<<INST_DATA) | (3<<NUM_ADDR);
}

/**************************************************************************
* Subroutine:	SQI_SetFastRead
* Function:		to set SQI flash read in higher speed mode(single high-speed read).
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_SetFastRead(void)
{
	// Fast Read, 3 addresses, 1 dummy
	SqiRegs.INST_READ.all = (1<<NUM_INST) | (SFC_FRD<<INST_DATA) | (3<<NUM_ADDR) \
		| (1<<NUM_DUMMY);
}

/**************************************************************************
* Subroutine:	SQI_SetDOFastRead
* Function:		to set SQI flash read in Dual Output Fast Read.
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_SetDOFastRead(void)
{
	// Dual Output Fast Read, 3 addresses, 1 dummy, dual data mode
	SqiRegs.INST_READ.all = (1<<NUM_INST) | (SFC_DOFRD<<INST_DATA) | (3<<NUM_ADDR) \
		 | (1<<NUM_DUMMY) | (1<<MODE_DATA);
}

/**************************************************************************
* Subroutine:	SQI_SetDIOFastRead
* Function:		to set SQI flash read in Dual I/O Fast Read with HPM.
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_SetDIOFastRead(void)
{
	// 1. assert HPM
	SQI_HPM();
	// 2. Dual I/O Fast Read with High Performache Mode, 3 addresses,
	// 		1 dummy (M7~M0), dual address, dummy, data mode
	SqiRegs.INST_READ.all = (1<<NUM_INST) | ((UINT32)SFC_DIOFRD<<INST_DATA) | \
		(3<<NUM_ADDR) | (1<<NUM_DUMMY) | \
		(1<<MODE_ADDR) | (1<<MODE_DUMMY) | (1<<MODE_DATA);
}

/**************************************************************************
* Subroutine:	SQI_SetQOFastRead
* Function:		to set SQI flash read in Quad Output Fast Read.
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_SetQOFastRead(void)
{
	UINT16 status_reg;
	// 1. check QE and to set it to 1
	status_reg = SQI_RDSR();
	if((status_reg&(1<<SFSR_QE))==0) SQI_WRSR(status_reg|(1<<SFSR_QE));
	// 2. Quad Output Fast Read, 3 addresses, 1 dummy, quad data mode
	SqiRegs.INST_READ.all = (1<<NUM_INST) | (SFC_QOFRD<<INST_DATA) | (3<<NUM_ADDR) \
		 | (1<<NUM_DUMMY) | (2<<MODE_DATA);
}

/**************************************************************************
* Subroutine:	SQI_SetQIOFastRead
* Function:		to set SQI flash read in Quad I/O Fast Read with HPM.
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_SetQIOFastRead(void)
{
	UINT16 status_reg;
	// 1. check QE and to set it to 1
	status_reg = SQI_RDSR();
	if((status_reg&(1<<SFSR_QE))==0) SQI_WRSR(status_reg|(1<<SFSR_QE));
	// 2. assert HPM
	SQI_HPM();
	// 3. Quad I/O Fast Read with High Performache Mode, 3 addresses,
	// 		1 dummy (M7~M0), 2 wait, quad address, dummy, wait, data mode
	SqiRegs.INST_READ.all = (1<<NUM_INST) | ((UINT32)SFC_QIOFRD<<INST_DATA) | \
		(3<<NUM_ADDR) | (1<<NUM_DUMMY) | (2<<NUM_WAIT) | \
		(2<<MODE_ADDR) | (2<<MODE_DUMMY) | (2<<MODE_WAIT) |(2<<MODE_DATA);
}

/**************************************************************************
* Subroutine:	SQI_SetQIOWFastRead
* Function:		to set SQI flash read in Quad I/O Word Fast Read with HPM.
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
void SQI_SetQIOWFastRead(void)
{
	UINT16 status_reg;
	// 1. check QE and to set it to 1
	status_reg = SQI_RDSR();
	if((status_reg&(1<<SFSR_QE))==0) SQI_WRSR(status_reg|(1<<SFSR_QE));
	// 2. assert HPM
	SQI_HPM();
	// 3. Quad I/O Word Fast Read with High Performache Mode, 3 addresses,
	// 		1 dummy (M7~M0), 1 wait, quad address, dummy, wait, data mode
	SqiRegs.INST_READ.all = (1<<NUM_INST) | ((UINT32)SFC_QIOWFRD<<INST_DATA) | \
		(3<<NUM_ADDR) | (1<<NUM_DUMMY) | (1<<NUM_WAIT) | \
		(2<<MODE_ADDR) | (2<<MODE_DUMMY) | (2<<MODE_WAIT) |(2<<MODE_DATA);
}

/**************************************************************************
* Subroutine:	SQI_Init
* Function:		to initialize SQI flash clock and read mode.
* Input:		None
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_Init(SQI_MAP mPortMap, UINT8 clk_div, UINT8 read_mode)
{	
	if((read_mode>QIQWOFR) | (clk_div==0)) return RT_FAIL;
	// 1. Release from reset and enable clock
	SCM_DisableModClk(ID_SQI);// disable SQI clock
	SCM_KeepModRst(ID_SQI);// reset
	SCM_EnableModClk(ID_SQI);// enable SQI clock
	SCM_ReleaseModRst(ID_SQI);// Relinquish from reset
	// 2. Select ports for SQI	
	switch(mPortMap)
	{
		case SQI_MAP_A10_15:
			// to select Pins for SSI signals
			if((read_mode==QIQOFR) || (read_mode==QIQWOFR) || (read_mode==SIQOFR))
			{
				GpioRegs.GPAUSE1.bit.GPA10=MUX_SQI;		//IO0 TP2-56
				GpioRegs.GPAUSE1.bit.GPA11=MUX_SQI;		//IO1 TP2-55
				GpioRegs.GPAUSE1.bit.GPA14=MUX_SQI;		//IO2 TP2-62
				GpioRegs.GPAUSE1.bit.GPA15=MUX_SQI;		//IO3 TP2-61
				GpioRegs.GPAUSE1.bit.GPA12=MUX_SQI;		//CLK TP2-58
				GpioRegs.GPAUSE1.bit.GPA13=MUX_SQI;		//CS  TP2-57
			}
			else
			{
				GpioRegs.GPAUSE1.bit.GPA10=MUX_SQI;		//IO0
				GpioRegs.GPAUSE1.bit.GPA11=MUX_SQI;		//IO1
				GpioRegs.GPAUSE1.bit.GPA12=MUX_SQI;		//CLK
				GpioRegs.GPAUSE1.bit.GPA13=MUX_SQI;		//CS	
			}
			break;
		case SQI_MAP_A16_21:
			if((read_mode==QIQOFR) || (read_mode==QIQWOFR) || (read_mode==SIQOFR))		// modify by coins
			{
				GpioRegs.GPAUSE2.bit.GPA17=MUX_SQI;		//IO0 TP2-68
				GpioRegs.GPAUSE2.bit.GPA20=MUX_SQI;		//IO1 TP2-63
				GpioRegs.GPAUSE2.bit.GPA16=MUX_SQI;		//IO2 TP2-67
				GpioRegs.GPAUSE2.bit.GPA21=MUX_SQI;		//IO3 TP2-64
				GpioRegs.GPAUSE2.bit.GPA18=MUX_SQI;		//CLK TP2-65
				GpioRegs.GPAUSE2.bit.GPA19=MUX_SQI;		//CS  TP2-66
			}
			else
			{
				GpioRegs.GPAUSE2.bit.GPA17=MUX_SQI;		//IO0
				GpioRegs.GPAUSE2.bit.GPA20=MUX_SQI;		//IO1
				GpioRegs.GPAUSE2.bit.GPA18=MUX_SQI;		//CLK
				GpioRegs.GPAUSE2.bit.GPA19=MUX_SQI;		//CS	
			} 
			break;
		default:
			return RT_FAIL;
	}
	gPortMap = mPortMap;
	// 3. Set clock
	if(clk_div<2)
	{
		ScmRegs.SYSCLKCTRL.bit.ClkSqiEn = 0;   //SQI时钟为系统时钟
		SqiRegs.CLKGEN.all = 0;
		gSqiClkDiv = 1;
	}	
	else
	{
		SqiRegs.CLKGEN.all = (1<<EN_DIV) | (clk_div/2-1);
		ScmRegs.SYSCLKCTRL.bit.ClkSqiEn = 1;
		gSqiClkDiv = clk_div;
	}
	// 4. Set Flash Read Mode
	SQI_WRSR(0x00);
	switch(read_mode)
	{
		case SISOR:		SQI_SetRead(); break;
		case SISOFR:	SQI_SetFastRead(); break;
		case SIDOFR:	SQI_SetDOFastRead(); break;
		case DIDOFR:	SQI_SetDIOFastRead(); break;
		case SIQOFR:	SQI_SetQOFastRead(); break;
		case QIQOFR:	SQI_SetQIOFastRead(); break;
		case QIQWOFR:	SQI_SetQIOWFastRead(); break;
		default: return RT_FAIL;
	}
	SqiRegs.INST_SET.bit.BurstEn = 1;
	gbInitSBF = 1;
	return RT_OK;
}

/**************************************************************************
* Subroutine:	SQI_WriteInfo
* Function:		write data to sqi flash
* Input:		pDataBuf-buffer to save data
                 lAddress-Offset address
                 lLen-The num of byte to write
* Output:		None
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{
	#ifndef	SQI_ROM_FUNC_USED
	UINT32 i;
	UINT32 addr_align_sect;
	UINT32 offset_sect;
	UINT8 buf[SQI_SECTOR_SIZE];
	UINT8 ret;
	UINT32 status32;

	// count sector aligned address and offset to sector
	addr_align_sect = (lAddress/SQI_SECTOR_SIZE)*SQI_SECTOR_SIZE;
	offset_sect = lAddress%SQI_SECTOR_SIZE;
	
	while(lLen)
	{
		// copy a sector to buffer
		memcpy(buf, (void *)(SQI_BASE_ADDR+addr_align_sect), SQI_SECTOR_SIZE);
		// erase a sector
		status32 = __get_PRIMASK();
		__disable_irq();
		ret = SQI_SectErase(addr_align_sect);
		SCM_FlushCache();
		__set_PRIMASK(status32);
		if(ret!=RT_OK) return ret;
		// copy data to buffer
		for(i=offset_sect;(i<SQI_SECTOR_SIZE)&&(lLen);i++,lLen--)
		{
			buf[i] = *pDataBuf++;
		}
		// write sector
		status32 = __get_PRIMASK();
		__disable_irq();
		for(i=0;i<SQI_SECTOR_SIZE/SQI_PAGE_SIZE;i++)
		{
			ret = SQI_PageProg(addr_align_sect+i*SQI_PAGE_SIZE, (UINT32 *)&buf[i*SQI_PAGE_SIZE]);
			if(ret!=RT_OK) break;
		}
		SCM_FlushCache();
		__set_PRIMASK(status32);
		if(ret!=RT_OK) return ret;
		// point to next sector and offset to be 0
		addr_align_sect += SQI_SECTOR_SIZE;
		offset_sect = 0;
	}
	return RT_OK;
	#else
	return _SQI_WriteInfo(pDataBuf, lAddress, lLen);
	#endif
}

/**************************************************************************
* Subroutine:	SQI_ReadData
* Function:		read data from serial flash in normal block
* Input:		pDataBuf-buffer to save data
                 nOffset-Offset address
                 nDataNum-The num of byte to read
* Output:		RT_OK-success
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_ReadData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum)
{
	if(gbInitSBF==0)
	{    	       
		SQI_Init(DEFAULT_MAP, DEFAULT_CLK_DIV, DEFAULT_RD_MODE);
	}      
	memcpy(pDataBuf,(UINT8*)(SQI_BASE_ADDR+nOffset),nDataNum)  ;
	return RT_OK; 
}

/**************************************************************************
* Subroutine:	SQI_WriteData
* Function:		write data to serial flash in normal block.
* Input:		pDataBuf-buffer to save data
                 nOffset-Offset address
                 nDataNum-The num of byte to write
* Output:		RT_OK-success
* Description:
* Date:			2010.09.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_WriteData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum)
{
	if(gbInitSBF==0)
	{     	          	       
		SQI_Init(DEFAULT_MAP, DEFAULT_CLK_DIV, DEFAULT_RD_MODE);
	} 
	#ifndef	SQI_ROM_FUNC_USED  
	return SQI_WriteInfo(pDataBuf,nOffset,nDataNum);
	#else 
	return _SQI_WriteInfo(pDataBuf,nOffset,nDataNum); 
	#endif      
}

/**************************************************************************
* Subroutine:	SQI_MpuWrEnable
* Function:		write data to serial flash in normal block.
* Input:		pDataBuf-buffer to save data
                 nOffset-Offset address
                 nDataNum-The num of byte to write
* Output:		RT_OK-success
* Description:
* Date:			2013.01.16
* ModifyRecord:
* *************************************************************************/
static void SQI_MpuWrEnable(UINT32 nOffsetAddr)
{
	if(nOffsetAddr<SQI_AREA_PROG) 	// parameter area
	{
		SQI_WrProtect(SQI_PARA_WE);	
	}
	else if(nOffsetAddr<(SqiRegs.DATAPRO-SQI_BASE_ADDR))	// program area
	{
		SQI_WrProtect(SQI_PROG_WE);
	}
	else if(nOffsetAddr<(SqiRegs.DATAOPEN-SQI_BASE_ADDR))	// protected data area
	{
		SQI_WrProtect(SQI_DATAPRO_WE);
	}
}

/**************************************************************************
* Subroutine:	SQI_ReadUniqueID
* Function:		Read SQI Flash Unique ID.
* Input:		pID --	Unique ID
*               UniqueIDAddr --	UniqueID Address
* Output:		RT_OK
* Description:
* Date:			2015.05.05
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_ReadUniqueID(UINT8 *pID,UINT32 UniqueIDAddr)
{
	UINT64 uID;
	UINT32 nInstRead;
	
	nInstRead = SqiRegs.INST_READ.all;
	SqiRegs.INST_READ.all = (1<<23)|(0x4b<<24)|(4<<16);
	uID = *(volatile UINT64*)UniqueIDAddr;
	WaitSqiCycles(24);
	SqiRegs.INST_READ.all = nInstRead; 
	CacheRegs.CTRL.bit.Flush = 1;//Flush cache 	
	memcpy(pID,(void*)&uID,8);
	return RT_OK; 
}

/**************************************************************************
* Subroutine:	SQI_SecRegProg
* Function:		to program the SQI flash Security Register(256 bytes) which the
* 				specified byte address is in.
* Input:		UINT32 nSecRegId -- 	to specify the number of the Security Register ID
*										from 1 to 3;
*				UINT32 nAddress	-- 	to specify the start address of the Security Register
* 									;
* 				UINT32 nNum --	the number of data to be written, size of word;
* 				UINT32* pData -- to specify the data 
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:	Note that the address is not the system address, only the
* 				offset of Security Register, which is start from 0.
* Date:			2016.12.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_SecRegProg(UINT32 nSecRegId, UINT32 nAddress, UINT32 *pData, UINT32 nNum)
{ 
	UINT8 i;
	volatile UINT32 *p;
	UINT32 status32;
	UINT8 nRet;

	if(((nSecRegId<=0)||(nSecRegId>3)) ||(nNum>64))
	{
		return RT_PARAM_ERR;
	}

	nSecRegId=nSecRegId<<12;
	p = (volatile UINT32 *)(SQI_BASE_ADDR+nSecRegId+(nAddress&0xFFFFFFFFC));
	SQI_MpuWrEnable(nSecRegId);		// mpu protected code added, 2013.01.16
	status32 = __get_PRIMASK();
	__disable_irq();
	// 1. Enable write
	SQI_WREN();
	// 2. Set INST (SecReg Program, 3 addresses)
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_PSR<<INST_DATA) | (3<<NUM_ADDR);
	// 3. Force CS to low
//	SqiRegs.INST_SET.all |= (1<<EN_ENCRYPT);
	SqiRegs.INST_SET.all |= (1<<CS_LOW);
	// 4. Word Program (only supportted word)
	for(i=0;i<nNum;i++)
	{
		*p++ = *pData++;
	}
	WaitSqiCycles(32);
	// 5. Release CS, and clear INST
//	SqiRegs.INST_SET.all &= ~(1<<EN_ENCRYPT);
	SqiRegs.INST_SET.all &= ~(UINT32)(1<<CS_LOW);
	SqiRegs.INST.all = 0;
	// 6. Wait SecReg Programming Time (tpp) 2.4ms
	nRet = SQI_WaitBusy(100);
	__set_PRIMASK(status32);
	return  nRet;
}

/**************************************************************************
* Subroutine:	SQI_SecRegErase
* Function:		to erase the SQI Security Register.
* Input:		UINT32 nSecRegId -- 	to specify the number of the Security Register ID
*										from 1 to 3;
* Output:		RT_OK-success
* Description:
* Date:			2016.12.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_SecRegErase(UINT32 nSecRegId)
{
	UINT32 status32;
	UINT8 nRet;

	if((nSecRegId<=0)||(nSecRegId>3))
	{
		return RT_PARAM_ERR;
	}
	
	SQI_MpuWrEnable(nSecRegId<<12);		// mpu protected code added, 2013.01.16
	nSecRegId=nSecRegId<<4;
	status32 = __get_PRIMASK();
	__disable_irq();	
	// 1. Enable write
	SQI_WREN();
	// 2. Force CS to low
	if(gPortMap == SQI_MAP_A10_15)
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xffffdfff)|0x00002000;
		GpioRegs.GPAUSE1.all=(GpioRegs.GPAUSE1.all&0xff0fffff)|0x00800000;
		WaitSqiCycles(10);
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xffffdfff)|0x00000000;
	}
	else
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xfff7ffff)|0x00080000;
		GpioRegs.GPAUSE2.all=(GpioRegs.GPAUSE2.all&0xffff0fff)|0x00008000;
		WaitSqiCycles(10);
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xfff7ffff)|0x00000000;
	}
	// 3. Set INST (SecReg)
	SqiRegs.INST.all = (1<<NUM_INST) | (SFC_ESR<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	// 3. Set addresses
	SqiRegs.INST.all = (1<<NUM_INST) | (0<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	SqiRegs.INST.all = (1<<NUM_INST) | (nSecRegId<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	SqiRegs.INST.all = (1<<NUM_INST) | (0<<INST_DATA);
	SqiRegs.INST_SET.all |= (1<<START);
	WaitSqiCycles(1);
	// 4. Force CS to High
	if(gPortMap == SQI_MAP_A10_15)
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xffffdfff)|0x00002000;
		GpioRegs.GPAUSE1.all=(GpioRegs.GPAUSE1.all&0xff0000ff)|0x00111100;
	}
	else
	{
		GpioRegs.GPADATA.all=(GpioRegs.GPADATA.all&0xfff7ffff)|0x00080000;
		GpioRegs.GPAUSE2.all=(GpioRegs.GPAUSE2.all&0xfff0000f)|0x00011110;
	}
	WaitSqiCycles(32);
	// 5. Clear INST
	SqiRegs.INST.all = 0;
	// 6. Wait SecReg Erase Time (tse) 400ms
	nRet = SQI_WaitBusy(8000);
	__set_PRIMASK(status32);
	return  nRet;
}

/**************************************************************************
* Subroutine:	SQI_SecRegRead
* Function:		read data from serial flash in Security Register.
* Input:		UINT32 nSecRegId -- 	to specify the number of the Security Register ID
*										from 1 to 3;
*				UINT32 nAddress	-- 	to specify the start address of the Security Register
* 									;
* 				UINT32 nNum --	the number of data to be read,size of byte;
* 				UINT32* pData -- to specify the data 
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:	Note that the address is not the system address, only the
* 				offset of Security Register, which is start from 0.
* Date:			2016.12.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_SecRegRead(UINT32 nSecRegId, UINT32 nAddress, UINT8 *pData, UINT32 nNum)
{ 	
	UINT32 temp = SqiRegs.INST_READ.all;

	if(((nSecRegId<=0)||(nSecRegId>3)) ||(nNum>256))
	{
		return RT_PARAM_ERR;
	}
	SqiRegs.INST_READ.all = 0;
	// SecReg, 3 addresses, 1 dummy
	SqiRegs.INST_READ.all = (1<<NUM_INST) | (SFC_RSR<<INST_DATA) | (3<<NUM_ADDR) \
		| (1<<NUM_DUMMY);
	nSecRegId=nSecRegId<<12;
	memcpy(pData,(UINT8*)(SQI_BASE_ADDR+nSecRegId+nAddress),nNum);
	SqiRegs.INST_READ.all = temp;
	return RT_OK;
}

/**************************************************************************
* Subroutine:	SQI_SecRegDisProg
* Function:		to disable program the SQI flash Security Register function.
* Input:		UINT32 nSecRegId -- 	to specify the number of the Security Register ID
*										from 1 to 3;
* Output:		UINT8 --	to return the result, RT_OK or RT_FAIL
* Description:	
* Date:			2016.12.07
* ModifyRecord:
* *************************************************************************/
UINT8 SQI_SecRegDisProg(UINT32 nSecRegId)
{
	T_FLASH_ID nReadId;
	SQI_ReadID(&nReadId);
	if(nReadId.Maufactor==0xef)
	{
		switch(nSecRegId)
		{				
            case 1:
				return SQI_WRSR(0x08);			
			case 2:
				return SQI_WRSR(0x10);			
			case 3:
				return SQI_WRSR(0x20);				
			default:
				return RT_PARAM_ERR;
		}		
	}
	else if(nReadId.Maufactor==0xc8)
	{
		return SQI_WRSR(0x04);
	}
	else
		return RT_FAIL;	
}

#pragma arm section code
