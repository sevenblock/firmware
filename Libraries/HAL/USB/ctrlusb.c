/***************************************************************************
* File name    :	CtrlUsb.c
* Function     :	Usb setup command 
* Author       : 	howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :     
* ModifyRecord :
****************************************************************************/
#include "AS569.h"
 

/***********************************************************************************
 * Subrountine: *StandardDeviceRequest[MAX_STD_REQUEST]
 * Function:	
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2013.03.28
 * ModifyRecord:
 **********************************************************************************/
void (* const StandardDeviceRequest[MAX_STD_REQUEST])(SETUPPACK* pSetupPack)=
{
	Chap9_GetStatus, 				//GET_STATUS
	Chap9_ClearFeature, 			//CLEAR_FEATURE
	Chap9_SendACK, 
	Chap9_SetFeature, 				//SET_FEATURE
	Chap9_SendACK, 
	Chap9_SetAddress, 				//SET_ADDRESS
	Chap9_GetDescriptor,			//GET_DESCRIPTOR
	Chap9_SendACK,    				//SET_DESCRIPTOR
	Chap9_GetConfig,    			//GET_CONFIGURATION
	Chap9_SetConfiguration,			//SET_CONFIGURATION
	Chap9_GetInterface,      		//GET_INTERFACE
	Chap9_SetInterface      		//SET_INTERFACE
};

/***********************************************************************************
 * Subrountine: USB_SetupHandle
 * Function:	do with setup package to enum usb device
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * Date:		2013.03.28
 * ModifyRecord:
 **********************************************************************************/
void USB_SetupHandle(void)
{
	UINT8 temp;
	SETUPPACK SetupPack;				//buffer to receive setup package 
    
    UsbRegs.INDEX=ENDPOINT0;   
    
    if(UsbRegs.OUTCOUNT<=0)
        return;
    
	USB_SetupReceive((UINT8*)&SetupPack);  
 
    temp = SetupPack.bmRequestType & USB_REQUEST_TYPE_MASK;	    
 	
    if(temp == USB_REQ_TYPE_STANDARD)     						// standard request type
    {
    	if(SetupPack.bRequest < MAX_STD_REQUEST)    			// handle 0--12
        {        
            (*StandardDeviceRequest[SetupPack.bRequest])(&SetupPack);           
        }
    }
    else if (temp == USB_REQ_TYPE_CLASS)  						// class request type
    {
        if(SetupPack.bRequest == GET_MAX_LUN)
        {
            Class_SendMaxLun(&SetupPack);
        }
    }
    else if(temp == USB_REQ_TYPE_VENDOR) 						//Vendor request
    {
    	;	
    }
    else
    	USB_SendACK(ENDPOINT0);
}

/*--------------------------- End of ctrlusb.c ---------------------------*/ 
 

 




