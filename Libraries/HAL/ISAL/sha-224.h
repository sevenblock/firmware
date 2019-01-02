/***************************************************************************************
* File name    :	SHA-256.h
* Function     :	The header of SHA-256.c
* Author       : 	Qiuj
* Date         :	2017/12/06
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA224_H_
#define _SHA224_H_
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SHA224_BLOCK_LEN_BYTE  64

/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
typedef enum 
{
	SHA224_Success = ALG_SUCCESS,
	SHA224_Para_Err = ALG_PAEA_ERR,
	SHA224_Timeover = ALG_TIMEOVER,
	SHA224_Err = ALG_FAIL,
}SHA224_RT;

typedef struct
{
	UINT8 	Buf[SHA224_BLOCK_LEN_BYTE];
	UINT32 	Count;
}SHA224_CONTEXT;
/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 SHA224_Init( SHA224_CONTEXT *hd);

UINT8 SHA224_Update( SHA224_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen );

UINT8 SHA224_Final( SHA224_CONTEXT *hd, UINT8 output[28] );

UINT8 SHA224(UINT8 *pcData,UINT32 nDataLen,UINT8 *pOut);

UINT8 HMAC_SHA224(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void SHA224Version(INT8 data[32]);  
 
#endif

