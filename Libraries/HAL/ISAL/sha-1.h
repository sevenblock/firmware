/***************************************************************************************
* File name    :	SHA-1.h
* Function     :	The header of SHA-1.c
* Author       : 	Qiuj
* Date         :	2017/12/06
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA1_H_
#define _SHA1_H_
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SHA1_BLOCK_LEN_BYTE  64

/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
typedef enum 
{
	SHA1_Success = ALG_SUCCESS,
	SHA1_Para_Err = ALG_PAEA_ERR,
	SHA1_Timeover = ALG_TIMEOVER,
	SHA1_Err = ALG_FAIL,
}SHA1_RT;

typedef struct
{
	UINT8 	Buf[SHA1_BLOCK_LEN_BYTE];
	UINT32 	Count;
}SHA1_CONTEXT;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 SHA1_Init( SHA1_CONTEXT *hd );

UINT8 SHA1_Update( SHA1_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen);

UINT8 SHA1_Final(SHA1_CONTEXT *hd,UINT8 output[20]);

UINT8 SHA1(UINT8 *pcData,UINT32 nDataLen,UINT8 *pOut);

UINT8 HMAC_SHA1(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);


void SHA1Version(INT8 data[32]);
 
#endif

