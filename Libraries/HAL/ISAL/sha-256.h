/***************************************************************************************
* File name    :	SHA-256.h
* Function     :	The header of SHA-256.c
* Author       : 	Qiuj
* Date         :	2017/12/06
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA256_H_
#define _SHA256_H_
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SHA256_BLOCK_LEN_BYTE  64

/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
typedef enum 
{
	SHA256_Success = ALG_SUCCESS,
	SHA256_Para_Err = ALG_PAEA_ERR,
	SHA256_Timeover = ALG_TIMEOVER,
	SHA256_Err = ALG_FAIL,
}SHA256_RT;

typedef struct
{
	UINT8 	Buf[SHA256_BLOCK_LEN_BYTE];
	UINT32 	Count;
}SHA256_CONTEXT;
/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 SHA256_Init( SHA256_CONTEXT *hd);

UINT8 SHA256_Update( SHA256_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen );

UINT8 SHA256_Final( SHA256_CONTEXT *hd, UINT8 output[32] );

UINT8 SHA256(UINT8 *pcData,UINT32 nDataLen,UINT8 *pOut);

UINT8 HMAC_SHA256(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void SHA256Version(INT8 data[32]);  
 
#endif

