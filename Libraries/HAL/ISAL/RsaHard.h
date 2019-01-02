/***************************************************************************************
* File name    :	RSAHard.h
* Function     :	The header of RsaHard.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _RSA_HARD_H_
#define _RSA_HARD_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

#define RSA_MODE_BIT	0
#define BM_MODE_BIT		6
#define PP_MODE_BIT		12
#define SM_MODE_BIT		17
typedef enum 
{	
	BigMult_MODE=0,		//常规大数乘法模式
	MonMult_MODE=1,		//蒙哥马利乘法模式
	PrePro_MODE=2,		//预处理模式
	SmallDiv_MODE=3,	//小数除法模式
	BigDiv_MODE=4,		//大数除法模式
	ModExp_MODE=5,		//模幂模式
	ModInv_MODE=6,		//ECC模逆模式
	PointAdd_MODE=7,	//ECC点加与点倍模式
	PointMult_MODE=8,	//ECC点乘模式
}RSA_MODE;

typedef enum 
{	
	RSA_CMP_EQUAL = 0,
	RSA_CMP_NOT_EQUAL = 1,
	RSA_CMP_MORE_THAN = 2,
	RSA_CMP_GREATER_OR_EQUAL=3,
}RSA_CMP_MODE;

typedef enum 
{	
	BM_MODE_0=0,		//C=A-B
	BM_MODE_1=1,		//C=C-B
	BM_MODE_2=2,		//C=A+B
	BM_MODE_3=3,		//C=C+B
	BM_MODE_5=5,		//C=C-A*B
	BM_MODE_6=6,		//C=A*B
	BM_MODE_7=7,		//C=C+A*B
}BM_MODE;

//只有RSA_mode选择PrePro模式时，才需要配置PP_mode寄存器域，在其它各种模式下可以忽略
typedef enum 
{	
	PP_CLR_MODE=0,		//清零，根据指定首地址和长度，连续向存储器写0
	PP_CNT_MODE=1,		//数零，分为两种情况，（1）当数据长度len_b=0时，返回此128'b中高位'0'的个数，即从最高位到第一个'1'之间'0'的个数；（2）当数据长度len_b>0时，返回此大数中高位非零word（128'b0）的个数；
	PP_SHIFT_MODE=2,	//移位，分为4种情况，（1）当位移量PP_shift=0时，执行copy操作，将数据由地址B复制到地址C；（2）当位移量PP_shift>0时，做移位操作，通过PP_direct设置移位方向，左移时可通过PP_ext设置是否向左扩展一个RSA word，右移时默认长度不变；
	PP_CMP_MODE=3,		//比较，根据指定首地址和长度，从高位开始比较两个大数据，比较结果通过RSA返回值寄存器（RSA_RETURN）中的PP_cmp域读取；
}PP_MODE;


typedef enum 
{	
	SM_MODE_1=1,		//只在做大数除法（BigDiv）时使用，被除数长度128位，除数长度64位，只返回商的低半部分
	SM_MODE_2=2,		//常规除法模式，即128/64位除法
}SM_MODE;
/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
/**************************************************************************
* Functon
***************************************************************************/

void AEA_Init(void);

UINT32 RSA_Start(void);
//#define  RSA_Start()  ((UINT8 (*)(void))(((UINT32)AlgSram.RsaStartCode)|1))()

UINT32 RSA_Add(UINT32 *c, UINT32 *a,UINT32 aDigits , UINT32 *b,UINT32 bDigits);

UINT32 RSA_Sub(UINT32 *c, UINT32 *a, UINT32 *b, UINT32 digits);

UINT32 RSA_Mult(UINT32 *c, UINT32 *a,UINT32 aDigits, UINT32 *b, UINT32 bDigits);

UINT32 RSA_Mod(UINT32 *a, UINT32 *b,UINT32 bDigits, UINT32 *c,UINT32 cDigits);
  
UINT32 RSA_ModMult(UINT32 *a, UINT32 *b, UINT32 bDigits,UINT32 *c,UINT32 cDigits, UINT32 *d, UINT32 dDigits);

UINT32 RSA_MonMult(UINT32 *d,UINT32 *a,UINT32 *b, UINT32 *n,UINT32 *inv,UINT32 digits);

UINT32 RSA_ModInv(UINT32 *a, UINT32 *b, UINT32 *c,UINT32 digits);

UINT32 RSA_ModExp(UINT32 *a, UINT32 *b, UINT32 bDigits,UINT32 *c, UINT32 cDigits, UINT32 *d, UINT32 dDigits);

UINT32 RSA_ModExpSafety(UINT32 *a, UINT32 *b, UINT32 bDigits,UINT32 *c, UINT32 cDigits, UINT32 *d, UINT32 dDigits);

UINT32 RSA_ModExpSign(UINT32 *a, UINT32 *b, UINT32 bDigits,UINT32 *c, UINT32 cDigits,UINT32 *dn, UINT32 *d, UINT32 dDigits);
 
UINT32 RSA_Gcd(UINT32 *a ,UINT32 *b ,UINT32 bDigits,UINT32 *c,UINT32 cDigits);

UINT32 RSA_CMP(UINT32 DataA,UINT32 DataB,UINT32 DataC,UINT32 DataD,UINT32 Condition,UINT32 *CmpRes);

UINT32 RSA_TotalValidBit (UINT8 *a, UINT32 digits);

INT32 RSA_CmpBit32 (UINT32 *a, UINT32 *b,UINT32 digits);

INT32 RSA_Zero (UINT32 *a,UINT32 digits);

#endif



