/***************************************************************************************
* File name    :	HardECC.h
* Function     :	The header of Hard.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _HARD_ECC_H_
#define _HARD_ECC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define MAX_DIG_LEN 	8
/**************************************************************************
* Global Type Definition
***************************************************************************/

/*仿射坐标下曲线上的点结构 ---> (x, y)*/
typedef struct{
	UINT32		x[MAX_DIG_LEN];
	UINT32		y[MAX_DIG_LEN];
}Ep_Point;

/*Jacobian投影坐标下曲线上的点结构 ---> (X, Y, Z)*/
typedef struct{
	UINT32		X[MAX_DIG_LEN];
	UINT32		Y[MAX_DIG_LEN];
	UINT32		Z[MAX_DIG_LEN];
}Ep_Point_J;


/* 曲线方程 : y^2 = x^3 + ax + b (mod p) */
typedef struct{
	UINT32 		len;					/*大素数  p 的NN_DIGIT单元数*/
	UINT32   	ecc_pm_p[8];
	UINT32   	ecc_pm_p0_inv[4];
	UINT32   	ecc_pm_a_M[8];
	UINT32   	ecc_pm_2_inv_M[8];
	UINT32   	ecc_pm_3_M[8];
	UINT32		p[MAX_DIG_LEN];			/* 大素数 p*/
	UINT32		a[MAX_DIG_LEN];			/* 系数 a, 通常设为  -3 或 p-3 */
	UINT32		b[MAX_DIG_LEN];			/* 系数 b */
	UINT32      inv[MAX_DIG_LEN];
	Ep_Point	G;						/* 基点 */
	/* 在第一类曲线中,  素数子群构成的基点的秩 r | p+1 and p mod 12 = 11 */
	UINT32		rlen;					/*基点的秩  r 的NN_DIGIT单元数*/
	UINT32		r[MAX_DIG_LEN];			/* 基点的秩  r  */
	UINT32		h;						/* h*r 为群的秩, 在第一类曲线中为 b (比如 Solinas 素数 2^a+2^b+1) */
}Ep_Curve;
#define B1_0	0x20008800 + 0x000
#define B1_1	0x20008800 + 0x020
#define B1_2	0x20008800 + 0x040
#define B1_3	0x20008800 + 0x060
#define B1_4	0x20008800 + 0x080
#define B1_5	0x20008800 + 0x0a0
#define B1_6	0x20008800 + 0x0c0
#define B1_7	0x20008800 + 0x0e0
#define B1_8	0x20008800 + 0x100
#define B1_9	0x20008800 + 0x120
#define B1_10	0x20008800 + 0x140
#define B1_11	0x20008800 + 0x160
#define B1_12	0x20008800 + 0x180
#define B1_13	0x20008800 + 0x1a0
#define B1_14	0x20008800 + 0x1c0
#define B1_15	0x20008800 + 0x1e0
#define B1_16	0x20008800 + 0x200
#define B1_17	0x20008800 + 0x220
#define B1_18	0x20008800 + 0x240
#define B1_19	0x20008800 + 0x260
#define B1_20	0x20008800 + 0x280
#define B1_21	0x20008800 + 0x2a0
#define B1_22	0x20008800 + 0x2c0
#define B1_23	0x20008800 + 0x2e0

#define B2_0	0x20008400 + 0x000
#define B2_1	0x20008400 + 0x020
#define B2_2	0x20008400 + 0x040
#define B2_3	0x20008400 + 0x060
#define B2_4	0x20008400 + 0x080
#define B2_5	0x20008400 + 0x0a0
#define B2_6	0x20008400 + 0x0c0
#define B2_7	0x20008400 + 0x0e0
#define B2_8	0x20008400 + 0x100
#define B2_9	0x20008400 + 0x120
#define B2_10	0x20008400 + 0x140
#define B2_11	0x20008400 + 0x160
#define B2_12	0x20008400 + 0x180
#define B2_13	0x20008400 + 0x1a0
#define B2_14	0x20008400 + 0x1c0
#define B2_15	0x20008400 + 0x1e0
#define B2_16	0x20008400 + 0x200
#define B2_17	0x20008400 + 0x220
#define B2_18	0x20008400 + 0x240
#define B2_19	0x20008400 + 0x260
#define B2_20	0x20008400 + 0x280
#define B2_21	0x20008400 + 0x2a0
#define B2_22	0x20008400 + 0x2c0
#define B2_23	0x20008400 + 0x2e0
#define B3_0	0x20008000 + 0x000
#define B3_1	0x20008000 + 0x020
#define B3_2	0x20008000 + 0x040
#define B3_3	0x20008000 + 0x060
#define B3_4	0x20008000 + 0x080
#define B3_5	0x20008000 + 0x0a0
#define B3_6	0x20008000 + 0x0c0
#define B3_7	0x20008000 + 0x0e0
#define B3_8	0x20008000 + 0x100
#define B3_9	0x20008000 + 0x120
#define B3_10	0x20008000 + 0x140
#define B3_11	0x20008000 + 0x160
#define B3_12	0x20008000 + 0x180
#define B3_13	0x20008000 + 0x1a0
#define B3_14	0x20008000 + 0x1c0
#define B3_15	0x20008000 + 0x1e0
#define B3_16	0x20008000 + 0x200
#define B3_17	0x20008000 + 0x220
#define B3_18	0x20008000 + 0x240
#define B3_19	0x20008000 + 0x260
#define B3_20	0x20008000 + 0x280
#define B3_21	0x20008000 + 0x2a0
#define B3_22	0x20008000 + 0x2c0
#define B3_23	0x20008000 + 0x2e0
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

void Ep_Point_Zero(Ep_Curve* E, Ep_Point *P);
void Ep_Point_Zero_J(Ep_Curve* E, Ep_Point_J *P);
UINT32 Ep_Point_Is_Zero(Ep_Curve* E, Ep_Point *P);
UINT32 Ep_Point_Is_Zero_J(Ep_Curve* E, Ep_Point_J *JP); 
void Ep_Point_Copy(Ep_Curve* E, Ep_Point *P, Ep_Point *Q);
void Ep_Point_Copy_J(Ep_Curve *E, Ep_Point_J *JP, Ep_Point_J *JQ);
UINT32 Ep_Is_On_Curve(Ep_Curve* E, Ep_Point *P);

UINT8 Affine_to_Jacobian(Ep_Curve *E, Ep_Point_J *P, Ep_Point *Q);
UINT8 Affine_to_Jacobian_Safety(Ep_Curve *E, Ep_Point_J *P, Ep_Point *Q); 
UINT8 Jacobian_to_Montgomery(Ep_Curve *E, Ep_Point_J *JQ, Ep_Point_J *JP);
UINT8 Montgomery_to_Jacobian(Ep_Curve *E, Ep_Point_J *JQ, Ep_Point_J *JP);
UINT8 Jacobian_to_Affine(Ep_Curve *E, Ep_Point *Q, Ep_Point_J *JP);
UINT32 ECC_PreProcess(Ep_Curve* E,Ep_Point *P,Ep_Point_J *JQ);
UINT32 ECC_AfterProcess(Ep_Point *R);


UINT32 Ep_Point_Inv(Ep_Curve* E, Ep_Point *Q, Ep_Point *P);
UINT32 ECC_ModAdd(UINT32 *pR, UINT32 *pA, UINT32 *pB, UINT32 nABLen, UINT32 *pM, UINT32 nMLen);
UINT32 ECC_ModSub(UINT32 *pR, UINT32 *pA, UINT32 *pB, UINT32 nABLen, UINT32 *pM, UINT32 nMLen);


void ECC_Point_Init(Ep_Curve *E);
UINT8 ECC_Point_Add_J(Ep_Curve* E,Ep_Point_J *JR, Ep_Point_J *JQ, Ep_Point_J *JP);
UINT8 EP_Point_Add(Ep_Curve* E,Ep_Point *R, Ep_Point *Q, Ep_Point *P);
UINT8 ECC_Point_Double_J(Ep_Curve* E, Ep_Point_J *JR, Ep_Point_J *JP);
UINT8 EP_Point_Double(Ep_Curve* E,Ep_Point *R, Ep_Point *P);
UINT8 ECC_PointMult_J(Ep_Curve *E, Ep_Point_J *JR, Ep_Point_J *JP, UINT32 *ecc_pm_key, UINT32 klen);
UINT8 EP_PointMult(Ep_Curve *E,Ep_Point *R, Ep_Point *P,UINT32 *ecc_pm_key, UINT32 klen);
UINT8 ECC_comb_PreMul(Ep_Curve *E, Ep_Point_J *JR, Ep_Point_J *JP,UINT32 KeyBit,UINT32 BankNum);
UINT8 ECC_PointMult_comb_J(Ep_Curve *E, Ep_Point_J *JR, Ep_Point_J *JP, UINT32 *k, UINT32 klen);
UINT8 EP_PointMult_Comb(Ep_Curve *E,Ep_Point *R, Ep_Point *P,UINT32 *ecc_pm_key, UINT32 klen);
#endif
