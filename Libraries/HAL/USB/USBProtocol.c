/***************************************************************************
* File name    :	UsbProtocol.c
* Function     :	Do with USB interrupt
* Author       : 	howard
* Date         :	2012/08/12
* Version      :    v1.11
* Description  :     
* ModifyRecord :
****************************************************************************/
#include "AS569.h"

UINT16 gSysClkPar; 
UINT8 gUSBFlag = 0;
UINT8 gUSBSuspendMode = USB_SUSPENDED_8DIV_HFROSC;
UINT8 gUSBSuspendFlag = 0;
DoMyUSBSleepProc g_pDoMyUSBSleep=NULL;
UINT16 gIntrOut=0;
UINT16 gIntrIn=0; 
/***********************************************************************************
 * Subrountine:	USBDoWithPackage
 * Function:	Deal receive package from PC
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * 				支持Setup为中断方式，Bulk为轮询方式 
 * Date:		2013.03.28
 * ModifyRecord:
 **********************************************************************************/
void USB_DoWithPackage(void)
{ 
 
    UINT8 IntrUSB;
	UINT16 IntrIn;
	UINT16 IntrOut;
	
	UINT8 tmp = UsbRegs.INDEX; 
	// 临时保存，防止中断程序打断bulk传输导致index改变 	
	
    IntrUSB = UsbRegs.INTRUSB.all; 
	IntrIn  = UsbRegs.INTRIN.all;
	IntrOut = UsbRegs.INTROUT.all; 

	if(gUsbMode==USB_SETUP_INT_MODE)
	{
		if(IntrIn>0)
		   gIntrIn|=IntrIn;      
		 if(IntrOut>0)
		    gIntrOut|=IntrOut;
	}
    
	if(IntrUSB&M_INTR_SUSPEND)
    {
		if(gUSBSuspendMode == USB_SUSPENDED_8DIV_HFROSC)
		{
			gSysClkPar = USB_SUSPENDED | (UINT8)((ScmRegs.SYSCLKCTRL.bit.FclkSel)<<2) | ((ScmRegs.HFROSCCTRL.bit.HFROscGrade)<<6);
			SCM_SetSysClk((SYS_CLK)((ScmRegs.HFROSCCTRL.bit.HFROscGrade)<<6));	// run in HFROSC/8 system clock #endif
		}
		else if(gUSBSuspendMode == USB_SUSPENDED_32K)
		{
		   gUSBSuspendFlag = 1;
		}
		gUSBFlag = 1; 
    }  
	if(IntrUSB&M_INTR_RESUME)
    {
		if(gSysClkPar&USB_SUSPENDED)
		{
			gSysClkPar &= ~USB_SUSPENDED;
			SCM_SetSysClk((SYS_CLK)gSysClkPar);	// resume system clock 
		}
		gUSBFlag = 1; 
    }    
    
	if(IntrUSB&M_INTR_RESET) //reset
    {  
		if(gSysClkPar&USB_SUSPENDED)
		{
			gSysClkPar &= ~USB_SUSPENDED;
			SCM_SetSysClk((SYS_CLK)gSysClkPar);	// resume system clock 
		}
		USB_Reset(); 
		gUSBFlag = 1;   
    }
	if(IntrIn&M_INTR_EP0)// setup packet received
    {	     
        USB_SetupHandle(); 		      
    }
 
    if(gUsbMode!=USB_SETUP_INT_MODE)
    { 
	  	if(gUSBFlag == 1)
			gUSBFlag = 0;  
        if((IntrOut&M_INTR_EP1)||(IntrOut&M_INTR_EP2)||(IntrOut&M_INTR_EP3)||(gIntrOut&M_INTR_EP4))// OUT request received
        {            
        	USB_CBWHandle(); 
        }
    } 
    else
    {     			
	    UsbRegs.INDEX = tmp;
    }          
}

/***********************************************************************************
 * Subrountine:	USBBulkPoll
 * Function:	Deal bulk by polling mode
 * Input:		None
 * Output:		None
 * Return:		None
 * Description:	
 * 			 
 * Date:		2013.03.28
 * ModifyRecord:
 **********************************************************************************/
 void USB_BulkPoll(void)
 {
 	UINT32 status;

	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ(USB_IRQn); 	// disable SPI0 interrupt 
  	if(gUSBFlag == 1)
		gUSBFlag = 0; 
	NVIC->ISER[0] = status;		// recover interrupt status

	if(gIntrOut&M_INTR_EP1)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP1);	 	 
 		USB_CBWHandle(); 	 		 				 
 	}
	else if(gIntrOut&M_INTR_EP2)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP2);	 	 	 		 				 
 	}
	else if(gIntrOut&M_INTR_EP3)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP3);	 	 	 		 				 
 	}
	else if(gIntrOut&M_INTR_EP4)// OUT request received
 	{ 
	    gIntrOut&=(~M_INTR_EP4);	 	 	 		 				 
 	}
 }

/***********************************************************************************
* Subrountine:	USB_Handler
* Function:	    USB interrupt 
* Input:		None
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
void USB_Handler(void)
{
	 USB_DoWithPackage();
}
	
/***************************************************************************
* Subroutine:	InitUSBSleepCallBackFunc
* Function:		Init USB Sleep call back function and init USB suspend mode.
* Input:		bUSBSuspendMode- USB suspend mode.
*								#define	USB_SUSPENDED_DIS		  	0	
*								#define	USB_SUSPENDED_8DIV_HFROSC  	1	
*								#define	USB_SUSPENDED_32K 			2
*               pDoMyUSBSleep- the call back function for do with USB Sleep function.
* Output:		None;
* Description:	 
* Date:		2015.10.28
* ModifyRecord:
* *************************************************************************/  
void InitUSBSleepCallBackFunc(UINT8 bUSBSuspendMode,DoMyUSBSleepProc pDoMyUSBSleep)
{
	if(bUSBSuspendMode <= USB_SUSPENDED_32K)
	{
		gUSBSuspendMode = bUSBSuspendMode;
		if(bUSBSuspendMode == USB_SUSPENDED_32K)
			g_pDoMyUSBSleep = pDoMyUSBSleep;
	}
}

/***********************************************************************************
* Subrountine:	USB_DoSuspend
* Function:	    USB  
* Input:		None
* Output:		None
* Description:
* Date:		2015.10.28
* ModifyRecord:
**********************************************************************************/
void USB_DoSuspend(void)
{
	UINT32 status;

	if(gUSBSuspendFlag)
	{
		status = NVIC->ISER[0]; 	// Save interrupt status
		NVIC_DisableIRQ(USB_IRQn); 	// disable SPI0 interrupt 
		gUSBSuspendFlag = 0;
		NVIC->ISER[0] = status;		// recover interrupt status
		if(g_pDoMyUSBSleep != NULL)
			g_pDoMyUSBSleep();	
	}	 
}
