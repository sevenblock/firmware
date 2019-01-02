/***************************************************************************
* File name    :	UsbBase.c
* Function     :	Usb API for communation
* Author       : 	howard
* Date         :	2013.03.28
* Version      :    v1.1
* Description  :     
* ModifyRecord : 
****************************************************************************/
#include "AS569.h"

 
UINT8 gUsbMode;

extern UINT8 gSysClkPar; 
extern UINT16 gIntrIn;
extern UINT16 gIntrOut;
extern UINT8 gUSBFlag;


/***********************************************************************************
* Subrountine:	USB_EndPointReceive
* Function:	    receive data from pc By endpoint EP1~4 
* Input:		nEndPoint-Endpoint, pBuf-buffer to receive data,nLen-the length to receice;
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:    增加 IntrOut = UsbRegs.INTROUT.all; 清除中断。 
**********************************************************************************/
void USB_EndPointReceive(UINT8 nEndPoint,UINT8* pBuf,UINT16 nLen)
{	 
	UINT16 nTLen,nTotalLen,nRecLen;
	UINT16 nMaxPackLen;
	VUINT16 IntrOut;    	
	UINT32 status;
	VUINT8  bOutRdy;  
	UINT32	nErrWhile = 0;
	UINT16 nMIntrEP;
			    
	UsbRegs.INDEX=nEndPoint; 

	if(nEndPoint == ENDPOINT1)
		nMIntrEP = M_INTR_EP1;
	else if(nEndPoint == ENDPOINT2)
		nMIntrEP = M_INTR_EP2;
	else if(nEndPoint == ENDPOINT3)
		nMIntrEP = M_INTR_EP3;
	else if(nEndPoint == ENDPOINT4)
		nMIntrEP = M_INTR_EP4;
	else 
		return; 

	nMaxPackLen=FS_MAX_PACK_LEN;
	nTotalLen=nLen;
	nRecLen=0;  
 	    
	while(nTotalLen>0)
	{
		bOutRdy = !(UsbRegs.OUTCSRL.bit.OutPktRdy);
		if(bOutRdy)
		{
			//TimeOut;
			if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
			{
				if(gUSBFlag == 1)
					gUSBFlag = 0;
				return;
			}
			continue;
		}
		status = NVIC->ISER[0]; 	// Save interrupt status
		NVIC_DisableIRQ(USB_IRQn); 	// disable SPI0 interrupt  	   	   
	   	gIntrOut |= UsbRegs.INTROUT.all; //for clear out interrupt
//	   	gIntrOut |= IntrOut;
	   	gIntrOut &= (~nMIntrEP);
		NVIC->ISER[0] = status;		// recover interrupt status	
				   
		if(UsbRegs.OUTCOUNT<nMaxPackLen)
		     nTLen=UsbRegs.OUTCOUNT;
         else
             nTLen=nMaxPackLen;	             
       
	    if(nTotalLen<nTLen)
	         nTLen=nTotalLen;	        

     	 USB_ReadFIFO(nEndPoint,nTLen,pBuf+nRecLen); 	
  
        nRecLen+=nTLen;	
        nTotalLen-=nTLen;

        UsbRegs.OUTCSRL.bit.OutPktRdy=0; 
	}	
//   UsbRegs.INCSRU.bit.Mode=1; 
}
/***********************************************************************************
* Subrountine:	USB_EndPointSend
* Function:		send data to pc by Endpoint EP1~4 
* Input:		nEndPoint-Endpoint, pBuf-buffer to send data,nLen-the length to send;
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
void USB_EndPointSend (UINT8 nEndPoint,UINT8* pBuf,UINT32 nLen)
{
	UINT32 i;
	UINT32 nOff;
	UINT32 nMaxPackLen;	
	UINT32 nErrWhile = 0; 
	VUINT8 bInRdy;
	UINT8 bIntrIn;	
	UINT32 status;
	UINT16 nMIntrEP; 
	 
	nMaxPackLen=64; 	   
    UsbRegs.INDEX=nEndPoint; 
	if(nEndPoint == ENDPOINT1)
		nMIntrEP = M_INTR_EP1;
	else if(nEndPoint == ENDPOINT2)
		nMIntrEP = M_INTR_EP2;
	else if(nEndPoint == ENDPOINT3)
		nMIntrEP = M_INTR_EP3;
	else if(nEndPoint == ENDPOINT4)
		nMIntrEP = M_INTR_EP4;
	else 
		return;
 
	for(i=0;i<nLen/nMaxPackLen;i++)
	{	  
        if(i!=0)
        {   
		    if(gUsbMode==USB_SETUP_INT_MODE)
			{
				nErrWhile=0;				
				do				
				{
					//TimeOut;
					if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
					{
						if(gUSBFlag == 1)
							gUSBFlag = 0;
						return;
					}					
					bIntrIn = !(gIntrIn&nMIntrEP);
				}while(bIntrIn);

				status = NVIC->ISER[0]; 	// Save interrupt status
				NVIC_DisableIRQ(USB_IRQn); 	// disable SPI0 interrupt 	
				gIntrIn &= (~nMIntrEP);
				NVIC->ISER[0] = status;		// recover interrupt status				        	      
			}
			else
			{
				while(!(UsbRegs.INTRIN.all&nMIntrEP))
	 	        	__NOP();
			}
		} 
		
        USB_WriteFIFO(nEndPoint,nMaxPackLen,pBuf+i*nMaxPackLen);              			
      	UsbRegs.STAS.INCSRL.bit.InPktRdy=1;
		nErrWhile=0;
		do
      	{
			//TimeOut;
			if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
			{
				if(gUSBFlag == 1)
					gUSBFlag = 0;
				return;
			}
      		bInRdy = UsbRegs.STAS.INCSRL.bit.InPktRdy;
      	}while(bInRdy);      		
	}
	
	nOff=nLen%nMaxPackLen;
	
	if((nOff>0))
	{ 
	    if(i!=0)
        {        	      
			if(gUsbMode==USB_SETUP_INT_MODE)
			{
				nErrWhile=0;				
				do				
				{
					//TimeOut;
					if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
					{
						if(gUSBFlag == 1)
							gUSBFlag = 0;
						return;
					}					
					bIntrIn = !(gIntrIn&nMIntrEP);
				}while(bIntrIn);

				status = NVIC->ISER[0]; 	// Save interrupt status
				NVIC_DisableIRQ(USB_IRQn); 	// disable SPI0 interrupt 	
				gIntrIn &= (~nMIntrEP);
				NVIC->ISER[0] = status;		// recover interrupt status	
			}
			else
			{
				while(!(UsbRegs.INTRIN.all&nMIntrEP))
	 	        	__NOP();
			}
        }     
		status = NVIC->ISER[0]; 	// Save interrupt status
		NVIC_DisableIRQ(USB_IRQn); 	// disable SPI0 interrupt      
		gIntrIn &= (~nMIntrEP);
		NVIC->ISER[0] = status;		// recover interrupt status	
			
    	USB_WriteFIFO(nEndPoint,nOff,pBuf+i*nMaxPackLen);    	     
    	UsbRegs.STAS.INCSRL.bit.InPktRdy=1; 
		nErrWhile=0;
		do
      	{
			//TimeOut;
			if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
			{
				if(gUSBFlag == 1)
					gUSBFlag = 0;
				return;
			}
      		bInRdy = UsbRegs.STAS.INCSRL.bit.InPktRdy;
      	}while(bInRdy); 
	}
}

/***********************************************************************************
* Subrountine:	USB_SendZero
* Function: 	send zero  to pc
* Input:		nEndPoint-the endpoint to send
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/	
void USB_SendZero(UINT8 nEndPoint)
{
	UINT32 nErrWhile = 0; 
	VUINT8 bInRdy;

	if(nEndPoint==ENDPOINT0)
	{
		UsbRegs.INDEX=nEndPoint;
		UsbRegs.STAS.CSR0.bit.DataEnd=1;
		UsbRegs.STAS.CSR0.bit.InPktRdy=1; 
	}
    else
    {
	    UsbRegs.INDEX=nEndPoint;
    	UsbRegs.STAS.INCSRL.bit.InPktRdy=1;
		nErrWhile=0;
		do
      	{
			//TimeOut;
			if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
			{
				if(gUSBFlag == 1)
					gUSBFlag = 0;
				return;
			}
      		bInRdy = UsbRegs.STAS.INCSRL.bit.InPktRdy;
      	}while(bInRdy);      	
    }    
}

/***********************************************************************************
* Subrountine:	USB_RecvZero
* Function: 	Receive zero from pc
* Input:		nEndPoint-the endpoint to be receive
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/	
void USB_RecvZero(UINT8 nEndPoint)
{
	if(nEndPoint==ENDPOINT0)
	{
		UsbRegs.INDEX=nEndPoint;
		UsbRegs.STAS.CSR0.bit.ServicedOutPktRdy=1;
	}
    else
    {
	    UsbRegs.INDEX=nEndPoint;
    	UsbRegs.OUTCSRL.bit.OutPktRdy=0;    	
    }    
}

/***********************************************************************************
* Subrountine:	USB_SetupReceive
* Function:	    receive Setup package from pc By Ctrl endpoint EP0
* Input:		None
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
	
void USB_SetupReceive(UINT8 *pBuf)
{	 
	UsbRegs.INDEX=ENDPOINT0;
	
	//Check for SentStall 
	if (UsbRegs.STAS.CSR0.bit.SentStall) {
		UsbRegs.STAS.CSR0.bit.SendStall=0;
	}
	
	// Check for SetupEnd 
	if (UsbRegs.STAS.CSR0.bit.SetupEnd) {
		UsbRegs.STAS.CSR0.bit.ServicedSetupEnd=1;
	}
	
	if(UsbRegs.STAS.CSR0.bit.OutPktRdy)
	{
		USB_ReadFIFO(ENDPOINT0,8,pBuf);
		UsbRegs.STAS.CSR0.bit.ServicedOutPktRdy=1;
		UsbRegs.STAS.CSR0.bit.ServicedSetupEnd=1;	 
	} 
}
/***********************************************************************************
* Subrountine:	USB_CtrlReceive
* Function:	    receive data from pc By Bulk endpoint EP0 by polling mode
* Input:		pBuf-buffer to receive data,nLen-the length to receice;
* Output:		RT_OK-success
                RT_FAIL-fail.
* Description:
* Date:		2013.03.28
* ModifyRecord: added by howard
**********************************************************************************/
UINT8 USB_CtrlReceive(UINT8* pBuf,UINT16 Len)
{
	UINT16 nTLen,nTotalLen,nRecLen;
	UINT16 nMaxPackLen;
	VUINT16 IntrIn;
	UINT32 nErrWhile = 0;
	UINT8 bIntrIn;

	UsbRegs.INDEX=ENDPOINT0;

	nErrWhile=0;
    if(gUsbMode==USB_SETUP_INT_MODE)
	{
		do
		{
			//TimeOut;
			if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
			{
				if(gUSBFlag == 1)
					gUSBFlag = 0;
				return RT_FAIL;
			}

			IntrIn = UsbRegs.INTRIN.all;
			if(IntrIn>0)
				gIntrIn |= IntrIn;
			bIntrIn = !(IntrIn&M_INTR_EP0);
		}while(bIntrIn);
		gIntrIn &= (~M_INTR_EP0);
	}
	else
	{
		while(!(UsbRegs.INTRIN.all&M_INTR_EP0))
			__NOP();
	}

	nTotalLen=UsbRegs.OUTCOUNT;
	if(nTotalLen<=0) return RT_FAIL;//no data

	nTotalLen=Len;
    nMaxPackLen=CTRL_PACKET_LEN;
 
	nRecLen=0;

	while(nTotalLen>0)
	{
		if(UsbRegs.OUTCOUNT<nMaxPackLen)
			nTLen=UsbRegs.OUTCOUNT;
		else
			nTLen=nMaxPackLen;
		
		if(nTotalLen<nTLen)
			nTLen=nTotalLen;
		
		USB_ReadFIFO(ENDPOINT0,nTLen,pBuf+nRecLen);
		
		nRecLen+=nTLen;
		nTotalLen-=nTLen;
 		UsbRegs.STAS.CSR0.bit.ServicedOutPktRdy=1;
		nErrWhile=0;
	    if(gUsbMode==USB_SETUP_INT_MODE)
		{
			do
			{
				//TimeOut;
				if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
				{
					if(gUSBFlag == 1)
						gUSBFlag = 0;
					return RT_FAIL;
				}
				IntrIn = UsbRegs.INTRIN.all;
				if(IntrIn>0)
					gIntrIn |= IntrIn;
				bIntrIn = !(IntrIn&M_INTR_EP0);
			}while(bIntrIn);
			gIntrIn &= (~M_INTR_EP0);
		}
		else
		{
			while(!(UsbRegs.INTRIN.all&M_INTR_EP0))
				__NOP();
		}
	}
   	return RT_OK;
}
/***********************************************************************************
* Subrountine:	USB_CtrlSend
* Function:  	send data to pc by Ctrl Endpoint EP0
* Input:		pBuf-buffer to send data,nLen-the length to send;
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
void USB_CtrlSend (UINT8* pBuf,UINT16 nLen)
{
	UINT16 i;
	UINT16 nOff;
	UINT32 nErrWhile = 0;
	UINT8 bIntrIn;
	VUINT8 bInRdy;
	VUINT16 IntrIn;
			
	UsbRegs.INDEX=ENDPOINT0;    
 
	for(i=0;i<nLen/CTRL_PACKET_LEN;i++)
	{
	     if(i!=0)
	     {
		    if(gUsbMode==USB_SETUP_INT_MODE)
			{
				nErrWhile=0;
				do
				{
					//TimeOut;
					if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
					{
						if(gUSBFlag == 1)
							gUSBFlag = 0;
						return;
					}				
					IntrIn = UsbRegs.INTRIN.all;
					if(IntrIn>0)
						gIntrIn |= IntrIn;
					bIntrIn = !(IntrIn&M_INTR_EP0);
				}while(bIntrIn);
				gIntrIn &= (~M_INTR_EP0);
			}
			else
			{
				while(!(UsbRegs.INTRIN.all&M_INTR_EP0))
					__NOP();
			}
	     }	     
		 USB_WriteFIFO(ENDPOINT0,CTRL_PACKET_LEN,pBuf+i*CTRL_PACKET_LEN);
		 UsbRegs.STAS.CSR0.bit.InPktRdy=1;
//		 if(((nLen%CTRL_PACKET_LEN)==0) && (i==(nLen/CTRL_PACKET_LEN-1)))
//		     UsbRegs.STAS.CSR0.bit.DataEnd=1;
		nErrWhile=0;
		do
      	{
			//TimeOut;
			if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
			{
				if(gUSBFlag == 1)
					gUSBFlag = 0;
				return;
			}     		
			bInRdy = UsbRegs.STAS.CSR0.bit.InPktRdy;
      	}while(bInRdy);
	}
	
	nOff=nLen%CTRL_PACKET_LEN;
	
	if(nOff>0)
	{   
	    if(i!=0)
	    {
		    if(gUsbMode==USB_SETUP_INT_MODE)
			{
				nErrWhile=0;
				do
				{
					//TimeOut;
					if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
					{
						if(gUSBFlag == 1)
							gUSBFlag = 0;
						return;
					}					
					IntrIn = UsbRegs.INTRIN.all;
					if(IntrIn>0)
						gIntrIn |= IntrIn;
					bIntrIn = !(IntrIn&M_INTR_EP0);
				}while(bIntrIn);
				gIntrIn &= (~M_INTR_EP0);
			}
			else
			{
				while(!(UsbRegs.INTRIN.all&M_INTR_EP0))
					__NOP();
			}
	    }
		gIntrIn &= (~M_INTR_EP0);	        
		USB_WriteFIFO(ENDPOINT0,nOff,pBuf+i*CTRL_PACKET_LEN);		
        UsbRegs.STAS.CSR0.bit.DataEnd=1;
       	UsbRegs.STAS.CSR0.bit.InPktRdy=1; 
		nErrWhile=0;
		do
		{
			//TimeOut;
			if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
			{	
				if(gUSBFlag == 1)
					gUSBFlag = 0;
				return;
			}			
			bInRdy = UsbRegs.STAS.CSR0.bit.InPktRdy;
		}while(bInRdy);      
	} 
}
 
/*******************************************************************************
 * Subrountine:	USB_WriteFIFO
 * Function:	to write USB FIFO
 * Input:		UINT8 *pBuf --		to point the buffer will be transfered;
 * 				UINT8 nLen --		the length of the buffer
 * 									it should be less than the MAX_PACKET_SIZE.
 * Output:		None
 * Description:	
 * Date:		2013.03.28
 * ModifyRecord:
 *******************************************************************************/
void USB_WriteFIFO(UINT8 nEP,UINT32 nLen,UINT8 *pBuf)
{
    UINT32 i;
    VUINT8* pFIFO=0;   
  
   if(nEP==ENDPOINT0)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP0;
    else if(nEP==ENDPOINT1)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP1;
    else if(nEP==ENDPOINT2)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP2;
    else if(nEP==ENDPOINT3)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP3;  
    else if(nEP==ENDPOINT4)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP4; 
		    
	for(i=0;i<nLen;i++) 
          *((VUINT8*)pFIFO)=pBuf[i];	
}

/***********************************************************************************
* Subrountine:	USB_ReadFIFO
* Function: 	Read current ep fifo
* Input:		pBuf-Data buffer
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
void USB_ReadFIFO(UINT8 nEP,UINT32 nLen,UINT8 *pBuf)
{ 
    UINT32 i;
    VUINT8* pFIFO=0;
    
    if(nEP==ENDPOINT0)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP0;
    else if(nEP==ENDPOINT1)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP1;
    else if(nEP==ENDPOINT2)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP2;
    else if(nEP==ENDPOINT3)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP3;
    else if(nEP==ENDPOINT4)
        pFIFO=(VUINT8*)&UsbRegs.FIFO_EP4; 
		    
	for(i=0;i<nLen;i++) 
          pBuf[i] = (UINT8)*(pFIFO);           
}

/***********************************************************************************
* Subrountine:	USB_SetAddress
* Function: 	Set USB IO Address
* Input:		nAddr-IO Address;
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/	
void USB_SetAddress(UINT8 nAddr)
{    
	UsbRegs.FADDR.all = nAddr ;
	do{
		__NOP();
		__NOP();
	}while(UsbRegs.FADDR.bit.Update);
           
    __NOP();        
     
}
/***********************************************************************************
* Subrountine:	USB_SendACK
* Function: 	send ACK  to pc
* Input:		nEndPoint-the endpoint to send
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/	
void USB_SendACK(UINT8 nEndPoint)
{
	UsbRegs.INDEX=nEndPoint;
	if(nEndPoint==ENDPOINT0)
	{		 
		UsbRegs.STAS.CSR0.bit.DataEnd=1;
		UsbRegs.STAS.CSR0.bit.InPktRdy=1; 
	}
	else
	{
		UsbRegs.STAS.INCSRL.bit.InPktRdy=1;
		while(UsbRegs.STAS.INCSRL.bit.InPktRdy==1)
		{
			__NOP();
		}     	
	}    
}
/***********************************************************************************
* Subrountine:	USB_SendStall
* Function:	    send Stall  to pc
* Input:		nEndPoint-the endpoint to send
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
	
void USB_SendStall(UINT8 nEndPoint)
{	
	USB_SendACK(nEndPoint);
	
}
/***********************************************************************************
* Subrountine:	USB_DisConnect
* Function:	    USB DisConnect
* Input:		None
* Output:		None
* Description:
* Date:		2009.07.09
* ModifyRecord:
**********************************************************************************/
	
void USB_DisConnect()
{
	UsbRegs.POWER.all=0x00;
}
	
/***********************************************************************************
* Subrountine:	USB_Init
* Function:	    Init USB register and mode ,parameter.
* Input:		cMode-the work mode of usb,0: intterupt mode,1:polling mode.
                 when work in intterupt mode, should set the tUsbIntModePara;
                 tUsbIntModePara-the parameter for intterupt mode.                 
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
void USB_Init(UINT8 cMode)
{
	UINT32    temp ;

	gUSBFlag = 0;
	gIntrOut=0;
	gIntrIn=0; 
	SCM_DisableUsbPuRes(); 	
	SCM_DisableModClk(ID_USB);
	SCM_KeepModRst(ID_USB);

	temp=1200;
	while(temp--);	
	
	SCM_EnableModClk(ID_USB);
	SCM_ReleaseModRst(ID_USB);

 	SCM_UsbPhySet();
	SCM_EnableUsbPuRes();
	UsbRegs.POWER.all=0x00;
 	SCM_HFRoscCRE();
//	SCM_HFRoscCRE1(1);

	temp=1200;
	while(temp--);
	
    UsbRegs.INDEX=ENDPOINT0;
	UsbRegs.FADDR.all=0;
	UsbRegs.INTROUTE.all=0x06;
    UsbRegs.INTRINE.all=0x07;
    UsbRegs.INTRUSBE.all=0x07;	  
 	
	UsbRegs.INDEX=ENDPOINT1;
 
    UsbRegs.INMAXP=FS_MAX_PACK_LEN;
    UsbRegs.OUTMAXP=FS_MAX_PACK_LEN;
    UsbRegs.INDEX=ENDPOINT0;	
 
 	UsbRegs.POWER.all=0x01;         
 	
 	gUsbMode=cMode; 
 	
	if(cMode==USB_INTTERRUPT_MODE || cMode==USB_SETUP_INT_MODE)
	{ 
     
		NVIC_EnableIRQ(USB_IRQn);
		__enable_irq();	      
	}
	 
	_delay_ms(10);//使得时钟稳定一点。
	gSetupStatus.bRemoteWakeup = 0;		
}
	
/***********************************************************************************
* Subrountine:	USB_Reset
* Function:	    USB host reset interrupt func
* Input:		None
* Output:		None
* Description:
* Date:		2009.07.09
* ModifyRecord:
* v1.1	2013/07/15	coins
*	1. to enable suspend mode, the power reigster will be cleared by USB reset 
**********************************************************************************/
void USB_Reset( void)
{
	gUSBFlag = 0;
	gIntrOut=0;
	gIntrIn=0; 

   UsbRegs.INDEX=ENDPOINT1;

   UsbRegs.INMAXP=FS_MAX_PACK_LEN;
   UsbRegs.OUTMAXP=FS_MAX_PACK_LEN;

   UsbRegs.STAS.INCSRL.bit.FlushFIFO=1;
   UsbRegs.STAS.INCSRL.bit.ClrDataTog=1; 
  
   UsbRegs.OUTCSRL.bit.FlushFIFO=1;
   UsbRegs.OUTCSRL.bit.ClrDataTog=1; 

   UsbRegs.INDEX=ENDPOINT0;
   UsbRegs.POWER.all=0x01; 	// enable suspend mode
   //gSetupStatus.bRemoteWakeup = 0;  	
}

 /***********************************************************************************
* Subrountine:	USB_SendStall_Out
* Function:	    send Stall  to pc
* Input:		nEndPoint-the endpoint to send
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
	
void USB_SendStall_Out(UINT8 nEndPoint)
{	
	UsbRegs.INDEX=nEndPoint;
	if(nEndPoint==ENDPOINT0)
	{
		UsbRegs.STAS.CSR0.bit.SentStall=1;		 
		UsbRegs.STAS.CSR0.bit.SendStall=1;
    	while(	UsbRegs.STAS.CSR0.bit.SentStall != 1)
      	{
      		__NOP();
      	}
		UsbRegs.STAS.CSR0.bit.SentStall=1;
    }
	else
    {
	    UsbRegs.OUTCSRL.bit.SentStall = 1;
		UsbRegs.OUTCSRL.bit.SendStall=1;
//    	while(	UsbRegs.OUTCSRL.bit.SentStall != 1)
//      	{
//      		__NOP();
//      	}
//		UsbRegs.OUTCSRL.bit.SendStall = 0; 
//        UsbRegs.OUTCSRL.bit.SentStall = 1;
    } 	
}

/***********************************************************************************
* Subrountine:	USB_SendStall_In
* Function:	    send Stall  to pc
* Input:		nEndPoint-the endpoint to send
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
	
void USB_SendStall_In(UINT8 nEndPoint)
{	
 #if 1
	 UsbRegs.INDEX=nEndPoint;
	if(nEndPoint==ENDPOINT0)
	{
		UsbRegs.STAS.CSR0.bit.SentStall=1;		 
		UsbRegs.STAS.CSR0.bit.SendStall=1;
    	while(	UsbRegs.STAS.CSR0.bit.SentStall != 1)
      	{
      		__NOP();
      	}
		UsbRegs.STAS.CSR0.bit.SentStall=1;
    }
	 else
	 {
		 UsbRegs.STAS.INCSRL.bit.SentStall=1;
		 UsbRegs.STAS.INCSRL.bit.SendStall=1;
		 while(  UsbRegs.STAS.INCSRL.bit.SentStall==0)
		 {
			 __NOP();
		 }
		 UsbRegs.STAS.INCSRL.bit.SendStall = 0;	
		 UsbRegs.STAS.INCSRL.bit.SentStall = 1; 
	 }	  
 #endif   
}
/***********************************************************************************
* Subrountine:	USB_RemoteWakeWp
* Function:	    USB Remote Wake Wp 
* Input:		None
* Output:		None
* Description:
* Date:		2013.03.28
* ModifyRecord:
**********************************************************************************/
void USB_RemoteWakeWp(void)
{
	UINT32 i = 0;
	UINT32 nUSBFRAME = 0;

	if(UsbRegs.POWER.bit.SuspendMode==1)
	{
		if(gSysClkPar&USB_SUSPENDED)
		{
			gSysClkPar &= ~USB_SUSPENDED;
			SCM_SetSysClk((SYS_CLK)gSysClkPar);	// resume system clock 
		}

		if(gSetupStatus.bRemoteWakeup == 1)
		{
			nUSBFRAME = UsbRegs.FRAME;
			UsbRegs.POWER.bit.Resume=1;///The Resume bit is used to force the USBCTR to generate Resume signaling on the USB to perform remote wake-up from
										//Suspend mode. Once set high, it should be left high for approximately 10 ms (at least 1 ms and no more than 15 ms), then
										//cleared.
			_delay_ms(8);
			UsbRegs.POWER.bit.Resume=0;
			for(i=0;i<10000;i++)
			{
				_delay_ms(1);
				if(nUSBFRAME != UsbRegs.FRAME)
				{
					return;
				}
			}
		}
	}

}
