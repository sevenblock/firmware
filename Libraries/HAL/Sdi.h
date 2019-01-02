/***************************************************************************
* File name    :	Sdi.h
* Function     :	Header of Sdi.c
* Author       : 	coins
* Date         :	2011/07/11
* Version      :    v1.0
* Description  :    Header of SDI Hardware Abstract Layer
* ModifyRecord :
****************************************************************************/
#ifndef SDI_H_
#define SDI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
//#define	SDI_DBG_PRT
#define		SDI_MAP_GPA

#define	TIMEOUT_MS		(250)			// default timeout in ms

// ---------- Define the SD Card Command supported by SDI ---------------------
#ifndef		_SDI_CMD_INDEX_
#define		_SDI_CMD_INDEX_
// Define Command Qualifier according the SDI controller
#define		CMD_TRAN				0x40
#define		LONG_RESP				0x80
#define		ADTC_READ				0x100
#define		ADTC_WRITE				0x200
// Class 0: Basic
#define		GO_IDLE_STATE			(0 | CMD_TRAN)
#define		ALL_SEND_CID			(2 | CMD_TRAN | LONG_RESP)
#define		SEND_RELATIVE_ADDR		(3 | CMD_TRAN)
#define		SET_DSR					(4 | CMD_TRAN)
#define		SWITCH_FUNC				(6 | CMD_TRAN | ADTC_READ)
#define		SELECT_DESELECT_CARD	(7 | CMD_TRAN)
#define		SEND_IF_COND			(8 | CMD_TRAN)
#define		SEND_CSD				(9 | CMD_TRAN | LONG_RESP)
#define		SEND_CID				(10 | CMD_TRAN | LONG_RESP)
#define		STOP_TRANSMISSION		(12 | CMD_TRAN)
#define		SEND_STATUS				(13 | CMD_TRAN)
#define		GO_INACTIVE_STATE		(15 | CMD_TRAN)
// Class 2: Block read
#define		SET_BLOCKLEN			(16 | CMD_TRAN)
#define		READ_SINGLE_BLOCK		(17 | CMD_TRAN | ADTC_READ)
#define		READ_MULTIPLE_BLOCK		(18 | CMD_TRAN | ADTC_READ)		// un-supported
// Class 4: Block write
#define		WRITE_BLOCK				(24 | CMD_TRAN )
#define		WRITE_MULTIPLE_BLOCK	(25 | CMD_TRAN | ADTC_WRITE)		// un-supported
#define		PROGRAM_CSD				(27 | CMD_TRAN | ADTC_WRITE)
// Class 5: Erase
#define		ERASE_WR_BLK_START		(32 | CMD_TRAN)
#define 	ERASE_WR_BLK_END		(33 | CMD_TRAN)
#define		ERASE					(38 | CMD_TRAN)
// Class 6: Write protection
#define		SET_WRITE_PROT			(28 | CMD_TRAN)
#define		CLR_WRITE_PROT			(29 | CMD_TRAN)
#define		SEND_WRITE_PROT			(30 | CMD_TRAN | ADTC_READ)
// Class 7: Lock Card
#define		LOCK_UNLOCK				(42 | CMD_TRAN | ADTC_WRITE)
// Class 8: Application specific
#define		APP_CMD					(55 | CMD_TRAN)
#define		GEN_CMD					(56 | CMD_TRAN | ADTC_READ | ADTC_WRITE)	
// Class 9: I/O mode
// Class 10: Switch
// Application Specific Commands
#define		SET_BUS_WIDTH			(6 | CMD_TRAN)
#define		SD_STATUS				(13 | CMD_TRAN | ADTC_READ)
#define		SEND_NUM_WR_BLOCKS		(22 | CMD_TRAN | ADTC_READ)
#define		SET_WR_BLK_ERASE_COUNT	(23 | CMD_TRAN)
#define		SEND_OP_COND			(41 | CMD_TRAN)
#define		SET_CLR_CARD_DETECT		(42 | CMD_TRAN)
#define		SEND_SCR				(51 | CMD_TRAN | ADTC_READ)

#endif

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef	union {
	struct {
		UINT32 	Rsvd0:1;
		UINT32 	Rsvd1:1;
		UINT32 	Rsvd2:1;
		UINT32	AkeSeqError:1;
		UINT32	Rsvd4:1;
		UINT32	AppCmd:1;		// the card will expect ACMD
		UINT32	Rsvd6:1;
		UINT32 	Rsvd7:1;
		UINT32	ReadyForData:1;	// Corresponds to buffer empty signaling on the bus
		UINT32	CurrentState:4;	// the state of the card when receiving the command
		UINT32	EraseReset:1;	// An erase sequence was cheared
		UINT32	CardEccDisabled:1;	// the command has been executed without using the internal ECC
		UINT32	WpEraseSkip:1;		// set when only partial address space was erased due to existing writeprotected blocks
		UINT32	CsdOverWrite:1;		// 
		UINT32	Rsvd17:1;
		UINT32	Rsvd18:1;
		UINT32	Error:1;		// general or an unknown error
		UINT32	CcError:1;		// Internal card controller error
		UINT32	CardEccFailed:1;	// Card internal ECC was applied but failed to correct
		UINT32	IllegalCmd:1;		// Command not legal foe the card state
		UINT32	ComCrcError:1;		// the CRC check of the pervious command failed
		UINT32	LockUnlockFailed:1;	// set when a sequence for password error in lock/unlock card command
		UINT32	CardIsLocked:1;		// when set, signals that the card is locked
		UINT32	WpViolation:1;		// set when the host attempts to write to a protected block
		UINT32	EraseParam:1;		// an invalid selection of write blocks for erase
		UINT32	EraseSeqError:1;
		UINT32	BlockLenError:1;
		UINT32	AddressError:1;
		UINT32	OutOfRange:1;		// the command's argument was out of range
	}bit;
	UINT32 all;
}SD_R1_STATE;


typedef	union	{
	struct	{
		UINT64	Rsvd:8;
		UINT64 	MDT_m:4;		// MDT(Manufacturing date), month code
		UINT64	MDT_y:8;		// MDT(Manufacturing date), year code
		UINT64	Rsvd1:4;		// Reserved
		UINT64	PSN:32;			// PSN(Product serial number) low 16 bits
		UINT64	PRV_m:4;		// PRV(Product Revision) BCD n.m
		UINT64	PRV_n:4;
		UINT8	PNM[5];			// PNM(Product Name) 5-character ASCII
		UINT8	OID[2];			// OID(OEM/Application ID) 2-characher ASCII
		UINT8	MID;			// MID(Manufacturer ID)
	}bit;
	UINT32 Word[4];
}SD_CID;


typedef	union	{
	struct	{
		UINT32	Rsvd0:10;
		UINT32	FILE_FORMAT:2;		// File format
		UINT32	TMP_WRITE_PROTECT:1;// Temporary write protection
		UINT32	PERM_WRITE_PROTECT:1;// Permanent write protection
		UINT32	COPY:1;				// Copy flag (OTP)
		UINT32	FILE_FORMAT_GRP:1;	// File format group
		UINT32	Rsvd1:5;
		UINT32	WRITE_BL_PARTIAL:1;	// Partial blocks for write allowed
		UINT32	WRITE_BL_LEN:4;		// Max. write data block length
		UINT32	R2W_FACTOR:3;		// Write speed factor
		UINT32	Rsvd2:2;
		UINT32	WP_GRP_ENABLE:1;	// Write protect group enable
		UINT64	WP_GRP_SIZE:7;		// write protect group size
		UINT64	SECTOR_SIZE:7;		// erase sector size
		UINT64	ERASE_BLK_EN:1;		// Erase single block enable
		UINT64	C_SIZE_MULT:3;		// device size multiplier
		UINT64	VDD_W_CURR_MAX:3;	// Max. write current@VDD max
		UINT64	VDD_W_CURR_MIN:3;	// Max. write current@VDD min
		UINT64	VDD_R_CURR_MAX:3;	// Max. read current@VDD max
		UINT64	VDD_R_CURR_MIN:3;	// Max. read current@VDD min
		UINT64	C_SIZE:12;			// device size
		UINT64	Rsvd3:2;
		UINT64	DSR_IMP:1;			// DSR implemented
		UINT64	READ_BLK_MISALIGN:1;// Read block misalignment
		UINT64	WRITE_BLK_MISALIGN:1;// Write block misalignment
		UINT64	READ_BL_PARTIAL:1;	// Partial blocks for read allowed
		UINT64	READ_BL_LEN:4;		// Max. read data block length
		UINT64	CCC:12;				// card command classes
		UINT8	TRAN_SPEED;			// Max. data transfer rate
		UINT8	NSAC;				// data read access-time-2 in CLK cycles (NSAC*100)
		UINT8	TAAC;				// data read access-time-1
		UINT8	Rsvd4:6;			
		UINT8	CSD_STRUCTURE:2;	// CSD structure
	}V10bit;
	struct	{
		UINT32	Rsvd0:10;
		UINT32	FILE_FORMAT:2;		// File format
		UINT32	TMP_WRITE_PROTECT:1;// Temporary write protection
		UINT32	PERM_WRITE_PROTECT:1;// Permanent write protection
		UINT32	COPY:1;				// Copy flag (OTP)
		UINT32	FILE_FORMAT_GRP:1;	// File format group
		UINT32	Rsvd1:5;
		UINT32	WRITE_BL_PARTIAL:1;	// Partial blocks for write allowed
		UINT32	WRITE_BL_LEN:4;		// Max. write data block length
		UINT32	R2W_FACTOR:3;		// Write speed factor
		UINT32	Rsvd2:2;
		UINT32	WP_GRP_ENABLE:1;	// Write protect group enable
		UINT64	WP_GRP_SIZE:7;		// write protect group size
		UINT64	SECTOR_SIZE:7;		// erase sector size
		UINT64	ERASE_BLK_EN:1;		// Erase single block enable
		UINT64	Rsvd3:1;
		UINT64	C_SIZE:22;			// device size
		UINT64	Rsvd4:6;
		UINT64	DSR_IMP:1;			// DSR implemented
		UINT64	READ_BLK_MISALIGN:1;// Read block misalignment
		UINT64	WRITE_BLK_MISALIGN:1;// Write block misalignment
		UINT64	READ_BL_PARTIAL:1;	// Partial blocks for read allowed
		UINT64	READ_BL_LEN:4;		// Max. read data block length
		UINT64	CCC:12;				// card command classes
		UINT32	TRAN_SPEED:8;			// Max. data transfer rate
		UINT32	NSAC:8;				// data read access-time-2 in CLK cycles (NSAC*100)
		UINT32	TAAC:8;				// data read access-time-1
		UINT32	Rsvd5:6;			
		UINT32	CSD_STRUCTURE:2;	// CSD structure
	}V20bit;
	UINT32	Word[4];
}SD_CSD;


typedef	union	{
	struct	{
		UINT32	Rsvd:16;
		UINT32	SdBusWidths:4;
		UINT32	SdSecurity:3;
		UINT32	DataStatAfterErase:1;
		UINT32	SdSpec:4;
		UINT32	ScrStructure:4;
	}bit;
	UINT32 Word;
}SD_SCR;


//--------------- SD memory card version ------------------------------
typedef enum	{
	SD1_X		=	0,	// Ver1.x standard capacity SD memory card
	SD2_0_SC	= 	1,	// Ver2.0 standard capacity SD memory card
	SD2_0_HC	=	2,	// Ver2.0 high capacity SD memory card
}SDMC_VER;

//----------------- SD memory card parameters ---------------------------
typedef	struct	{
	SDMC_VER	Ver;		// SD Memory Card Version (1byte)
	UINT8	    Rsvd[3];
	UINT16		Capacity;	// SD Memory Card Capacity unit in MB
	UINT16		RCA;		// SD Memory Card Relative Card Address 
	SD_CID		CID;		// CID register
	SD_CSD		CSD;		// CSD register
}SDMC_PARA;




// -------------- SD card operation returns ----------------------------
typedef enum	{
	SDI_OK = 0,	
	
	/* Error detect from SDI controller Status Register */
	SDI_TIMEOUT	= 1,		// no response, and time out
	SDI_RESP_CRC_ERR,		// response CRC error
	SDI_DATA_CRC_ERR,
	/* Error from CRC status response when write block */
	SDI_FLASH_PROG_ERR = 0x10,		// CRC Status Token returns 3'b111
	SDI_WR_TRANS_ERR,		// CRC Status Token returns 3'b101
	SDI_CRC_TOKEN_ERR,		// CRC Status Token returns undefined status
	
	/* Error from Card Status (R1 response)*/
	SDI_OUT_OF_RANGE = 0x20,// Status bit 31
	SDI_ADDRESSp_ERROR,		// Status bit 30
	SDI_BLOCK_LEN_ERROR,	// Status bit 29
	SDI_ERASE_SEQ_ERROR,	// Status bit 28
	SDI_ERASE_PARAM,		// Status bit 27
	SDI_WP_VIOLATION,		// Status bit 26
	SDI_LOCK_UNLOCK_FAILED,	// Status bit 24
	SDI_COM_CRC_ERROR,		// Status bit 23
	SDI_ILLEGAL_COMMAND,	// Status bit 22
	SDI_CARD_ECC_FAILED,	// Status bit 21
	SDI_CC_ERROR,			// Status bit 20
	SDI_ERROR,				// Status bit 19
	SDI_CSD_OVERWRITE,		// Status bit 16
	SDI_WP_ERASE_SKIP,		// Status bit 15
	
	/* Error from other operation */
	SDI_VOL_UNAPT = 0x30,	// voltage un-accepted, non-compatible voltage range
	SDI_HS_UNSUPPORT,		// High speed unsupported
	
}SDI_ERR;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern SDMC_PARA gSdmcPara;


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void SDI_SendCmd(UINT16 nCmd, UINT32 nArg);
SDI_ERR SDI_SendACmd(UINT16 nRCA, UINT16 nCmd, UINT32 nArg);

SDI_ERR SDI_Init(UINT8 nClkDiv);
SDI_ERR SDI_ReadCSD(SD_CSD *pCSD);
SDI_ERR SDI_ReadCID(SD_CID *pCID);
SDI_ERR SDI_SelectCard(void);
void SDI_DeselectCard(void);
SDI_ERR SDI_Set4BitBusWidth(void);
SDI_ERR SDI_GetNumOfWrBlk(UINT32 *pNum);

SDI_ERR SDI_ReadBlock(UINT32 nBlkAddr, UINT8 *pBuf);
SDI_ERR SDI_ReadMultiBlock1st(UINT32 nBlkAddr, UINT32 nBlkNum, UINT8 *pBuf);
SDI_ERR SDI_ReadMultiBlockNxt(UINT32 nBlkNum, UINT8 *pBuf);
SDI_ERR SDI_StopTransmission(void);
SDI_ERR SDI_WriteBlock(UINT32 nBlkAddr, UINT8 *pBuf);
SDI_ERR SDI_WriteMultiBlock1st(UINT32 nBlkAddr, UINT32 nBlkNum, UINT8 *pBuf);
SDI_ERR SDI_WriteMultiBlockNxt(UINT32 nBlkNum, UINT8 *pBuf);
SDI_ERR SDI_EraseBlock(UINT32 nBlkAddrStt, UINT32 nBlkAddrEnd);

SDI_ERR SDI_ReadStatus(UINT32 *pR1Sta);
void SDI_GoInactiveState(void);
SDI_ERR SDI_SetBlockLen(UINT32 nBlkLen);
SDI_ERR SDI_ReadSCR(SD_SCR *pSCR);
SDI_ERR	SDI_Switch2HighSpeed(void);

SDI_ERR SDI_ReadMultiBlockWithCmd(UINT32 nBlkAddr, UINT8 nBuffBank, UINT32 nBlkNum);
SDI_ERR SDI_ReadMultiBlockNoCmd(UINT8 nBuffBank, UINT32 nBlkNum);
SDI_ERR SDI_WriteMultiBlockWithCmd(UINT32 nBlkAddr, UINT8 nBuffBank, UINT32 nBlkNum);
SDI_ERR SDI_WriteMultiBlockNoCmd(UINT8 nBuffBank, UINT32 nBlkNum);

#endif	  /*SDI_H_*/

