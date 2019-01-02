/***************************************************************************************
* File name    :	MD5.h
* Function     :	The header of MD5c.c
* Author       : 	Qiuj
* Date         :	2017/12/18
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _MD5_H_
#define _MD5_H_
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
typedef enum 
{
	MD5_Success = ALG_SUCCESS, 
	MD5_Err = ALG_FAIL,
}MD5_RT;
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef struct {
  UINT32 state[4];                                   /* state (ABCD) */
  UINT32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  UINT8 buffer[64];                         /* input buffer */
} MD5_CONTEXT;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

void MD5_Init(MD5_CONTEXT *context);

void MD5_Update(MD5_CONTEXT *context, UINT8 *pData, UINT32 lDataLen);

void MD5_Final(MD5_CONTEXT *context,UINT8 Result[16]);
 
void HMAC_MD5(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void MD5Version(INT8 data[32]); 

#endif /* _MD5_H_ */
