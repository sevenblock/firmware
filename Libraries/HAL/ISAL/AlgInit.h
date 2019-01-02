/***************************************************************************************
* File name    :	AlgInit.h
* Function     :	The header of AlgBase.c
* Author       : 	hongjz
* Date         :	2015/09/23
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef _ALG_INIT_H_
#define _ALG_INIT_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
//------- ALG_RT --------------
#define ALG_SUCCESS         0x00 //success
#define ALG_FAIL		    0x01 //fail
#define ALG_OTHER_ERR		0x02 //other reeor
#define ALG_PAEA_ERR        0x03 //param error
#define ALG_TIMEOVER	    0x04 //over time
#define ALG_CHECK_ERR		0x05 //ecc error
#define	ALG_POINT_ERR		0x06 //point not on the curve	
#define ALG_VERIFY_ERR		0x07 //verify error
#define ALG_SM2ZEROALL        	0x08 //SM2 Trng err
#define ALG_ADD_CARRY 		0x0E//addition carry
#define ALG_SUB_BORROW 		0x0F//subtraction borrow
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

UINT8 InitAlgSram(UINT32 *pBuf);


#endif
