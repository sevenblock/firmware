/***************************************************************************************
* File name    :	Trng.h
* Function     :	The header of Trng.c
* Author       : 	Hongjz
* Date         :	2015/09/29
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _TRNG_H_
#define _TRNG_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	TRNG_Success = ALG_SUCCESS,			//成功
	TRNG_PARA_Err = ALG_PAEA_ERR,		//输入参数错误
	TRNG_OTHER_Err = ALG_OTHER_ERR,		//超时错误
}TRNG_RT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/

UINT32 Trng_Init(UINT32 nMode);
 
UINT32 Trng_GenRandom(UINT8 * pRandBuf, UINT16 nSize);
 
UINT32 Trng_GenRandomBit32(UINT32 * pRandBuf, UINT16 nSize);

UINT32 Trng_GenRandomBit32_Safety(UINT32 * pRandBuf, UINT16 nSize);

UINT32 Trng_GenRandomCheck(UINT8 * pRandBuf, UINT16 nSize);

void Prng_GenRandom(UINT16 *pRandBuf,UINT16 nSize);

extern	void TrngVersion(INT8 data[32]);

#endif

