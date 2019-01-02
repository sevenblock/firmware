/***************************************************************************************
* File name    :	SM1.h
* Function     :	The header of SM1.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SM1_H_
#define _SM1_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SM1_ENCRYPT 1
#define SM1_DECRYPT 2
#define SM1_CBC 1
#define SM1_ECB 2
#define SM1_CFB 3
#define SM1_OFB 4

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	SM1_Success = ALG_SUCCESS,  
	SM1_Para_Err = ALG_PAEA_ERR,     
	SM1_Timeover = ALG_TIMEOVER,
	SM1_Err = ALG_FAIL,	 
}SM1_RT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 SM1_Init(UINT8* pEK,UINT8* pAK);

UINT8 _SM1_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen);

UINT8 SM1_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen);

UINT8 SM1_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SM1_Run_CFB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SM1_Run_OFB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SM1_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

void Sm1Version(INT8 data[32]);

#endif
