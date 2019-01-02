/***************************************************************************
* File name    :	BulkUsb.c
* Function     :	CBW and CSW package for SCSI command
* Author       : 	howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :     
* ModifyRecord :
****************************************************************************/
#include "AS569.h"

extern UINT8 gUSBFlag;
 
DoMySCSICmdProc g_pDoMySCSICmd=NULL;
WriteLBAProc    g_pWriteLBA=NULL;
ReadLBAProc     g_pReadLBA=NULL;
UINT32 glDiskCap=0; 
/***************************************************************************
* Subroutine:	InitSCSICallBackFunc
* Function:		Init SCSI call back function and init the capcity of disk.
* Input:		lDiskCap- the capcity of cd rom, the size=lDiskCap*K bytes.
                pDoMySCSICmd- the call back function for do with own command.
                pWirteLBA- the call back function for write data to CDROM
                pReadLBA - the call back function for read data from CDROM.
* Output:		None;
* Description:	 
* Date:		2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 
void InitSCSICallBackFunc(UINT32 lDiskCap,DoMySCSICmdProc pDoMySCSICmd,WriteLBAProc pWriteLBA,ReadLBAProc pReadLBA)
{
	glDiskCap=lDiskCap;
	g_pDoMySCSICmd=pDoMySCSICmd;
	g_pWriteLBA=pWriteLBA;
	g_pReadLBA=pReadLBA;
}
/***************************************************************************
* Subroutine:	DealCBW
* Function:	 Judge command type ,and do with the command;
* Input:		None
* Output:		None;
* Description:	 
* Date:		2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 
void DealCBW(CBW* uCBW)
{
	if(g_pDoMySCSICmd!=NULL)
	{
		if(g_pDoMySCSICmd(uCBW)==RT_OK)//说明执行了自定义命令;
			return;
	} 
	DoSCSICmd(uCBW);//do with command about disk;   	    
}
/***************************************************************************
* Subroutine:	ReceiveCBW
* Function:	    Judge command type ,and do with the command;
* Input:		None
* Output:		RT_OK-success
                RT_FAIL-fail
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 
UINT16 ReceiveCBW(CBW *uCBW)
{
    UINT8 i, count ;
    UINT32 lLen; 
    VUINT8  bOutRdy; 
	UINT32	nErrWhile = 0;

    UsbRegs.INDEX=ENDPOINT1;
	do				
	{
		//TimeOut;
		if((nErrWhile++ > USB_TIMEOUT)||(gUSBFlag == 1))
		{
			if(gUSBFlag == 1)
				gUSBFlag = 0;
			return RT_FAIL;
		}					
		bOutRdy = !(UsbRegs.OUTCSRL.bit.OutPktRdy);
	}while(bOutRdy);
	  
	count=UsbRegs.OUTCOUNT;         
	if(count<=0) return RT_FAIL;//no data       
    
	USB_BulkReceive((UINT8*)uCBW,count);    
    
    //judge if the cmd is ums command 
    if( uCBW->cCBWSignature[0]!=0x55 || uCBW->cCBWSignature[1]!=0x53  ||
    		uCBW->cCBWSignature[2]!=0x42  || uCBW->cCBWSignature[3]!=0x43 ) 
    {
    	      // SendStall(IN_ENDPOINT);
    	       USB_SendStall(OUT_ENDPOINT);
    	       return RT_FAIL;
    }
    uCBW->lCBWXferLength=0;
    for(i=0;i<4;i++)
    {
      lLen=uCBW->cCBWXferLength[i];
      uCBW->lCBWXferLength+=(lLen<<(i*8));
    } 
    
    return RT_OK;   
}
/***************************************************************************
* Subroutine:	UsbCBWHandle
* Function:	 Judge the data receive,if the command is cbw,then deal with the cbw
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 
void USB_CBWHandle( void)
{   
	CBW uCBW; 
	if(ReceiveCBW(&uCBW)!=RT_OK)
		return;  
	//if is ,then do with the command
	DealCBW(&uCBW) ;
}
 
/***************************************************************************
* Subroutine:	SendCSWBack
* Function:	 After the cbw has done, the device must send csw back to device
* Input:		cResult-the result of cbw
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/  
void SendCSWBack(CBW* uCBW, UINT8  cResult )
{
	UINT8 i;
	CSW uCSW;
	uCSW.cCSWSignature[0]=0x55;
	uCSW.cCSWSignature[1]=0x53;
	uCSW.cCSWSignature[2]=0x42;
	uCSW.cCSWSignature[3]=0x53;
	memcpy(uCSW.cCSWtag,uCBW->cCBWtag,4);
	for(i=0;i<4;i++)
	{      
		uCSW.cCSWResidue[i]=(uCBW->lCBWXferLength>>(i*8))&0xff;
	}
	
	uCSW.bCSWStatus=cResult;
	USB_BulkSend((UINT8*)&uCSW,13);
}  
  

