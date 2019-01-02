 /**********************************************************************************
* File name    :	XFlashMap.c
* Function     :	External Flash operate by map mode.
* Author       : 	Emil
* Date         :	2016/07/08
* Version      :    v1.0
* Description  :
* The structure of External Flash Area:
*
*   |<-------------------------- XF_Init:------------------------>|
*												   |<-- 1_PAGE -->|
* 	--------------------------------------------------------------------
*	|*****************数据存储区*******************|**坏块记录区**|
* 	-------------------------------------------------------------------------
*  /|\							   				  /|\            /|\
*	|  											   |              |
* gXF_Start_Block			  				  gXF_End_Block	                
* nStartBlock				             						gXF_Capacity
* ModifyRecord :
* 1. 修改XF_MapWriteData函数写大数据时的BUG 。	2016.10.21	Emil
**********************************************************************************/
/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"

#ifdef	XFLASH_MAP_MODE_USED
/**********************************************************************************
* Local Macro Definition
**********************************************************************************/ 
#define	XF_REPLACE_WITH_BAD_BLOCK			// if using the bad block replacement function, please open XF_REPLACE_WITH_BAD_BLOCK					

#define LOGIC_BLOCK_SECTOR_NUM  			(1)													//每一逻辑块包含页数(用户可根据实际修改)
#define LOGIC_BLOCK_SIZES  	 				(XF_SECTOR_SIZE*LOGIC_BLOCK_SECTOR_NUM)				//逻辑块大小
#define MAX_XFLASH_SECTOR_NUMS  			(512)												//XF最大页数(用户可根据实际修改)
#define MAX_XF_LOGIC_BLOCK_NUMS  			(MAX_XFLASH_SECTOR_NUMS/LOGIC_BLOCK_SECTOR_NUM)		//XF最大逻辑块数
#define XF_MAP_STATUS_SIZE 					(sizeof(STRUCT_MAP_STATUS)) 						//状态大小
#define	XF_MAP_STATUS_ADDR					(LOGIC_BLOCK_SIZES-XF_MAP_STATUS_SIZE)				//在逻辑块中状态存放地址
#define	XF_MAP_STATUS_SECTOR_ADDR			(LOGIC_BLOCK_SECTOR_NUM-1)							//在逻辑块中状态存放页地址 
#define XF_LOGIC_BLOCK_SIZE  				(XF_MAP_STATUS_ADDR)								//安全存储数据页大小
#define XF_LOGIC_BLOCK_STATUS_USED 			(0x22339876) 										//XFLASH中BLOCK使用状态.
#define	XF_BAD_BLOCK_STORE_SECTOR_NUM		(4)													//坏块存放页数
#define	XF_BAD_BLOCK_STORE_SECTOR			(gXF_End_Block)										//坏块存放地址
#define XF_RSVD_LOGIC_BLOCK_NUMS  			(2)													//预留逻辑块数，用于确保编程安全 
#define XF_BAD_BLOCK_STORE_SECTOR_FLAG 		(0x12349876) 										//坏块存放页标志.
#define XF_BAD_BLOCK_STORE_SECTOR_FLAG_ADDR (0x00) 												//坏块存放页标志地址.
#define XF_MAX_BAD_BLOCK_STORE_NUMS  		(XF_BAD_BLOCK_STORE_SECTOR_NUM*XF_SECTOR_SIZE/XF_PAGE_SIZE-1)	//最大坏块记录数


//---block Status used for index map --------
#ifndef	STATUS_NOTUSED
#define STATUS_NOTUSED 				(0xffff)
#endif
#ifndef	STATUS_USED
#define STATUS_USED    				(0x9876)
#endif
#ifndef	STATUS_BAD
#define STATUS_BAD     				(0x1234)
#endif

/**********************************************************************************
* Local Type Definition
**********************************************************************************/
typedef struct { 	
    UINT32 lLogicPage;			//((~逻辑页地址)<<16)|逻辑页地址
	UINT32 lPageWriteSeq;		//((~页写操作序号)<<16)|页写操作序号
	UINT64 lWriteSeq;			//写操作序号
	UINT64 lWriteSeqCHK;		//lWriteSeq校验值
    UINT32 lMapStatusPageCHK;	//状态页校验
	UINT32 lStatus;				//状态标志 
}STRUCT_MAP_STATUS; 	

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static UINT32 gXF_Capacity = 2*1024*1024;
static UINT16 gXF_Start_Block = 0, gXF_End_Block, gXF_Block_Nums, gXF_UsedBlock_Nums, gXF_BadBlock_Nums;
static UINT16 gXFMap[MAX_XF_LOGIC_BLOCK_NUMS]; //save the block map
static UINT16 gXFStatus[MAX_XF_LOGIC_BLOCK_NUMS];//save the block status
static UINT16 gXFPageWriteSeq[MAX_XF_LOGIC_BLOCK_NUMS];//save the block write seq
static UINT64 gWriteSeq;
static UINT8 gWrite2BlockFlag = 0;
static UINT8 gEraseBlockNum = 0;
static UINT16 gEraseBlock[2] = {0};
static UINT8 gEraseBlockNum1 = 0;
static UINT16 gEraseBlock1[2] = {0};
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
* Subroutine:	XF_MapIsBufAllOne
* Function:		Check buffer all 1 
* Input:	 	void *pData -- the buffer
*				UINT32 nSize -- the buffer size (word number)
* Output:		RT_OK
* Description:	 
* Date:			2013.05.14
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_MapIsBufAllOne(void *pData, UINT32 nSize)
{
	UINT32 i;
	UINT32 *p = pData;
	for(i=0;i<nSize;i++) 
	{
		if(p[i] != 0xFFFFFFFF) return RT_FAIL;
	}
	return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_MapEraseSector
* Function:		to erase n specified sectors 
* Input:	 	UINT32 nAddr --	to specify the start flash address;
*				UINT32 nSectNum -- 	to specify the number of sector.
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
UINT8 XF_MapEraseSector(UINT32 nAddr, UINT32 nSectNum)
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
* Subroutine:	XF_MapPageProgram
* Function:		to program n specified pages 
* Input:	 	UINT8 *pData -- to specify the data to be programmed;
*				UINT32 nAddr --	to specify the start flash address;
*				UINT32 nPageNum -- 	to specify the number of page.
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_MapPageProgram(UINT8 *pData, UINT32 nAddr, UINT32 nPageNum)
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
* Subroutine:	XF_MapProgram
* Function:		to program flash without erase
* Input:	 	UINT8 *pData -- to specify the data to be programmed;
*				UINT32 nAddr --	to specify the start flash address;
*				UINT32 nPageNum -- 	to specify the number of page.
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
UINT8 XF_MapProgram(UINT8 *pData, UINT32 nAddr, UINT32 nLen)
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
* Subroutine:	XF_MapRead
* Function:		to read data random in all external flash address
* Input:	 	UINT8 *pData --	 the data buffer to be read 
 *				UINT32 nAddr --	 the flash address 
 *				UINT32 nLen  --	 the length of data buffer
* Output:		RT_OK or RT_FAIL
* Description:	 
* Date:			2013.04.08
* ModifyRecord:
* *************************************************************************/
static void XF_MapRead(UINT8 *pData, UINT32 nAddr, UINT32 nLen)
{
	#ifndef	SQI_DRV
	SPIFL_Read(pData, nAddr, nLen);
	#else
  	memcpy(pData,(UINT8*)(SQI_BASE_ADDR+nAddr),nLen);
	#endif
}

/***************************************************************************
* Subroutine:	XF_ReadBlockData
* Function:		read data from a block
* Input:		UINT8* pDataBuf-the buffer read 
*	   			UINT32 nBlock- the block num will read data
*	   			UINT32 lOffset- the offset address in a block;
*	   			UINT32 lDataNum- the data number by byte will read;		
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_ReadBlockData(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{      
	if((nBlock>=gXF_Block_Nums)||((nOffset+nDataNum)>LOGIC_BLOCK_SIZES))
	    return RT_PARAM_ERR;

	XF_MapRead(pDataBuf, ((gXF_Start_Block+nBlock*LOGIC_BLOCK_SECTOR_NUM)*XF_SECTOR_SIZE+nOffset), nDataNum);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_WriteBlockData
* Function:		write data to a block
* Input:	   	UINT8* pDataBuf-the buffer write 
*	   			UINT32 nBlock- the block num will write data
*	   			UINT32 nOffset- the offset address in a block;
*	   			UINT32 nDataNum- the data number by byte will write;		
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_WriteBlockData(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{
	UINT32 j;
	UINT8 *pAddr;
	UINT8 *pTmp;
	UINT8 Buf[XF_SECTOR_SIZE];
	UINT32 lStartAddr,lBlockNum,lWriteLen;
	UINT32 lStartAddrTemp;
		
	if((nBlock>=gXF_Block_Nums)||((nOffset+nDataNum)>LOGIC_BLOCK_SIZES))
	    return RT_PARAM_ERR;	
 	
 	//计算要修改的页数 
 	if(nOffset==0)
 	   lBlockNum=(nDataNum+XF_SECTOR_SIZE-1)/XF_SECTOR_SIZE;
 	else
 	   lBlockNum=(nOffset+XF_SECTOR_SIZE-1)/XF_SECTOR_SIZE+(nDataNum-(XF_SECTOR_SIZE-nOffset)+XF_SECTOR_SIZE-1)/XF_SECTOR_SIZE;
 	
 	if(lBlockNum>LOGIC_BLOCK_SECTOR_NUM) 
 	    return RT_PARAM_ERR;  
	  
	lStartAddr = (gXF_Start_Block+nBlock*LOGIC_BLOCK_SECTOR_NUM)*XF_SECTOR_SIZE;	
      
    //2---写新数据  
    pAddr=pDataBuf;
	pTmp = (UINT8 *)Buf;
    for(j=0;j<lBlockNum;j++)
    {
		lStartAddrTemp = lStartAddr;	 
		XF_MapRead(pTmp, lStartAddrTemp, XF_SECTOR_SIZE);
		//erase the block
		// 判断是否已擦除 2013.05.13 coins
//		if(XF_MapIsBufAllOne((void *)Buf, XF_SECTOR_SIZE/4) != RT_OK)
		{
			if(XF_MapEraseSector(lStartAddrTemp, 1) != RT_OK)
			{
				SQI_WrProtect(SQI_PROT_DI);	
				return RT_FAIL;
			}
		}
    	// copy data to write buffer
    	if((nDataNum+nOffset)>XF_SECTOR_SIZE)
    	     lWriteLen=XF_SECTOR_SIZE-nOffset;    	    
    	else
    	     lWriteLen=nDataNum;
    	     
        memcpy((pTmp+nOffset), pAddr, lWriteLen); 
    
        //write to flash
		if(XF_MapPageProgram(pTmp, lStartAddrTemp, XF_SECTOR_SIZE/XF_PAGE_SIZE) != RT_OK)
		{	
			SQI_WrProtect(SQI_PROT_DI);	
			return RT_FAIL;
		}
		// next page
        lStartAddr+=XF_SECTOR_SIZE;
        pAddr+=lWriteLen;
        nOffset=0;
        nDataNum-=lWriteLen;
    }
	
	SQI_WrProtect(SQI_PROT_DI);	 
    return RT_OK;
}

#ifdef	XF_REPLACE_WITH_BAD_BLOCK
/***************************************************************************
* Subroutine:	XF_MapBadBlockStorePageInit
* Function:		
* Input:			
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_MapBadBlockStorePageInit(void)
{
	UINT16 i;
	UINT32 lAddr;	
	UINT32 nData = XF_BAD_BLOCK_STORE_SECTOR_FLAG;
		
	lAddr = XF_BAD_BLOCK_STORE_SECTOR*XF_SECTOR_SIZE;
	if(*((UINT32*)(XFLASH_BASE_ADDR+lAddr+XF_BAD_BLOCK_STORE_SECTOR_FLAG_ADDR)) != nData)
	{
		for(i=0;i<XF_BAD_BLOCK_STORE_SECTOR_NUM;i++)
		{
			if(XF_MapEraseSector(lAddr, 1) != RT_OK)
			{
				SQI_WrProtect(SQI_PROT_DI);	
		   	  	return RT_FAIL;
			}
			lAddr += XF_SECTOR_SIZE;		
		}
		lAddr = (XF_BAD_BLOCK_STORE_SECTOR*XF_SECTOR_SIZE + XF_BAD_BLOCK_STORE_SECTOR_FLAG_ADDR);
		XF_MapProgram((UINT8 *)&nData, lAddr, 4);
		SQI_WrProtect(SQI_PROT_DI);	
		if(nData == *(UINT32 *)(XFLASH_BASE_ADDR+lAddr)) return RT_OK;
		else return RT_FAIL;
	}
    return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_MapFindBadBlock
* Function:		
* Input:			
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_MapFindBadBlock(void)
{
	UINT16 i;
	UINT32 lAddr;	
	UINT32 nData = XF_BAD_BLOCK_STORE_SECTOR_FLAG;
	UINT32 nBadBlockData[XF_MAX_BAD_BLOCK_STORE_NUMS];
	UINT32 nBadBlockTemp;

	gXF_BadBlock_Nums = 0;
	lAddr = XF_BAD_BLOCK_STORE_SECTOR*XF_SECTOR_SIZE;
	if(*((UINT32*)(XFLASH_BASE_ADDR+lAddr+XF_BAD_BLOCK_STORE_SECTOR_FLAG_ADDR)) == nData)
	{
		for(i=0;i<XF_MAX_BAD_BLOCK_STORE_NUMS;i++)
		{
			XF_MapRead((UINT8*)(nBadBlockData+i), (lAddr+(i+1)*XF_PAGE_SIZE), 4);
			if(nBadBlockData[i] != 0xffffffff)
			{
				nBadBlockTemp = (nBadBlockData[i]&0x0000FFFF);
				gXF_BadBlock_Nums++;
				if(((nBadBlockTemp^(nBadBlockData[i]>>16)) == 0x0000FFFF)&&(nBadBlockTemp<gXF_Block_Nums))
				{
					gXFStatus[nBadBlockTemp] = STATUS_BAD;
					gXFMap[nBadBlockTemp] = 0xffff;
					gXFPageWriteSeq[nBadBlockTemp]=0xffff;				
				}
			}
		}
	    return RT_OK;
	}
	else
    	return RT_FAIL;
}

/***************************************************************************
* Subroutine:	XF_MapMarkBadBlock
* Function:		
* Input:		UINT32 nBadBlock-- bad block	
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_MapMarkBadBlock(UINT32 nBadBlock)
{
	UINT16 i;
	UINT32 lAddr;	
	UINT32 nData = XF_BAD_BLOCK_STORE_SECTOR_FLAG;
	UINT16 XFBadBlockNums =	gXF_BadBlock_Nums+1;
	UINT32 nBadBlockData[XF_MAX_BAD_BLOCK_STORE_NUMS];
	UINT32 nBadBlockTemp;

	gXFMap[nBadBlock]=0xffff;
	gXFStatus[nBadBlock]=STATUS_BAD;                			
	gXFPageWriteSeq[nBadBlock]=0xffff;	

	lAddr = XF_BAD_BLOCK_STORE_SECTOR*XF_SECTOR_SIZE+XF_BAD_BLOCK_STORE_SECTOR_FLAG_ADDR;
	if(*((UINT32*)(XFLASH_BASE_ADDR+lAddr)) == nData)
	{
		for(i=0;i<XF_MAX_BAD_BLOCK_STORE_NUMS;i++)
		{
			XF_MapRead((UINT8*)(nBadBlockData+i), (lAddr+(i+1)*XF_PAGE_SIZE), 4);
		}
		nBadBlockTemp = (nBadBlock |((~nBadBlock)<<16));
		for(i=0;i<gXF_BadBlock_Nums;i++)
		{
			if(nBadBlockData[i] == nBadBlockTemp)
				return RT_OK;			
		}
		if(XFBadBlockNums>XF_MAX_BAD_BLOCK_STORE_NUMS)
		{
			gXF_BadBlock_Nums++;
			return RT_PARAM_ERR;
		}
		lAddr += XFBadBlockNums*XF_PAGE_SIZE;
		gXF_BadBlock_Nums++;
		if(*(UINT32 *)(XFLASH_BASE_ADDR+lAddr) == 0xFFFFFFFF)
		{
			XF_MapProgram((UINT8 *)&nBadBlockTemp, lAddr, 4);
			SQI_WrProtect(SQI_PROT_DI);	
			if(nBadBlockTemp == *(UINT32 *)(XFLASH_BASE_ADDR+lAddr)) return RT_OK;
			else return RT_FAIL;
		}
		else 
			return RT_FAIL;
	}
    return RT_FAIL;
}
#endif

/***************************************************************************
* Subroutine:	XF_EraseBlock
* Function:		
* Input:		UINT32 nBlock- the block num will Erase	
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_EraseBlock(UINT32 nBlock)
{
	UINT16 i;
	UINT32 lAddr;
	    
	if(nBlock>=gXF_Block_Nums)
		return RT_PARAM_ERR;	
	
	lAddr = (gXF_Start_Block+nBlock*LOGIC_BLOCK_SECTOR_NUM)*XF_SECTOR_SIZE;
	for(i=0;i<LOGIC_BLOCK_SECTOR_NUM;i++)
	{
		if(XF_MapEraseSector(lAddr, 1) != RT_OK)
		{
			SQI_WrProtect(SQI_PROT_DI);	
	   	  	return RT_FAIL;
		}
		lAddr += XF_SECTOR_SIZE;		
	}
    return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_EraseBlockMapStatus
* Function:		erase the map status of the block
* Input:		UINT32 nBlock- the block num will Erase	
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_EraseBlockMapStatus(UINT32 nBlock)
{
	UINT32 lAddr;
//	UINT8 Buf[XF_SECTOR_SIZE];
		    
	if(nBlock>=gXF_Block_Nums)
		return RT_PARAM_ERR;	
				
	lAddr = (gXF_Start_Block+nBlock*LOGIC_BLOCK_SECTOR_NUM+XF_MAP_STATUS_SECTOR_ADDR)*XF_SECTOR_SIZE;
//	XF_MapRead(Buf, lAddr, XF_SECTOR_SIZE);
	//erase the block
	// 判断是否已擦除 2013.05.13 coins
//	if(XF_MapIsBufAllOne((void *)Buf, XF_SECTOR_SIZE/4) != RT_OK)
	{
		if(XF_MapEraseSector(lAddr, 1) != RT_OK)
		{
			gXFMap[nBlock]=0xffff;
			gXFStatus[nBlock]=STATUS_NOTUSED;                			
			gXFPageWriteSeq[nBlock]=0xffff;	
			SQI_WrProtect(SQI_PROT_DI);	
			#ifdef	XF_REPLACE_WITH_BAD_BLOCK
			XF_MapMarkBadBlock(nBlock);
			#endif
	   	  	return RT_FAIL;
		}
	}
	gXFMap[nBlock]=0xffff;
	gXFStatus[nBlock]=STATUS_NOTUSED;                			
	gXFPageWriteSeq[nBlock]=0xffff;		
    return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_SetMapStatus
* Function:		Set the map status of the block
* Input:		UINT8* pDataBuf-the status data write buff 
*	   			UINT32 nOldPhyBlock- the phy block;
*	   			UINT32 NewnPhyBlock- the phy block;
*	   			UINT16 nLogicBlock- the logic block;
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_SetMapStatus(UINT8* pDataBuf,UINT32 nOldPhyBlock,UINT32 NewnPhyBlock,UINT16 nLogicBlock)
{
    STRUCT_MAP_STATUS nMapStatus;
	UINT32 nPageDataCHK,i;
	UINT32* lMapStatusPageCHK = (UINT32*)(pDataBuf+24);
	UINT32* pData = (UINT32*)(pDataBuf-(XF_PAGE_SIZE-XF_MAP_STATUS_SIZE));

	memset((UINT8*)&nMapStatus,0xff,sizeof(nMapStatus));
   	if(nOldPhyBlock == 0xffff)
	{
//		gXFMap[NewnPhyBlock]=nLogicBlock;              			
		gXFPageWriteSeq[NewnPhyBlock]=0;
//		gXFStatus[nBlock]=STATUS_USED;  
	}
	else
	{
//		gXFMap[NewnPhyBlock]=nLogicBlock;              			
		gXFPageWriteSeq[NewnPhyBlock]=gXFPageWriteSeq[nOldPhyBlock]+1;
//		gXFStatus[nBlock]=STATUS_USED; 
	}

	nMapStatus.lLogicPage = ((~nLogicBlock)<<16)|nLogicBlock;
//	nMapStatus.lLogicPageCHK = ~nMapStatus.lLogicPage;
	nMapStatus.lPageWriteSeq = ((~gXFPageWriteSeq[NewnPhyBlock])<<16)|gXFPageWriteSeq[NewnPhyBlock];
	if(gWrite2BlockFlag != 2)
	{
		if((gWriteSeq+1) == ((UINT64)0x7FFFFFFFFFFFFFFF)) 
			nMapStatus.lWriteSeq = 0;
		else
			nMapStatus.lWriteSeq = gWriteSeq+1;
		if(gWrite2BlockFlag)
			nMapStatus.lWriteSeq |= ((UINT64)1<<63);
	}
	else
		nMapStatus.lWriteSeq = gWriteSeq|((UINT64)1<<63);		
	nMapStatus.lWriteSeqCHK = ~nMapStatus.lWriteSeq;	
	nMapStatus.lStatus = XF_LOGIC_BLOCK_STATUS_USED;
   	memcpy((UINT8 *)pDataBuf, (UINT8 *)&nMapStatus, sizeof(STRUCT_MAP_STATUS));		
	nPageDataCHK = 0;
	for(i=0;i<(XF_PAGE_SIZE/4-2);i++)
	{
		nPageDataCHK ^= pData[i];
	}
	* lMapStatusPageCHK = nPageDataCHK;    			
    return RT_OK;
}
/***************************************************************************
* Subroutine:	XF_ReadDataInBlock
* Function:		read data from a logic block
* Input:		UINT8* pDataBuf-the buffer read 
*	   			UINT32 nBlock- the logic block num will read data
*	   			UINT32 lOffset- the offset address in a block;
*	   			UINT32 lDataNum- the data number by byte will read;		
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_ReadDataInBlock(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{
	UINT16 i;
	for(i=0;i<gXF_Block_Nums;i++)
	{  
		if(gXFMap[i]==nBlock)	//find the block in map table;
		{
			return XF_ReadBlockData(pDataBuf,i,nOffset,nDataNum);
	    }
	}
	memset(pDataBuf,0,nDataNum);	//not find logic block;
	return RT_FAIL;
}

/***************************************************************************
* Subroutine:	XF_FindBlock
* Function:		find a free block to write
* Input:		UINT32 nLogicBlock- the logic block will be to write;
*	   			UINT16* nFindPhyBlock- the logical block corresponding to the physical block;
*	   			UINT16* nFindFreePhyBlock- a free phy block to write;		
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static void XF_FindBlock(UINT32 nLogicBlock,UINT16* nFindPhyBlock,UINT16* nFindFreePhyBlock)
{
	UINT16 i;
	UINT16 nFind=0xffff;
	UINT16 nBlank=0xffff;

	for(i=0;i<gXF_Block_Nums;i++)
	{  
		if(gXFMap[i]==nLogicBlock)
		{
			nFind=i;
			break;
		}
	}
    
	//-----find a blank block;
	if(nFind != 0xffff)
	{
	    for(i=nFind;i<gXF_Block_Nums;i++)
	    {        
			if((gXFStatus[i]!=STATUS_USED)&&(gXFStatus[i]!=STATUS_BAD))
			{
			      nBlank=i;	
			      break;
			}
		}
	}        

	if(nBlank == 0xffff)
	{
	    for(i=nLogicBlock;i<gXF_Block_Nums;i++)
	    {        
			if((gXFStatus[i]!=STATUS_USED)&&(gXFStatus[i]!=STATUS_BAD))
			{
			      nBlank=i;	
			      break;
			}
		}
		if(nBlank == 0xffff)
		{
		    for(i=0;i<nLogicBlock;i++)
		    {        
				if((gXFStatus[i]!=STATUS_USED)&&(gXFStatus[i]!=STATUS_BAD))
				{
				      nBlank=i;	
				      break;
				}
			}
		}
	}
	* nFindPhyBlock = nFind;
	* nFindFreePhyBlock = nBlank;
}

/***************************************************************************
* Subroutine:	XF_WriteDataInBlock
* Function:		write data to a logic block
* Input:		UINT8* pDataBuf-the buffer write 
*	   			UINT32 nBlock- the logic block num will write data
*	   			UINT32 lOffset- the offset address in a block;
*	   			UINT32 lDataNum- the data number by byte will write;		
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_WriteDataInBlock(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{
	UINT16 nFind=0xffff;
	UINT16 nBlank=0xffff;
	UINT16 nFindNext=0xffff;
	UINT16 nBlankNext=0xffff;
	UINT16 nTempPhyBlock=0xffff;
	UINT8* pAddr;
	UINT8 pBuf[LOGIC_BLOCK_SIZES];
	UINT16 nFreeBlock = 1;
	UINT8 nRet;
				
	if(nBlock>=gXF_Block_Nums) return RT_PARAM_ERR;	
	pAddr=pBuf;	

	XF_FindBlock(nBlock,&nFind,&nBlank);
	 
	if(gWrite2BlockFlag == 1)
		XF_FindBlock((nBlock+1),&nFindNext,&nBlankNext);

	if(nFind!=0xffff)//
	{
		nFreeBlock = 0;
		if((gWrite2BlockFlag == 1)&&(nFindNext == 0xffff))
			nFreeBlock++;

		if((gXF_UsedBlock_Nums+XF_RSVD_LOGIC_BLOCK_NUMS+nFreeBlock) > gXF_Block_Nums)
			return RT_PARAM_ERR;

		//read data from old page 
		XF_ReadBlockData(pAddr,nFind,0,LOGIC_BLOCK_SIZES);
		memcpy(pAddr+nOffset,pDataBuf,nDataNum);

		if(nBlank == 0xffff)
			nTempPhyBlock = nFind; //write back
		else
			nTempPhyBlock = nBlank;	//write to blank page
		//set block status;
		XF_SetMapStatus(pAddr+XF_MAP_STATUS_ADDR,nFind,nTempPhyBlock,nBlock); 
		nRet = XF_WriteBlockData(pAddr,nTempPhyBlock,0,LOGIC_BLOCK_SIZES);
		if(nRet != RT_OK)
		{
			if(XF_EraseBlockMapStatus(nTempPhyBlock) != RT_FAIL)
			{
				#ifdef	XF_REPLACE_WITH_BAD_BLOCK
				XF_MapMarkBadBlock(nTempPhyBlock);
				#endif
			}
			return nRet;
		}	
		gXFMap[nTempPhyBlock]=nBlock;        
		gXFStatus[nTempPhyBlock]=STATUS_USED;
		gEraseBlock1[gEraseBlockNum1++] = nTempPhyBlock;
		if((nFind != 0xffff)&&(nBlank != 0xffff))
		{
//			gXFMap[nFind]=0xffff;
//			gXFPageWriteSeq[nFind]=0;
//			gXFStatus[nFind]=STATUS_NOTUSED;
			gEraseBlock[gEraseBlockNum++] = nFind;
		}
	}
	else//
	{
		if(nBlank==0xffff) return RT_PARAM_ERR;
		else
		{
			if(gWrite2BlockFlag == 1)
			{
				if(nFindNext == 0xffff)
					nFreeBlock++;
			}	
			if((gXF_UsedBlock_Nums+XF_RSVD_LOGIC_BLOCK_NUMS+nFreeBlock) > gXF_Block_Nums)
				return RT_PARAM_ERR;
			//read data from old page 
			XF_ReadBlockData(pAddr,nBlank,0,LOGIC_BLOCK_SIZES);
			//set block status;
			memcpy(pAddr+nOffset,pDataBuf,nDataNum);
			XF_SetMapStatus(pAddr+XF_MAP_STATUS_ADDR,nFind,nBlank,nBlock);       
			//write to blank page.
		    nRet = XF_WriteBlockData(pAddr,nBlank,0,LOGIC_BLOCK_SIZES);
			if(nRet != RT_OK)
			{
				if(XF_EraseBlockMapStatus(nBlank) != RT_FAIL)
				{
					#ifdef	XF_REPLACE_WITH_BAD_BLOCK
					XF_MapMarkBadBlock(nBlank);
					#endif
				}
				return nRet;
			}			 
			gXFMap[nBlank]=nBlock;  
			gXFStatus[nBlank]=STATUS_USED;
			gEraseBlock1[gEraseBlockNum1++] = nTempPhyBlock;
			gXF_UsedBlock_Nums++;
		}
	}
	if(gWrite2BlockFlag != 2)
	{
		if((++gWriteSeq) == ((UINT64)0x7FFFFFFFFFFFFFFF)) 
			gWriteSeq = 0;
	}    		
	return RT_OK;  
}

/***************************************************************************
* Subroutine:	XF_BuildMap
* Function:		build index map
* Input:		
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 XF_BuildMap(void)
{
    UINT16 i,j,k=0;
	UINT8 t;    
    STRUCT_MAP_STATUS nMapStatus;
	UINT32 nLogic;
	UINT32 nPageData[XF_PAGE_SIZE/4];
	UINT32 nPageDataCHK;
	UINT32 nPageWriteSeq;
    UINT32 nBlockUsedNum;
	UINT64 nTempWriteSeq[4][2];
	UINT16 nTempPage[4][2]; 
	UINT16 nFlag = 0; 
	UINT64 nWriteSeq[MAX_XF_LOGIC_BLOCK_NUMS];

	gXF_UsedBlock_Nums = 0;
	nBlockUsedNum=gXF_Block_Nums;          
    memset((UINT8*)&gXFMap,0xff,sizeof(gXFMap)); 
	memset((UINT8*)&gXFPageWriteSeq,0xff,sizeof(gXFPageWriteSeq));  
	memset((UINT8*)&nWriteSeq,0xff,sizeof(nWriteSeq));    
    memset((UINT8*)&gXFStatus,STATUS_NOTUSED,sizeof(gXFStatus)); 
	memset((UINT8*)&nTempWriteSeq,0xff,sizeof(nTempWriteSeq));    
    memset((UINT8*)&nTempPage,0xff,sizeof(nTempPage));
	   
	for(i=0;i<gXF_Block_Nums;i++)
	{
		XF_ReadBlockData((UINT8*)&nMapStatus,i,XF_MAP_STATUS_ADDR,XF_MAP_STATUS_SIZE); 
		if(nMapStatus.lStatus==XF_LOGIC_BLOCK_STATUS_USED)// used
		{			
			if((nMapStatus.lWriteSeq^nMapStatus.lWriteSeqCHK) != ((UINT64)0xFFFFFFFFFFFFFFFF)) 
				continue;
			nPageWriteSeq = (nMapStatus.lPageWriteSeq&0x0000FFFF);
			if((nPageWriteSeq^(nMapStatus.lPageWriteSeq>>16)) != 0x0000FFFF)
				continue;
			nLogic = (nMapStatus.lLogicPage&0x0000FFFF);
//			if(((nMapStatus.lLogicPage^nMapStatus.lLogicPageCHK) != 0xFFFFFFFF)||((nLogic^(nMapStatus.lLogicPage>>16)) != 0x0000FFFF))
			if((nLogic^(nMapStatus.lLogicPage>>16)) != 0x0000FFFF)	
				continue;
			if(nLogic >= nBlockUsedNum)
			   	continue;			
			nPageDataCHK = 0;
			memset((UINT8*)&nPageData,0x00,XF_PAGE_SIZE);
			XF_ReadBlockData((UINT8*)&nPageData,i,(LOGIC_BLOCK_SIZES-XF_PAGE_SIZE),XF_PAGE_SIZE);	
			for(t=0;t<(XF_PAGE_SIZE/4-2);t++)
			{
				nPageDataCHK ^= nPageData[t];
			}
			if(nMapStatus.lMapStatusPageCHK != nPageDataCHK)
				continue;

			//set the map status
			gXFMap[i]=nLogic;//store the logic page
			gXFStatus[i]=STATUS_USED;                			
			gXFPageWriteSeq[i]=nPageWriteSeq;
			gXF_UsedBlock_Nums++;
			nWriteSeq[i]=nMapStatus.lWriteSeq;
			if(gWriteSeq<((nWriteSeq[i]<<1)>>1)) gWriteSeq = ((nWriteSeq[i]<<1)>>1);
			//find whether exist the same logic page
			for(j=0;j<i;j++) 
			{
				if(gXFMap[j] == nLogic)//find the same page
				{
					if(((UINT16)(gXFPageWriteSeq[i]+1)) == gXFPageWriteSeq[j])
					{
						if((nWriteSeq[j]&((UINT64)1<<63)) == ((UINT64)1<<63))//
						{
							nTempWriteSeq[k][1] = nWriteSeq[j];
							nTempPage[k][1] = j;
							nTempWriteSeq[k][0] = nWriteSeq[i];
							nTempPage[k][0] = i;
							k++;
						}
						else
						{
							XF_EraseBlockMapStatus(i);
							gXF_UsedBlock_Nums--;
						}
					}
					else if(((UINT16)(gXFPageWriteSeq[j]+1)) == gXFPageWriteSeq[i])
					{
						if((nWriteSeq[i]&((UINT64)1<<63)) == ((UINT64)1<<63))//
						{
							nTempWriteSeq[k][1] = nWriteSeq[i];
							nTempPage[k][1] = i;
							nTempWriteSeq[k][0] = nWriteSeq[j];
							nTempPage[k][0] = j;
							k++;
						}
						else
						{
							XF_EraseBlockMapStatus(j);
							gXF_UsedBlock_Nums--;
						}
					}
					else{}
////					if(gXFPageWriteSeq[i]<gXFPageWriteSeq[j])
////					{
////						if((gXFPageWriteSeq[i] == 0)&&(gXFPageWriteSeq[j] == 0xffff))
////						{
////							if((nWriteSeq[i]&((UINT64)1<<63)) == ((UINT64)1<<63))//
////							{
////								nTempWriteSeq[k][1] = nWriteSeq[i];
////								nTempPage[k][1] = i;
////								nTempWriteSeq[k][0] = nWriteSeq[j];
////								nTempPage[k][0] = j;
////								k++;
////							}
////							else
////								XF_EraseBlockMapStatus(j);
////						}
////						else
////						{
////							if((nWriteSeq[j]&((UINT64)1<<63)) == ((UINT64)1<<63))//
////							{
////								nTempWriteSeq[k][1] = nWriteSeq[j];
////								nTempPage[k][1] = j;
////								nTempWriteSeq[k][0] = nWriteSeq[i];
////								nTempPage[k][0] = i;
////								k++;
////							}
////							else
////								XF_EraseBlockMapStatus(i);
////						}
////					}
////					else(gXFPageWriteSeq[i]>gXFPageWriteSeq[j])
////					{
////						if((gXFPageWriteSeq[j] == 0)&&(gXFPageWriteSeq[i] == 0xffff))
////						{
////							if((nWriteSeq[j]&((UINT64)1<<63)) == ((UINT64)1<<63))//
////							{
////								nTempWriteSeq[k][1] = nWriteSeq[j];
////								nTempPage[k][1] = j;
////								nTempWriteSeq[k][0] = nWriteSeq[i];
////								nTempPage[k][0] = i;
////								k++;
////							}
////							else
////								XF_EraseBlockMapStatus(i);
////						}
////						else
////						{
////							if((nWriteSeq[i]&((UINT64)1<<63)) == ((UINT64)1<<63))//
////							{
////								nTempWriteSeq[k][1] = nWriteSeq[i];
////								nTempPage[k][1] = i;
////								nTempWriteSeq[k][0] = nWriteSeq[j];
////								nTempPage[k][0] = j;
////								k++;
////							}
////							else
////								XF_EraseBlockMapStatus(j);
////						}
////					}
////					else{}					                   
				}
			}              
		}                     
	}	
	if(k>0)
	{
		if(k==1)
		{
			for(i=0;i<gXF_Block_Nums;i++)
			{
			 	if(gXFStatus[i]==STATUS_USED)
				{
					if((nTempPage[0][1] != i)&&(nTempPage[0][0] != i)&&(nWriteSeq[i] == nTempWriteSeq[0][1]))
					{
					 	if(((gXFMap[i]+1) == gXFMap[nTempPage[0][1]])||((gXFMap[nTempPage[0][1]]+1) == gXFMap[i]))
							nFlag++;
					}
				}
			}
			if(nFlag)
				XF_EraseBlockMapStatus(nTempPage[0][0]);
			else
				XF_EraseBlockMapStatus(nTempPage[0][1]);
			gXF_UsedBlock_Nums-=1;
		}
		else
		{
			if(nTempWriteSeq[0][1] == nTempWriteSeq[1][1])
			{
				XF_EraseBlockMapStatus(nTempPage[0][0]);
				XF_EraseBlockMapStatus(nTempPage[1][0]);
			}
			else
			{
				XF_EraseBlockMapStatus(nTempPage[0][1]);
				XF_EraseBlockMapStatus(nTempPage[1][1]);
			}
			gXF_UsedBlock_Nums-=2;
		}
	} 
	return RT_OK;
}

/***************************************************************************
* Subroutine:	XF_MapSetStrAddr
* Function:		set start addr
* Input:		UINT32 nStartAddr --	the start Offset address.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 XF_MapSetStrAddr(UINT32 nStartAddr)
{
	if(nStartAddr > XF_MAX_ADDR) return RT_PARAM_ERR; 
	gXF_Start_Block = (nStartAddr+XF_SECTOR_SIZE-1)/XF_SECTOR_SIZE;
	return RT_OK; 
}
	 
/***************************************************************************
* Subroutine:	XF_MapInit
* Function:		to initialize external flash interface 
* Input:	 	UINT32 nCapacity --	 the flash address 
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 XF_MapInit(UINT32 nCapacity)
{
	UINT8 bBadPage=0;
	UINT8 nRet=0;
	UINT16 nStartBlock = gXF_Start_Block;
	UINT16 nEndBlock = nCapacity/XF_SECTOR_SIZE;

	gXF_End_Block = 0;
	gXF_Block_Nums = 0;
	gWriteSeq = 0;
	gXF_UsedBlock_Nums = 0;
	gXF_BadBlock_Nums = 0;
	gXF_Capacity = 0;
				
#ifdef	XF_REPLACE_WITH_BAD_BLOCK
	bBadPage = XF_BAD_BLOCK_STORE_SECTOR_NUM;
#endif

	if(nCapacity>XF_MAX_ADDR) return RT_PARAM_ERR;
	if(nEndBlock<nStartBlock) return RT_PARAM_ERR; 
    if((nEndBlock-nStartBlock)<(LOGIC_BLOCK_SECTOR_NUM+bBadPage))
    	return RT_PARAM_ERR;

	#ifndef	SQI_DRV
	SPIFL_Init(2);
	#else
	if(SQI_Init(DEFAULT_MAP, DEFAULT_CLK_DIV, DEFAULT_RD_MODE) != RT_OK) return RT_FAIL;
	#endif

    gXF_End_Block = nEndBlock;
	gXF_Capacity = nCapacity;

#ifdef	XF_REPLACE_WITH_BAD_BLOCK
	gXF_End_Block = nEndBlock - XF_BAD_BLOCK_STORE_SECTOR_NUM;
#endif
	gXF_Block_Nums=(gXF_End_Block-gXF_Start_Block)/LOGIC_BLOCK_SECTOR_NUM;

	if(gXF_Block_Nums>1)
		XF_BuildMap();

#ifdef	XF_REPLACE_WITH_BAD_BLOCK
	nRet = XF_MapBadBlockStorePageInit();   
	if(nRet != RT_OK) return nRet;
	nRet = XF_MapFindBadBlock();
	if(nRet != RT_OK) return nRet;
#endif
     
	return RT_OK;   
}

/***************************************************************************
* Subroutine:	XF_MapFormat
* Function:		to erase the XSF flash
* Input:		
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	Before using XF_MapFormat, please call XF_MapInit 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 XF_MapFormat(void)
{
	UINT16 i;
	UINT16 nStartBlock = gXF_Start_Block; 
	UINT16 nEndBlock = gXF_End_Block;
	UINT32 lAddr;

	if(((nStartBlock == 0)&&(nEndBlock == 0))||(nEndBlock<nStartBlock))
		return RT_PARAM_ERR; 

#ifdef	XF_REPLACE_WITH_BAD_BLOCK
	nEndBlock += XF_BAD_BLOCK_STORE_SECTOR_NUM;
#endif

	for(i=nStartBlock;i<nEndBlock;i++)
	{					
		lAddr = i*FLASH_PAGE_SIZE;
		if(XF_MapEraseSector(lAddr, 1) != RT_OK)
		{
			SQI_WrProtect(SQI_PROT_DI);	
			return RT_FAIL;
		}
	}
	return RT_OK;  	
} 

/***************************************************************************
* Subroutine:	XF_MapWriteData
* Function:		write data to save flash area.
* Input:		UINT8* pDataBuf --	the data to be written save flash area;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 XF_MapWriteData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	UINT32 lWriteLen;
	UINT16 nNum;
	UINT16 nNumTemp = nDataNum;
	UINT16 nStartBlock,nStartOffset;
	UINT8 *pBuf;
	UINT8 nRet;
	UINT16 i;
	UINT16 nXFUsedBlockNumsTemp;
		
	if(nDataNum<=0)	return RT_PARAM_ERR;
	if((nOffset+nDataNum)>((gXF_Block_Nums-XF_RSVD_LOGIC_BLOCK_NUMS)*XF_LOGIC_BLOCK_SIZE)) return RT_PARAM_ERR;
#ifdef	XF_REPLACE_WITH_BAD_BLOCK
	if(gXF_BadBlock_Nums>XF_MAX_BAD_BLOCK_STORE_NUMS) return RT_PARAM_ERR;
#endif

	pBuf=pDataBuf;

   	while(nNumTemp)
	{
		if(nNumTemp > XF_LOGIC_BLOCK_SIZE)
			nNum = XF_LOGIC_BLOCK_SIZE;
		else
			nNum = nNumTemp;
		nNumTemp-=nNum;		
		gEraseBlockNum = 0;
		gEraseBlockNum1 = 0;
		gWrite2BlockFlag = 0;
		nXFUsedBlockNumsTemp = gXF_UsedBlock_Nums;
		//loop to write data to flash
		do{
			nStartOffset=(UINT16)(nOffset%XF_LOGIC_BLOCK_SIZE);
			nStartBlock=(UINT16)(nOffset/XF_LOGIC_BLOCK_SIZE);
					 	
	    	if(nNum>(XF_LOGIC_BLOCK_SIZE-nStartOffset))
			{ 	
	    		lWriteLen=XF_LOGIC_BLOCK_SIZE-nStartOffset;
				gWrite2BlockFlag = 1;
			}
	        else
			{
	           	lWriteLen=nNum;
				if(gWrite2BlockFlag == 1)gWrite2BlockFlag = 2;
			}
	
	        nRet=XF_WriteDataInBlock(pBuf,nStartBlock,nStartOffset,lWriteLen);  	 
			if(nRet!=RT_OK) 
			{
				//erase new block
				for(i=0;i<gEraseBlockNum1;i++)
				{
					gXFMap[gEraseBlock1[i]]=0xffff;
					gXFPageWriteSeq[gEraseBlock1[i]]=0xffff;
					gXFStatus[gEraseBlock1[i]]=STATUS_NOTUSED;
					XF_EraseBlockMapStatus(gEraseBlock1[i]);
				}
				gXF_UsedBlock_Nums = nXFUsedBlockNumsTemp;
				return nRet; 
			} 
			    
			pBuf+=lWriteLen;
			nOffset+=lWriteLen;
			nNum-=lWriteLen;
					   
		}while(nNum>0);
		
		//erase old block
		for(i=0;i<gEraseBlockNum;i++)
		{
			gXFMap[gEraseBlock[i]]=0xffff;
			gXFPageWriteSeq[gEraseBlock[i]]=0xffff;
			gXFStatus[gEraseBlock[i]]=STATUS_NOTUSED;
			XF_EraseBlockMapStatus(gEraseBlock[i]);
		}
	}	    
	return RT_OK;
}
 
/***************************************************************************
* Subroutine:	XF_MapReadData
* Function:		read data from save flash area
* Input:		UINT8* pDataBuf -- 	the buffer to save data for read;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 XF_MapReadData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	UINT8 nRet;
	UINT16 nStartBlock,nStartOffset; 
	UINT32 lReadLen;

	if((nOffset+nDataNum)>((gXF_Block_Nums-XF_RSVD_LOGIC_BLOCK_NUMS)*XF_LOGIC_BLOCK_SIZE)) return RT_PARAM_ERR;

	while(nDataNum)
	{
		nStartOffset=(UINT16)(nOffset%XF_LOGIC_BLOCK_SIZE);
		nStartBlock=(UINT16)(nOffset/XF_LOGIC_BLOCK_SIZE);

		if(nDataNum>(XF_LOGIC_BLOCK_SIZE-nStartOffset)) 	
    		lReadLen=XF_LOGIC_BLOCK_SIZE-nStartOffset;
        else
           	lReadLen=nDataNum;

		nRet=XF_ReadDataInBlock(pDataBuf,nStartBlock,nStartOffset,lReadLen);
		if(nRet!=RT_OK) return nRet;
		nDataNum -= lReadLen;
		nOffset += lReadLen;
		pDataBuf += lReadLen;
	}	
	return RT_OK;
}

#endif
