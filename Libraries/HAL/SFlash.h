/***************************************************************************
* File name    :	SFlash.h
* Function     :	the header of SFlash.c
* Author       : 	Emil
* Date         :	2016/07/02
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
#ifndef _SFLASH_H_
#define _SFLASH_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SFLASH_BACKUP_MODE_USED		//			
#define SFLASH_MAP_MODE_USED		//		

#define SFLASH_BACKUP_MODE			(0) 		
#define SFLASH_MAP_MODE				(1)		 
#define SFLASH_INTERFACE_SEL		(SFLASH_BACKUP_MODE) //		 
/**************************************************************************
* Global Type Definition
***************************************************************************/
#if(SFLASH_INTERFACE_SEL == SFLASH_BACKUP_MODE)
	#ifdef	SFLASH_BACKUP_MODE_USED 
		#define SF_Init				SF_BkInit
		#define SF_WriteData		SF_BkWriteData
		#define SF_ReadData			SF_BkReadData
	#endif
	#ifndef	SFLASH_BACKUP_MODE_USED 
		#error "Undefined SF backup mode!"
	#endif
#elif(SFLASH_INTERFACE_SEL == SFLASH_MAP_MODE)
	#ifdef	SFLASH_MAP_MODE_USED 
		#define SF_Init				SF_MapInit
		#define SF_WriteData		SF_MapWriteData
		#define SF_ReadData			SF_MapReadData
	#endif
	#ifndef	SFLASH_MAP_MODE_USED 
		#error "Undefined SF map mode!"
	#endif
#else
	#error "Undefined SF interface!"
#endif
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/ 
#ifdef	SFLASH_BACKUP_MODE_USED 
UINT8 SF_BkInit(UINT16 nStartBlock,UINT16 nEndBlock);
UINT8 SF_BkWriteData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
UINT8 SF_BkReadData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
#endif

#ifdef	SFLASH_MAP_MODE_USED 
UINT8 SF_MapInit(UINT16 nStartBlock,UINT16 nEndBlock);
UINT8 SF_MapReadData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
UINT8 SF_MapWriteData(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
UINT8 SF_MapFormat(void);
#endif
/**************************************************************************
* Functon
***************************************************************************/

#endif
