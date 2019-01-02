/***************************************************************************************
* File name    :	CtrlUsb.h
* Function     :	The header of CtrlUsb.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _CTRLUSB_H_
#define _CTRLUSB_H_

/*************************************************************************************
 * Constant	Macro-Definition													 	 *
 ************************************************************************************/
typedef struct _SetupPack
{
	UINT8	bmRequestType;
	UINT8	bRequest;
	UINT16	wValue ;
	UINT16	wIndex ;
	UINT16	wLength ;
}SETUPPACK, *PSETUPPACK;

typedef struct _SetupStatus
{
	UINT8 bRemoteWakeup;
	UINT8 bConfigNum;
	UINT8 bInterfaceNum;
	UINT8 bEpHalt[9];
}SETUPSTATUS, *PSETUPSTATUS; 

 
extern SETUPSTATUS gSetupStatus;

/*************************************************************************************
 * System Fuctions Declaration														 *
 ************************************************************************************/
void USB_SetupHandle(void);

#endif
