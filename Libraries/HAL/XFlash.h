/***************************************************************************
* File name    :	XFlash.h
* Function     :	Header of XFlash.c
* Author       : 	coins
* Date         :	2016/07/08
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef XFLASH_H_
#define XFLASH_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define XFLASH_BACKUP_MODE_USED		//			
#define XFLASH_MAP_MODE_USED		//		

#define XFLASH_BACKUP_MODE			(0) 		
#define XFLASH_MAP_MODE				(1)		 
#define XFLASH_INTERFACE_SEL		(XFLASH_BACKUP_MODE) //


#define	SQI_DRV			// if SQI interface is used, please open SQI_DRV

// define the external flash page size (bytes per programable page) 
#ifndef	_XF_PAGE_SIZE_
#define	XF_PAGE_SIZE		256
#endif

// define the external flash sector size (bytes the minimum erasable sector) 
#ifndef	_XF_SECTOR_SIZE_
#define	XF_SECTOR_SIZE		(XF_PAGE_SIZE * 16)
#endif

// define the buffer size for read back (XF_PAGE_SIZE ~ XF_SECTOR_SIZE)
#ifndef	_XF_BUFF_SIZE_		
#define XF_BUFF_SIZE		(XF_PAGE_SIZE * 4)
#endif

// define the buffer size for read back (XF_PAGE_SIZE ~ XF_SECTOR_SIZE)
#ifndef	_XF_MAX_ADDR_		
#define XF_MAX_ADDR		(0x4000000)
#endif

#ifndef	_XF_MAX_ADDR_		
#define XFLASH_BASE_ADDR	(SQI_BASE_ADDR)
#endif
		 
/**************************************************************************
* Global Type Definition
***************************************************************************/
#if(XFLASH_INTERFACE_SEL == XFLASH_BACKUP_MODE)
	#ifdef	XFLASH_BACKUP_MODE_USED 
		#define XF_Init					XF_BkInit
		#define XF_WriteData			XF_BkWriteData
		#define XF_ReadData				XF_BkReadData
		#define XF_EraseSector			XF_BkEraseSector
		#define XF_Program				XF_BkProgram
	#endif
	#ifndef	XFLASH_BACKUP_MODE_USED 
		#error "Undefined XF backup mode!"
	#endif
#elif(XFLASH_INTERFACE_SEL == XFLASH_MAP_MODE)
	#ifdef	XFLASH_MAP_MODE_USED 
		#define XF_Init						XF_MapInit
		#define XF_WriteData				XF_MapWriteData
		#define XF_ReadData					XF_MapReadData
		#define XF_EraseSector				XF_MapEraseSector
		#define XF_Program					XF_MapProgram
	#endif
	#ifndef	XFLASH_MAP_MODE_USED 
		#error "Undefined XF map mode!"
	#endif
#else
	#error "Undefined XF interface!"
#endif
/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/ 
#ifdef	XFLASH_BACKUP_MODE_USED 
UINT8 XF_BkInit(UINT32 nCapacity);
UINT8 XF_BkEraseSector(UINT32 nStartAddr, UINT32 nSectNum);
UINT8 XF_BkProgram(UINT8 *pData, UINT32 nAddr, UINT32 nLen);
UINT8 XF_BkWriteData(UINT8* pDataBuf, UINT32 nAddr, UINT16 nLen);
UINT8 XF_BkReadData(UINT8* pDataBuf, UINT32 nAddr, UINT16 nLen);
#endif

#ifdef	XFLASH_MAP_MODE_USED
UINT8 XF_MapSetStrAddr(UINT32 nStartAddr); 
UINT8 XF_MapInit(UINT32 nCapacity);
UINT8 XF_MapEraseSector(UINT32 nStartAddr, UINT32 nSectNum);
UINT8 XF_MapProgram(UINT8 *pData, UINT32 nAddr, UINT32 nLen);
UINT8 XF_MapWriteData(UINT8* pDataBuf, UINT32 nAddr, UINT16 nLen);
UINT8 XF_MapReadData(UINT8* pDataBuf, UINT32 nAddr, UINT16 nLen);
UINT8 XF_MapFormat(void);
#endif
/**************************************************************************
* Functon
***************************************************************************/

#endif
