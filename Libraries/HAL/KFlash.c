/**********************************************************************************
* File name    :	KFlash.c
* Function     :	The base function for Key area
* Author       : 	footman
* Date         :	2015/10/16
* Version      :    v1.0
* Description  :	
*   |<------- KF_Init:	             56 PAGE           	    ---------->|
*									|<--KF_BK_PAGE_NUM->|<-- 1_PAGE -->|
* 	--------------------------------------------------------------------
*	|*********数据存储区************|***写操作备份区****|**备份状态区**|
* 	-------------------------------------------------------------------------
*  /|\							   /|\				   /|\            /|\
*	|  								|					|              |
* KF_START_PAGE			  	KF_BK_START_PAGE     KF_BK_STATUS_PAGE	                
* 				            
* ModifyRecord :
* 1. 将STRUCT_BK_STATUS结构体成员lStatus移至单独备份状态区页，解决在写备份页时掉电存在的标志
*    已经写入，而其它参数未正确写入的隐患。
*	 2015/10/12	Emil
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"

/**********************************************************************************
* Local Macro Definition
**********************************************************************************/
#define KF_AREA_NUM				7			//密钥区子分区数
#define KF_BK_PAGE_NUM   		5 			//备份区页数
#define KF_BK_STATUS_PAGE_NUM   1 			//备份状态页数
#define KF_SUB_PAGE_NUM			8			//密钥子分区页数
#define KF_START_PAGE			960 		//密钥区起始页
#define	KF_BK_START_PAGE		(KF_START_PAGE+56-KF_BK_PAGE_NUM-KF_BK_STATUS_PAGE_NUM)		//备份区起始页
#define	KF_BK_STATUS_PAGE		(KF_BK_START_PAGE+KF_BK_PAGE_NUM)		//备份状态起始页
#define STATUS_BK 				0x22339876 		//说明是备份状态. 
#define	KF_PAGE_SIZE			FLASH_PAGE_SIZE	// 安全存储数据页大小（FLASH_PAGE_SIZE字节）
/**********************************************************************************
* Local Type Definition
**********************************************************************************/
typedef struct { 
	UINT32 lStatus;			//状态标志 
    UINT32 lStartPage;		//((~备份数据起始页地址)<<16)|备份数据起始页地址
	UINT32 lStartPageCHK;	//lStartPage校验值
	UINT32 lPageNum;		//((~备份的页数)<<16)|备份的页数
	UINT32 lPageNumCHK;		//lPageNum校验值 
}STRUCT_KF_BK_STATUS;

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
extern UINT32 nKeyPermission[7];
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
* Subroutine:	KF_WriteBKStatus
* Function:		Write KEY Flash data in backup status.
* Input:	 	UINT32 lStartPage -- the Save Flash data Start page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK RT_FAIL
* Description:	 
* Date:			2015.10.14
* ModifyRecord: 
* *************************************************************************/
static UINT8 KF_WriteBKStatus(UINT32 lStartPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	STRUCT_KF_BK_STATUS tBk;

	EFC_KEYWrProtect(nKeyPermission[(KF_BK_STATUS_PAGE-KF_START_PAGE)/KF_SUB_PAGE_NUM]);
	// 判断是否已擦除 2013.05.13 coins
	//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
	if(EFC_IsEraseOK(KF_BK_STATUS_PAGE*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4,0) != RT_OK)	
	{		
		if(EFC_Erase(KF_BK_STATUS_PAGE*FLASH_PAGE_SIZE,1) != RT_OK)
		{
			EFC_KEYWrProtect(AREA_PROT_DI);
			return RT_FAIL;
		}
	}
	tBk.lStartPage = ((~lStartPage)<<16)|lStartPage;
	tBk.lStartPageCHK = ~tBk.lStartPage;
	tBk.lPageNum = ((~lPageNum)<<16)|lPageNum;
	tBk.lPageNumCHK = ~tBk.lPageNum;
	tBk.lStatus = STATUS_BK;
   	memcpy((UINT8 *)pPageBuf, (UINT8 *)&tBk, sizeof(STRUCT_KF_BK_STATUS));
	if(EFC_PageProg(KF_BK_STATUS_PAGE*FLASH_PAGE_SIZE, pPageBuf) != RT_OK)
	{
		EFC_KEYWrProtect(AREA_PROT_DI);
		return RT_FAIL;	
	}
	EFC_KEYWrProtect(AREA_PROT_DI);				
	return RT_OK;
}

/***************************************************************************
* Subroutine:	KF_BackupPage
* Function:		Move several pages from Key area to backup area.
* Input:	 	UINT32 lSrcPage -- the source flash offset page number;
* 				UINT32 lDstPage --	the destination flash offset page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK.
* Description:	the destination pages and source pages must be not overlapped.	 
* Date:			2015.09.25
* ModifyRecord: 
* *************************************************************************/
static UINT8 KF_BackupPage(UINT32 lSrcPage, UINT32 lDstPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	UINT8 i;

	if((lDstPage<KF_BK_START_PAGE)||(lDstPage>=KF_BK_STATUS_PAGE)||((lDstPage+lPageNum)>KF_BK_STATUS_PAGE))
	   return RT_PARAM_ERR;

	for(i=0;i<lPageNum;i++)
	{
		EFC_ReadInfo((UINT8 *)pPageBuf,(lSrcPage+i)*FLASH_PAGE_SIZE, KF_PAGE_SIZE);
		EFC_KEYWrProtect(nKeyPermission[(lDstPage+i-KF_START_PAGE)/KF_SUB_PAGE_NUM]);	
		// 判断是否已擦除 2013.05.13 coins		
		//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
		if(EFC_IsEraseOK((lDstPage+i)*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4,0) != RT_OK)	
		{		
			if(EFC_Erase((lDstPage+i)*FLASH_PAGE_SIZE,1) != RT_OK)
			{
				EFC_KEYWrProtect(AREA_PROT_DI);		
				return RT_FAIL;
			}
		}
		if(EFC_PageProg((lDstPage+i)*FLASH_PAGE_SIZE, pPageBuf) != RT_OK)
		{
			EFC_KEYWrProtect(AREA_PROT_DI);	
			return RT_FAIL;
		}		
	}
	return KF_WriteBKStatus(lSrcPage, lPageNum, pPageBuf);	
}

/***************************************************************************
* Subroutine:	KF_RecoverPage
* Function:		Move several pages from backup area to data area.
* Input:	 	UINT32 lSrcPage -- the source flash page number;
*               UINT32 lDstPage --	the destination flash offset page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK.
* Description:	the destination pages and source pages must be not overlapped.	 
* Date:			2015.09.25
* ModifyRecord: 
* *************************************************************************/
static UINT8 KF_RecoverPage(UINT32 lSrcPage, UINT32 lDstPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	UINT8 i;

	if((lDstPage<KF_START_PAGE)||(lDstPage>=KF_BK_START_PAGE)||((lDstPage+lPageNum)>KF_BK_START_PAGE))
	   return RT_PARAM_ERR;

	for(i=0;i<lPageNum;i++)
	{
		EFC_ReadInfo((UINT8 *)pPageBuf,(lSrcPage+i)*FLASH_PAGE_SIZE, KF_PAGE_SIZE);	
		EFC_KEYWrProtect(nKeyPermission[(lDstPage+i-KF_START_PAGE)/KF_SUB_PAGE_NUM]);
		// 判断是否已擦除 2013.05.13 coins
		//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
		if(EFC_IsEraseOK((lDstPage+i)*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4,0) != RT_OK)	
		{	
			if(EFC_Erase((lDstPage+i)*FLASH_PAGE_SIZE,1) != RT_OK)
			{
				EFC_KEYWrProtect(AREA_PROT_DI);
				return RT_FAIL;
			}
		}
		if(EFC_PageProg((lDstPage+i)*FLASH_PAGE_SIZE, pPageBuf) != RT_OK)
		{
			EFC_KEYWrProtect(AREA_PROT_DI);
			return RT_FAIL;
		}	
	}
	EFC_KEYWrProtect(AREA_PROT_DI);	
	return RT_OK;
}

/***************************************************************************
* Subroutine:	KF_WriteBlocks
* Function:		Multi-page save write, the page number should be less than
*				or equal to KF_BK_PAGE_NUM. 
* Input:	 	UINT8 *pBuf -- the data to be written;
*             	UINT32 lStartPage -- the start page;
*             	UINT32 lOffset -- the offset in the page; 
*				UINT32 lLen -- the length to be written.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2015.09.25
* ModifyRecord:	
* *************************************************************************/
static UINT8 KF_WriteBlocks(UINT8 *pBuf, UINT32 lStartPage, UINT32 lOffset, UINT32 lLen)
{
	UINT32 j;
	UINT8 *pAddr;
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	UINT32 lStartAddr,lBlockNum,lWriteLen;  
	UINT8 nKeyAreaID;
	UINT8 *pTmp;

 	lStartAddr=lStartPage*FLASH_PAGE_SIZE;
 	
 	//计算要备份的页数 
 	if(lOffset==0)
 	   lBlockNum=(lLen+KF_PAGE_SIZE-1)/KF_PAGE_SIZE;
 	else
 	   lBlockNum=(lOffset+KF_PAGE_SIZE-1)/KF_PAGE_SIZE+(lLen-(KF_PAGE_SIZE-lOffset)+KF_PAGE_SIZE-1)/KF_PAGE_SIZE;
 	
 	if(lBlockNum>KF_BK_PAGE_NUM)	//跨过的区域不能超过备份区的页数 
 	    return RT_PARAM_ERR;
    
    if((lStartPage+lBlockNum)>KF_BK_START_PAGE)	// 不能超过.
 	    return RT_PARAM_ERR; 	
 	
 	//1---把数据搬移到备份区，并设置备份标志
	KF_BackupPage(lStartPage, KF_BK_START_PAGE, lBlockNum, (UINT32 *)Buf);
      
    //2---写新数据  
    pAddr=pBuf;
	pTmp = (UINT8 *)Buf;	
    for(j=0;j<lBlockNum;j++)
    {     	 
		EFC_ReadInfo(pTmp, lStartAddr, FLASH_PAGE_SIZE);
		nKeyAreaID = ((lStartAddr/FLASH_PAGE_SIZE)- KF_START_PAGE)/KF_SUB_PAGE_NUM;
		EFC_KEYWrProtect(nKeyPermission[nKeyAreaID]);
		if(EFC_IsEraseOK(lStartAddr, FLASH_PAGE_SIZE/4,0) != RT_OK)
        {
			if(EFC_Erase(lStartAddr,1) != RT_OK)
			{
				EFC_KEYWrProtect(AREA_PROT_DI);
				return RT_FAIL;
			}
		}
    	// copy data to write buffer
    	if((lLen+lOffset)>KF_PAGE_SIZE)
    	     lWriteLen=KF_PAGE_SIZE-lOffset;    	    
    	else
    	     lWriteLen=lLen;
    	     
        memcpy((pTmp+lOffset), pAddr, lWriteLen); 
        //write to flash
        if(EFC_PageProg(lStartAddr, (UINT32 *)Buf) != RT_OK)
		{
			EFC_KEYWrProtect(AREA_PROT_DI);
			return RT_FAIL;
		}
		// next page
        lStartAddr+=FLASH_PAGE_SIZE;
        pAddr+=lWriteLen;
        lOffset=0;
        lLen-=lWriteLen;
    }

    // 3--擦除备份页 	
	EFC_KEYWrProtect(nKeyPermission[(KF_BK_STATUS_PAGE-KF_START_PAGE)/KF_SUB_PAGE_NUM]);	
	if(EFC_Erase(KF_BK_STATUS_PAGE*FLASH_PAGE_SIZE,1) != RT_OK)
	{
		EFC_KEYWrProtect(AREA_PROT_DI);
		return RT_FAIL;
	}
	EFC_KEYWrProtect(AREA_PROT_DI); 
    return RT_OK;
}

/***************************************************************************
* Subroutine:	KF_Init
* Function:		Init key area. Check Key Flash data in backup status. when in back up, to recover data.
* Input:		
* Output:		RT_OK;
* Description:	 
* Date:			2015.09.25
* ModifyRecord:
* *************************************************************************/
UINT8 KF_Init(void)
{    
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	STRUCT_KF_BK_STATUS tBK; 
    UINT8 nRet = RT_OK;
	UINT32 lStartPage; 
	UINT32 lPageNum;
	// to read back up status from backup page
	EFC_ReadInfo((UINT8*)&tBK,KF_BK_STATUS_PAGE*FLASH_PAGE_SIZE,sizeof(tBK));	
	if(((tBK.lStartPage^tBK.lStartPageCHK) == 0xFFFFFFFF)&&((tBK.lPageNum^tBK.lPageNumCHK) == 0xFFFFFFFF)&&(tBK.lStatus == STATUS_BK))	// the page have been backup, need to recover
	{
		lStartPage = (tBK.lStartPage&0x0000FFFF);
		lPageNum = (tBK.lPageNum&0x0000FFFF);
		if(((lStartPage^(tBK.lStartPage>>16)) == 0x0000FFFF)&&((lPageNum^(tBK.lPageNum>>16)) == 0x0000FFFF))
		{
	        EFC_WrProtect(AREA_KEY_WE);
			nRet = KF_RecoverPage(KF_BK_START_PAGE, lStartPage, lPageNum, Buf);	// copy back from backup pages					 			
			EFC_WrProtect(AREA_PROT_DI);
			if(nRet != RT_OK)
				return RT_FAIL;
		}	      	
	}
	// erase backup Status page	
	if(EFC_IsEraseOK(KF_BK_STATUS_PAGE*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4,0) != RT_OK)
	{
		EFC_WrProtect(AREA_KEY_WE);
		EFC_KEYWrProtect(nKeyPermission[(KF_BK_STATUS_PAGE-KF_START_PAGE)/KF_SUB_PAGE_NUM]);
		nRet = EFC_Erase(KF_BK_STATUS_PAGE*FLASH_PAGE_SIZE,1);	// erase current backup page
		EFC_WrProtect(AREA_PROT_DI);
		EFC_KEYWrProtect(AREA_PROT_DI);
		if(nRet != RT_OK)
			return RT_FAIL;
	}				
	return RT_OK;	  
}

/***************************************************************************
* Subroutine:	KF_WriteArea
* Function:		Write data to key area
* Input:		EFC_AREA_KEY_ID nKeyAreaID	-- 	to specify the number of the KEY AREA ID
*				UINT8* pDataBuf --	the data to be written key flash area;
*				UINT32 nOffset --	the offset address in sub key flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK;
* Description:	 
* Date:			2015.09.25
* ModifyRecord:
* *************************************************************************/
UINT8 KF_WriteArea(EFC_AREA_KEY_ID nKeyAreaID, UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	UINT32 lWriteLen;
	UINT16 nNum;
	UINT16 nStartPage,nStartOffset;
	UINT8 *pBuf;
	UINT8 nRet;
	
	//check the data range;
	if(nDataNum<=0) return RT_PARAM_ERR;
	if(nKeyAreaID>EFC_AREA_KEY_3) return RT_PARAM_ERR;
	if(nOffset>0xFFF) return RT_PARAM_ERR;
	if((nOffset+nDataNum)>0x1000) return RT_PARAM_ERR;

	pBuf=pDataBuf;
	nNum=nDataNum;

	EFC_WrProtect(AREA_KEY_WE);
	//loop to write data to flash
	do{
		nStartOffset=(UINT16)(nOffset%KF_PAGE_SIZE);
		nStartPage=(UINT16)(KF_START_PAGE + nKeyAreaID*KF_SUB_PAGE_NUM + nOffset/KF_PAGE_SIZE);
		 	
    	if(nNum>(KF_BK_PAGE_NUM*KF_PAGE_SIZE-nStartOffset)) 
    	{
    		lWriteLen=KF_BK_PAGE_NUM*KF_PAGE_SIZE-nStartOffset;
    	}
        else lWriteLen=nNum;
   	
 		nRet=KF_WriteBlocks(pBuf, nStartPage, nStartOffset, lWriteLen);
		if(nRet!=RT_OK) 
		{
			EFC_WrProtect(AREA_PROT_DI); 
			return nRet;
		}       
		pBuf+=lWriteLen;
		nOffset+=lWriteLen;
		nNum-=lWriteLen;
		   
	}while(nNum>0);
	
	EFC_WrProtect(AREA_PROT_DI);   
	return RT_OK;
}
 
/***************************************************************************
* Subroutine:	KF_ReadData
* Function:		read data from key area
* Input:		UINT8* pDataBuf -- 	the buffer to save data for read;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be read.
* Output:		RT_OK;
* Description:	 
* Date:			2015.09.25
* ModifyRecord:
* *************************************************************************/
UINT8 KF_ReadData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	VUINT8* pAddr;
	UINT16 nStartPage,nStartOffset; 

	if((nOffset+nDataNum)>(KF_AREA_NUM*KF_SUB_PAGE_NUM*KF_PAGE_SIZE)) return RT_PARAM_ERR;

	while(nDataNum)
	{
		nStartOffset=(UINT16)(nOffset%KF_PAGE_SIZE);
		nStartPage=(UINT16)(KF_START_PAGE+nOffset/KF_PAGE_SIZE);
		pAddr = (VUINT8*)(FLASH_BASE_ADDR+nStartPage*FLASH_PAGE_SIZE+nStartOffset);
		if(nDataNum>KF_PAGE_SIZE-nStartOffset)
		{
			memcpy(pDataBuf,(void*)pAddr,KF_PAGE_SIZE-nStartOffset);
			nDataNum -= KF_PAGE_SIZE-nStartOffset;
			nOffset += KF_PAGE_SIZE-nStartOffset;
			pDataBuf += KF_PAGE_SIZE-nStartOffset;	
		}
		else
		{
			memcpy(pDataBuf,(void*)pAddr,nDataNum);
			nDataNum = 0;
		}
	}	
	return RT_OK;
}

/***************************************************************************
* Subroutine:	KF_WriteData
* Function:		Write data to key area
* Input:		UINT8* pDataBuf --	the data to be written key flash area;
*				UINT32 nOffset --	the offset address in key flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK;
* Description:	 
* Date:			2015.09.25
* ModifyRecord:
* *************************************************************************/
UINT8 KF_WriteData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	UINT32 lWriteLen;
	UINT16 nNum;
	UINT16 nStartBlock,nBlockOffset;

	UINT8 *pBuf;
	UINT8 nRet;
	
	//check the data range;
	if((nDataNum+nOffset) > KF_AREA_NUM*KF_SUB_PAGE_NUM*KF_PAGE_SIZE)	return RT_PARAM_ERR;

	pBuf=pDataBuf;
	nNum=nDataNum;
	EFC_WrProtect(AREA_KEY_WE);
	//loop to write data to flash
	do{
		nBlockOffset=(UINT16)(nOffset%KF_PAGE_SIZE);
		nStartBlock=(UINT16)(KF_START_PAGE + nOffset/KF_PAGE_SIZE);
		 	
    	if(nNum>(KF_BK_PAGE_NUM*KF_PAGE_SIZE-nBlockOffset)) 
    	{
    		lWriteLen=KF_BK_PAGE_NUM*KF_PAGE_SIZE-nBlockOffset;
    	}
        else lWriteLen=nNum;
   	
 		nRet=KF_WriteBlocks(pBuf,nStartBlock,nBlockOffset,lWriteLen);
		if(nRet!=RT_OK) 
		{
			EFC_WrProtect(AREA_PROT_DI);   
			return nRet;       
		}
		pBuf+=lWriteLen;
		nOffset+=lWriteLen;
		nNum-=lWriteLen;
		   
	}while(nNum>0);
	EFC_WrProtect(AREA_PROT_DI);      
	return RT_OK;
}


