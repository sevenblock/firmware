/***************************************************************************************
* File name    :	ECC.h
* Function     :	The header of ECC.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef SECP256K1_H_
#define SECP256K1_H_ 
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
	SECP_Success = ALG_SUCCESS,
	SECP_PARA_Err = ALG_PAEA_ERR,		//输入参数错误	
	SECP_OVERTIME_Err = ALG_TIMEOVER,	//超时
	SECP_POINT_Err = ALG_POINT_ERR,		//点不在曲线上		
	SECP_VERIFY_Err = ALG_VERIFY_ERR,	//验签错
	SECP_OTHER_Err = ALG_OTHER_ERR,		//其它错误
}SECP_RT;

typedef struct 
{
	UINT32      nBits;		//算法位数，目前只支持256			
    UINT8*		PrivateKey;	 //私钥数据，32字节长度 
}  SECP_PriKey;

typedef struct 
{
	UINT32      nBits;		
    UINT8*		X; //公钥坐标X,32字节长度 
	UINT8*		Y; //公钥坐标Y，32字节长度 
}  SECP_PubKey; 
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/ 


UINT8 SECP_GenKey(SECP_PriKey *pPriKey, SECP_PubKey *pPubKey, UINT32 nBits, BOOL bPriKeyExist);

UINT8 SECP_Sign(SECP_PriKey* PriKey, UINT8 *pHash, UINT32 HashLen, UINT8 *pResult, UINT32 *pResultLen);

UINT8 SECP_Verify(SECP_PubKey* PubKey, UINT8 *pHash, UINT32 HashLen, UINT8 *pResult, UINT32 ResultLen);

UINT8 SECP_PubKeyEncrypt(SECP_PubKey* PubKey,UINT8 *Buf, UINT32 *pBufLen, UINT8 *pM, UINT32 nMLen);

UINT8 SECP_PriKeyDecrypt(SECP_PriKey* PriKey,UINT8 *Buf, UINT32 *pBufLen, UINT8 *pEM, UINT32 nEMLen);

UINT8 SECP_KeyExchange(SECP_PubKey* PubKey,SECP_PriKey* PriKey,UINT8 *pKey, UINT32 *pKeyLen);    

UINT8 InitCurveSecp(UINT32 nBits,Ep_Curve **EC);

void SecpVersion(INT8 data[32]);

#endif
