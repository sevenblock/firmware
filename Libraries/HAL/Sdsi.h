/***************************************************************************************
* File name    :	Sdsi.h
* Function     :	The header of Sdsi.c
* Author       : 	coins
* Date         :	2014/02/17
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef SDSI_H_
#define SDSI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#define	RAM_16K_USED				0
#define	RAM_8K_USED					1
#define	RAM_FILE_USED				2
#define	SDSI_RAM_USED_MODE		  RAM_16K_USED

#define	SDSI_FILE_FAST_MODE	
/**************************************************************************
* Global Macro Definition
***************************************************************************/																				
#define	SDSI_MSG_READ_STT		1
#define	SDSI_MSG_READ_END		2
#define	SDSI_MSG_WRITE_STT		4
#define	SDSI_MSG_WRITE_END		8

#define	SD_PHY_V1_00		0x100
#define	SD_PHY_V1_10		0x110
#define	SD_PHY_V2_00		0x200

#define	SD_CAPACITY_G(N)	((N)*0x200000)
#define	SD_CAPACITY_M(N)	((N)*0x800)
#define	SD_CAPACITY_K(N)	((N)*0x2)	

/**************************************************************************
* Global Type Definition
***************************************************************************/
// Descripter:	the function type defined for SDSI initializaiton call back
// Parameter: 	void *p -- reserved for future
// Return:		the result for operation
typedef	UINT8 (*FP_SDSI_INIT_CB)(void *p);

// Descripter:	the function type defined for SDSI initializaiton call back
// Parameter: 	void *p -- reserved for future
// Return:		the result for operation
typedef	UINT8 (*FP_SDSI_FILE_INIT_CB)(void *p);

// Descripter:	the function type defined for SDSI read/write operation call back
// Parameter: 	UINT32 nAddr -- the block address 
//				UINT8 nBuffBank -- the buffer bank for data read/write
//				UINT32 nSize -- the size unit in block
// Return:		the result for operation
typedef UINT8 (*FP_SDSI_RW_CB)(UINT32 nAddr, UINT8 nBuffBank, UINT32 nSize);

// Descripter:	the function type defined for SDSI to send message when command received
// Parameter: 	UINT32 nAddr -- the start block address for the command multi-read or multi-write
//				UINT8 nMsg -- the buffer bank for data read/write
// Return:		the result for operation	
typedef	UINT8 (*FP_SDSI_RW_MSG)(UINT32 nAddr, UINT8 nMsg); 

// Descripter:	the function type defined for SDSI to erase operation call back
// Parameter: 	UINT32 nStartAddr -- the start block address for erase 
//				UINT32 nEndAddr -- the end block address for erase
// Return:		the result for operation
typedef	UINT8 (*FP_SDSI_ERASE_CB)(UINT32 nStartAddr, UINT32 nEndAddr);

#define	MAX_FILE_NUM	3

typedef	struct 
{
 	UINT32 nFileNum;
	UINT32 nFileAddr[MAX_FILE_NUM][2];					
}SDSI_FILE, *P_SDSI_FILE;

typedef	struct 
{
	UINT32 				nSDPhyVer;	  	// SD Physical Layer Specification Version, BCD type, such as 0x110 is Version 1.10
	UINT32 				nCapacity;		// the SD card capacity unit in block(512byte)
										// SD V1.0 and V1.1, the capacity must be less than 1GB, multiple of 256KB 
										// SD V2.0, the capacity must be  multiple of 512KB
	const UINT8 *		pCID;
	P_SDSI_FILE			pSdsiFile;
	FP_SDSI_INIT_CB 	fpSdsiInitCb;
	FP_SDSI_RW_CB 		fpSdsiReadCb;
	FP_SDSI_RW_CB 		fpSdsiWriteCb;
	FP_SDSI_RW_MSG		fpSdsiReadWriteMsg;
	FP_SDSI_ERASE_CB	fpSdsiEraseCb;
	FP_SDSI_FILE_INIT_CB fpSdsiFileInitCb;				
}SDSI_DEV, *P_SDSI_DEV;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 SDSI_Init(P_SDSI_DEV pSdsiDev);


/**************************************************************************
* Functon
***************************************************************************/

#endif	  /*SDSI_H_*/

