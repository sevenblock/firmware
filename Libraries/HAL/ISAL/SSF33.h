/***************************************************************************************
* File name    :	SSF33.h
* Function     :	The header of SSF33.c
* Author       : 	Qiuj
* Date         :	2017/07/25
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SSF33Alg_H_
#define _SSF33Alg_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SSF33_ENCRYPT 1
#define SSF33_DECRYPT 2 

#define SSF33_CBC 1
#define SSF33_ECB 2
#define SSF33_CFB 3
#define SSF33_OFB 4 
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	SSF33_Success = ALG_SUCCESS,
	SSF33_Para_Err = ALG_PAEA_ERR,
	SSF33_Err = ALG_FAIL,	
}SSF33_RT;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 SSF33_Init(UINT8* pEK);

UINT8 SSF33_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SSF33_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

UINT8 SSF33_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SSF33_Run_CFB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 SSF33_Run_OFB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

void SSF33Version(INT8 data[32]);
 
#endif
