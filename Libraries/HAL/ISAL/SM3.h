/***************************************************************************************
* File name    :	SM3.h
* Function     :	The header of SM3.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SMS3_H_
#define _SMS3_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define SM3_BLOCK_LEN_BYTE  64

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	SM3_Success = ALG_SUCCESS,
	SM3_Para_Err = ALG_PAEA_ERR,
	SM3_Timeover = ALG_TIMEOVER,
	SM3_Err = ALG_FAIL,
}SM3_RT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/


typedef struct
{
	UINT8 	Buf[SM3_BLOCK_LEN_BYTE];
	UINT32 	Count;
}SM3_CONTEXT;
/**************************************************************************
* Global Functon Declaration
***************************************************************************/

UINT8 SM3_Init( SM3_CONTEXT *hd );

UINT8 SM3_Update( SM3_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen);

UINT8 SM3_Final(SM3_CONTEXT *hd,UINT8 output[32]);

UINT8 SM3(UINT8 *pcData,UINT32 nDataLen,UINT8 *pOut);

void Sm3Version(INT8 data[32]);

#endif
