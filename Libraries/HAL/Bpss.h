/***************************************************************************************
* File name    :	Bpss.h
* Function     :	Header of Bpss.c
* Author       : 	hongjz
* Date         :	2017/05/24
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _BPSS_H_
#define _BPSS_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SCM_DECAP_INFO_PORT GPIOB
#define SCM_DECAP_INFO_BIT 	5
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	RES_OK = 0,		//命令成功
	RES_TIME_OUT,		//超时错误
	RES_HEAD_ERR,		//包头错误
	RES_END_FLAG_ERR,	//结束标志错误
	RES_CHECK_ERR,		//校验错误	
	RES_PARA_ERR,		//参数错误
}TSResType;

struct	BPSS_SMODECTRL0_BITS{
	UINT8	D0InfoEn:1;
	UINT8	D0SdEn:1;
	UINT8	D1InfoEn:1;
	UINT8	D1SdEn:1;
	UINT8	D2InfoEn:1;
	UINT8	D2SdEn:1;
	UINT8	D3InfoEn:1;
	UINT8	D3SdEn:1;
};
union	BPSS_SMODECTRL0_REG{
	UINT8	all;
	struct	BPSS_SMODECTRL0_BITS	bit;
};

struct	BPSS_SMODECTRL1_BITS{
	UINT8	D4InfoEn:1;
	UINT8	D4SdEn:1;
	UINT8	D5InfoEn:1;
	UINT8	D5SdEn:1;
	UINT8	Rsvd0:4;
};
union	BPSS_SMODECTRL1_REG{
	UINT8	all;
	struct	BPSS_SMODECTRL1_BITS	bit;
};

struct	BPSS_DMODECTRL_BITS{
	UINT8	D0InfoEn:1;
	UINT8	D0SdEn:1;
	UINT8	D1InfoEn:1;
	UINT8	D1SdEn:1;	
	UINT8	D2InfoEn:1;
	UINT8	D2SdEn:1;
	UINT8	Rsvd0:2;
};
union	BPSS_DMODECTRL_REG{
	UINT8	all;
	struct	BPSS_DMODECTRL_BITS	bit;
};

typedef struct _T_DECAP{
	UINT8	TbusDiv;
	UINT8	Intval;
	union	BPSS_SMODECTRL0_REG 	SMODECTRL0;
	union	BPSS_SMODECTRL1_REG 	SMODECTRL1;
	union	BPSS_DMODECTRL_REG 		DMODECTRL;	
}T_DECAP;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void	Bpss_Init(void);
void 	Bpss_Reset(void);
UINT8	Bpss_WriteSram(UINT32 nAddr,UINT8 *inBuf,UINT32 inLen);
UINT8	Bpss_ReadSram(UINT32 nAddr,UINT8 *outBuf,UINT32 outLen);
UINT8	Bpss_WriteRtc(UINT32 nCnt);
UINT8	Bpss_ReadRtc(UINT32 *nCCVR);
UINT8	Bpss_DecapSet(T_DECAP *DecapReg);
UINT32	Bpss_GetLibVersion(void);

#endif /*_BPSS_H_*/
