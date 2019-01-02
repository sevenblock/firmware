/**********************************************************************************
* File name    :	Sdsi.c
* Function     :	The SDSI driver for AS569
* Author       : 	coins
* Date         :	2014/02/17
* Version      :    v1.0
* Description  :	
* ModifyRecord :
*
* 1. 修改写的时候联系写超过0x4000块时出错的问题，原因是NUM_WR_BLKS只有10位，当写到0x4000块时，
*	NUM_WR_BLKS的值就变为0了，用SdsiRegs.WRBLKCNT-SdsiRegs.NUM_WR_BLKS来计算需要编程多少块时，
*	得出的值是0x4000，所以会错。	2014/03/12 
* 2. 增加对擦除命令的支持，同时将初始化和复位是SDSI的寄存器初始化操作合并。	2014/03/18
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"


/**************************************************************************
* Local Macro Definition
***************************************************************************/
#define	CLK_50M				0
#define	CLK_25M_HS			1
#define	CLK_25M				2

#define	CLK_MODE		  CLK_50M

//#define DBG_STATE(Val)		{GpioRegs.GPBDATA.all = (Val)<<16;}
//#define	DBG_STATE(Val)

//#define	DBG_TRIGGER(Val)	{GpioRegs.GPADATA.bit.GPA0 = (Val);}
//#define	DBG_TRIGGER(Val)

//#define MSG(arg) { printf arg ; }
//#define MSG(arg) 
UINT8 gInitSDICompleted =1;

/**********************************************************************************
* Local Macro Definition
**********************************************************************************/
#define	SDSI_TOTAL_BUFF_BLKS		32

#if(SDSI_RAM_USED_MODE == RAM_16K_USED)
#define	SDSI_CMD_BUFF_BLKS			32
#define	SDSI_BUFF_BLKS				32
#endif

#if(SDSI_RAM_USED_MODE == RAM_8K_USED)
#define	SDSI_CMD_BUFF_BLKS			32
#define	SDSI_BUFF_BLKS				16
#endif

#if(SDSI_RAM_USED_MODE == RAM_FILE_USED)
#define	SDSI_CMD_BUFF_BLKS			32
#define	SDSI_BUFF_BLKS				2
#endif

#define	SDSI_BUFF_SUB_BLKS			2

#if(SDSI_BUFF_BLKS%SDSI_BUFF_SUB_BLKS)
#error"SDSI Buffer Block Error!"
#endif
#if(SDSI_CMD_BUFF_BLKS%SDSI_BUFF_SUB_BLKS)
#error"SDSI CMD Buffer Block Error!"
#endif
#if(SDSI_TOTAL_BUFF_BLKS%SDSI_BUFF_SUB_BLKS)
#error"SDSI TOTAL Buffer Block Error!"
#endif

#define	SDSI_ClearSdSdioInt(BIT_MASK)		(SdsiRegs.SD_SDIO_INT_STATUS.all = (BIT_MASK))
#define	SDSI_ClearMemInt(BIT_MASK)			(SdsiRegs.MEM_INT_STUTAS.all = (BIT_MASK))
#define	SDSI_ClearFn1Int(BIT_MASK)			(SdsiRegs.AHB_INT_STATUS.all = (BIT_MASK))

#define SDSI_DisableMemInt(BIT_MASK) 		(SdsiRegs.MEM_INT_EN.all &= ~(BIT_MASK))
#define SDSI_EnableMemInt(BIT_MASK) 		(SdsiRegs.MEM_INT_EN.all |= (BIT_MASK))

#define	BYTE_ADDR	(!SdsiRegs.MEM_OCR.bit.CCS)

/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static P_SDSI_DEV gpSdsiDev;
static P_SDSI_FILE gpSdsiFile;
static UINT32 gTotalWrBlk = 0;		// NUM_WR_BLKS only 10 bit, must be define a variable for saving
volatile UINT8 gSDSIFlush;
UINT8 gRstFlag=0;
/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/



/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/

/**********************************************************************************
* Functon
**********************************************************************************/

/***************************************************************************
* Subroutine:	SDSI_ConfigRegs
* Function:		to config SDSI registers when initialization or reset.
* Input:		None
* Output:		None 
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
void SDSI_ConfigRegs(void)
{
//	while(!SdsiRegs.ESW_CARD_READY.bit.CardRdy);
	SdsiRegs.ESW_CCCR.bit.MemPre = MEM_CARD;	// memory only card
	switch(gpSdsiDev->nSDPhyVer)
	{
		case SD_PHY_V1_00:
			SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
			SdsiRegs.CSD0 = 0x16804000;
			SdsiRegs.CSD1 = 0x36DBFF80 | ((gpSdsiDev->nCapacity/1024-1)<<30);	// 60-80mA, 512-mult,   	
			SdsiRegs.CSD2 = 0x5B5A0000 | (((gpSdsiDev->nCapacity/1024-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
			SdsiRegs.CSD3 = 0x007F0032;												// CSD1.0, 80ms TAAC, 25Mbit/s
			SdsiRegs.SCR1 = 0x00250000;												// SD spec V1.0
			SdsiRegs.ACCESS_MODE.all = 0x00008821;									// SDR25 un-supported
			break;
		case SD_PHY_V1_10:
			SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
			SdsiRegs.CSD0 = 0x16804000;
			SdsiRegs.CSD1 = 0x36DBFF80 | ((gpSdsiDev->nCapacity/1024-1)<<30);	// 60-80mA, 512-mult,   	
			SdsiRegs.CSD2 = 0x5B5A0000 | (((gpSdsiDev->nCapacity/1024-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
			SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			SdsiRegs.SCR1 = 0x01250000;												// SD spec V1.1
			SdsiRegs.ACCESS_MODE.all = 0x00008823;									// SDR25 supported
			break;
		case SD_PHY_V2_00:
			if(gpSdsiDev->nCapacity>SD_CAPACITY_G(2))								// >2GB, SDHC
			{
				SdsiRegs.MEM_OCR.all = 0x40FF8000;									// SDHC
				SdsiRegs.CSD0 = 0x0A404000;
				SdsiRegs.CSD1 = 0x00007F80 | ((gpSdsiDev->nCapacity/1024-1)<<16);	  	
				SdsiRegs.CSD2 = 0x5B590000 | (((gpSdsiDev->nCapacity/1024-1)>>16)&0x3F);// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x400E0032;												// CSD2.0, 25Mbit/s
			}
			else if(gpSdsiDev->nCapacity>SD_CAPACITY_M(16)) 	// SDSC
			{
				SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
				SdsiRegs.CSD0 = 0x16804000;
				SdsiRegs.CSD1 = 0x36DBFF80 | ((gpSdsiDev->nCapacity/1024-1)<<30);		// 60-80mA, 512-mult,   	
				SdsiRegs.CSD2 = 0x5B5A0000 | (((gpSdsiDev->nCapacity/1024-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			}
			else if(gpSdsiDev->nCapacity > SD_CAPACITY_K(44)) 	// SDSC
			{
				SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
				SdsiRegs.CSD0 = 0x16804000;
				SdsiRegs.CSD1 = 0x36D8FF80 | ((gpSdsiDev->nCapacity/8-1)<<30); 		// 60-80mA, 512-mult,   	
				SdsiRegs.CSD2 = 0x5B590000 | (((gpSdsiDev->nCapacity/8-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			}
			else if(gpSdsiDev->nCapacity == SD_CAPACITY_K(44)) 	// SDSC
			{
				SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
				SdsiRegs.CSD0 = 0x16804000;
				SdsiRegs.CSD1 = 0xB6D8FF80; 		// 60-80mA, 512-mult,   	
				SdsiRegs.CSD2 = 0x5B590002 ;// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			}
			else 	// SDSC
			{
				SdsiRegs.MEM_OCR.all = 0x00FF8000;	// SDSC
				SdsiRegs.CSD0 = 0x16804000;
				SdsiRegs.CSD1 = 0x36DBFF80 | ((gpSdsiDev->nCapacity/1024-1)<<30);	// 60-80mA, 512-mult,   	
				SdsiRegs.CSD2 = 0x5B5A0000 | (((gpSdsiDev->nCapacity/1024-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x007F0032;		// CSD1.1, 80ms TAAC, 25Mbit/s
			}
			SdsiRegs.SCR1 = 0x02350000;	// SD spec V2.0		//这里各版本需要更改
			#if(CLK_MODE == CLK_50M)
			SdsiRegs.ACCESS_MODE.all = 0x00008823;		// SDR25 supported
			#else
			SdsiRegs.ACCESS_MODE.all = 0x00008821;		// SDR25 un-supported
			#endif
			break;
		default:
			break;
	}
	SdsiRegs.MAX_CURR = 50; 					// 50mA bit[511:496] in cmd6 status data structure field.
	SdsiRegs.SD_CARD_TYPE.all = 0x01010000;		// class 2  	

	if(gpSdsiDev->nCapacity>=SD_CAPACITY_G(1))	// >=1G
	{
		SdsiRegs.AU_SIZE = 9;	// AU size = 4M	
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(512))
	{
		SdsiRegs.AU_SIZE = 8;	// AU size = 2M	
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(128))
	{
		SdsiRegs.AU_SIZE = 7;	// AU size = 1M	
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(16)) 
	{
		SdsiRegs.AU_SIZE = 6;	// AU size = 512K
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(1))
	{
		SdsiRegs.AU_SIZE = 3;	// AU size = 512K
	}
	else
	{
		SdsiRegs.AU_SIZE = 1;	// AU size = 16K
	}
	SdsiRegs.SD_ERASE.all = 0x0;	//  

	SdsiRegs.GLOBAL_INT_STATUS.all = 7;
	SdsiRegs.GLOBAL_INT_EN.all = 0x3;	// Fn0 and Memory Interrupt enable
	SdsiRegs.SD_SDIO_INT_EN.bit.SoftRst = 1;	// Soft reset Interrupt enable (CMD0)
//	if(gpSdsiDev->fpSdsiInitCb != NULL) gpSdsiDev->fpSdsiInitCb(NULL);

	SdsiRegs.MEM_INT_EN.all = SDSI_MEM_INT_SD_RD_START | SDSI_MEM_INT_SD_WR_START | SDSI_MEM_INT_CMD6_SWITCH | \
							SDSI_MEM_INT_ADMA_END | SDSI_MEM_INT_WR_OVER | SDSI_MEM_INT_RD_OVER | \
							SDSI_MEM_INT_ERASE;
}

/***************************************************************************
* Subroutine:	SDSI_ConfigRegs0
* Function:		to config SDSI registers when initialization or reset.
* Input:		None
* Output:		None 
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
void SDSI_ConfigRegs0(void)
{
	SdsiRegs.ESW_CCCR.bit.MemPre = MEM_CARD;	// memory only card
	switch(gpSdsiDev->nSDPhyVer)
	{
		case SD_PHY_V1_00:
//			SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
			SdsiRegs.ACCESS_MODE.all = 0x00008821;									// SDR25 un-supported
			break;
		case SD_PHY_V1_10:
//			SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
			SdsiRegs.ACCESS_MODE.all = 0x00008823;									// SDR25 supported
			break;
		case SD_PHY_V2_00:	
			#if(CLK_MODE == CLK_50M)
			SdsiRegs.ACCESS_MODE.all = 0x00008823;		// SDR25 supported
			#else
			SdsiRegs.ACCESS_MODE.all = 0x00008821;		// SDR25 un-supported
			#endif
			break;
		default:
			break;
	}
	SdsiRegs.MAX_CURR = 50; 					// 50mA bit[511:496] in cmd6 status data structure field.
	SdsiRegs.SD_CARD_TYPE.all = 0x01010000;		// class 2  	
	SdsiRegs.SD_ERASE.all = 0x0;	//  

	SdsiRegs.GLOBAL_INT_STATUS.all = 7;
	SdsiRegs.GLOBAL_INT_EN.all = 0x3;				// Fn0 and Memory Interrupt enable
	SdsiRegs.SD_SDIO_INT_EN.bit.SoftRst = 1;		// Soft reset Interrupt enable (CMD0)
	SdsiRegs.MEM_INT_EN.all = SDSI_MEM_INT_SD_RD_START | SDSI_MEM_INT_SD_WR_START | SDSI_MEM_INT_CMD6_SWITCH | \
								SDSI_MEM_INT_ADMA_END | SDSI_MEM_INT_WR_OVER | SDSI_MEM_INT_RD_OVER | \
								SDSI_MEM_INT_ERASE;
}
/***************************************************************************
* Subroutine:	SDSI_ConfigRegs1
* Function:		to config SDSI Capacity registers when initialization or reset.
* Input:		None
* Output:		None 
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
void SDSI_ConfigRegs1(void)
{
//	while(!SdsiRegs.ESW_CARD_READY.bit.CardRdy);
//	SdsiRegs.ESW_CCCR.bit.MemPre = MEM_CARD;	// memory only card
	switch(gpSdsiDev->nSDPhyVer)
	{
		case SD_PHY_V1_00:
//			SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
			SdsiRegs.CSD0 = 0x16804000;
			SdsiRegs.CSD1 = 0x36DBFF80 | ((gpSdsiDev->nCapacity/1024-1)<<30);		// 60-80mA, 512-mult,   	
			SdsiRegs.CSD2 = 0x5B5A0000 | (((gpSdsiDev->nCapacity/1024-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
			SdsiRegs.CSD3 = 0x007F0032;												// CSD1.0, 80ms TAAC, 25Mbit/s
			SdsiRegs.SCR1 = 0x00250000;												// SD spec V1.0
			break;
		case SD_PHY_V1_10:
//			SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
			SdsiRegs.CSD0 = 0x16804000;
			SdsiRegs.CSD1 = 0x36DBFF80 | ((gpSdsiDev->nCapacity/1024-1)<<30);		// 60-80mA, 512-mult,   	
			SdsiRegs.CSD2 = 0x5B5A0000 | (((gpSdsiDev->nCapacity/1024-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
			SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			SdsiRegs.SCR1 = 0x01250000;												// SD spec V1.1
			break;
		case SD_PHY_V2_00:
			if(gpSdsiDev->nCapacity>SD_CAPACITY_G(2))								// >2GB, SDHC
			{
				SdsiRegs.MEM_OCR.all = 0x40FF8000;									// SDHC
				SdsiRegs.CSD0 = 0x0A404000;
				SdsiRegs.CSD1 = 0x00007F80 | ((gpSdsiDev->nCapacity/1024-1)<<16);	  	
				SdsiRegs.CSD2 = 0x5B590000 | (((gpSdsiDev->nCapacity/1024-1)>>16)&0x3F);// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x400E0032;												// CSD2.0, 25Mbit/s
			}
			//else if(gpSdsiDev->nCapacity>SD_CAPACITY_G(1)) 	// SDSC
			else if(gpSdsiDev->nCapacity>SD_CAPACITY_M(16)) 	// SDSC
			{
				SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
				SdsiRegs.CSD0 = 0x16804000;
				SdsiRegs.CSD1 = 0x36DBFF80 | ((gpSdsiDev->nCapacity/1024-1)<<30);		// 60-80mA, 512-mult,   	
				SdsiRegs.CSD2 = 0x5B5A0000 | (((gpSdsiDev->nCapacity/1024-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			}
			else if(gpSdsiDev->nCapacity > SD_CAPACITY_K(44)) 	// SDSC
			{
				SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
				SdsiRegs.CSD0 = 0x16804000;
				SdsiRegs.CSD1 = 0x36D8FF80 | ((gpSdsiDev->nCapacity/8-1)<<30); 		// 60-80mA, 512-mult,   	
				SdsiRegs.CSD2 = 0x5B590000 | (((gpSdsiDev->nCapacity/8-1)>>2)&0x3FF);// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			}
			else if(gpSdsiDev->nCapacity == SD_CAPACITY_K(44)) 	// SDSC
			{
				SdsiRegs.MEM_OCR.all = 0x00FF8000;										// SDSC
				SdsiRegs.CSD0 = 0x16804000;
				SdsiRegs.CSD1 = 0xB6D8FF80; 		// 60-80mA, 512-mult,   	
				SdsiRegs.CSD2 = 0x5B590002 ;// class 0/2/4/5/7/8/10, 512B/block 
				SdsiRegs.CSD3 = 0x007F0032;												// CSD1.1, 80ms TAAC, 25Mbit/s
			}
			SdsiRegs.SCR1 = 0x02350000;													// SD spec V2.0		

			break;
		default:
			break;
	}
//	SdsiRegs.MAX_CURR = 50; 					// 50mA bit[511:496] in cmd6 status data structure field.
//	SdsiRegs.SD_CARD_TYPE.all = 0x01010000;		// class 2  	

	if(gpSdsiDev->nCapacity>=SD_CAPACITY_G(1))	// >=1G
	{
		SdsiRegs.AU_SIZE = 9;	// AU size = 4M	
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(512))
	{
		SdsiRegs.AU_SIZE = 8;	// AU size = 2M	
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(128))
	{
		SdsiRegs.AU_SIZE = 7;	// AU size = 1M	
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(16)) 
	{
		SdsiRegs.AU_SIZE = 6;	// AU size = 512K
	}
	else if(gpSdsiDev->nCapacity>=SD_CAPACITY_M(1))
	{
		SdsiRegs.AU_SIZE = 3;	// AU size = 512K
	}
	else
	{
		SdsiRegs.AU_SIZE = 1;	// AU size = 16K
	}
}

/***************************************************************************
* Subroutine:	SDSI_Init
* Function:		to initialize SDSI for card.
* Input:		P_SDSI_DEV pSdsiDev -- to specify the parameter and function for SD device
* Output:		RT_OK 
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
UINT8 SDSI_Init(P_SDSI_DEV pSdsiDev)
{
	UINT32 i = 0;
	// to select Pins 
	GpioRegs.GPAUSE2.bit.GPA16 = MUX_SDSI;		// DAT1	
	GpioRegs.GPAUSE2.bit.GPA17 = MUX_SDSI;		// DAT0	
	GpioRegs.GPAUSE2.bit.GPA18 = MUX_SDSI;		// CLK	
	GpioRegs.GPAUSE2.bit.GPA19 = MUX_SDSI;		// CMD
	GpioRegs.GPAUSE2.bit.GPA20 = MUX_SDSI;		// DAT3	
	GpioRegs.GPAUSE2.bit.GPA21 = MUX_SDSI;		// DAT2	

	// to enable SDSI module and reset all logic
	SCM_EnableModClk(ID_SDSI);
	SCM_ReleaseModRst(ID_SDSI);

	gpSdsiDev = pSdsiDev;
	gpSdsiFile = pSdsiDev->pSdsiFile;
	if(gpSdsiFile != NULL)
		memset((UINT8*)gpSdsiFile,0,sizeof(SDSI_FILE));
	SDSI_ConfigRegs0();
	
	// to initialize ADMA2 descriptor table
	for(i=0;i<(SDSI_TOTAL_BUFF_BLKS/SDSI_BUFF_SUB_BLKS);i++)
	{
		SdsiADMA2DescTbl[i].ATTRIBUTE = SDSI_ADMA2_DT_VALID | SDSI_ADMA2_DT_END | SDSI_ADMA2_DT_ACT_TRAN ;
		SdsiADMA2DescTbl[i].LENGTH = SDSI_BUFF_SUB_BLKS*SDSI_BUFF_BANK_SIZE;
	   	SdsiADMA2DescTbl[i].ADDRESS = SDSI_BUFF_BANK_ADDR(i*SDSI_BUFF_SUB_BLKS);
	}

	NVIC_EnableIRQ(SDSI_IRQn);
	if(gpSdsiDev->fpSdsiInitCb != NULL) gpSdsiDev->fpSdsiInitCb(NULL);	  //SDI利用SDSI AMCD41 上电过程初始化 SDI
		gInitSDICompleted = 0;

	if(gpSdsiDev->fpSdsiFileInitCb != NULL) gpSdsiDev->fpSdsiFileInitCb(NULL);	  //SDI利用SDSI AMCD41 上电过程初始化 FILE SYS
	SDSI_ConfigRegs1();
	SdsiRegs.ESW_FUN_READY.all = 0x1;  	// Ready for ACMD41
	return RT_OK;	
}

/***************************************************************************
* Subroutine:	SDSI_IsInFile
* Function:		Check the block address is in file or not.
* Input:		UINT32 nAddr -- the block address
* Output:		TURE/FALSE 
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
BOOL SDSI_IsInFile(UINT32 nAddr)
{
	UINT8 i;
	if(gpSdsiFile == NULL) return FALSE;
	if(gpSdsiFile->nFileNum == 0) return FALSE;
	for(i=0;i<gpSdsiFile->nFileNum;i++)
	{
		if((nAddr >= gpSdsiFile->nFileAddr[i][0])&&(nAddr <= gpSdsiFile->nFileAddr[i][1]))
			return TRUE;
	}
	return FALSE;
}

/***************************************************************************
* Subroutine:	SDSI_Handler
* Function:		to handle SDSI interrupt.
* Input:		None
* Output:		None 
* Description:	
* Date:			2014/01/21
* ModifyRecord:
* *************************************************************************/
void SDSI_Handler(void)
{
	UINT32 nBlkAddr;
	UINT32 nStartAddr;
	UINT32 nBlkNum;
	UINT8 nMul;
	UINT8 nBuffBank;
	UINT32 nGlobalInEn = SdsiRegs.GLOBAL_INT_EN.all;		// save global interrupt enable
	union SDSI_GLOBAL_INT_REG uGlobalIntStatus;
	uGlobalIntStatus.all = SdsiRegs.GLOBAL_INT_STATUS.all;
	SdsiRegs.GLOBAL_INT_EN.all = 0;							// disable all inerrupt
	SdsiRegs.GLOBAL_INT_STATUS.all = uGlobalIntStatus.all;	// clear all global interrupt
	if(uGlobalIntStatus.bit.IntFrmFn0)						// SD SDIO reset and Fn0 interrupt
	{
		if(SdsiRegs.SD_SDIO_INT_STATUS.bit.SoftRst)			// CMD0 reset
		{
			SDSI_ClearSdSdioInt(SDSI_SD_SDIO_INT_SOFT_RST);	// clear interrupt flag
			SdsiRegs.SOFT_RESET_VALID.bit.ahbsoft_valid = 1;// reset SDSI
			SDSI_ConfigRegs();
			if(gInitSDICompleted)
				if(gpSdsiDev->fpSdsiInitCb != NULL) gpSdsiDev->fpSdsiInitCb(NULL);
			SdsiRegs.ESW_FUN_READY.all = 0x1;  				// Ready for ACMD41	
		}	
	}	
	if(uGlobalIntStatus.bit.IntFrmMem)	// Memory Interrupt
	{
		#if(CLK_MODE == CLK_25M_HS)
		SdsiRegs.SDSI_ADMA_GRANT |= 2;
		#endif
		if(SdsiRegs.MEM_INT_STUTAS.bit.SdRdStart) 	// read start
		{
			SDSI_ClearMemInt(SDSI_MEM_INT_SD_RD_START | SDSI_MEM_INT_RD_OVER | SDSI_MEM_INT_ADMA_END); 	// clear interrupt flag	
			if(BYTE_ADDR)nStartAddr = SdsiRegs.ARGUMENT>>9;	// to read address
			else nStartAddr = SdsiRegs.ARGUMENT;	// to read address
			nBlkAddr = nStartAddr;
			nMul = SdsiRegs.BL0CK_SIZE.bit.SinMulBlk;	// read sin_mul_blk
			if(gpSdsiDev->fpSdsiReadWriteMsg != NULL) gpSdsiDev->fpSdsiReadWriteMsg(nStartAddr, SDSI_MSG_READ_STT);	
			nBuffBank = 0;
			#if((SDSI_RAM_USED_MODE == RAM_8K_USED)||(SDSI_RAM_USED_MODE == RAM_FILE_USED))
			#ifndef SDSI_FILE_FAST_MODE
			if(SDSI_IsInFile(nStartAddr))//	函数
			#else
			if((nStartAddr >= gpSdsiFile->nFileAddr[0][0])&&(nStartAddr <= gpSdsiFile->nFileAddr[gpSdsiFile->nFileNum-1][1]))
			#endif
			{
				nBuffBank+=SDSI_BUFF_BLKS;
			}				
			#endif
			if(nMul)	// multi-block read
			{
				if(gpSdsiDev->fpSdsiReadCb != NULL) gpSdsiDev->fpSdsiReadCb(nBlkAddr, nBuffBank, SDSI_BUFF_SUB_BLKS); // read for all buffer
			}
			else
			{
				if(gpSdsiDev->fpSdsiReadCb != NULL) gpSdsiDev->fpSdsiReadCb(nBlkAddr, nBuffBank, 1);	// read for all buffer
			} 	
			gSDSIFlush =1;
			if(SdsiRegs.MEM_INT_STUTAS.bit.SdRdStart==0) 	// 某些读卡器发送了读命令后，不读数据马上又会发送读命令
			{
				SdsiRegs.ADMA_SYS_ADDR = (UINT32)&SdsiADMA2DescTbl[nBuffBank/SDSI_BUFF_SUB_BLKS];	// when write ADMA2 system address, start ADMA2 and transfer data to host
			}
			SdsiRegs.SDSI_ADMA_GRANT |= 1;
	
			while((!SdsiRegs.MEM_INT_STUTAS.bit.MemReadOver) && (!SdsiRegs.MEM_INT_STUTAS.bit.SdRdStart))	// wait for read over (stop transmission)
			{
				if(SdsiRegs.MEM_INT_STUTAS.bit.AdmaEnd && nMul)	// if ADMA2 end, prepare for next buffer data
				{
					SDSI_ClearMemInt(SDSI_MEM_INT_ADMA_END); 	// clear interrupt flag	
					SdsiRegs.SDSI_ADMA_GRANT &= (~1);			// hold ADMA2
					nBuffBank += SDSI_BUFF_SUB_BLKS;
					#if(SDSI_RAM_USED_MODE == RAM_16K_USED)
					if(nBuffBank>=SDSI_BUFF_BLKS)nBuffBank = 0;
					#endif
					#if((SDSI_RAM_USED_MODE == RAM_8K_USED)||(SDSI_RAM_USED_MODE == RAM_FILE_USED))
					#ifndef SDSI_FILE_FAST_MODE
					if(SDSI_IsInFile(nStartAddr))//	函数
					#else
					if((nStartAddr >= gpSdsiFile->nFileAddr[0][0])&&(nStartAddr <= gpSdsiFile->nFileAddr[gpSdsiFile->nFileNum-1][1]))
					#endif
					{ 	
						if(nBuffBank>=SDSI_CMD_BUFF_BLKS)nBuffBank = SDSI_BUFF_BLKS;
					}
					else
					{
						if(nBuffBank>=SDSI_BUFF_BLKS)nBuffBank = 0;
					}
				   	#endif
//					if(nBuffBank>=SDSI_BUFF_BLKS)nBuffBank = 0;
					SdsiRegs.ADMA_SYS_ADDR = (UINT32)&SdsiADMA2DescTbl[nBuffBank/SDSI_BUFF_SUB_BLKS];	// when write ADMA2 system address, start ADMA2 and transfer data to host
					nBlkAddr += SDSI_BUFF_SUB_BLKS;
					if(gpSdsiDev->fpSdsiReadCb != NULL) gpSdsiDev->fpSdsiReadCb(nBlkAddr, nBuffBank, SDSI_BUFF_SUB_BLKS);	// read for all buffer
					gSDSIFlush =1;
//					SdsiRegs.ADMA_SYS_ADDR = (UINT32)&SdsiADMA2DescTbl[nBuffBank/SDSI_BUFF_SUB_BLKS];	// when write ADMA2 system address, start ADMA2 and transfer data to host
					SdsiRegs.SDSI_ADMA_GRANT |= 1;
//					_delay_us(10);  	
				}
				if(SdsiRegs.SD_SDIO_INT_STATUS.bit.SoftRst) break;
			}
			SdsiRegs.RDBLKCNT = 0;
			if(gpSdsiDev->fpSdsiReadWriteMsg != NULL) gpSdsiDev->fpSdsiReadWriteMsg(nStartAddr, SDSI_MSG_READ_END);					
			SDSI_ClearMemInt(SDSI_MEM_INT_RD_OVER); 	// clear interrupt flag				
		}
		if((SdsiRegs.MEM_INT_STUTAS.bit.SdWrStart)&& (!SdsiRegs.MEM_INT_STUTAS.bit.SdRdStart))	// write start
		{
			SDSI_ClearMemInt(SDSI_MEM_INT_SD_WR_START | SDSI_MEM_INT_WR_OVER | SDSI_MEM_INT_ADMA_END); 	// clear interrupt flag
			SdsiRegs.SDSI_ADMA_GRANT |= 1;
			nBuffBank = 0;
			if(BYTE_ADDR)nStartAddr = SdsiRegs.ARGUMENT>>9;	// to read address
			else nStartAddr = SdsiRegs.ARGUMENT;	// to read address
			nBlkAddr = nStartAddr;
			#if((SDSI_RAM_USED_MODE == RAM_8K_USED)||(SDSI_RAM_USED_MODE == RAM_FILE_USED))
			#ifndef SDSI_FILE_FAST_MODE
			if(SDSI_IsInFile(nStartAddr))//	函数
			#else
			if((nStartAddr >= gpSdsiFile->nFileAddr[0][0])&&(nStartAddr <= gpSdsiFile->nFileAddr[gpSdsiFile->nFileNum-1][1]))
			#endif
			{
				nBuffBank+=SDSI_BUFF_BLKS;
			}				
			#endif
			SdsiRegs.ADMA_SYS_ADDR = (UINT32)&SdsiADMA2DescTbl[nBuffBank/SDSI_BUFF_SUB_BLKS];	// when write ADMA2 system address, start ADMA2 and transfer data to SRAM
			nMul = SdsiRegs.BL0CK_SIZE.bit.SinMulBlk;	// read sin_mul_blk
			if(gpSdsiDev->fpSdsiReadWriteMsg != NULL) gpSdsiDev->fpSdsiReadWriteMsg(nStartAddr, SDSI_MSG_WRITE_STT);
			gTotalWrBlk = 0;
			SdsiRegs.NUM_WR_BLKS = gTotalWrBlk;
			while(!SdsiRegs.MEM_INT_STUTAS.bit.MemWriteOver)
			{
				if(SdsiRegs.MEM_INT_STUTAS.bit.AdmaEnd && nMul)		// if ADMA2 end for multi-block write
				{
					SDSI_ClearMemInt(SDSI_MEM_INT_ADMA_END); 	// clear interrupt flag	
					gSDSIFlush =1;
					if(gpSdsiDev->fpSdsiWriteCb != NULL) gpSdsiDev->fpSdsiWriteCb(nBlkAddr, nBuffBank, SDSI_BUFF_SUB_BLKS);
					gTotalWrBlk += SDSI_BUFF_SUB_BLKS;
					SdsiRegs.NUM_WR_BLKS = gTotalWrBlk; 
					nBlkAddr += SDSI_BUFF_SUB_BLKS; 
					nBuffBank += SDSI_BUFF_SUB_BLKS;
					#if(SDSI_RAM_USED_MODE == RAM_16K_USED)
					if(nBuffBank>=SDSI_BUFF_BLKS)nBuffBank = 0;
					#endif
					#if((SDSI_RAM_USED_MODE == RAM_8K_USED)||(SDSI_RAM_USED_MODE == RAM_FILE_USED))
					#ifndef SDSI_FILE_FAST_MODE
					if(SDSI_IsInFile(nStartAddr))//	函数
					#else
					if((nStartAddr >= gpSdsiFile->nFileAddr[0][0])&&(nStartAddr <= gpSdsiFile->nFileAddr[gpSdsiFile->nFileNum-1][1]))
					#endif
					{ 	
						if(nBuffBank>=SDSI_CMD_BUFF_BLKS)nBuffBank = SDSI_BUFF_BLKS;
					}
					else
					{
						if(nBuffBank>=SDSI_BUFF_BLKS)nBuffBank = 0;
					}
				   	#endif
					if(!SdsiRegs.MEM_INT_STUTAS.bit.MemWriteOver)
					{
						SdsiRegs.ADMA_SYS_ADDR = (UINT32)&SdsiADMA2DescTbl[nBuffBank/SDSI_BUFF_SUB_BLKS]; // when write ADMA2 system address, start ADMA2 and transfer data to SRAM
					}	
				}
				if(SdsiRegs.SD_SDIO_INT_STATUS.bit.SoftRst) break;
			}
			SDSI_ClearMemInt(SDSI_MEM_INT_WR_OVER); 	// clear interrupt flag
			if(nMul)	// multi-block write
			{
				nBlkNum = SdsiRegs.WRBLKCNT-gTotalWrBlk;	
			}
			else	// single block write
			{
				nBlkNum = 1;
			}
			SdsiRegs.WRBLKCNT = 0;
			
			if(SdsiRegs.SD_SDIO_INT_STATUS.bit.SoftRst) nBlkNum = 0;
			if(nBlkNum)
			{
				gSDSIFlush =1;
				if(gpSdsiDev->fpSdsiWriteCb != NULL) gpSdsiDev->fpSdsiWriteCb(nBlkAddr, nBuffBank, nBlkNum);		
			}
			gTotalWrBlk += nBlkNum;
			SdsiRegs.NUM_WR_BLKS = gTotalWrBlk;
			if(gpSdsiDev->fpSdsiReadWriteMsg != NULL) gpSdsiDev->fpSdsiReadWriteMsg(nStartAddr, SDSI_MSG_WRITE_END);
			SdsiRegs.SD_MEM_PROGRAM.bit.WrProgDone = 1;	// when data processed by soft, release SD bus to idle state
				
		}
		if(SdsiRegs.MEM_INT_STUTAS.bit.Cmd6Switch)	// Function Switch
		{
			SDSI_ClearMemInt(SDSI_MEM_INT_CMD6_SWITCH); 	// clear interrupt flag	
		}
		if(SdsiRegs.MEM_INT_STUTAS.bit.Erase)	// Erase
		{
			UINT32 nStartAddr, nEndAddr;
			SDSI_ClearMemInt(SDSI_MEM_INT_ERASE); 	// clear interrupt flag	
			nStartAddr = SdsiRegs.ERASE_START_ADDR;	// to read erase address
			nEndAddr = SdsiRegs.ERASE_END_ADDR;
			if(nStartAddr<=nEndAddr)	// to check the address
			{
				if(BYTE_ADDR)
				{
					nStartAddr >>= 9;	
					nEndAddr >>= 9;	
				}
				if(gpSdsiDev->fpSdsiEraseCb != NULL) gpSdsiDev->fpSdsiEraseCb(nStartAddr, nEndAddr);	
			}
			else
			{
				SdsiRegs.SD_MEM_PROGRAM.bit.EraseParam = 1;	// parameter error
			}
			SdsiRegs.SD_MEM_PROGRAM.bit.WrProgDone = 1;
		}	
	}
	
	if(uGlobalIntStatus.bit.IntFrmFn1)	// Fn1 Interrupt
	{
		
	}
	SdsiRegs.GLOBAL_INT_EN.all = nGlobalInEn;	// recover global interrupt enable	
}


