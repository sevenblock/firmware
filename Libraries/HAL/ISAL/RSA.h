/***************************************************************************************
* File name    :	RSA.h
* Function     :	The header of RSA.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SYRSA_H_
#define _SYRSA_H_ 

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define  RSAPublicEncrypt  RSA_PublicEncrypt
#define  RSAPublicDecrypt  RSA_PublicDecrypt
#define  RSAPrivateEncrypt RSA_PrivateEncrypt
#define  RSAPrivateDecrypt RSA_PrivateDecrypt
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	RSA_Success = ALG_SUCCESS,
	RSA_PARA_Err = ALG_PAEA_ERR,
	RSA_TRNG_Err = ALG_TIMEOVER,
	RSA_OTHER_Err = ALG_OTHER_ERR,
}RSA_RT;

/* RSA prototype key. */
typedef struct{
	UINT32 bits;                // length in bits of modulus 
	UINT32 PublicExponent;      // public exponent 
}R_RSA_PROTO_KEY;
	
/* RSA public key. */	
	
typedef struct{
	UINT16 bits;				// length in bits of modulus
	UINT8* modulus;				// modules �൱��n
	UINT8* exponent;            // public exponent �൱��e 
}R_RSA_PUBLIC_KEY;

/* RSA private key. */	
typedef struct{
	UINT16 bits;               // length in bits of modulus
	UINT8* modulus;            // modulus   �൱��n
	UINT8* publicExponent;     // public exponent  �൱��e
	UINT8* exponent;           // private exponent �൱��d,�����CRTģʽ���㣬�˲������Դ�NULL,�Խ�ʡ��ջ�ռ�.
	UINT8* prime[2];           // prime factors   �൱��p,q
	UINT8* primeExponent[2];   // exponents for CRT �൱��dP,dQ
	UINT8* coefficient;        // CRT coefficient  �൱��qInv
	UINT8* dmodulus;           // dn
}R_RSA_PRIVATE_KEY,R_RSA_KEYPAIR;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/


UINT8 RSA_GenKeyPair(R_RSA_PROTO_KEY protoKey,R_RSA_PRIVATE_KEY *privateKey);

UINT8 RSA_PubKeyOpt(UINT8 *output,UINT32 *outputLen,UINT8 *input,UINT32 inputLen,R_RSA_PUBLIC_KEY *publicKey);

UINT8 RSA_PriKeyOpt_NoCRT(UINT8 *output,UINT32 *outputLen,UINT8 *input,UINT32 inputLen,R_RSA_PRIVATE_KEY *privateKey);

UINT8 RSA_PriKeyOpt(UINT8 *output,UINT32 *outputLen,UINT8 *input,UINT32 inputLen,R_RSA_PRIVATE_KEY *privateKey);

UINT8 RSA_PublicEncrypt(UINT8 *output,UINT32 *outputLen,UINT8 *input,UINT32 inputLen,R_RSA_PUBLIC_KEY *publicKey);

UINT8 RSA_PublicDecrypt(UINT8 *output,UINT32 *outputLen,UINT8 *input,UINT32 inputLen,R_RSA_PUBLIC_KEY *publicKey);

UINT8 RSA_PrivateEncrypt(UINT8 *output,UINT32 *outputLen,UINT8 *input,UINT32 inputLen,R_RSA_PRIVATE_KEY *privateKey);

UINT8 RSA_PrivateDecrypt(UINT8 *output,UINT32 *outputLen,UINT8 *input,UINT32 inputLen,R_RSA_PRIVATE_KEY *privateKey);

UINT8 RSA_Check(void);

void RsaVersion(INT8 data[32]);

#endif
