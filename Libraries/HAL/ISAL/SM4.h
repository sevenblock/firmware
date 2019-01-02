/***************************************************************************************
* File name    :	SM4.h
* Function     :	The header of SM4.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SMS4_H_
#define _SMS4_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SMS4_ENCRYPT 1
#define SMS4_DECRYPT 2
#define SMS4_CBC 1
#define SMS4_ECB 2
#define SMS4_CFB 3
#define SMS4_OFB 4

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	SM4_Success = ALG_SUCCESS,
	SM4_Para_Err = ALG_PAEA_ERR,     
	SM4_Timeover = ALG_TIMEOVER,
	SM4_Err = ALG_FAIL,	
}SM4_RT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 SMS4_Init(UINT8* pEK);

UINT8 _SMS4_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen);

UINT8 SMS4_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen);

UINT8 SMS4_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SMS4_Run_CFB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SMS4_Run_OFB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SMS4_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

void Sm4Version(INT8 data[32]);

#endif

