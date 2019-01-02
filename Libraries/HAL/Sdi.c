/***************************************************************************
* File name    :	Sdi.c
* Function     :	The functions for SDI
* Author       : 	coins
* Date         :	2011/07/11
* Version      :    v1.0
* Description  :    SDI Hardware Abstract Layer compatible with AS569
* ModifyRecord :
****************************************************************************/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "AS569.h"

/**************************************************************************
* Local Macro Definition
***************************************************************************/

/**************************************************************************
* Local Type Definition
***************************************************************************/

/**************************************************************************
* Local static Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
SDMC_PARA gSdmcPara;
volatile UINT8 gFlush;

/**************************************************************************
* Local Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/

/***************************************************************************
 * Subroutine:	SDI_FlushDatBuf
 * Function:	Flush data buffer
 * Input:		None
 * Output:		None		
 * Description:
 * Date:		2014.01.10
 * ModifyRecord:
 * *************************************************************************/
void SDI_FlushDatBuf(void)
{
	//*(volatile UINT8 *)0x20004001 = *(UINT8 *)0x20004001;
	gFlush = 1;
}

/***************************************************************************
 * Subroutine:	SDI_Wait
 * Function:	SDI send command
 * Input:		UINT16 nClkNum -- 	to wait n SDI clock;
 * Output:		None		
 * Description:
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
void SDI_Wait(UINT16 nClkNum)
{
	UINT32 i = (SdiRegs.CLK_DIVIDER.all+1)*2*nClkNum;
	while(i--);
}

/***************************************************************************
 * Subroutine:	SDI_SendCmd
 * Function:	SDI send command
 * Input:		UINT16 nCmd -- 		to specify the command;
 * 				UINT32 nArg --		to specify the argument
 * Output:		None		
 * Description:
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
void SDI_SendCmd(UINT16 nCmd, UINT32 nArg)
{
	if((nCmd & (ADTC_READ | ADTC_WRITE)) == (ADTC_READ | ADTC_WRITE))		// ADTC Read and Write Command
	{
		if(nArg&0x1) nCmd &= ~(ADTC_WRITE);		// ADTC Read operation
		else  nCmd &= ~(ADTC_READ);				// ADTC Write operation
	}
	SdiRegs.CMD_BUF0 = nCmd;
	SdiRegs.CMD_BUF1 = nArg;
	SdiRegs.CTRL.bit.CmdSendStart = 1;		// start to send command
	while(SdiRegs.CTRL.bit.CmdSendStart);	// wait for command send complete
//	_delay_us(100);
	#ifdef	SDI_DBG_PRT
	print(">>>CMD%d, argument = 0x%x\n", nCmd, nArg);
	#endif
}


/***************************************************************************
 * Subroutine:	SDI_GetRespR1
 * Function:	SDI get Response R1 or R1b
 * Input:		UINT8 bBusyWait -- 		to specify whether busy wait or not;
 * Output:		SDI_ERR		
 * Description: if bBusyWait = 0, to get R1, while bBusyWait = 1, to get R1b
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_GetRespR1(UINT8 bBusyWait)
{
	SD_R1_STATE sR1Sta;
	while(!SdiRegs.STATUS.bit.RespRecOver)
	{
		if(SdiRegs.STATUS.bit.Timeout) return SDI_TIMEOUT;
	}
	if(SdiRegs.STATUS.bit.RespCrcErr) return SDI_RESP_CRC_ERR;
	sR1Sta.all = SdiRegs.RSP_BUF0;
	if(bBusyWait)while(!SdiRegs.STATUS.bit.BusReady);		// busy wait
	
	#ifdef	SDI_DBG_PRT
	print("<<<Response R1 0x%x\n", sR1Sta.all);
	#endif
	
	if(sR1Sta.all&0xFDF98000)		// error check
	{
		if(sR1Sta.bit.OutOfRange) return SDI_OUT_OF_RANGE;
		if(sR1Sta.bit.AddressError) return SDI_ADDRESSp_ERROR;
		if(sR1Sta.bit.BlockLenError) return SDI_BLOCK_LEN_ERROR;
		if(sR1Sta.bit.EraseSeqError) return SDI_ERASE_SEQ_ERROR;
		if(sR1Sta.bit.EraseParam) return SDI_ERASE_PARAM;
		if(sR1Sta.bit.WpViolation) return SDI_WP_VIOLATION;
		if(sR1Sta.bit.LockUnlockFailed) return SDI_LOCK_UNLOCK_FAILED;
		if(sR1Sta.bit.ComCrcError) return SDI_COM_CRC_ERROR;
//		if(sR1Sta.bit.IllegalCmd) return SDI_ILLEGAL_COMMAND;
		if(sR1Sta.bit.CardEccFailed) return SDI_CARD_ECC_FAILED;
		if(sR1Sta.bit.CcError) return SDI_CC_ERROR;
		if(sR1Sta.bit.Error) return SDI_ERROR;
		if(sR1Sta.bit.CsdOverWrite) return SDI_CSD_OVERWRITE;
		if(sR1Sta.bit.WpEraseSkip) return SDI_WP_ERASE_SKIP;
	}
	return SDI_OK;
}

/***************************************************************************
 * Subroutine:	SDI_GetRespR2
 * Function:	SDI get Response R2 
 * Input:		UINT32 *pCID_CSD -- 	to return the card CID for CMD2 and CMD10
 * 										or return the card CSD for CMD9;
 * 										the size of ppCID_CSD should be 4 32-bit-words										
 * Output:		SDI_ERR		
 * Description:
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_GetRespR2(UINT32 *pCID_CSD)
{
	while(!SdiRegs.STATUS.bit.RespRecOver)
	{
		if(SdiRegs.STATUS.bit.Timeout) return SDI_TIMEOUT;
	}
	if(SdiRegs.STATUS.bit.RespCrcErr) return SDI_RESP_CRC_ERR;
	pCID_CSD[3] = SdiRegs.RSP_BUF0;
	pCID_CSD[2] = SdiRegs.RSP_BUF1;
	pCID_CSD[1] = SdiRegs.RSP_BUF2;
	pCID_CSD[0] = SdiRegs.RSP_BUF3;
	
	#ifdef	SDI_DBG_PRT
	print("<<<Response R2 0x%x 0x%x 0x%x 0x%x\n", pCID_CSD[3], pCID_CSD[2], pCID_CSD[1], pCID_CSD[0]);
	#endif
	
	return SDI_OK;
}

/***************************************************************************
* Subroutine:	SDI_GetRespR3
* Function:		SDI get Response R3 
* Input:		UINT32 *pOCR -- 		to return the card OCR;
* Output:		SDI_ERR		
* Description:	the R3 is the response for ACMD41 only, no CRC check field
* Date:		2011.07.11
* ModifyRecord:
* *************************************************************************/
SDI_ERR SDI_GetRespR3(UINT32 *pOCR)
{
	while(!SdiRegs.STATUS.bit.RespRecOver)
	{
		if(SdiRegs.STATUS.bit.Timeout) return SDI_TIMEOUT;
	}
	*pOCR = SdiRegs.RSP_BUF0;
	
	#ifdef	SDI_DBG_PRT
	print("<<<Response R3 0x%x\n", *pOCR);
	#endif
	
	return SDI_OK;
}

/***************************************************************************
* Subroutine:	SDI_GetRespR6
* Function:		SDI get Response R6 
* Input:		UINT16 *pRCA -- 		to return the card RCA;
* Output:		SDI_ERR		
* Description:	the R6 is the response for CMD3 only
* Date:		2011.07.11
* ModifyRecord:
* *************************************************************************/
SDI_ERR SDI_GetRespR6(UINT16 *pRCA)
{
	UINT8 nState;
	while(!SdiRegs.STATUS.bit.RespRecOver)
	{
		if(SdiRegs.STATUS.bit.Timeout) return SDI_TIMEOUT;
	}
	if(SdiRegs.STATUS.bit.RespCrcErr) return SDI_RESP_CRC_ERR;
	*pRCA = SdiRegs.RSP_BUF0>>16;
	nState = (SdiRegs.RSP_BUF0 & 0xFF00);
	
	#ifdef	SDI_DBG_PRT
	print("<<<Response R3 0x%x\n", SdiRegs.RSP_BUF0);
	#endif
	
	if(nState&0x8000) return SDI_COM_CRC_ERROR;
	if(nState&0x4000) return SDI_ILLEGAL_COMMAND;
	if(nState&0x2000) return SDI_ERROR;
	return SDI_OK;
}

/***************************************************************************
 * Subroutine:	SDI_GetRespR7
 * Function:	SDI get Response R7 
 * Input:		UINT8 *pVolApt -- 		to return the card voltage accepted of R7;
 * Output:		SDI_ERR		
 * Description:	the R7 is the response for CMD8 only
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_GetRespR7(UINT8 *pVolApt)
{
	while(!SdiRegs.STATUS.bit.RespRecOver)
	{
		if(SdiRegs.STATUS.bit.Timeout) return SDI_TIMEOUT;
	}
	if(SdiRegs.STATUS.bit.RespCrcErr) return SDI_RESP_CRC_ERR;
	
	#ifdef	SDI_DBG_PRT
	print("<<<Response R7 0x%x\n", SdiRegs.RSP_BUF0);
	#endif
	
	*pVolApt = SdiRegs.RSP_BUF0>>8;
	return SDI_OK;
}

/***************************************************************************
 * Subroutine:	SDI_SendACmd
 * Function:	SDI send APP command
 * Input:		UINT16 nRCA -- 		to specify the card RCA;
 * 				UINT16 nCmd -- 		to specify the command;
 * 				UINT32 nArg --		to specify the argument
 * Output:		None		
 * Description:
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_SendACmd(UINT16 nRCA, UINT16 nCmd, UINT32 nArg)
{
	SDI_ERR emErr;
	// send CMD55 and get response R1
	SDI_SendCmd(APP_CMD, (UINT32)nRCA<<16);
	emErr = SDI_GetRespR1(0);
	if(emErr == SDI_OK)
	{
		SDI_SendCmd(nCmd, nArg);
	}
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_Init
 * Function:	SDI initialazation
 * Input:		UINT8 nClkDiv -- 		to specify the CLK divider;
 * Output:		SDI_ERR		
 * Description:
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_Init(UINT8 nClkDiv)
{
	SDI_ERR emErr;
	UINT8 nVolApt;
	UINT32 nOCR;
	UINT32 i;
	SD_SCR sScr;
	
	//gSdDataBuf = (UINT8 *)0x90007000;//(UINT8 *)_SDI_BUF_START_;
    SCM_EnableModClk(ID_SDI);
	SCM_ReleaseModRst(ID_SDI);	
	
	GpioRegs.GPAUSE1.bit.GPA10 = MUX_SDI;
	GpioRegs.GPAUSE1.bit.GPA11 = MUX_SDI;
	GpioRegs.GPAUSE1.bit.GPA12 = MUX_SDI;
	GpioRegs.GPAUSE1.bit.GPA13 = MUX_SDI;
	GpioRegs.GPAUSE1.bit.GPA14 = MUX_SDI;
	GpioRegs.GPAUSE1.bit.GPA15 = MUX_SDI;
		
//	SdiRegs.CLK_DIVIDER.bit.Div = 50;
	SdiRegs.CLK_DIVIDER.bit.Div = SCM_GetSysClk()/400000/2+1;
	SdiRegs.TIMEOUT = SCM_GetSysClk()/1000*TIMEOUT_MS;
	SdiRegs.CTRL.bit.BufBankSel = 0;
	SdiRegs.CTRL.bit.ClkEn = 1;
	memset((void *)&gSdmcPara, 0, sizeof(SDMC_PARA));
	
	SDI_Wait(256);
	
	// step 1: CMD0 GO_IDLE_STATE
	SDI_SendCmd(GO_IDLE_STATE, 0);
	SDI_Wait(8);
	
	// step 2: CMD8 SEND_IF_COND
	// VHS is 0x01 for SDHC, echo pattern is 0xaa recommended
	SDI_SendCmd(SEND_IF_COND, 0x000001aa);	
	emErr = SDI_GetRespR7(&nVolApt);	// should return 0x1 for SD2.0
	
	// step 3: ACMD41 SEND_OP_COND
	if(emErr == SDI_OK)		// card reponsed
	{
		if(nVolApt == 0x1)	// valid response
		{
			nOCR = 0x40000000;
			for(i=0;i<300000;i++)
			{
				SDI_SendACmd(gSdmcPara.RCA, SEND_OP_COND, nOCR | 0x40000000);
				emErr = SDI_GetRespR3(&nOCR);
				if(emErr == SDI_OK)
				{
					if(!(nOCR&0x80000000)) 
					{	
						SDI_Wait(1024);
						continue;	// SD Card not ready	
					}
					else
					{
						if(nOCR&0x40000000) gSdmcPara.Ver = SD2_0_HC;  
						else gSdmcPara.Ver = SD2_0_SC;
						break;
					}
				}
				else 
				{
					SDI_Wait(8);
					SdiRegs.CTRL.bit.ClkEn = 0;
					return emErr;
				}					
			}			
		}
		else		// voltage non-compatible
		{
			SDI_Wait(8);
			SdiRegs.CTRL.bit.ClkEn = 0;
			return SDI_VOL_UNAPT;
		}
	}
	else if(emErr == SDI_TIMEOUT)
	{
		for(i=0;i<30000;i++)
		{
			nOCR = 0x00FF8000;
			emErr = SDI_SendACmd(gSdmcPara.RCA, SEND_OP_COND, nOCR&(~0x40000000));
			if(emErr == SDI_OK) emErr = SDI_GetRespR3(&nOCR);
			if(emErr == SDI_OK)
			{
				if(!(nOCR&0x80000000)) continue;	// SD Card not ready	
				else
				{
					gSdmcPara.Ver = SD1_X;  
					break;
				}
			}
			else 
			{
				SDI_Wait(8);
				SdiRegs.CTRL.bit.ClkEn = 0;
				return emErr;
			}
		}
	}
	else 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
			
	// step 4: CMD2 ALL_SEND_CID
	// get CID Register
	SDI_SendCmd(ALL_SEND_CID, 0x0);	
	emErr = SDI_GetRespR2(gSdmcPara.CID.Word);	
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
	
	// step 5: CMD3 SEND_RELATIVE_ADDR
	SDI_SendCmd(SEND_RELATIVE_ADDR, 0x0);	
	emErr = SDI_GetRespR6(&gSdmcPara.RCA);
	if(emErr != SDI_OK)
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
	
	// step 6: CMD9 SEND_CSD
	// to calculate capacity according CSD register
	emErr = SDI_ReadCSD(&gSdmcPara.CSD);
	if(emErr != SDI_OK) return emErr;
	if(gSdmcPara.Ver == SD2_0_SC || gSdmcPara.Ver == SD1_X)	
	{
		// standard card capacity = (C_SIZE+1) * 2^(C_SIZE_MULT+2) * 2^RD_BL_LEN
		//gSdmcPara.Capacity = ((UINT32)(gSdmcPara.CSD.V10bit.C_SIZE+1)<<(gSdmcPara.CSD.V10bit.C_SIZE_MULT+2+gSdmcPara.CSD.V10bit.READ_BL_LEN))>>20;
		gSdmcPara.Capacity = ((UINT32)(((((gSdmcPara.CSD.Word[2] & 0x3FF)<<2)|(gSdmcPara.CSD.Word[1]>>30))+1)<<(((gSdmcPara.CSD.Word[1]>>15) & 0x7)+2+((gSdmcPara.CSD.Word[2]>>16) & 0xF))))>>20;
	}
	else 
	{
		// high capacity card capacity = (C_SIZE+1)*512K
		//gSdmcPara.Capacity = (gSdmcPara.CSD.V20bit.C_SIZE+1)>>1;	
		gSdmcPara.Capacity = ((gSdmcPara.CSD.Word[1]>>16 | (gSdmcPara.CSD.Word[2] & 0x3F))+1)>>1;	
	}
		
	// step 7: CMD7 SELECT_DESELECT_CARD
	// to enter card into transfer state (tran)
	emErr = SDI_SelectCard();	
	if(emErr != SDI_OK) return emErr;
	
	emErr = SDI_SetBlockLen(512);
	if(emErr != SDI_OK) return emErr;
	
	// step 8: ACMD6 SET_BUS_WIDTH
	// to set bus width for 4 bit
	emErr = SDI_Set4BitBusWidth();
	if(emErr != SDI_OK) return emErr;
	
	// step 9: ACMD51 SEND_SCR
	emErr = SDI_ReadSCR(&sScr);
	if(emErr != SDI_OK) return emErr;
	if(sScr.bit.SdSpec>0)
	{
		// CMD6
		emErr = SDI_Switch2HighSpeed();
		/*
		if(emErr == SDI_OK)
		{
			if(nClkDiv == 0) SdiRegs.CLK_DIVIDER.bit.Div = 0;
			i = (nClkDiv+1)*2*256;
			while(i--);
		}
		*/
	}
	else emErr = SDI_HS_UNSUPPORT;
	if(emErr == SDI_OK)
		SdiRegs.CLK_DIVIDER.bit.Div = nClkDiv;
	else if(!nClkDiv) SdiRegs.CLK_DIVIDER.bit.Div = 1;
	else SdiRegs.CLK_DIVIDER.bit.Div = nClkDiv;
	SdiRegs.CTRL.bit.ClkEn = 0;
	if(emErr == SDI_HS_UNSUPPORT) emErr = SDI_OK; 
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_ReadCSD
 * Function:	to read CSD register by assert CMD9
 * Input:		SD_CSD *pCSD -- 		to return CSD register value;
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_ReadCSD(SD_CSD *pCSD)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(SEND_CSD, gSdmcPara.RCA<<16);	
	emErr = SDI_GetRespR2((void*)pCSD);	
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_ReadCID
 * Function:	to read CID register by assert CMD10
 * Input:		SD_CSD *pCSD -- 		to return CSD register value;
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_ReadCID(SD_CID *pCID)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(SEND_CID, gSdmcPara.RCA<<16);	
	emErr = SDI_GetRespR2((void*)pCID);	
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_SelectCard
 * Function:	to select card  by assert CMD7
 * Input:		None
 * Output:		SDI_ERR		
 * Description: the CMD7 only response by selected card with busy
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_SelectCard(void)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(SELECT_DESELECT_CARD, gSdmcPara.RCA<<16);	
	emErr = SDI_GetRespR1(1);
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_DeselectCard
 * Function:	to de-select card  by assert CMD7, no response
 * Input:		None
 * Output:		None		
 * Description: 
 * Date:		2011.07.21
 * ModifyRecord:
 * *************************************************************************/
void SDI_DeselectCard(void)
{
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(SELECT_DESELECT_CARD, 0);		// parameter is not RCA, RCA=0 deselect all	
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
}

/***************************************************************************
 * Subroutine:	SDI_Set4BitBusWidth
 * Function:	to set bus width to 4 bit  by assert ACMD6
 * Input:		None
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.11
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_Set4BitBusWidth(void)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.ClkEn = 1;
	emErr = SDI_SendACmd(gSdmcPara.RCA, SET_BUS_WIDTH, 0x2);
	if(emErr == SDI_OK)
	{
		emErr = SDI_GetRespR1(0);
	}
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_GetNumOfWrBlk
 * Function:	to send number of write block after mul-write  by assert ACMD22
 * Input:		UINT32 *pNum --  to specify the buffer for return the number
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2014.03.12
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_GetNumOfWrBlk(UINT32 *pNum)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.ClkEn = 1;
	emErr = SDI_SendACmd(gSdmcPara.RCA, SEND_NUM_WR_BLOCKS, 0x0);
	if(emErr == SDI_OK)
	{
		emErr = SDI_GetRespR1(0);
		if(emErr == SDI_OK)
		{
			while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
			*pNum = (*SDI_DAT_BUF<<24 | *(SDI_DAT_BUF+1)<<16 | *(SDI_DAT_BUF+2)<<8 | *(SDI_DAT_BUF+3));
		}
	}
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return emErr;
}


/***************************************************************************
 * Subroutine:	SDI_ReadBlock
 * Function:	to read single block by assert CMD17
 * Input:		UINT32 nBlkAddr --		to specify the block address (512 byte);
 * 				UINT8 *pBuf --			to specify the data buf for storing block data;
 * 										the buffer size should be a block length;
 * 										if pBuf = NULL, the data will keep in SDI data buffer; 
 * Output:		SDI_ERR		
 * Description: The specified block data will be transfered from card to SDI data buffer;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried to pBuf, too.
 * Date:		2011.07.12
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_ReadBlock(UINT32 nBlkAddr, UINT8 *pBuf)
{
	SDI_ERR emErr;
	if(gSdmcPara.Ver != SD2_0_HC) nBlkAddr <<= 9;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SdiRegs.CTRL.bit.BufBankSel  = 0;
	SDI_SendCmd(READ_SINGLE_BLOCK, nBlkAddr);	
	emErr = SDI_GetRespR1(0);
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}	
	while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
	if(SdiRegs.STATUS.bit.DatCrcErr) 
	{
		return SDI_DATA_CRC_ERR;		// receive data CRC error
	}
	if(pBuf)	// pBuf is not NULL
	{
		memcpy(pBuf, SDI_DAT_BUF, SdiRegs.BLK_LEN);
		//VUINT32 *p = (VUINT32 *)0xFC008200;
		//UINT32 *q = (UINT32 *)pBuf;
		//for(UINT32 i=0;i<512/4;i++) q[i] = p[i];
	}
	SDI_FlushDatBuf();
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_ReadMultiBlock1st
 * Function:	to read  multi-block by assert CMD18 for the first nBlkNum block
 * Input:		UINT32 nBlkAddr --		to specify the first block address (512 byte);
 * 				UINT32 nBlkNum -- 		to specify the number for the first read block;
 * 				UINT8 *pBuf --			to specify the data buf for storing block data;
 * 										the buffer size should be nBlkNum block length;
 * 										if pBuf = NULL, the last block data will keep in SDI data buffer; 
 * Output:		SDI_ERR		
 * Description: The specified block data will be transfered from card to SDI data buffer;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried to pBuf, too.
 * Date:		2011.08.04
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_ReadMultiBlock1st(UINT32 nBlkAddr, UINT32 nBlkNum, UINT8 *pBuf)
{
	SDI_ERR emErr;
	
	if(gSdmcPara.Ver != SD2_0_HC) nBlkAddr <<= 9;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SdiRegs.CTRL.bit.BufBankSel  = 0;
	SDI_SendCmd(READ_MULTIPLE_BLOCK, nBlkAddr);	
	emErr = SDI_GetRespR1(0);
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}	
	while(nBlkNum--)
	{
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
		if(SdiRegs.STATUS.bit.DatCrcErr) 
		{
			SdiRegs.CTRL.bit.BufBankSel  = 0;
			return SDI_DATA_CRC_ERR;		// receive data CRC error
		}
		
		if(nBlkNum) 
		{
			SdiRegs.CTRL.bit.BufBankSel += 1;
			SDI_Wait(1);
			SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
			SdiRegs.CTRL.bit.ClkEn = 1;
			
		}
	}
	if(pBuf)	// pBuf is not NULL
	{
		memcpy(pBuf, SDI_DAT_BUF, SdiRegs.BLK_LEN*(SdiRegs.CTRL.bit.BufBankSel+1));
	}
	SdiRegs.CTRL.bit.BufBankSel = 0;
	SDI_FlushDatBuf();
	SDI_Wait(1);
	return SDI_OK;
}

/***************************************************************************
* Subroutine:	SDI_ReadMultiBlockNxt
* Function:		to read  multi-block for the next nBlkNum block without assert command
* Input:		UINT32 nBlkNum -- 		to specify the number for the next read block;
* 				UINT8 *pBuf --			to specify the data buf for storing block data;
* 										the buffer size should be nBlkNum block length;
* 										if pBuf = NULL, the last block data will keep in SDI data buffer; 
* Output:		SDI_ERR		
* Description: The specified block data will be transfered from card to SDI data buffer;
* 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
* 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
* 				if the parameter pBuf is not NULL, the data will be carried to pBuf, too.
* Date:		2011.08.04
* ModifyRecord:
* *************************************************************************/
SDI_ERR SDI_ReadMultiBlockNxt(UINT32 nBlkNum, UINT8 *pBuf)
{
	SdiRegs.CTRL.bit.BufBankSel  = 0;
	while(nBlkNum--)
	{
		SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
		SdiRegs.CTRL.bit.ClkEn = 1;
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
		if(SdiRegs.STATUS.bit.DatCrcErr) 
		{
			SdiRegs.CTRL.bit.BufBankSel = 0;
			SDI_Wait(1);
			return SDI_DATA_CRC_ERR;		// receive data CRC error
		}
		if(nBlkNum)
		{
			SdiRegs.CTRL.bit.BufBankSel += 1;
			SDI_Wait(1);
		}
	}
	if(pBuf)	// pBuf is not NULL
	{
		//GpioRegs.GPADATA.bit.GPA0 = 1;
		memcpy(pBuf, SDI_DAT_BUF, SdiRegs.BLK_LEN*(SdiRegs.CTRL.bit.BufBankSel+1));
		//GpioRegs.GPADATA.bit.GPA0 = 0;
	}
	SdiRegs.CTRL.bit.BufBankSel = 0;
	SDI_FlushDatBuf();
	SDI_Wait(1);
	return SDI_OK;
}

/***************************************************************************
* Subroutine:	SDI_StopTransmission
* Function:		to stop multi-block read or write by assert CMD12
* Input:		None 
* Output:		SDI_ERR		
* Description:  the command must be assert after multi-block read and write operation
* Date:		2011.08.04
* ModifyRecord:
* *************************************************************************/
SDI_ERR SDI_StopTransmission(void)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(STOP_TRANSMISSION, 0);	
	emErr = SDI_GetRespR1(1);
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return emErr;
}


/***************************************************************************
 * Subroutine:	SDI_WriteBlock
 * Function:	to write single block by assert CMD24
 * Input:		UINT32 nBlkAddr --		to specify the block address (512 byte);
 * 				UINT8 *pBuf --			to specify the data buf to be written;
 * 										the buffer size should be a block length;
 * 										if pBuf = NULL, indicates that the data to be written 
 * 										have been kept in SDI data buffer for transfer; 
 * Output:		SDI_ERR		
 * Description: The specified block data will be transfered from SDI data buffer to card;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried from pBuf to 
 * 				the SDI data buffer for transfer.
 * Date:		2011.07.12
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_WriteBlock(UINT32 nBlkAddr, UINT8 *pBuf)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.BufBankSel  = 0;
	if(gSdmcPara.Ver != SD2_0_HC) nBlkAddr <<= 9;
	if(pBuf) memcpy(SDI_DAT_BUF, pBuf, SdiRegs.BLK_LEN);
	SDI_FlushDatBuf();
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(WRITE_BLOCK, nBlkAddr);	
	emErr = SDI_GetRespR1(0);
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
	while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over(busy wait done)
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	switch(SdiRegs.CRC_TOKEN.bit.Status)	// write block CRC status token response
	{
		case 0x7:	return SDI_FLASH_PROG_ERR;
		case 0x2:	return SDI_OK;
		case 0x5:	return SDI_WR_TRANS_ERR;
		default:	return SDI_CRC_TOKEN_ERR;
	}
}

/***************************************************************************
 * Subroutine:	SDI_WriteMultiBlock1st
 * Function:	to write multi-block by assert CMD25 for the first nBlkNum block
 * Input:		UINT32 nBlkAddr --		to specify the block address (512 byte);
 * 				UINT32 nBlkNum -- 		to specify the number for the first write block; 
 * 				UINT8 *pBuf --			to specify the data buf to be written;
 * 										the buffer size should be nBlkNum block length;
 * 										if pBuf = NULL, indicates that the data to be written 
 * 										have been kept in SDI data buffer for transfer; 
 * Output:		SDI_ERR		
 * Description: The specified block data will be transfered from SDI data buffer to card;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried from pBuf to 
 * 				the SDI data buffer for transfer.
 * Date:		2011.07.12
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_WriteMultiBlock1st(UINT32 nBlkAddr, UINT32 nBlkNum, UINT8 *pBuf)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.BufBankSel  = 0;
	if(gSdmcPara.Ver != SD2_0_HC) nBlkAddr <<= 9;
	if(pBuf)
	{
		memcpy(SDI_DAT_BUF, pBuf, SdiRegs.BLK_LEN*nBlkNum);
	}
	SDI_FlushDatBuf();
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(WRITE_MULTIPLE_BLOCK, nBlkAddr);	
	emErr = SDI_GetRespR1(0);
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
	do
	{
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over(busy wait done)
		switch(SdiRegs.CRC_TOKEN.bit.Status)	// write block CRC status token response
		{
			case 0x7:	emErr =  SDI_FLASH_PROG_ERR; break;
			case 0x2:	emErr =  SDI_OK; break;
			case 0x5:	emErr =  SDI_WR_TRANS_ERR; break;
			default:	emErr =  SDI_CRC_TOKEN_ERR; break;
		}
		if(emErr != SDI_OK)
		{
			SdiRegs.CTRL.bit.BufBankSel = 0;
			SDI_Wait(1);
			SdiRegs.CTRL.bit.ClkEn = 0;
			return emErr;
		}
		if(--nBlkNum)
		{
			SdiRegs.CTRL.bit.BufBankSel += 1;
			SDI_Wait(1);
			SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
//			SDI_Wait(256);
			while(SdiRegs.CTRL.bit.NxtDatStart);
		}
	}while(nBlkNum);
	SdiRegs.CTRL.bit.BufBankSel = 0;
	SDI_Wait(1);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return SDI_OK;
}

/***************************************************************************
* Subroutine:	SDI_WriteMultiBlockNxt
* Function:		to write multi-block for the next nBlkNum block without assert command
* Input:		UINT32 nBlkNum -- 		to specify the number for the next wrtie block;
* 				UINT8 *pBuf --			to specify the data buf to be written;
* 										the buffer size should be nBlkNum block length;
* 										if pBuf = NULL, the last block data will keep in SDI data buffer; 
* Output:		SDI_ERR		
* Description: 	The specified block data will be transfered from SDI data buffer to card;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried from pBuf to 
 * 				the SDI data buffer for transfer.
* Date:		2011.08.04
* ModifyRecord:
* *************************************************************************/
SDI_ERR SDI_WriteMultiBlockNxt(UINT32 nBlkNum, UINT8 *pBuf)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.BufBankSel  = 0;
	SdiRegs.CTRL.bit.ClkEn = 1;
	if(pBuf)	// pBuf is not NULL
	{
		//GpioRegs.GPADATA.bit.GPA0 = 1;
		memcpy(SDI_DAT_BUF, pBuf, SdiRegs.BLK_LEN*nBlkNum);
		//GpioRegs.GPADATA.bit.GPA0 = 0;
	}
	SDI_FlushDatBuf();
	while(nBlkNum--)
	{
		SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
//		SDI_Wait(2560);
		while(SdiRegs.CTRL.bit.NxtDatStart);
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over(busy wait done)
		switch(SdiRegs.CRC_TOKEN.bit.Status)	// write block CRC status token response
		{
			case 0x7:	emErr =  SDI_FLASH_PROG_ERR; break;
			case 0x2:	emErr =  SDI_OK; break;
			case 0x5:	emErr =  SDI_WR_TRANS_ERR; break;
			default:	emErr =  SDI_CRC_TOKEN_ERR; break;
		}
		if(emErr != SDI_OK)
		{
			SdiRegs.CTRL.bit.BufBankSel = 0;
			SDI_Wait(1);
			SdiRegs.CTRL.bit.ClkEn = 0;
			return emErr;
		}
		SdiRegs.CTRL.bit.BufBankSel += 1;
		SDI_Wait(1);
	}
	SdiRegs.CTRL.bit.BufBankSel = 0;
	SDI_Wait(1);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return SDI_OK;
}

/***************************************************************************
 * Subroutine:	SDI_EraseBlock
 * Function:	to erase blocks from nBlkAddrStt to nBlkAddrEnd by assert 
 * 				CMD32 CMD33 CMD38 orderly.
 * Input:		UINT32 nBlkAddrStt --	to specify the start block address (512 byte);
 * 				UINT32 nBlkAddrEnd --	to specify the end block address (512 byte);
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.14
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_EraseBlock(UINT32 nBlkAddrStt, UINT32 nBlkAddrEnd)
{
	SDI_ERR emErr;
	if(gSdmcPara.Ver != SD2_0_HC)
	{
		nBlkAddrStt <<= 9;
		nBlkAddrEnd <<= 9;
	}
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(ERASE_WR_BLK_START, nBlkAddrStt);	
	emErr = SDI_GetRespR1(0);
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
	
	SDI_SendCmd(ERASE_WR_BLK_END, nBlkAddrEnd);	
	emErr = SDI_GetRespR1(0);	
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
	
	SDI_SendCmd(ERASE, 0);	
	emErr = SDI_GetRespR1(1);
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_ReadStatus
 * Function:	to read Status by assert CMD13
 * Input:		UINT32 *pR1Sta -- 		to return the card status of R1; 
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.14
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_ReadStatus(UINT32 *pR1Sta)
{
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(SEND_STATUS, gSdmcPara.RCA<<16);
	while(!SdiRegs.STATUS.bit.RespRecOver)
	{
		if(SdiRegs.STATUS.bit.Timeout) 
		{
			SdiRegs.CTRL.bit.ClkEn = 0;
			return SDI_TIMEOUT;
		}
	}
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	if(SdiRegs.STATUS.bit.RespCrcErr) return SDI_RESP_CRC_ERR;
	*pR1Sta = SdiRegs.RSP_BUF0;
	return SDI_OK;
}

/***************************************************************************
 * Subroutine:	SDI_GoInactiveState
 * Function:	to send the card into th inactive state by assert 
 * Input:		None
 * Output:		None		
 * Description: the operation is used when the host exolicitly wants to deactive a card.
 * Date:		2011.07.14
 * ModifyRecord:
 * *************************************************************************/
void SDI_GoInactiveState(void)
{
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(GO_INACTIVE_STATE, gSdmcPara.RCA<<16);	
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
}

/***************************************************************************
 * Subroutine:	SDI_SetBlockLen
 * Function:	to set block length by assert CMD16
 * Input:		UINT32 nBlkLen -- 		to specify the block length; 
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.14
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_SetBlockLen(UINT32 nBlkLen)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(SET_BLOCKLEN, nBlkLen);	
	emErr = SDI_GetRespR1(0);
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	if(emErr == SDI_OK) SdiRegs.BLK_LEN = nBlkLen;
	return emErr;
}


/***************************************************************************
 * Subroutine:	SDI_ReadSCR
 * Function:	to read SCR register by assert ACMD51
 * Input:		SD_SCR *pSCR -- 		to return SCR; 
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.26
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_ReadSCR(SD_SCR *pSCR)
{
	SDI_ERR emErr;
	UINT32 nBlkLen = SdiRegs.BLK_LEN;
	SdiRegs.BLK_LEN = 8;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendACmd(gSdmcPara.RCA, SEND_SCR, 0);
	emErr = SDI_GetRespR1(0);
	if(emErr == SDI_OK)
	{
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
		if(SdiRegs.STATUS.bit.DatCrcErr) emErr=SDI_DATA_CRC_ERR;		// receive data CRC error
		else pSCR->Word = (*SDI_DAT_BUF<<24 | *(SDI_DAT_BUF+1)<<16 | *(SDI_DAT_BUF+2)<<8 | *(SDI_DAT_BUF+3));
	}
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	SdiRegs.BLK_LEN = nBlkLen;	
	return emErr;
}

/***************************************************************************
 * Subroutine:	SDI_Switch2HighSpeed
 * Function:	to switch card to high speed mode by assert CMD6
 * Input:		None
 * Output:		SDI_ERR		
 * Description: 
 * Date:		2011.07.26
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR	SDI_Switch2HighSpeed(void)
{
	SDI_ERR emErr;
	UINT32 nBlkLen = SdiRegs.BLK_LEN;
	SdiRegs.BLK_LEN = 64;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(SWITCH_FUNC, 0x80FFFFF1);	
	emErr = SDI_GetRespR1(0);
	if(emErr == SDI_OK)
	{
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
		if(SdiRegs.STATUS.bit.DatCrcErr) emErr=SDI_DATA_CRC_ERR;		// receive data CRC error
		else 
		{
			if((*(SDI_DAT_BUF+16) & 0xF) == 0x1) emErr = SDI_OK;
			else emErr = SDI_HS_UNSUPPORT;
		}
	}
	SDI_Wait(8);
	SdiRegs.CTRL.bit.ClkEn = 0;
	SdiRegs.BLK_LEN = nBlkLen;	
	return emErr;
}


/***************************************************************************
 * Subroutine:	SDI_ReadMultiBlockWithCmd
 * Function:	to read  multi-block by assert CMD18 for the first nBlkNum block
 * Input:		UINT32 nBlkAddr --		to specify the first block address (512 byte);
 * 				UINT32 nBlkNum -- 		to specify the number for the first read block;
 * 				UINT8 *pBuf --			to specify the data buf for storing block data;
 * 										the buffer size should be nBlkNum block length;
 * 										if pBuf = NULL, the last block data will keep in SDI data buffer; 
 * Output:		SDI_ERR		
 * Description: The specified block data will be transfered from card to SDI data buffer;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried to pBuf, too.
 * Date:		2011.08.04
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_ReadMultiBlockWithCmd(UINT32 nBlkAddr, UINT8 nBuffBank, UINT32 nBlkNum)
{
	SDI_ERR emErr;
	
	if(gSdmcPara.Ver != SD2_0_HC) nBlkAddr <<= 9;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SdiRegs.CTRL.bit.BufBankSel  = nBuffBank;
	SDI_SendCmd(READ_MULTIPLE_BLOCK, nBlkAddr);	
	emErr = SDI_GetRespR1(0);
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}	
	while(nBlkNum--)
	{
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
		if(SdiRegs.STATUS.bit.DatCrcErr) 
		{
			return SDI_DATA_CRC_ERR;		// receive data CRC error
		}
		
		if(nBlkNum) 
		{
			SdiRegs.CTRL.bit.BufBankSel += 1;
			SDI_Wait(1);
			SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
			SdiRegs.CTRL.bit.ClkEn = 1;
			
		}
	}
	SDI_FlushDatBuf();
	SDI_Wait(1);
	return SDI_OK;
}

/***************************************************************************
* Subroutine:	SDI_ReadMultiBlockNoCmd
* Function:		to read  multi-block for the next nBlkNum block without assert command
* Input:		UINT32 nBlkNum -- 		to specify the number for the next read block;
* 				UINT8 *pBuf --			to specify the data buf for storing block data;
* 										the buffer size should be nBlkNum block length;
* 										if pBuf = NULL, the last block data will keep in SDI data buffer; 
* Output:		SDI_ERR		
* Description: The specified block data will be transfered from card to SDI data buffer;
* 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
* 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
* 				if the parameter pBuf is not NULL, the data will be carried to pBuf, too.
* Date:		2011.08.04
* ModifyRecord:
* *************************************************************************/
SDI_ERR SDI_ReadMultiBlockNoCmd(UINT8 nBuffBank, UINT32 nBlkNum)
{
	SdiRegs.CTRL.bit.BufBankSel  = nBuffBank;
	while(nBlkNum--)
	{
		SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
		SdiRegs.CTRL.bit.ClkEn = 1;
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over
		if(SdiRegs.STATUS.bit.DatCrcErr) 
		{
			SDI_Wait(1);
			return SDI_DATA_CRC_ERR;		// receive data CRC error
		}
		if(nBlkNum)
		{
			SdiRegs.CTRL.bit.BufBankSel += 1;
			SDI_Wait(1);
		}
	}
	SDI_FlushDatBuf();
	SDI_Wait(1);
	return SDI_OK;
}

/***************************************************************************
 * Subroutine:	SDI_WriteMultiBlock1st
 * Function:	to write multi-block by assert CMD25 for the first nBlkNum block
 * Input:		UINT32 nBlkAddr --		to specify the block address (512 byte);
 * 				UINT32 nBlkNum -- 		to specify the number for the first write block; 
 * 				UINT8 *pBuf --			to specify the data buf to be written;
 * 										the buffer size should be nBlkNum block length;
 * 										if pBuf = NULL, indicates that the data to be written 
 * 										have been kept in SDI data buffer for transfer; 
 * Output:		SDI_ERR		
 * Description: The specified block data will be transfered from SDI data buffer to card;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried from pBuf to 
 * 				the SDI data buffer for transfer.
 * Date:		2011.07.12
 * ModifyRecord:
 * *************************************************************************/
SDI_ERR SDI_WriteMultiBlockWithCmd(UINT32 nBlkAddr, UINT8 nBuffBank, UINT32 nBlkNum)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.BufBankSel  = nBuffBank;
	if(gSdmcPara.Ver != SD2_0_HC) nBlkAddr <<= 9;
	SDI_FlushDatBuf();
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_SendCmd(WRITE_MULTIPLE_BLOCK, nBlkAddr);	
	emErr = SDI_GetRespR1(0);
	if(emErr != SDI_OK) 
	{
		SDI_Wait(8);
		SdiRegs.CTRL.bit.ClkEn = 0;
		return emErr;
	}
	do
	{
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over(busy wait done)
		switch(SdiRegs.CRC_TOKEN.bit.Status)	// write block CRC status token response
		{
			case 0x7:	emErr =  SDI_FLASH_PROG_ERR; break;
			case 0x2:	emErr =  SDI_OK; break;
			case 0x5:	emErr =  SDI_WR_TRANS_ERR; break;
			default:	emErr =  SDI_CRC_TOKEN_ERR; break;
		}
		if(emErr != SDI_OK)
		{
			SDI_Wait(1);
			SdiRegs.CTRL.bit.ClkEn = 0;
			return emErr;
		}
		if(--nBlkNum)
		{
			SdiRegs.CTRL.bit.BufBankSel += 1;
			SDI_Wait(1);
			SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
//			SDI_Wait(256);
			while(SdiRegs.CTRL.bit.NxtDatStart);
		}
	}while(nBlkNum);
	SDI_Wait(1);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return SDI_OK;
}

/***************************************************************************
* Subroutine:	SDI_WriteMultiBlockNxt
* Function:		to write multi-block for the next nBlkNum block without assert command
* Input:		UINT32 nBlkNum -- 		to specify the number for the next wrtie block;
* 				UINT8 *pBuf --			to specify the data buf to be written;
* 										the buffer size should be nBlkNum block length;
* 										if pBuf = NULL, the last block data will keep in SDI data buffer; 
* Output:		SDI_ERR		
* Description: 	The specified block data will be transfered from SDI data buffer to card;
 * 				the SDI data buffer(global variable gSdDataBuf points to) is located in 
 * 				0x90007200 DCCM space or 0xFC008200 BVCI Peripheral space;
 * 				if the parameter pBuf is not NULL, the data will be carried from pBuf to 
 * 				the SDI data buffer for transfer.
* Date:		2011.08.04
* ModifyRecord:
* *************************************************************************/
SDI_ERR SDI_WriteMultiBlockNoCmd(UINT8 nBuffBank, UINT32 nBlkNum)
{
	SDI_ERR emErr;
	SdiRegs.CTRL.bit.BufBankSel  = nBuffBank;
	SdiRegs.CTRL.bit.ClkEn = 1;
	SDI_FlushDatBuf();
	while(nBlkNum--)
	{
		SdiRegs.CTRL.bit.NxtDatStart = 1;		// next block data start
//		SDI_Wait(2560);
		while(SdiRegs.CTRL.bit.NxtDatStart);
		while(!SdiRegs.STATUS.bit.DataTranOver); // wait data transfer over(busy wait done)
		switch(SdiRegs.CRC_TOKEN.bit.Status)	// write block CRC status token response
		{
			case 0x7:	emErr =  SDI_FLASH_PROG_ERR; break;
			case 0x2:	emErr =  SDI_OK; break;
			case 0x5:	emErr =  SDI_WR_TRANS_ERR; break;
			default:	emErr =  SDI_CRC_TOKEN_ERR; break;
		}
		if(emErr != SDI_OK)
		{
			SDI_Wait(1);
			SdiRegs.CTRL.bit.ClkEn = 0;
			return emErr;
		}
		SdiRegs.CTRL.bit.BufBankSel += 1;
		SDI_Wait(1);
	}
	SDI_Wait(1);
	SdiRegs.CTRL.bit.ClkEn = 0;
	return SDI_OK;
}



