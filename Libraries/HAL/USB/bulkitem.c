/***************************************************************************
* File name    :	BulkItem.c
* Function     :	Do with scsi command
* Author       : 	howard
* Date         :	2013.03.28
* Version      :    v1.2
* Description  :     
* ModifyRecord :   增加SCSI_DefaultCmd,对不支持的CD命令，调用此函数处理，解决繁体操作系统CD识别有问题的Bug 
                   对健壮性方面做了加强 
****************************************************************************/
#include "AS569.h"
 
 
UINT8 gSensePtr;
UINT8 gStartAutorun=0;
UINT8 gCountAuto=0;

#define LBA_SIZE 2048

#define senseCRCError               0  
#define senseInvalidFieldInCDB      1
#define senseOk                     2
#define senseNoMedia                3
#define senseWriteFault             4
#define senseReadError              5
#define senseAddrNotFound           6
#define senseInvalidOpcode          7
#define senseInvalidLBA             8
#define senseInvalidParameter       9
#define senseCantEject              0xa
#define senseMediaChanged           0xb
#define senseDeviceReset            0xc 
#define senseWriteProtected         0xd


const UINT8 gsenseArray[14][3] = 
{{0x0b, 0x08, 0x03},   // senseCRCError               0  
{0x05, 0x24, 0x00},    // senseInvalidFieldInCDB      1
{0x00, 0x00, 0x00},    // senseOk                     2
{0x02, 0x3a, 0x00},    // senseNoMedia                3
{0x03, 0x03, 0x00},    // senseWriteFault             4
{0x03, 0x11, 0x00},    // senseReadError              5
{0x03, 0x12, 0x00},    // senseAddrNotFound           6
{0x05, 0x20, 0x00},    // senseInvalidOpcode          7
{0x05, 0x21, 0x00},    // senseInvalidLBA             8
{0x05, 0x26, 0x00},    // senseInvalidParameter       9
{0x05, 0x53, 0x02},    // senseCantEject              0xa
{0x06, 0x28, 0x00},    // senseMediaChanged           0xb
{0x06, 0x29, 0x00},    // senseDeviceReset            0xc 
{0x07, 0x27, 0x00}};   // senseWriteProtected         0xd

/***************************************************************************
* Subroutine:	SCSI_ModeSense6
* Function:		do with scsi command modesense6.
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 	                    

UINT8 SCSI_ModeSense6(CBW* uCBW)          // 0x06
{	
	const UINT8 SenseInfo[8]={0,6,0,0,0,0,0,0};
	UINT32 nLen;
	
    nLen = (uCBW->lCBWXferLength > 8) ? 8 :uCBW->lCBWXferLength;
	 
    USB_BulkSend((UINT8*)SenseInfo,nLen);  
    
    uCBW->lCBWXferLength=0; 
	    
    gSensePtr=senseOk;

    return USBS_PASSED;
           
}
/***************************************************************************
* Subroutine:	SCSI_ModeSense10
* Function:		do with scsi command modesense10
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_ModeSense10(CBW* uCBW)
{
	const UINT8 SenseInfo[8]={0,6,0,0,0,0,0,0};
    UINT32 nLen;     	
	 
    nLen = (uCBW->lCBWXferLength > 8) ? 8 :uCBW->lCBWXferLength;
    USB_BulkSend((UINT8*)SenseInfo,nLen);		 
					
	uCBW->lCBWXferLength=0;
    gSensePtr=senseOk;		

    return USBS_PASSED;
}
/***************************************************************************
* Subroutine:	SCSI_ReadCapacity
* Function:		do with scsi command readcap
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_ReadCapacity(CBW* uCBW)  // 0x25
{	
	UINT8 Read_Cap_SendBack[8]  = { 0x00, 0x00, 0x06, 0x80, 0x00, 0x00, 0x08, 0x00 } ;
	 
	UINT32 nLen;			 
  	 
	nLen = (uCBW->lCBWXferLength > 8) ? 8 : uCBW->lCBWXferLength;
	
	Read_Cap_SendBack[2]=(glDiskCap/2)/256;
	Read_Cap_SendBack[3]=(glDiskCap/2)%256;
	
	USB_BulkSend(Read_Cap_SendBack,nLen);
	
	uCBW->lCBWXferLength=0; 
	
	gSensePtr=senseOk;
	
	return USBS_PASSED;      
}
/***************************************************************************
* Subroutine:	SCSI_Inquiry
* Function:		do with scsi command inquiry
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/  
 	 
UINT8 SCSI_Inquiry(CBW* uCBW)          // 0x12
{

    UINT8 nLen;
    const UINT8 InquriySendBack[36]= 
	{
		0x05, //0x05 mean CD ROM ,0x00 mean u Disk
		0x80, //0x80 mean removeable disk,0x00 mean local disk;
		0x00, 0x01, 0x1F, 0x00, 0x00, 0x00,			// 0x12
		'S', 'y', 'n', 'o', 0x00,0x00,0x00,0x00,	//Verdon infomation,8 bytes
		'U', 's','b', ' ', 'K', 'e', 'y',0x00, 0x00,0x00,0x00,0x00, 0x00, 0x00, 0x00, 0x00, //product infomation , 16 bytes
		'0', '1', '1', '0' 	 //*/ //version 1.1 //modify by chester 12:12 2012-02-12 
	};	       
   
    nLen=sizeof(InquriySendBack);

    if(uCBW->lCBWXferLength<sizeof(InquriySendBack))
    {    	   
		nLen=uCBW->lCBWXferLength;     	   
    }
    
    uCBW->lCBWXferLength=0;
       
    USB_BulkSend((UINT8*)InquriySendBack,nLen);
    
    gSensePtr=senseOk;
    
    return USBS_PASSED;      
             
}
/***************************************************************************
* Subroutine:	SCSI_ReadFormatCapacity
* Function:		do with scsi command readformatcap 
* Input:		None
* Output:		None;
* Description:	 
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_ReadFormatCapacity(CBW* uCBW)  // 0x23
{
	//格式化磁盘容量
	UINT8 DiskFormatCapacity[12] =
	{
		0x00, 0x00,0x00,		//Reserved
		0x08,					//Capacity List Length
		0x00,0x00,0x00,0xff,	//Number of Blocks
		0x02, 					//Descriptor Type:Formatted Media.
		0x00,0x08,0x00,			//Block Length: 2048bytes
	};
	UINT32 nLen=0;
	
	DiskFormatCapacity[6] = (glDiskCap/2)/256;
	DiskFormatCapacity[7] = (glDiskCap/2)%256;	
	
	nLen = (uCBW->lCBWXferLength > sizeof(DiskFormatCapacity)) ? sizeof(DiskFormatCapacity) : uCBW->lCBWXferLength;
		
	USB_BulkSend((UINT8 *)DiskFormatCapacity, nLen);
	uCBW->lCBWXferLength=0;
	gSensePtr=senseOk;
	return USBS_PASSED;
}
/***************************************************************************
* Subroutine:	SCSI_ReadToc
* Function:		do with scsi command ReadToc
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
UINT8 SCSI_ReadToc(CBW* uCBW)
{ 

	UINT8 Form00[20]=
	{   0x00,0x12,0x01,0x01,
        0x00,0x17,0x01,0x00,0x00,0x00,0x00,0x00,
        0x00,0x17,0xaa,0x00,0x00,0x00,0x00,0x00};
		
	UINT8 Form01[12] =
	{   0x00,0x0A,0x01,0x01,
		0x00,0x17,0x01,0x00,0x00,0x00,0x00,0x00};

	const UINT8 Form02[48]=
	{
		0x00, 0x2E, 0x01, 0x01,
		0x01, 0x16, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
		0x01, 0x16, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x01, 0x16, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
		0x01, 0x16, 0x00, 0xA2, 0x00, 0x00, 0x00, 0x00, 0x07, 0x16, 0x68
	};
	
	UINT8 MSF = (uCBW->CBWCB[1]&0x02);
	UINT8 MMC5Format = (uCBW->CBWCB[2]&0x0f);
	UINT8 Format = (uCBW->CBWCB[9]&0xc0);
	UINT32 nLen=0;
	UINT8 *pFormBuf; 
	
	if(MSF == 0x02)
	{
        Form00[10] = 0x02;
        Form00[18] = 0x18;
        Form00[19] = 0x43;
        
        Form01[10] = 0x02;
	}
	
	if((MMC5Format == 0x00)&&(Format == 0x00))
	{
		nLen = (uCBW->lCBWXferLength > sizeof(Form00)) ? sizeof(Form00) : uCBW->lCBWXferLength;
		pFormBuf = (UINT8 *)Form00;          		
	}
    else if	(((MMC5Format == 0x00)&&(Format == 0x40))||(MMC5Format == 0x01))
    {
		nLen = (uCBW->lCBWXferLength > sizeof(Form01)) ? sizeof(Form01) : uCBW->lCBWXferLength;
		pFormBuf = (UINT8 *)Form01;    	
    }    
	else if	(((MMC5Format == 0x00)&&(Format == 0x80))||(MMC5Format == 0x02))
	{
		nLen = (uCBW->lCBWXferLength > sizeof(Form02)) ? sizeof(Form02) : uCBW->lCBWXferLength;
		pFormBuf = (UINT8 *)Form02;				
	}
    else
    {
		nLen = (uCBW->lCBWXferLength > sizeof(Form00)) ? sizeof(Form00) : uCBW->lCBWXferLength;
		pFormBuf = (UINT8 *)Form00;		       
    }	
    
	USB_BulkSend((UINT8 *)pFormBuf, nLen);          
    uCBW->lCBWXferLength=0;
	gSensePtr=senseOk;
	return USBS_PASSED;
		  	    
}
/***************************************************************************
* Subroutine:	SCSI_GetConfigration
* Function:		do with scsi command GetConfigration 0x46
* Input:		None 
* Output:		None;
* Description:	 
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_GetConfigration(CBW* uCBW) //0x46
{
	UINT8 Feature[8] =
	{
		0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x08
	};
	
	UINT8 Feature0000[16] =
	{
		0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x08,
		0x00,0x00,0x03,0x04,0x00,0x08,0x01,0x00
	};
	
	UINT8 Feature0003[16] =
	{
		0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x08,
		0x00,0x03,0x03,0x04,0x20,0x00,0x00,0x00
	};
	
	UINT32 nLen=0;
	UINT8 *pBuf; 
	
#if	(0)
	nLen = (uCBW->lCBWXferLength > sizeof(Feature)) ? sizeof(Feature) : uCBW->lCBWXferLength;	
    pBuf = 	(UINT8 *)Feature;
#else	
	UINT16 FeatureCodes = (uCBW->CBWCB[2]<<8) + uCBW->CBWCB[3];
	
	if(FeatureCodes == 0x0000)
	{
		nLen = (uCBW->lCBWXferLength > sizeof(Feature0000)) ? sizeof(Feature0000) : uCBW->lCBWXferLength;
        pBuf = 	(UINT8 *)Feature0000;	
	}
	else if(FeatureCodes == 0x0003)
	{
		nLen = (uCBW->lCBWXferLength > sizeof(Feature0003)) ? sizeof(Feature0003) : uCBW->lCBWXferLength;
        pBuf = 	(UINT8 *)Feature0003;
	}
	else
	{
		nLen = (uCBW->lCBWXferLength > sizeof(Feature)) ? sizeof(Feature) : uCBW->lCBWXferLength;
        pBuf = 	(UINT8 *)Feature;
	}
#endif	
	USB_BulkSend((UINT8 *)pBuf, nLen);	
	uCBW->lCBWXferLength=0;
	gSensePtr=senseOk;
	return USBS_PASSED;
}
/***************************************************************************
* Subroutine:	SCSI_RequestSense
* Function:		do with scsi command requestsense 0x03
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_RequestSense(CBW* uCBW) //0x03
{ 
	UINT8 SenseInfo[18];	 
	UINT32 length;	
	 
	memset(SenseInfo , 0 , 18);
	SenseInfo[0] = 0x70;
	SenseInfo[2] = gsenseArray[gSensePtr][0];//0x07;
	SenseInfo[7] = 18-8;
	SenseInfo[12] = gsenseArray[gSensePtr][1];//0x27;     
	SenseInfo[13] = gsenseArray[gSensePtr][2];	   
	
	if(!gStartAutorun)
	{
	SenseInfo[2] =  0x02;
	SenseInfo[12]  =  0x3a;
	}      			 
	
	length = (uCBW->lCBWXferLength > 18) ? 18 :uCBW->lCBWXferLength;
	USB_BulkSend(SenseInfo,length);
	uCBW->lCBWXferLength=0;

	return USBS_PASSED;
}  
/***************************************************************************
* Subroutine:	SCSI_CommonCmd
* Function:		do common cmd
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_CommonCmd( CBW* uCBW)     // 0x00
{   
	gSensePtr=senseOk;

	return USBS_PASSED;
}
/***************************************************************************
* Subroutine:	SCSI_TestUnitReady
* Function:		Test Unit ready cmd
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_TestUnitReady(CBW* uCBW)     // 0x00
{   
	if(gStartAutorun)
	{
		gSensePtr = senseOk;
	}
	else	
	{
		gCountAuto ++;
		gSensePtr = senseInvalidParameter; 
	} 
			
	if(gCountAuto >= 4)
	{
		gCountAuto=10;
		gStartAutorun = 1;				 
	}
	if (gSensePtr == senseOk)
		return USBS_PASSED;
	else
		return USBS_FAILED;	 
}

/***************************************************************************
* Subroutine:	SCSI_ModeSense10
* Function:		Read data from disk ,and send to PC
* Input:		lStartLBA-start address page,nLBACount-count of read 
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 Disk_ReadLBA(UINT32 lStartLBA,UINT16 nLBACount)     // 0x00
{   
	UINT16 i,j;
	UINT8 pData[512];
	
	for(i=0;i<nLBACount;i++)
	{
		for(j=0;j<LBA_SIZE/512;j++)
		{
			if(g_pReadLBA!=NULL)
			{
				g_pReadLBA(pData,(lStartLBA+i)*LBA_SIZE+j*512,512);
			}	
			USB_BulkSend(pData,512);
		}
	} 
	return USBS_PASSED;
}
/***************************************************************************
* Subroutine:	Disk_WriteLBA
* Function:		Write data to disk 
* Input:		lStartLBA-start address page,nLBACount-count of write 
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 
UINT8 Disk_WriteLBA(UINT32 lStartLBA,UINT16 nLBACount)     // 0x00
{   
	UINT16 i,j;
	UINT8 pData[512];
	
	for(i=0;i<nLBACount;i++)
	{
		for(j=0;j<LBA_SIZE/512;j++)
		{
			USB_BulkReceive(pData,512);          
			if(g_pWriteLBA!=NULL)	  	   
				g_pWriteLBA(pData,(lStartLBA+i)*LBA_SIZE+j*512,512);
		}   	        
	} 
	return USBS_PASSED;
}
/***************************************************************************
* Subroutine:	SCSI_ReadLBA
* Function:		do SCSI Command ,Read6,Read10 
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/  	 
UINT8 SCSI_ReadLBA(CBW* uCBW)     // 0x00
{   
	UINT32 lStartLBA,lTmp;
	UINT16 nLBACount;
	UINT8 nRet;
	if(uCBW->CBWCB[0] == READ6_OPCODE)
	{
		lStartLBA = (uCBW->CBWCB[2]<<8)+uCBW->CBWCB[3];
		nLBACount = uCBW->CBWCB[4];
	}
	else
	{
		lTmp = (uCBW->CBWCB[2]<<8)+uCBW->CBWCB[3];
		lStartLBA = (lTmp<<16)+(uCBW->CBWCB[4]<<8)+uCBW->CBWCB[5];
		nLBACount = (uCBW->CBWCB[7]<<8)+uCBW->CBWCB[8];
	}
	gSensePtr=senseOk;
	//--the code will make the pc bios run slow.?
	nRet = Disk_ReadLBA(lStartLBA,nLBACount);
	uCBW->lCBWXferLength -= nLBACount*LBA_SIZE;
	uCBW->lCBWXferLength = 0;
	
	return nRet;
}
/***************************************************************************
* Subroutine:	SCSI_WriteLBA
* Function:		do SCSI command,Write6,Write10
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/
 	 
UINT8 SCSI_WriteLBA( CBW* uCBW)     // 0x00
{  	 
	UINT32 lStartLBA,lTmp;
	UINT16 nLBACount ;
	UINT8 nRet;
	
	if(uCBW->CBWCB[0]==WRITE6_OPCODE)
	{
		lStartLBA=(uCBW->CBWCB[2]<<8)+uCBW->CBWCB[3];
		nLBACount=uCBW->CBWCB[4];
	}
	else
	{
		lTmp=(uCBW->CBWCB[2]<<8)+uCBW->CBWCB[3];
		lStartLBA=(lTmp<<16)+(uCBW->CBWCB[4]<<8)+uCBW->CBWCB[5];
		nLBACount=(uCBW->CBWCB[7]<<8)+uCBW->CBWCB[8];
	}
	gSensePtr=senseOk;
	nRet = Disk_WriteLBA(lStartLBA,nLBACount);
	uCBW->lCBWXferLength-=nLBACount*LBA_SIZE;
	uCBW->lCBWXferLength=0;
	return nRet;
}

/***************************************************************************
* Subroutine:	SCSI_DoOutCmd
* Function:		Do some cmd use out endpoint;
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
 	 
UINT8 SCSI_DoOutCmd(CBW* uCBW)
{
    UINT8 nRet;
	 switch( uCBW->CBWCB[0] )
	 {
	 	   case    WRITE6_OPCODE:
	 	   case    WRITE10_OPCODE :    // 0x2A        
 					nRet=SCSI_WriteLBA(uCBW) ;
 					break ;      	
       
	       default :        	
                     nRet=SCSI_CommonCmd(uCBW);            			 
                     break;
        	
    }

    if(uCBW->lCBWXferLength>0)
    {
    	 uCBW->lCBWXferLength=0;
       
    	 USB_SendStall(OUT_ENDPOINT);
    }  
    return nRet; 	

}

/***************************************************************************
* Subroutine:	SCSI_DoInCmd
* Function:		Do some cmd use in endpoint
* Input:		None
* Output:		uccess-USBS_PASSED,fail-USBS_FAILED;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:  对Default的处理为调用SCSI_DefaultCmd 
                  对Default命令，直接返回ACK。 
* *************************************************************************/ 
 	 
UINT8 SCSI_DoInCmd(CBW* uCBW)
{
	UINT8 nRet=0; 
	switch(uCBW->CBWCB[0])
	{
		case	INQUIRY_OPCODE:
				{
					nRet=SCSI_Inquiry(uCBW);
					break;
				}
		case	READ_FORMAT_CAPACITIES:
				{
					nRet=SCSI_ReadFormatCapacity(uCBW);
					break;
				}
		case	READ_CAPACITY10_OPCODE:
				{
					nRet=SCSI_ReadCapacity(uCBW);
					break;  
				}
		case	READ_TOC:
				nRet=SCSI_ReadToc(uCBW);
				break;
		case	READ6_OPCODE:   		 	
		case	READ10_OPCODE :             // 0x28
				{
				    nRet=SCSI_ReadLBA(uCBW ) ;
					break ;
				}
		case	REQUEST_SENSE_OPCODE:
				{
					nRet=SCSI_RequestSense(uCBW);
					break;
				}
		case	MODE_SENSE6_OPCODE :        // 0x06
				{
					nRet=SCSI_ModeSense6(uCBW ) ;
					break ;
				}
		case	MODE_SENSE10_OPCODE :        // 0x06
				{
					nRet=SCSI_ModeSense10(uCBW ) ;
					break ;
				}
		case	TEST_UNIT_READY_OPCODE :    // 0x00
				{
					nRet=SCSI_TestUnitReady(uCBW);  
					break;                     
				}
		case	GET_CONFIGRATION:            // 0x46
				{
					nRet=SCSI_GetConfigration(uCBW);
					break;
				}
		case    PREVENT_REMOVAL_OPCODE :    // 0x1E
		case    VERIFY10_OPCODE :           // 0x2F    
		         {
                      nRet=SCSI_CommonCmd(uCBW ) ;
		              break ;
		         }                
		default :
		         {      
                      
                       break;
		         }
    }
    if(uCBW->lCBWXferLength>0)
    {
    	 uCBW->lCBWXferLength=0;
      
    	 USB_SendACK(IN_ENDPOINT);
    }  
    return nRet; 	
}
/***************************************************************************
* Subroutine:	DoSCSICmd
* Function:		Do SCSI command
* Input:		None
* Output:		None;
* Description:	 
* Date:			2013.03.28
* ModifyRecord:
* *************************************************************************/ 
  
void  DoSCSICmd(CBW* uCBW)
{   
	if((uCBW->cCBWFlags & 0x80) || (!uCBW->lCBWXferLength))
		SendCSWBack(uCBW, SCSI_DoInCmd(uCBW));
	else
		SendCSWBack(uCBW, SCSI_DoOutCmd(uCBW));   	  
        		
}
 
