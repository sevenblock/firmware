/***************************************************************************************
* File name    :	AES.h
* Function     :	The header of AES.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef AES_H_
#define AES_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/

#define AES_ENCRYPT 1
#define AES_DECRYPT 2 

#define AES128_CBC 1
#define AES128_ECB 2 
#define AES192_CBC 3
#define AES192_ECB 4
#define AES256_CBC 5
#define AES256_ECB 6

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	AES_Success = ALG_SUCCESS, 	
	AES_TIME_OUT = ALG_TIMEOVER,
	AES_PARA_ERR = ALG_PAEA_ERR,	
	AES_CHECK_ERR = ALG_CHECK_ERR,
	AES_Err = ALG_FAIL,
}AES_RT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 AES_Init(UINT8* pKey,UINT32 nKeyLen);

UINT8 _Aes_Run(UINT8 nType,UINT8 nMode,UINT32 nAddr,UINT16 nDataLen);

UINT8 AES_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

void AesVersion(INT8 data[32]);

#endif

