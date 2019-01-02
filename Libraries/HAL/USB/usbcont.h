/***************************************************************************************
* File name    :	UsbCont.h
* Function     :	Some Usb const define for Usb slave device 
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef		_USB_TOOL_H
#define		_USB_TOOL_H

/*************************************************************************************
 * Constant	Macro-Definition													 	 *
 ************************************************************************************/  

#define     MAX_STD_REQUEST         13  

#define     EP1IN_NUMBER            0x81
#define     EP1OUT_NUMBER           0x01

#define     EP2IN_NUMBER            0x82
#define     EP2OUT_NUMBER           0x02

#define     EP3IN_NUMBER            0x83
#define     EP3OUT_NUMBER           0x03

#define     EP4IN_NUMBER            0x84
#define     EP4OUT_NUMBER           0x04
// Class Request Codes
#define     GET_MAX_LUN			 	0xFE

// Usb Request Type
#define     USB_REQUEST_TYPE_MASK   	0x60 
#define		USB_REQUEST_RECIPIENT_MASK	0x1F

#define     USB_REQ_TYPE_STANDARD   	0x00
#define     USB_REQ_TYPE_CLASS      	0x20
#define     USB_REQ_TYPE_VENDOR     	0x40
#define		USB_REQ_TYPE_RECIPIENT		0x1F

#define		USB_RECIPIENT_DEVICE		0
#define		USB_RECIPIENT_INTERFACE		1
#define		USB_RECIPIENT_ENDPOINT		2

// Standard Request Codes
#define     GET_STATUS             	0
#define     CLEAR_FEATURE          	1
#define     SET_FEATURE            	3
#define     SET_ADDRESS            	5
#define     GET_DESCRIPTOR         	6
#define     SET_DESCRIPTOR         	7
#define     GET_CONFIGURATION      	8
#define     SET_CONFIGURATION      	9
#define     GET_INTERFACE         	10
#define     SET_INTERFACE         	11
#define     SYNCH_FRAME           	12

// Descriptor Types
#define     DEVICE_DESC_TYPE			0x01
#define     CONFIGURATION_DESC_TYPE		0x02
#define     STRING_DESC_TYPE			0x03
#define     INTERFACE_DESC_TYPE			0x04
#define     ENDPOINT_DESC_TYPE			0x05
#define     DEVICE_QUAL_TYPE            0x06
#define     OTHER_SPEED_TYPE            0x07
#define     TEST_BOS_TYPE            	0x0f
#define		HID_DESC_TYPE				0x21
#define		REPORT_DESC_TYPE			0x22
#define		PHYSICAL_DESC_TYPE			0x23

// USB HID Descriptor Types
#define		HID_GET_REPORT				0x01
#define		HID_GET_IDLE				0x02
#define		HID_GET_PROTOCOL			0x03
#define		HID_SET_REPORT				0x09
#define		HID_SET_IDLE				0x0A
#define		HID_SET_PROTOCOL			0x0B


// Scsi Command Operation Code List.
#define     TEST_UNIT_READY_OPCODE      0x00
#define     REQUEST_SENSE_OPCODE        0x03
#define     MODE_SELECT6_OPCODE         0x15   
#define     MODE_SENSE6_OPCODE          0x1A   
#define     INQUIRY_OPCODE              0x12
#define     START_STOP_UNIT_OPCODE      0x1B
#define     PREVENT_REMOVAL_OPCODE      0x1E
#define     READ_FORMAT_CAPACITIES      0x23
#define     READ_CAPACITY10_OPCODE      0x25
#define     READ10_OPCODE               0x28
#define     READ6_OPCODE               	0x08
#define     WRITE10_OPCODE              0x2A
#define     WRITE6_OPCODE              	0x0A
#define     VERIFY10_OPCODE             0x2F
#define     MODE_SENSE10_OPCODE         0x5A
#define     READ_TOC                    0x43 
#define     GET_EVENT_STATUS_NOTIFICATION  0x4A
#define     READ_DISC_INFORMATION       0x51
#define     GET_CONFIGRATION            0x46 


#endif

