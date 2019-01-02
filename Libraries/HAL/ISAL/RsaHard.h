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
	BigMult_MODE=0,		//��������˷�ģʽ
	MonMult_MODE=1,		//�ɸ������˷�ģʽ
	PrePro_MODE=2,		//Ԥ����ģʽ
	SmallDiv_MODE=3,	//С������ģʽ
	BigDiv_MODE=4,		//��������ģʽ
	ModExp_MODE=5,		//ģ��ģʽ
	ModInv_MODE=6,		//ECCģ��ģʽ
	PointAdd_MODE=7,	//ECC�����㱶ģʽ
	PointMult_MODE=8,	//ECC���ģʽ
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

//ֻ��RSA_modeѡ��PreProģʽʱ������Ҫ����PP_mode�Ĵ���������������ģʽ�¿��Ժ���
typedef enum 
{	
	PP_CLR_MODE=0,		//���㣬����ָ���׵�ַ�ͳ��ȣ�������洢��д0
	PP_CNT_MODE=1,		//���㣬��Ϊ�����������1�������ݳ���len_b=0ʱ�����ش�128'b�и�λ'0'�ĸ������������λ����һ��'1'֮��'0'�ĸ�������2�������ݳ���len_b>0ʱ�����ش˴����и�λ����word��128'b0���ĸ�����
	PP_SHIFT_MODE=2,	//��λ����Ϊ4���������1����λ����PP_shift=0ʱ��ִ��copy�������������ɵ�ַB���Ƶ���ַC����2����λ����PP_shift>0ʱ������λ������ͨ��PP_direct������λ��������ʱ��ͨ��PP_ext�����Ƿ�������չһ��RSA word������ʱĬ�ϳ��Ȳ��䣻
	PP_CMP_MODE=3,		//�Ƚϣ�����ָ���׵�ַ�ͳ��ȣ��Ӹ�λ��ʼ�Ƚ����������ݣ��ȽϽ��ͨ��RSA����ֵ�Ĵ�����RSA_RETURN���е�PP_cmp���ȡ��
}PP_MODE;


typedef enum 
{	
	SM_MODE_1=1,		//ֻ��������������BigDiv��ʱʹ�ã�����������128λ����������64λ��ֻ�����̵ĵͰ벿��
	SM_MODE_2=2,		//�������ģʽ����128/64λ����
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



