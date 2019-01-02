/***************************************************************************************
* File name    :	SHA-512.h
* Function     :	The header of SHA-512.c
* Author       : 	Qiuj
* Date         :	2017/12/06
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA4_H
#define _SHA4_H 
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
typedef enum 
{
	SHA512_Success = ALG_SUCCESS, 
	SHA512_Err = ALG_FAIL,
}SHA512_RT;
/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
typedef struct
{
    UINT64 total[2];    /*!< number of bytes processed  */
    UINT64 state[8];    /*!< intermediate digest state  */
    UINT8 buffer[128];  /*!< data block being processed */  
    INT32 is384;        /*!< 0 => SHA-512, else SHA-384 */
}SHA4_CONTEXT;
/**************************************************************************
* Global Functon Declaration
***************************************************************************/
 
void SHA4_Init( SHA4_CONTEXT *ctx, INT32 is384 );

void SHA4_Update( SHA4_CONTEXT *ctx, UINT8 *input, INT32 ilen );

void SHA4_Final( SHA4_CONTEXT *ctx, UINT8 output[64] );
 
void SHA384(UINT8 *pcData,UINT32 nDataLen,UINT8 *pOut);

void SHA512(UINT8 *pcData,UINT32 nDataLen,UINT8 *pOut);

void HMAC_SHA384(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void HMAC_SHA512(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void SHA4Version(INT8 data[32]); 

#endif 
