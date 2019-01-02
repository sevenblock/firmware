/***************************************************************************************
* File name    :	DES.h
* Function     :	The header of DES.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _DES_H_
#define _DES_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define DES_ENCRYPT 1
#define DES_DECRYPT 2

#define DES_CBC     1
#define DES_ECB     2
#define DES3_CBC    3
#define DES3_ECB    4
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	DES_Success = ALG_SUCCESS,
	DES_TIME_OUT = ALG_TIMEOVER,
	DES_PARA_ERR = ALG_PAEA_ERR,
	DES_CHECK_ERR = ALG_CHECK_ERR,
	DES_Err = ALG_FAIL,
}DES_RT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 DES_Init(UINT8* pKey,UINT32 nKeyLen);

UINT8 _Des_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen);

UINT8 DesHard(UINT8 nType,UINT8 nMode,UINT32 nAddr,UINT16 nDataLen);

UINT8 DES_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

UINT8 DES_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 DES3_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

UINT8 DES3_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 DES_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

UINT8 Des_Check(void);

void DesVersion(INT8 data[32]);
 
#endif
