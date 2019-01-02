 /**********************************************************************************
* File name    :	SFlashMap.c
* Function     :	Safe flash operate by map mode.
* Author       : 	Emil
* Date         :	2016/07/03
* Version      :    v1.0
* Description  :
* The structure of Save Flash Area:
*
*   |<-------------- SF_Init:nEndBlock - nStartBlock------------->|
*												   |<-- 1_PAGE -->|
* 	--------------------------------------------------------------------
*	|*****************数据存储区*******************|**坏块记录区**|
* 	-------------------------------------------------------------------------
*  /|\							   				  /|\            /|\
*	|  											   |              |
* gSF_Start_Block			  				  gSF_End_Block	                
* nStartBlock				             						nEndBlock
* ModifyRecord :
* 1. 修改SF_MapWriteData函数写大数据时的BUG 。	2016.10.21	Emil

**********************************************************************************/
/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"

#ifdef	SFLASH_MAP_MODE_USED
/**********************************************************************************
* Local Macro Definition
**********************************************************************************/ 
#define	REPLACE_WITH_BAD_BLOCK			// if using the bad block replacement function, please open REPLACE_WITH_BAD_BLOCK					

#define LOGIC_BLOCK_PAGE_NUM  		(2)												//每一逻辑块包含页数(用户可根据实际修改)
#define LOGIC_BLOCK_SIZE  	 		(FLASH_PAGE_SIZE*LOGIC_BLOCK_PAGE_NUM)			//逻辑块大小
#define MAX_FLASH_PAGE_NUMS  		(AREA_END_PARA_PAGE-1-120)						//SF最大页数(用户可根据实际修改)
#define MAX_LOGIC_BLOCK_NUMS  		(MAX_FLASH_PAGE_NUMS/LOGIC_BLOCK_PAGE_NUM)		//SF最大逻辑块数
#define MAP_STATUS_SIZE 			(sizeof(STRUCT_MAP_STATUS)) 					//状态大小
#define	MAP_STATUS_ADDR				(LOGIC_BLOCK_SIZE-MAP_STATUS_SIZE)				//在逻辑块中状态存放地址
#define	MAP_STATUS_PAGE_ADDR		(LOGIC_BLOCK_PAGE_NUM-1)						//在逻辑块中状态存放页地址 
#define SF_LOGIC_BLOCK_SIZE  		(MAP_STATUS_ADDR)								//安全存储数据页大小
#define LOGIC_BLOCK_STATUS_USED 	(0x22339876) 									//FLASH中BLOCK使用状态.
#define	BAD_BLOCK_STORE_PAGE_NUM	(1)												//坏块存放页数
#define	BAD_BLOCK_STORE_PAGE		(gSF_End_Block)									//坏块存放地址
#define RSVD_LOGIC_BLOCK_NUMS  		(2)												//预留逻辑块数，用于确保编程安全 
#define BAD_BLOCK_STORE_PAGE_FLAG 	(0x12349876) 									//坏块存放页标志.
#define BAD_BLOCK_STORE_PAGE_FLAG_ADDR 	(0x00) 										//坏块存放页标志地址.
#define MAX_BAD_BLOCK_STORE_NUMS  	(BAD_BLOCK_STORE_PAGE_NUM*FLASH_PAGE_SIZE/4-1)	//最大坏块记录数

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
	UINT32 lStatus;			//状态标志 
    UINT32 lLogicPage;		//((~逻辑页地址)<<16)|逻辑页地址
	UINT32 lLogicPageCHK;	//lLogicPage校验值
    UINT32 lPageWriteSeq;	//((~页写操作序号)<<16)|页写操作序号
	UINT64 lWriteSeq;		//写操作序号
	UINT64 lWriteSeqCHK;	//lSeq校验值
}STRUCT_MAP_STATUS; 	

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static UINT16 gSF_Start_Block, gSF_End_Block, gSF_Block_Nums, gSF_UsedBlock_Nums, gSF_BadBlock_Nums;
static UINT16 gSFMap[MAX_LOGIC_BLOCK_NUMS]; //save the block map
static UINT16 gSFStatus[MAX_LOGIC_BLOCK_NUMS];//save the block status
static UINT16 gSFPageWriteSeq[MAX_LOGIC_BLOCK_NUMS];//save the block write seq
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
/**************************************************************************
* Subroutine:	SF_MapMpuWrEnable
* Function:		Embedded flash write and erase protect enable or disable with proper check code.
* Input:		nOffsetAddr -- Offset address
* Output:		
* Description:
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_MapMpuWrEnable(UINT32 nOffsetAddr)
{
	if(nOffsetAddr<(EfcRegs.DATA-FLASH_BASE_ADDR))// EO or PROG area		   
	{
		//EfcRegs.CHK0 = AREA_PROG_WE;
		EfcRegs.CHK0 = AREA_PROT_DI;
		return RT_PARAM_ERR;
	}
	else if(nOffsetAddr<ASA_PARA) // DATA or KEY area							
	{
		EfcRegs.CHK0 = AREA_DATAPRO_WE;
		if(EfcRegs.STS.bit.Key_en && (nOffsetAddr>=ASA_KEY)) 	// KEY area enable and address in
			return RT_FAIL;	
	}
	else
	{
		EfcRegs.CHK0 = AREA_PROT_DI;
		return RT_PARAM_ERR;
	}
	return RT_OK;
}

/***************************************************************************
* Subroutine:	SF_MapIsBufAllOne
* Function:		Check buffer all 1 
* Input:	 	void *pData -- the buffer
*				UINT32 nSize -- the buffer size (word number)
* Output:		RT_OK
* Description:	 
* Date:			2013.05.14
* ModifyRecord:
* *************************************************************************/
//static UINT8 SF_MapIsBufAllOne(void *pData, UINT32 nSize)
//{
//	UINT32 i;
//	UINT32 *p = pData;
//	for(i=0;i<nSize;i++) 
//	{
//		if(p[i] != 0xFFFFFFFF) return RT_FAIL;
//	}
//	return RT_OK;
//}

/***************************************************************************
* Subroutine:	SF_ReadBlockData
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
static UINT8 SF_ReadBlockData(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{      
	if((nBlock>=gSF_Block_Nums)||((nOffset+nDataNum)>LOGIC_BLOCK_SIZE))
	    return RT_PARAM_ERR;
	    
    return EFC_ReadInfo(pDataBuf,((gSF_Start_Block+nBlock*LOGIC_BLOCK_PAGE_NUM)*FLASH_PAGE_SIZE+nOffset),nDataNum); 
}

/***************************************************************************
* Subroutine:	SF_WriteBlockData
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
static UINT8 SF_WriteBlockData(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{
	UINT32 j;
	UINT8 *pAddr;
	UINT8 *pTmp;
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	UINT32 lStartAddr,lBlockNum,lWriteLen;
	UINT32 lStartAddrTemp;
		
	if((nBlock>=gSF_Block_Nums)||((nOffset+nDataNum)>LOGIC_BLOCK_SIZE))
	    return RT_PARAM_ERR;	
 	
 	//计算要修改的页数 
 	if(nOffset==0)
 	   lBlockNum=(nDataNum+FLASH_PAGE_SIZE-1)/FLASH_PAGE_SIZE;
 	else
 	   lBlockNum=(nOffset+FLASH_PAGE_SIZE-1)/FLASH_PAGE_SIZE+(nDataNum-(FLASH_PAGE_SIZE-nOffset)+FLASH_PAGE_SIZE-1)/FLASH_PAGE_SIZE;
 	
 	if(lBlockNum>LOGIC_BLOCK_PAGE_NUM) 
 	    return RT_PARAM_ERR;  
	  
	lStartAddr = (gSF_Start_Block+nBlock*LOGIC_BLOCK_PAGE_NUM)*FLASH_PAGE_SIZE;	
      
    //2---写新数据  
    pAddr=pDataBuf;
	pTmp = (UINT8 *)Buf;
    for(j=0;j<lBlockNum;j++)
    {
		lStartAddrTemp = lStartAddr;
		if(SF_MapMpuWrEnable(lStartAddrTemp) != RT_OK)
			return RT_PARAM_ERR;
	 
		EFC_ReadInfo(pTmp, lStartAddrTemp, FLASH_PAGE_SIZE);
		//erase the block
		// 判断是否已擦除 2013.05.13 coins
		//if(SF_IsBufAllOne((void *)Buf, FLASH_PAGE_SIZE/4) != RT_OK)
		if(EFC_IsEraseOK(lStartAddrTemp, FLASH_PAGE_SIZE/4,0) != RT_OK)
		{
        	if(EFC_Erase(lStartAddrTemp,1) != RT_OK)
			{	
				EFC_WrProtect(AREA_PROT_DI);
				return RT_FAIL;
			}
		}
    	// copy data to write buffer
    	if((nDataNum+nOffset)>FLASH_PAGE_SIZE)
    	     lWriteLen=FLASH_PAGE_SIZE-nOffset;    	    
    	else
    	     lWriteLen=nDataNum;
    	     
        memcpy((pTmp+nOffset), pAddr, lWriteLen); 
    
        //write to flash
        if(EFC_PageProg(lStartAddrTemp, (UINT32 *)Buf) != RT_OK)
		{	
			EFC_WrProtect(AREA_PROT_DI);
			return RT_FAIL;
		}
		// next page
        lStartAddr+=FLASH_PAGE_SIZE;
        pAddr+=lWriteLen;
        nOffset=0;
        nDataNum-=lWriteLen;
    }
	
	EFC_WrProtect(AREA_PROT_DI); 
    return RT_OK;
}

#ifdef	REPLACE_WITH_BAD_BLOCK
/***************************************************************************
* Subroutine:	SF_MapBadBlockStorePageInit
* Function:		
* Input:			
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_MapBadBlockStorePageInit(void)
{
	UINT16 i;
	UINT32 lAddr;	
	UINT32 nData = BAD_BLOCK_STORE_PAGE_FLAG;
		
	lAddr = BAD_BLOCK_STORE_PAGE*FLASH_PAGE_SIZE;
	if(*((UINT32*)(FLASH_BASE_ADDR+lAddr+BAD_BLOCK_STORE_PAGE_FLAG_ADDR)) != nData)
	{
		for(i=0;i<BAD_BLOCK_STORE_PAGE_NUM;i++)
		{
			if(EFC_IsEraseOK(lAddr, FLASH_PAGE_SIZE/4,0) != RT_OK)
			{
				if(SF_MapMpuWrEnable(lAddr) != RT_OK)
					return RT_PARAM_ERR;
			   	if(EFC_Erase(lAddr,1) != RT_OK)		    //erase the block
				{
					EFC_WrProtect(AREA_PROT_DI);
			   	  	return RT_FAIL;
				}
			}
			lAddr += FLASH_PAGE_SIZE;		
		}
		lAddr = BAD_BLOCK_STORE_PAGE*FLASH_PAGE_SIZE + BAD_BLOCK_STORE_PAGE_FLAG_ADDR;
		if(SF_MapMpuWrEnable(lAddr) != RT_OK)
			return RT_PARAM_ERR;
		EFC_Program(&nData, lAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(nData == *(UINT32 *)(FLASH_BASE_ADDR+lAddr)) return RT_OK;
		else return RT_FAIL;
	}
    return RT_OK;
}

/***************************************************************************
* Subroutine:	SF_MapFindBadBlock
* Function:		
* Input:			
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_MapFindBadBlock(void)
{
	UINT16 i;
	UINT32 lAddr;	
	UINT32 nData = BAD_BLOCK_STORE_PAGE_FLAG;
	UINT32 nBadBlockData[MAX_BAD_BLOCK_STORE_NUMS];
	UINT32 nBadBlockTemp;

	gSF_BadBlock_Nums = 0;
	lAddr = BAD_BLOCK_STORE_PAGE*FLASH_PAGE_SIZE;
	if(*((UINT32*)(FLASH_BASE_ADDR+lAddr+BAD_BLOCK_STORE_PAGE_FLAG_ADDR)) == nData)
	{
		EFC_ReadInfo((UINT8*)nBadBlockData, (lAddr+4), MAX_BAD_BLOCK_STORE_NUMS*4);
		for(i=0; i<MAX_BAD_BLOCK_STORE_NUMS; i++)
		{
			if(nBadBlockData[i] != 0xffffffff)
			{
				nBadBlockTemp = (nBadBlockData[i]&0x0000FFFF);
				gSF_BadBlock_Nums++;
				if(((nBadBlockTemp^(nBadBlockData[i]>>16)) == 0x0000FFFF)&&(nBadBlockTemp<gSF_Block_Nums))
				{
					gSFStatus[nBadBlockTemp] = STATUS_BAD;
					gSFMap[nBadBlockTemp] = 0xffff;
					gSFPageWriteSeq[nBadBlockTemp]=0xffff;				
				}
			}
		}
	    return RT_OK;
	}
	else
    	return RT_FAIL;
}

/***************************************************************************
* Subroutine:	SF_MapMarkBadBlock
* Function:		
* Input:		UINT32 nBadBlock-- bad block	
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_MapMarkBadBlock(UINT32 nBadBlock)
{
	UINT16 i;
	UINT32 lAddr;	
	UINT32 nData = BAD_BLOCK_STORE_PAGE_FLAG;
	UINT16 SFBadBlockNums =	gSF_BadBlock_Nums+1;
	UINT32 nBadBlockData[MAX_BAD_BLOCK_STORE_NUMS];
	UINT32 nBadBlockTemp;

	gSFMap[nBadBlock]=0xffff;
	gSFStatus[nBadBlock]=STATUS_BAD;                			
	gSFPageWriteSeq[nBadBlock]=0xffff;	

	lAddr = BAD_BLOCK_STORE_PAGE*FLASH_PAGE_SIZE+BAD_BLOCK_STORE_PAGE_FLAG_ADDR;
	if(*((UINT32*)(FLASH_BASE_ADDR+lAddr)) == nData)
	{
		EFC_ReadInfo((UINT8*)nBadBlockData, (lAddr+4), MAX_BAD_BLOCK_STORE_NUMS*4);
		nBadBlockTemp = (nBadBlock |((~nBadBlock)<<16));
		for(i=0;i<gSF_BadBlock_Nums;i++)
		{
			if(nBadBlockData[i] == nBadBlockTemp)
				return RT_OK;			
		}
		if(SFBadBlockNums>MAX_BAD_BLOCK_STORE_NUMS)
		{
			gSF_BadBlock_Nums++;
			return RT_PARAM_ERR;
		}
		lAddr += SFBadBlockNums*4;
		if(SF_MapMpuWrEnable(lAddr) != RT_OK)
			return RT_PARAM_ERR;
		gSF_BadBlock_Nums++;
		if(*(UINT32 *)(FLASH_BASE_ADDR+lAddr) == 0xFFFFFFFF)
		{
			EFC_Program(&nBadBlockTemp, lAddr, 1);
			EFC_WrProtect(AREA_PROT_DI);
			if(nBadBlockTemp == *(UINT32 *)(FLASH_BASE_ADDR+lAddr)) return RT_OK;
			else return RT_FAIL;
		}
		else 
			return RT_FAIL;
	}
    return RT_FAIL;
}
#endif

/***************************************************************************
* Subroutine:	SF_EraseBlock
* Function:		
* Input:		UINT32 nBlock- the block num will Erase	
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_EraseBlock(UINT32 nBlock)
{
	UINT16 i;
	UINT32 lAddr;
	    
	if(nBlock>=gSF_Block_Nums)
		return RT_PARAM_ERR;	
	
	lAddr = (gSF_Start_Block+nBlock*LOGIC_BLOCK_PAGE_NUM)*FLASH_PAGE_SIZE;
	for(i=0;i<LOGIC_BLOCK_PAGE_NUM;i++)
	{
		if(EFC_IsEraseOK(lAddr, FLASH_PAGE_SIZE/4,0) != RT_OK)
		{
			if(SF_MapMpuWrEnable(lAddr) != RT_OK)
				return RT_PARAM_ERR;
		   	if(EFC_Erase(lAddr,1) != RT_OK)		    //erase the block
			{
				EFC_WrProtect(AREA_PROT_DI);
		   	  	return RT_FAIL;
			}
		}
		lAddr += FLASH_PAGE_SIZE;		
	}
    return RT_OK;
}

/***************************************************************************
* Subroutine:	SF_EraseBlockMapStatus
* Function:		erase the map status of the block
* Input:		UINT32 nBlock- the block num will Erase	
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_EraseBlockMapStatus(UINT32 nBlock)
{
	UINT32 lAddr;
	    
	if(nBlock>=gSF_Block_Nums)
		return RT_PARAM_ERR;	
				
	lAddr = (gSF_Start_Block+nBlock*LOGIC_BLOCK_PAGE_NUM+MAP_STATUS_PAGE_ADDR)*FLASH_PAGE_SIZE;
	if(EFC_IsEraseOK(lAddr, FLASH_PAGE_SIZE/4,0) != RT_OK)
	{
		if(SF_MapMpuWrEnable(lAddr) != RT_OK)
			return RT_PARAM_ERR;
	   	if(EFC_Erase(lAddr,1) != RT_OK)		    //erase the map status of the block
		{
			gSFMap[nBlock]=0xffff;
			gSFStatus[nBlock]=STATUS_NOTUSED;                			
			gSFPageWriteSeq[nBlock]=0xffff;	
			EFC_WrProtect(AREA_PROT_DI);
			#ifdef	REPLACE_WITH_BAD_BLOCK
			SF_MapMarkBadBlock(nBlock);
			#endif
	   	  	return RT_FAIL;
		}
	}
	gSFMap[nBlock]=0xffff;
	gSFStatus[nBlock]=STATUS_NOTUSED;                			
	gSFPageWriteSeq[nBlock]=0xffff;		
    return RT_OK;
}

/***************************************************************************
* Subroutine:	SF_SetMapStatus
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
static UINT8 SF_SetMapStatus(UINT8* pDataBuf,UINT32 nOldPhyBlock,UINT32 NewnPhyBlock,UINT16 nLogicBlock)
{
    STRUCT_MAP_STATUS nMapStatus;

	memset((UINT8*)&nMapStatus,0xff,sizeof(nMapStatus));
   	if(nOldPhyBlock == 0xffff)
	{
//		gSFMap[NewnPhyBlock]=nLogicBlock;              			
		gSFPageWriteSeq[NewnPhyBlock]=0;
//		gSFStatus[nBlock]=STATUS_USED;  
	}
	else
	{
//		gSFMap[NewnPhyBlock]=nLogicBlock;              			
		gSFPageWriteSeq[NewnPhyBlock]=gSFPageWriteSeq[nOldPhyBlock]+1;
//		gSFStatus[nBlock]=STATUS_USED; 
	}

	nMapStatus.lLogicPage = ((~nLogicBlock)<<16)|nLogicBlock;
	nMapStatus.lLogicPageCHK = ~nMapStatus.lLogicPage;
	nMapStatus.lPageWriteSeq = ((~gSFPageWriteSeq[NewnPhyBlock])<<16)|gSFPageWriteSeq[NewnPhyBlock];
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
	nMapStatus.lStatus = LOGIC_BLOCK_STATUS_USED;
   	memcpy((UINT8 *)pDataBuf, (UINT8 *)&nMapStatus, sizeof(STRUCT_MAP_STATUS));		    			
    return RT_OK;
}
/***************************************************************************
* Subroutine:	SF_ReadDataInBlock
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
static UINT8 SF_ReadDataInBlock(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{
	UINT16 i;
	for(i=0;i<gSF_Block_Nums;i++)
	{  
		if(gSFMap[i]==nBlock)	//find the block in map table;
		{
			return SF_ReadBlockData(pDataBuf,i,nOffset,nDataNum);
	    }
	}
	memset(pDataBuf,0,nDataNum);	//not find logic block;
	return RT_FAIL;
}

/***************************************************************************
* Subroutine:	SF_FindBlock
* Function:		find a free block to write
* Input:		UINT32 nLogicBlock- the logic block will be to write;
*	   			UINT16* nFindPhyBlock- the logical block corresponding to the physical block;
*	   			UINT16* nFindFreePhyBlock- a free phy block to write;		
* Output:		RT_OK or RT_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static void SF_FindBlock(UINT32 nLogicBlock,UINT16* nFindPhyBlock,UINT16* nFindFreePhyBlock)
{
	UINT16 i;
	UINT16 nFind=0xffff;
	UINT16 nBlank=0xffff;

	for(i=0;i<gSF_Block_Nums;i++)
	{  
		if(gSFMap[i]==nLogicBlock)
		{
			nFind=i;
			break;
		}
	}
    
	//-----find a blank block;
	if(nFind != 0xffff)
	{
	    for(i=nFind;i<gSF_Block_Nums;i++)
	    {        
			if((gSFStatus[i]!=STATUS_USED)&&(gSFStatus[i]!=STATUS_BAD))
			{
			      nBlank=i;	
			      break;
			}
		}
	}        

	if(nBlank == 0xffff)
	{
	    for(i=nLogicBlock;i<gSF_Block_Nums;i++)
	    {        
			if((gSFStatus[i]!=STATUS_USED)&&(gSFStatus[i]!=STATUS_BAD))
			{
			      nBlank=i;	
			      break;
			}
		}
		if(nBlank == 0xffff)
		{
		    for(i=0;i<nLogicBlock;i++)
		    {        
				if((gSFStatus[i]!=STATUS_USED)&&(gSFStatus[i]!=STATUS_BAD))
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
* Subroutine:	SF_WriteDataInBlock
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
static UINT8 SF_WriteDataInBlock(UINT8* pDataBuf,UINT32 nBlock,UINT32 nOffset,UINT32 nDataNum)
{
	UINT16 nFind=0xffff;
	UINT16 nBlank=0xffff;
	UINT16 nFindNext=0xffff;
	UINT16 nBlankNext=0xffff;
	UINT16 nTempPhyBlock=0xffff;
	UINT8* pAddr;
	UINT8 pBuf[LOGIC_BLOCK_SIZE];
	UINT16 nFreeBlock = 1;
	UINT8 nRet;
				
	if(nBlock>=gSF_Block_Nums) return RT_PARAM_ERR;	
	pAddr=pBuf;	

	SF_FindBlock(nBlock,&nFind,&nBlank);
	 
	if(gWrite2BlockFlag == 1)
		SF_FindBlock((nBlock+1),&nFindNext,&nBlankNext);

	if(nFind!=0xffff)//
	{
		nFreeBlock = 0;
		if((gWrite2BlockFlag == 1)&&(nFindNext == 0xffff))
			nFreeBlock++;

		if((gSF_UsedBlock_Nums+RSVD_LOGIC_BLOCK_NUMS+nFreeBlock) > gSF_Block_Nums)
			return RT_PARAM_ERR;

		//read data from old page 
		SF_ReadBlockData(pAddr,nFind,0,LOGIC_BLOCK_SIZE);
		memcpy(pAddr+nOffset,pDataBuf,nDataNum);

		if(nBlank == 0xffff)
			nTempPhyBlock = nFind; //write back
		else
			nTempPhyBlock = nBlank;	//write to blank page
		//set block status;
		SF_SetMapStatus(pAddr+MAP_STATUS_ADDR,nFind,nTempPhyBlock,nBlock); 
		nRet = SF_WriteBlockData(pAddr,nTempPhyBlock,0,LOGIC_BLOCK_SIZE);
		if(nRet != RT_OK)
		{
			if(SF_EraseBlockMapStatus(nTempPhyBlock) != RT_FAIL)
			{
				#ifdef	REPLACE_WITH_BAD_BLOCK
				SF_MapMarkBadBlock(nTempPhyBlock);
				#endif
			}
			return nRet;
		}	
		gSFMap[nTempPhyBlock]=nBlock;        
		gSFStatus[nTempPhyBlock]=STATUS_USED;
		gEraseBlock1[gEraseBlockNum1++] = nTempPhyBlock;
		if((nFind != 0xffff)&&(nBlank != 0xffff))
		{
//			gSFMap[nFind]=0xffff;
//			gSFPageWriteSeq[nFind]=0;
//			gSFStatus[nFind]=STATUS_NOTUSED;
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
			if((gSF_UsedBlock_Nums+RSVD_LOGIC_BLOCK_NUMS+nFreeBlock) > gSF_Block_Nums)
				return RT_PARAM_ERR;
			//read data from old page 
			SF_ReadBlockData(pAddr,nBlank,0,LOGIC_BLOCK_SIZE);
			//set block status;
			memcpy(pAddr+nOffset,pDataBuf,nDataNum);
			SF_SetMapStatus(pAddr+MAP_STATUS_ADDR,nFind,nBlank,nBlock);       
			//write to blank page.
		    nRet = SF_WriteBlockData(pAddr,nBlank,0,LOGIC_BLOCK_SIZE);
			if(nRet != RT_OK)
			{
				if(SF_EraseBlockMapStatus(nBlank) != RT_FAIL)
				{
					#ifdef	REPLACE_WITH_BAD_BLOCK
					SF_MapMarkBadBlock(nBlank);
					#endif
				}
				return nRet;
			}			 
			gSFMap[nBlank]=nBlock;  
			gSFStatus[nBlank]=STATUS_USED;
			gEraseBlock1[gEraseBlockNum1++] = nTempPhyBlock;
			gSF_UsedBlock_Nums++;
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
* Subroutine:	SF_BuildMap
* Function:		build index map
* Input:		
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2016.07.04
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_BuildMap(void)
{
    UINT16 i,j,k=0;    
    STRUCT_MAP_STATUS nMapStatus;
	UINT32 nLogic;
	UINT32 nPageWriteSeq;
    UINT32 nBlockUsedNum;
	UINT64 nTempWriteSeq[4][2];
	UINT16 nTempPage[4][2]; 
	UINT16 nFlag = 0; 
	UINT64 nWriteSeq[MAX_LOGIC_BLOCK_NUMS];

	gSF_UsedBlock_Nums = 0;
	nBlockUsedNum=gSF_Block_Nums;          
    memset((UINT8*)&gSFMap,0xff,sizeof(gSFMap)); 
	memset((UINT8*)&gSFPageWriteSeq,0xff,sizeof(gSFPageWriteSeq));  
	memset((UINT8*)&nWriteSeq,0xff,sizeof(nWriteSeq));    
    memset((UINT8*)&gSFStatus,STATUS_NOTUSED,sizeof(gSFStatus)); 
	memset((UINT8*)&nTempWriteSeq,0xff,sizeof(nTempWriteSeq));    
    memset((UINT8*)&nTempPage,0xff,sizeof(nTempPage));
	   
	for(i=0;i<gSF_Block_Nums;i++)
	{
		SF_ReadBlockData((UINT8*)&nMapStatus,i,MAP_STATUS_ADDR,MAP_STATUS_SIZE); 
		if(nMapStatus.lStatus==LOGIC_BLOCK_STATUS_USED)// used
		{			
			if((nMapStatus.lWriteSeq^nMapStatus.lWriteSeqCHK) != ((UINT64)0xFFFFFFFFFFFFFFFF)) 
				continue;
			nPageWriteSeq = (nMapStatus.lPageWriteSeq&0x0000FFFF);
			if((nPageWriteSeq^(nMapStatus.lPageWriteSeq>>16)) != 0x0000FFFF)
				continue;
			nLogic = (nMapStatus.lLogicPage&0x0000FFFF);
			if(((nMapStatus.lLogicPage^nMapStatus.lLogicPageCHK) != 0xFFFFFFFF)||((nLogic^(nMapStatus.lLogicPage>>16)) != 0x0000FFFF))	
				continue;
			if(nLogic >= nBlockUsedNum)
			   	continue; 
			//set the map status
			gSFMap[i]=nLogic;//store the logic page
			gSFStatus[i]=STATUS_USED;                			
			gSFPageWriteSeq[i]=nPageWriteSeq;
			gSF_UsedBlock_Nums++;
			nWriteSeq[i]=nMapStatus.lWriteSeq;
			if(gWriteSeq<((nWriteSeq[i]<<1)>>1)) gWriteSeq = ((nWriteSeq[i]<<1)>>1);
			//find whether exist the same logic page
			for(j=0;j<i;j++) 
			{
				if(gSFMap[j] == nLogic)//find the same page
				{
					if(((UINT16)(gSFPageWriteSeq[i]+1)) == gSFPageWriteSeq[j])
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
							SF_EraseBlockMapStatus(i);
							gSF_UsedBlock_Nums--;
						}
					}
					else if(((UINT16)(gSFPageWriteSeq[j]+1)) == gSFPageWriteSeq[i])
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
							SF_EraseBlockMapStatus(j);
							gSF_UsedBlock_Nums--;
						}
					}
					else{}
////					if(gSFPageWriteSeq[i]<gSFPageWriteSeq[j])
////					{
////						if((gSFPageWriteSeq[i] == 0)&&(gSFPageWriteSeq[j] == 0xffff))
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
////								SF_EraseBlockMapStatus(j);
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
////								SF_EraseBlockMapStatus(i);
////						}
////					}
////					else(gSFPageWriteSeq[i]>gSFPageWriteSeq[j])
////					{
////						if((gSFPageWriteSeq[j] == 0)&&(gSFPageWriteSeq[i] == 0xffff))
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
////								SF_EraseBlockMapStatus(i);
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
////								SF_EraseBlockMapStatus(j);
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
			for(i=0;i<gSF_Block_Nums;i++)
			{
			 	if(gSFStatus[i]==STATUS_USED)
				{
					if((nTempPage[0][1] != i)&&(nTempPage[0][0] != i)&&(nWriteSeq[i] == nTempWriteSeq[0][1]))
					{
					 	if(((gSFMap[i]+1) == gSFMap[nTempPage[0][1]])||((gSFMap[nTempPage[0][1]]+1) == gSFMap[i]))
							nFlag++;
					}
				}
			}
			if(nFlag)
				SF_EraseBlockMapStatus(nTempPage[0][0]);
			else
				SF_EraseBlockMapStatus(nTempPage[0][1]);
			gSF_UsedBlock_Nums-=1;
		}
		else
		{
			if(nTempWriteSeq[0][1] == nTempWriteSeq[1][1])
			{
				SF_EraseBlockMapStatus(nTempPage[0][0]);
				SF_EraseBlockMapStatus(nTempPage[1][0]);
			}
			else
			{
				SF_EraseBlockMapStatus(nTempPage[0][1]);
				SF_EraseBlockMapStatus(nTempPage[1][1]);
			}
			gSF_UsedBlock_Nums-=2;
		}
	} 
	return RT_OK;
}
 
/***************************************************************************
* Subroutine:	SF_MapInit
* Function:		Init Safe flash block
* Input:		UINT16 nStartBlock --	the start block(page).
                UINT16 nEndBlock --	the end block(page).
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_MapInit(UINT16 nStartBlock, UINT16 nEndBlock)
{
	UINT8 bBadPage=0;
	UINT8 nRet=0;

	gSF_Start_Block = 0;
	gSF_End_Block = 0;
	gSF_Block_Nums = 0;
	gWriteSeq = 0;
	gSF_UsedBlock_Nums = 0;
	gSF_BadBlock_Nums = 0;
			
#ifdef	REPLACE_WITH_BAD_BLOCK
	bBadPage = BAD_BLOCK_STORE_PAGE_NUM;
#endif

	if(nStartBlock<ASA_PROG/FLASH_PAGE_SIZE) return RT_PARAM_ERR;
	if(nEndBlock>AREA_END_PARA_PAGE) return RT_PARAM_ERR;
	if(nEndBlock<nStartBlock) return RT_PARAM_ERR;  
    if((nEndBlock-nStartBlock)>AREA_END_PARA_PAGE || (nEndBlock-nStartBlock)<(LOGIC_BLOCK_PAGE_NUM+bBadPage))
    	return RT_PARAM_ERR; 

	gSF_Start_Block=nStartBlock;
    gSF_End_Block=nEndBlock;

#ifdef	REPLACE_WITH_BAD_BLOCK
	gSF_End_Block = nEndBlock - BAD_BLOCK_STORE_PAGE_NUM;
#endif
	gSF_Block_Nums=(gSF_End_Block-gSF_Start_Block)/LOGIC_BLOCK_PAGE_NUM;
   
 	//安全Flash存储区必须位于MPU的保护数据区空间
	if(EfcRegs.DATA>FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE) 
 		EfcRegs.DATA=FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE;
	SCM_SetCacheRegion0(FLASH_BASE_ADDR, EfcRegs.DATA);

	if(gSF_Block_Nums>1)
		SF_BuildMap();

#ifdef	REPLACE_WITH_BAD_BLOCK
	nRet = SF_MapBadBlockStorePageInit();   
	if(nRet != RT_OK) return nRet;
	nRet = SF_MapFindBadBlock();
	if(nRet != RT_OK) return nRet;
#endif
     
	return RT_OK;   
}

/***************************************************************************
* Subroutine:	SF_MapFormat
* Function:		to erase the SF flash
* Input:		
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	Before using SF_MapFormat, please call SF_MapInit 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_MapFormat(void)
{
	UINT16 i;
	UINT16 nStartBlock = gSF_Start_Block; 
	UINT16 nEndBlock = gSF_End_Block;
	UINT32 lAddr;

	if(((nStartBlock == 0)&&(nEndBlock == 0))||(nEndBlock<nStartBlock))
		return RT_PARAM_ERR; 

#ifdef	REPLACE_WITH_BAD_BLOCK
	nEndBlock += BAD_BLOCK_STORE_PAGE_NUM;
#endif

	for(i=nStartBlock;i<nEndBlock;i++)
	{					
		lAddr = i*FLASH_PAGE_SIZE;
		if(EFC_IsEraseOK(lAddr, FLASH_PAGE_SIZE/4,0) != RT_OK)
		{
			if(SF_MapMpuWrEnable(lAddr) != RT_OK)
				return RT_PARAM_ERR;
		   	if(EFC_Erase(lAddr,1) != RT_OK)		    
			{
				EFC_WrProtect(AREA_PROT_DI);
		   	  	return RT_FAIL;
			}
		}
	}
	return RT_OK;  	
} 

/***************************************************************************
* Subroutine:	SF_MapWriteData
* Function:		write data to save flash area.
* Input:		UINT8* pDataBuf --	the data to be written save flash area;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_MapWriteData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	UINT32 lWriteLen;
	UINT16 nNum;
	UINT16 nNumTemp = nDataNum;
	UINT16 nStartBlock,nStartOffset;
	UINT8 *pBuf;
	UINT8 nRet;
	UINT16 i;
	UINT16 nSFUsedBlockNumsTemp;
		
	if(nDataNum<=0)	return RT_PARAM_ERR;
	if((nOffset+nDataNum)>((gSF_Block_Nums-RSVD_LOGIC_BLOCK_NUMS)*SF_LOGIC_BLOCK_SIZE)) return RT_PARAM_ERR;
#ifdef	REPLACE_WITH_BAD_BLOCK
	if(gSF_BadBlock_Nums>MAX_BAD_BLOCK_STORE_NUMS) return RT_PARAM_ERR;
#endif

	pBuf=pDataBuf;

   	while(nNumTemp)
	{
		if(nNumTemp > SF_LOGIC_BLOCK_SIZE)
			nNum = SF_LOGIC_BLOCK_SIZE;
		else
			nNum = nNumTemp;
		nNumTemp-=nNum;		
		gEraseBlockNum = 0;
		gEraseBlockNum1 = 0;
		gWrite2BlockFlag = 0;
		nSFUsedBlockNumsTemp = gSF_UsedBlock_Nums;
		//loop to write data to flash
		do{
			nStartOffset=(UINT16)(nOffset%SF_LOGIC_BLOCK_SIZE);
			nStartBlock=(UINT16)(nOffset/SF_LOGIC_BLOCK_SIZE);
					 	
	    	if(nNum>(SF_LOGIC_BLOCK_SIZE-nStartOffset))
			{ 	
	    		lWriteLen=SF_LOGIC_BLOCK_SIZE-nStartOffset;
				gWrite2BlockFlag = 1;
			}
	        else
			{
	           	lWriteLen=nNum;
				if(gWrite2BlockFlag == 1)gWrite2BlockFlag = 2;
			}
	
	        nRet=SF_WriteDataInBlock(pBuf,nStartBlock,nStartOffset,lWriteLen);  	 
			if(nRet!=RT_OK) 
			{
				//erase new block
				for(i=0;i<gEraseBlockNum1;i++)
				{
					gSFMap[gEraseBlock1[i]]=0xffff;
					gSFPageWriteSeq[gEraseBlock1[i]]=0xffff;
					gSFStatus[gEraseBlock1[i]]=STATUS_NOTUSED;
					SF_EraseBlockMapStatus(gEraseBlock1[i]);
				}
				gSF_UsedBlock_Nums = nSFUsedBlockNumsTemp;
				return nRet; 
			} 
			    
			pBuf+=lWriteLen;
			nOffset+=lWriteLen;
			nNum-=lWriteLen;
					   
		}while(nNum>0);
		
		//erase old block
		for(i=0;i<gEraseBlockNum;i++)
		{
			gSFMap[gEraseBlock[i]]=0xffff;
			gSFPageWriteSeq[gEraseBlock[i]]=0xffff;
			gSFStatus[gEraseBlock[i]]=STATUS_NOTUSED;
			SF_EraseBlockMapStatus(gEraseBlock[i]);
		}
	}	    
	return RT_OK;
}
 
/***************************************************************************
* Subroutine:	SF_MapReadData
* Function:		read data from save flash area
* Input:		UINT8* pDataBuf -- 	the buffer to save data for read;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_MapReadData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	UINT8 nRet;
	UINT16 nStartBlock,nStartOffset; 
	UINT32 lReadLen;

	if((nOffset+nDataNum)>((gSF_Block_Nums-RSVD_LOGIC_BLOCK_NUMS)*SF_LOGIC_BLOCK_SIZE)) return RT_PARAM_ERR;

	while(nDataNum)
	{
		nStartOffset=(UINT16)(nOffset%SF_LOGIC_BLOCK_SIZE);
		nStartBlock=(UINT16)(nOffset/SF_LOGIC_BLOCK_SIZE);

		if(nDataNum>(SF_LOGIC_BLOCK_SIZE-nStartOffset)) 	
    		lReadLen=SF_LOGIC_BLOCK_SIZE-nStartOffset;
        else
           	lReadLen=nDataNum;

		nRet=SF_ReadDataInBlock(pDataBuf,nStartBlock,nStartOffset,lReadLen);
		if(nRet!=RT_OK) return nRet;
		nDataNum -= lReadLen;
		nOffset += lReadLen;
		pDataBuf += lReadLen;
	}	
	return RT_OK;
}

#endif
