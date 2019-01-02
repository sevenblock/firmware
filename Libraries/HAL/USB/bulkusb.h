/***************************************************************************************
* File name    :	Bulkusb.h
* Function     :	The header of BulkUsb.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _BULKUSB_H_
#define _BULKUSB_H_

//------------------------------------------------------------------------------
// command descriptor block
typedef UINT8 CDB[16];
//------------------------------------------------------------------------------
// command block wrapper
typedef struct _CBW
{
	UINT8 cCBWSignature[4];		// cCBWSignature: the constant 0x55 0x53 0x42 0x43(LSB)
	UINT8 cCBWtag[4];			// cCBWtag:
	UINT8 cCBWXferLength[4];	// cCBWXferLength:number of bytes to transfer
	UINT8 cCBWFlags;			// cCBWFlags:
                    				//   Bit 7: direction - the device shall ignore this bit if the
                    				//   dCBWDataTransferLength field is zero, otherwise:
                    				//   0 = Data-Out from host to the device,
                    				//   1 = Data-In from the device to the host.
                    				//   Bit 6: obsolete. The host shall set this bit to zero.
                    				//   Bits 5..0: reserved - the host shall set these bits to zero.
	UINT8 cCBWlun;				// bmCBWlun:
	UINT8 cCBWCBLength;			// bCBWLength: 0x01..0x10
	CDB   CBWCB;               				 // CBWCB: the command descriptor block
	UINT32 lCBWXferLength;		//lCBWXferLength=cCBWXferLength[4];
}CBW;
#define CBW_SIGNATURE		0x43425355
#define CBW_FLAG_DIR_MASK	0x80
#define CBW_FLAG_DIR_IN		0x80
#define CBW_FLAG_DIR_OUT	0x00

//------------------------------------------------------------------------------
//command status wrapper
typedef struct _CSW
{
	UINT8 cCSWSignature[4];		// cCSWSignature: the constant 0x53 0x42 0x53 0x55
	UINT8 cCSWtag[4];			// cCSWtag:
	UINT8 cCSWResidue[4];		// lCBWXferLength:  number of bytes not transferred
	UINT8 bCSWStatus;			// cCSWStatus:
									//  00h command Passed ("good status")
									//  01h command failed
									//  02h phase error
									//  03h to FFh reserved
}CSW;
#define CSW_SIGNATURE 		0x53425355
#define USBS_PASSED     	0
#define USBS_FAILED      	1
#define USBS_PHASE_ERROR 	2
#define USBS_STATUS_MAX		2
 
//----------define call back function for scsi device------------
typedef UINT8 (*DoMySCSICmdProc)(CBW* uCBW); 

typedef UINT8 (*WriteLBAProc)(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
typedef UINT8 (*ReadLBAProc)(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum); 

extern WriteLBAProc		g_pWriteLBA;
extern ReadLBAProc		g_pReadLBA;

extern UINT32 glDiskCap;

void InitSCSICallBackFunc(UINT32 lDiskCap,DoMySCSICmdProc pDoMySCSICmd,WriteLBAProc pWriteLBA,ReadLBAProc pReadLBA);
void  DoSCSICmd(CBW* uCBW);
void SendCSWBack(CBW* uCBW,UINT8  cResult );
void USB_CBWHandle( void);
void USB_DoWithPackage(void);

#endif
