/***************************************************************************************
* File name    :	ECC.h
* Function     :	The header of ECC.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SM2_H_
#define _SM2_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "HardEcc.h"

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define MAX_BYTE_LEN 32 //ECC 公私钥数组的最大字节个数，支持的最大ECC算法长度为MAX_BYTE_LEN*8 
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	SM2_Success = ALG_SUCCESS,
	SM2_PARA_Err = ALG_PAEA_ERR,		//输入参数错误
	SM2_OVERTIME_Err = ALG_TIMEOVER,	//超时
	SM2_POINT_Err = ALG_POINT_ERR,		//点不在曲线上	
	SM2_VERIFY_Err = ALG_VERIFY_ERR,	//验签错
	SM2_OTHER_Err = ALG_OTHER_ERR,		//其它错误
}SM2_RT;

typedef struct _sm3Value
{
	UINT8*  Buf;
}SM3Value; 

typedef struct 
{		
	UINT8* PrivateKey;
}SM2_PriKey;

typedef struct 
{	
    UINT8* X;
	UINT8* Y;
}SM2_PubKey;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 InitCurveSM2(UINT32 nBits,Ep_Curve **EC);

UINT8 SM3_GetSignIDValue(SM3Value *pSm3v,SM2_PubKey* PubKey,UINT8 *pID,UINT32 IDLen);

UINT8 SM2_GetHashValue(SM2_PubKey* PubKey,SM3Value* sm3v,UINT8 *pMsg,UINT32 msgLen,UINT8 *pID,UINT32 IDLen);

UINT8 SM2_GenKey(SM2_PriKey *pPriKey,SM2_PubKey *pPubKey,UINT32 nBits,BOOL bPriKeyExist);

UINT8 SM2_CheckKey(SM2_PriKey *pPriKey, SM2_PubKey *pPubKey, UINT32 nBits);

UINT8 SM2_Sign(SM2_PriKey* PriKey,UINT8 *pHash,UINT32 HashLen,UINT8 *pResult,UINT32 *pResultLen);

UINT8 SM2_Verify(SM2_PubKey* PubKey,UINT8 *pHash, UINT32 HashLen,UINT8 *pResult,UINT32 ResultLen);

UINT8 SM2_KeyExc_Init(UINT32 *rA, Ep_Point *RA);

UINT8 SM2_KeyExc_Response(Ep_Point *RA, SM2_PriKey* sm2PriKeyB, SM2_PubKey* sm2PubKeyA,Ep_Point *RB, SM3Value *SB, SM3Value *S2,SM3Value* ZA,SM3Value* ZB,UINT8* pKey, UINT32 KLen);

UINT8 SM2_KeyExc_Spons(UINT32 *rA, Ep_Point *RA,Ep_Point *RB, SM3Value *SB,SM2_PriKey* sm2PriKeyA, SM2_PubKey* sm2PubKeyB,SM3Value *SA,SM3Value* ZA,SM3Value* ZB,UINT8* pKey, UINT32 KLen);

UINT8 SM2_Key_Exchange(SM2_PriKey *sm2TmpPriA , SM2_PubKey *sm2TmpPubA , SM2_PubKey  *sm2TmpPubB ,SM2_PriKey *sm2PriKeyA , SM2_PubKey *sm2PubKeyB ,UINT8 ZA[32], UINT8 ZB[32],UINT8 KeyLen, UINT8 *pKey);

UINT8 SM2_PubKeyEncrypt_Old(SM2_PubKey* PubKey, UINT8 *Buf, UINT32 *pBufLen, UINT8 *pM, UINT32 nMLen);

UINT8 SM2_PriKeyDecrypt_Old(SM2_PriKey* PriKey,UINT8   *Buf,UINT32  *pBufLen, UINT8   *pEM, UINT32  nEMLen);

UINT8 SM2_PubKeyEncrypt(SM2_PubKey* PubKey,UINT8 *Buf, UINT32 *pBufLen, UINT8 *pM, UINT32 nMLen);

UINT8 SM2_PriKeyDecrypt(SM2_PriKey* PriKey,UINT8   *Buf, UINT32  *pBufLen, UINT8   *pEM, UINT32  nEMLen);

UINT8 SM2Check(void);

void Sm2Version(INT8 data[32]);

#endif
