/**********************************************************************************
* File name    :	XFlashBk.c
* Function     :	External Flash operate by backup mode.
* Author       : 	Coins
* Date         :	2016/07/08
* Version      :    v1.2
* Description  :
* The structure of External Flash Area:
*
*									|<-- BK_SECTOR_NUM SECTOR -->|<-- 1 SECTOR -->|
* 	-------------------------------------------------------------------------
*	|*********数据存储区************|**备份数据扇区**|**备份状态扇区**|	
* 	-------------------------------------------------------------------------
*  /|\							   /|\				/|\				 /|\
*	|  								|				 |				  |
* 	0				   		BK_DATA_SECTOR	  	BK_STAT_SECTOR		  |
*							XF_END_DATA_ADDR	   				gXF_Capacity
* ModifyRecord :
*	v1.1	2013/11/06	coins
*		1. to add the function for setting backup sectors
*	v1.2	2016/08/16	coins
*		1. 修改备份恢复函数恢复错误的BUG。
*		2. 在编程，擦除操作时屏蔽中断。
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"

#ifdef	XFLASH_BACKUP_MODE_USED 
/**********************************************************************************
* Local Macro Definition
**********************************************************************************/
#define BK_SECTOR_NUM   2 		//备份扇区数（保证跨扇区写入时数据的完整性） 
#define BK_DATA_SECTOR		(gXF_Capacity/XF_SECTOR_SIZE - BK_SECTOR_NUM - 1)	// 备份数据扇区号
#define	BK_STAT_SECTOR		(gXF_Capacity/XF_SECTOR_SIZE - 1)	// 备份状态扇区号
#define	XF_END_DATA_ADDR	(gXF_Capacity-(BK_SECTOR_NUM+1)*XF_SECTOR_SIZE)	// 外部flash数据地址上限	

#define STATUS_BK 		0x22339876 		//说明是备份状态	

/**********************************************************************************
* Local Type Definition
**********************************************************************************/
typedef struct { 
	UINT32 nSector;		//备份的扇区地址
	UINT32 nNum;
	UINT32 nStatus;		//备份状态标志 
}STRUCT_BK_STATUS; 	

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static UINT32 gXF_Capacity = 2*1024*1024;

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
* Subroutine:	XF_BkEraseSector
* Function:		to erase n specified sectors 
* Input:	 	UINT32 nAddr --	to specify the start flash address;
*				UINT32 nSectNum -- 	to specify the number of sector.
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
UINT8 XF_BkEraseSector(UINT32 nAddr, UINT32 nSectNum)
{
	#ifndef	SQI_DRV
	return SPIFL_EraseSector(nAddr, nSectNum, XF_SECTOR_SIZE);
	#else
	UINT32 i;
	UINT8 nRet;
	UINT32 status32;

	for(i=0;i<nSectNum;i++)
	{
		status32 = __get_PRIMASK();
		__disable_irq();
		nRet = SQI_SectErase(nAddr+i*XF_SECTOR_SIZE);
		__set_PRIMASK(status32);
		if(nRet != RT_OK) return RT_FAIL;
	}
	return RT_OK;
	#endif
}

/***************************************************************************
* Subroutine:	XF_BkPageProgram
* Function:		to program n specified pages 
* Input:	 	UINT8 *pData -- to specify the data to be programmed;
*				UINT32 nAddr --	to specify the start flash address;
*				UINT32 nPageNum -- 	to specify the number of page.
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_BkPageProgram(UINT8 *pData, UINT32 nAddr, UINT32 nPageNum)
{
	#ifndef	SQI_DRV
	return SPIFL_PageProgram(pData, nAddr, nPageNum, XF_PAGE_SIZE);
	#else
	UINT32 i;
	UINT8 nRet;
	UINT32 status32;

	if(((UINT32)pData)&0x3)	 return RT_FAIL;
	for(i=0;i<nPageNum;i++)
	{
		status32 = __get_PRIMASK();
		__disable_irq();
		nRet = SQI_PageProg(nAddr+i*XF_PAGE_SIZE, (UINT32 *)(pData+i*XF_PAGE_SIZE));
		__set_PRIMASK(status32);
		if(nRet != RT_OK) return RT_FAIL;
	}
	return RT_OK;
	#endif
}

/***************************************************************************
* Subroutine:	XF_BkProgram
* Function:		to program flash without erase
* Input:	 	UINT8 *pData -- to specify the data to be programmed;
*				UINT32 nAddr --	to specify the start flash address;
*				UINT32 nPageNum -- 	to specify the number of page.
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
UINT8 XF_BkProgram(UINT8 *pData, UINT32 nAddr, UINT32 nLen)
{
	UINT32 nOffset = nAddr%XF_PAGE_SIZE;
	UINT32 nLen1;
	UINT8 nRet;
	UINT32 status32;

	#ifdef	SQI_DRV
   	UINT32 buf[XF_PAGE_SIZE/4];
	#endif
	while(nLen)
	{
		nLen1 = (XF_PAGE_SIZE-nOffset)>nLen? nLen : XF_PAGE_SIZE-nOffset;
		#ifndef	SQI_DRV	
		nRet = SPIFL_PageProgram(pData, nAddr, 1, nLen1);
		#else
		memset(buf, 0xFF, XF_PAGE_SIZE);
		memcpy(((UINT8 *)buf)+nOffset, pData, nLen1);
		status32 = __get_PRIMASK();
		__disable_irq();
		nRet = SQI_PageProg(nAddr, buf);
		__set_PRIMASK(status32);
		#endif
		if(nRet != RT_OK) return RT_FAIL;
		nOffset = 0;
		nLen -= nLen1;
		pData += nLen1;
		nAddr += nLen1;
	}
	return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_BkRead
* Function:		to read data random in all external flash address
* Input:	 	UINT8 *pData --	 the data buffer to be read 
 *				UINT32 nAddr --	 the flash address 
 *				UINT32 nLen  --	 the length of data buffer
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
static void XF_BkRead(UINT8 *pData, UINT32 nAddr, UINT32 nLen)
{
	#ifndef	SQI_DRV
	SPIFL_Read(pData, nAddr, nLen);
	#else
  	memcpy(pData,(UINT8*)(SQI_BASE_ADDR+nAddr),nLen);
	#endif
}

/***************************************************************************
* Subroutine:	XF_CheckBKStatus
* Function:		Check External Flash data in backup status sector. when in back up, to recover data. 
* Input:	 	None
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_CheckBKStatus(void)
{
	UINT8 Buf[XF_BUFF_SIZE];
	UINT32 nAddr;
	UINT8 nRet;
	UINT32 i;
	STRUCT_BK_STATUS tBK; 
    // to read backup status from status sector
	XF_BkRead((UINT8*)&tBK,BK_STAT_SECTOR*XF_SECTOR_SIZE,sizeof(tBK));
	if(tBK.nStatus==STATUS_BK)
	{
		for(i=0;i<tBK.nNum;i++)
		{
			// erase the sector 
			nRet = XF_BkEraseSector((tBK.nSector+i)*XF_SECTOR_SIZE, 1);
			if(nRet != RT_OK) return RT_FAIL;

			// copy back data form backup data sector to the sector
			for(nAddr=0;nAddr<XF_SECTOR_SIZE;nAddr+=XF_BUFF_SIZE)
			{
				XF_BkRead(Buf, (BK_DATA_SECTOR+i)*XF_SECTOR_SIZE+nAddr, XF_BUFF_SIZE);	// read a buffer data from backup data sector
				nRet = XF_BkPageProgram(Buf, ((tBK.nSector+i)*XF_SECTOR_SIZE+nAddr), XF_BUFF_SIZE/XF_PAGE_SIZE);
				if(nRet != RT_OK) return RT_FAIL;	
			}
		}
		// erase backup status
		nRet = XF_BkEraseSector(BK_STAT_SECTOR*XF_SECTOR_SIZE, 1);
		if(nRet != RT_OK) return RT_FAIL;	      	
	}	
	else
	{
		// erase backup status
		nRet = XF_BkEraseSector(BK_STAT_SECTOR*XF_SECTOR_SIZE, 1);
		if(nRet != RT_OK) return RT_FAIL;
	}	

	return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_BkInit
* Function:		to initialize external flash interface 
* Input:	 	UINT32 nCapacity --	 the flash address 
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
UINT8 XF_BkInit(UINT32 nCapacity)
{
	if(nCapacity<(XF_SECTOR_SIZE*(BK_SECTOR_NUM*2+1))) return RT_FAIL;
	gXF_Capacity = nCapacity;
	#ifndef	SQI_DRV
	SPIFL_Init(2);
	#else
	SQI_Init(DEFAULT_MAP, DEFAULT_CLK_DIV, DEFAULT_RD_MODE);
	#endif

	return XF_CheckBKStatus();
}

/***************************************************************************
* Subroutine:	XF_WriteSector
* Function:		to write bytes in a sector 
* Input:	 	UINT8 *pData --	 the data buffer to be read
*				UINT32 nSectAddr -- the sector to be written 
*				UINT32 nOffset --	the offset flash address in the sector 
*				UINT32 nLen  --	 the length of data buffer
* Output:		RT_OK, RT_PARAM_ERR or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_WriteSector(UINT8 *pData, UINT32 nSectAddr, UINT32 nOffset, UINT32 nLen)
{
	UINT8 Buf[XF_BUFF_SIZE];
	UINT32 nAddr;
	UINT8 nRet;
	STRUCT_BK_STATUS sBK;
	UINT32 i, nBackSectNum;

	if(nLen == 0) return RT_PARAM_ERR;
	nBackSectNum = ((nOffset+nLen-1)/XF_SECTOR_SIZE)+1;	// backup sectors needed
	if(nBackSectNum > BK_SECTOR_NUM) return RT_PARAM_ERR;

	// 1. erase backup data sector
	nRet = XF_BkEraseSector(BK_DATA_SECTOR*XF_SECTOR_SIZE, nBackSectNum);
	if(nRet != RT_OK) return RT_FAIL;

	for(i=0;i<nBackSectNum;i++)
	{
		// 2. write the sector with new data into backup data sector
		for(nAddr=0;nAddr<XF_SECTOR_SIZE;nAddr+=XF_BUFF_SIZE)
		{
			XF_BkRead(Buf, (nSectAddr+i)*XF_SECTOR_SIZE+nAddr, XF_BUFF_SIZE);		// read a buffer data from the sector
			if(nOffset<(nAddr+XF_BUFF_SIZE)) 	// offset is in the buffer, need to modify the data for program
			{
				if((nAddr+XF_BUFF_SIZE-nOffset)>=nLen)	// all new data in this buffer
				{
					memcpy(Buf+nOffset%XF_BUFF_SIZE, pData, nLen);	// copy new data
					nLen = 0;
					nOffset = XF_SECTOR_SIZE; 	 
				}
				else 	// not all new data in this buffer
				{
					memcpy(Buf+nOffset%XF_BUFF_SIZE, pData, nAddr+XF_BUFF_SIZE-nOffset);
					nLen -= nAddr+XF_BUFF_SIZE-nOffset;
					pData += nAddr+XF_BUFF_SIZE-nOffset;
					nOffset = nAddr+XF_BUFF_SIZE;		
				}
			}
			nRet = XF_BkPageProgram(Buf, ((BK_DATA_SECTOR+i)*XF_SECTOR_SIZE+nAddr), XF_BUFF_SIZE/XF_PAGE_SIZE);
			if(nRet != RT_OK) return RT_FAIL;
		}

		nOffset = 0;
	}

	// 2. set backup state into backup status sector
	sBK.nSector = nSectAddr;
	sBK.nNum = nBackSectNum;
    sBK.nStatus = STATUS_BK;
	memset(Buf, 0xFF, XF_PAGE_SIZE);
	memcpy(Buf, (UINT8 *)&sBK, sizeof(STRUCT_BK_STATUS));
	nRet = XF_BkPageProgram(Buf, BK_STAT_SECTOR*XF_SECTOR_SIZE, 1);
	if(nRet != RT_OK) return RT_FAIL;

	// 3. erase the sectors with the old data
	nRet = XF_BkEraseSector(nSectAddr*XF_SECTOR_SIZE, nBackSectNum);
	if(nRet != RT_OK) return RT_FAIL;

   	// 4. copy back data form backup data sectors to the specified sectors
	for(i=0;i<nBackSectNum;i++)
	{
		for(nAddr=0;nAddr<XF_SECTOR_SIZE;nAddr+=XF_BUFF_SIZE)
		{
			XF_BkRead(Buf, (BK_DATA_SECTOR+i)*XF_SECTOR_SIZE+nAddr, XF_BUFF_SIZE);	// read a buffer data from backup data sector
			nRet = XF_BkPageProgram(Buf, ((nSectAddr+i)*XF_SECTOR_SIZE+nAddr), XF_BUFF_SIZE/XF_PAGE_SIZE);
			if(nRet != RT_OK) return RT_FAIL;	
		}
	}
	// 5. erase backup status
	nRet = XF_BkEraseSector(BK_STAT_SECTOR*XF_SECTOR_SIZE, 1);
	if(nRet != RT_OK) return RT_FAIL;

	return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_BkWriteData
* Function:		write data to external flash.
* Input:		UINT8* pDataBuf --	the data to be written save flash area;
*				UINT32 nAddr --	the external flash address;
*				UINT16 nLen --	the length to be written.
* Output:		RT_OK, RT_FAIL, or RT_PARAM_ERR;
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
UINT8 XF_BkWriteData(UINT8* pDataBuf, UINT32 nAddr, UINT16 nLen)
{
	UINT8 nRet;
	UINT32 nOffset = nAddr%XF_SECTOR_SIZE;	// offset in a sector 
	UINT32 nSect;
	UINT32 nLen1;

	if((nAddr+nLen)>XF_END_DATA_ADDR) return RT_PARAM_ERR;
	while(nLen)	
	{
		nSect = nAddr/XF_SECTOR_SIZE;	// the sector to be written
		nLen1 = (XF_SECTOR_SIZE*BK_SECTOR_NUM-nOffset)>=nLen? nLen : (XF_SECTOR_SIZE*BK_SECTOR_NUM-nOffset);
		nRet = XF_WriteSector(pDataBuf, nSect, nOffset, nLen1);
		if(nRet != RT_OK) return RT_FAIL;
		nLen -= nLen1;
		pDataBuf += nLen1;
		nOffset = 0;
		nAddr += nLen1;
	}
	return RT_OK;	
}

/***************************************************************************
* Subroutine:	XF_BkReadData
* Function:		read data from external flash.
* Input:		UINT8* pDataBuf --	the data for reading;
*				UINT32 nAddr --	the external flash address;
*				UINT16 nLen --	the length to be read.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
UINT8 XF_BkReadData(UINT8* pDataBuf, UINT32 nAddr, UINT16 nLen)
{
	if((nAddr+nLen)>XF_END_DATA_ADDR) return RT_PARAM_ERR;
	XF_BkRead(pDataBuf, nAddr, (UINT32)nLen);
	return RT_OK;	
}

#endif
