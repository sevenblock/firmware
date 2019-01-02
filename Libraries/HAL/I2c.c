/***************************************************************************
* File name    :	I2c.c
* Function     :	The functions for I2c
* Author       : 	Leixj
* Date         :	2014/11/26
* Version      :    v1.0
* Description  :    I2c Hardware Abstract Layer compatible with AS569
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
#if(I2C_SLAVE)
static STRUCT_FIFO gI2cTxFIFO[I2C_NUM];	// the global for I2c transmitting
static STRUCT_FIFO gI2cRxFIFO[I2C_NUM];	// the global for I2c receiving
#endif

#if(I2C_DMA)
static UINT8 gI2cTxDmaChn;
static UINT8 gI2cRxDmaChn;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern volatile uint32_t SystemFrequency;
static volatile UINT32 gWaitClk;

/**************************************************************************
* Local Functon Declaration
***************************************************************************/
#if(I2C_SLAVE)
void I2C_SetRxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size);
UINT16 I2C_GetRxFIFOValidSize(UINT8 bI2cNum);
void I2C_SetTxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size);
UINT16 I2C_GetTxFIFOValidSize(UINT8 bI2cNum);
void I2C_InitTxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pTxBuf);
void I2C_InitRxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pRxBuf);
#endif

/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	I2C_PortInit
 * Function:	I2C Port initialization  
 * Input:		I2C_MAP mPortMap --	to specify the I2C port map to. 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012/12/21
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_PortInit(I2C_MAP mPortMap)
{
	switch(mPortMap)
	{
		case I2C_MAP_A1_A2:
			// to select Pins for I2C signals
			GpioRegs.GPAUSE0.bit.GPA1 = MUX_I2C;	// I2C_SCL
   			GpioRegs.GPAUSE0.bit.GPA2 = MUX_I2C;	// I2C_SDA
			break;
		case I2C_MAP_A5_A6:
			// to select Pins for I2C signals
			GpioRegs.GPAUSE0.bit.GPA5 = MUX_I2C;	// I2C_SCL
   			GpioRegs.GPAUSE0.bit.GPA6 = MUX_I2C;	// I2C_SDA
			break;
		case I2C_MAP_A16_A21:
			// to select Pins for I2C signals
			GpioRegs.GPAUSE2.bit.GPA16 = MUX_I2C;	// I2C_SCL
   			GpioRegs.GPAUSE2.bit.GPA21 = MUX_I2C;	// I2C_SDA
			break;	
		default:
			return RT_FAIL;
	}
	return RT_OK;	
}

/***************************************************************************
 * Subroutine:	I2C_GetRegs
 * Function:	Get I2c register file address
 * Input:		UINT8 bI2cNum -- 	I2c number, I2C0 only
 * 								if it is out of range, dead loops.
 * Output:		volatile struct I2C_REGS * --	Return I2c register file address
 * Description:	Get I2C register file address.
 * Date:		2012.07.26
 * ModifyRecord:
 * *************************************************************************/
volatile struct I2C_REGS * I2C_GetRegs(UINT8 bI2cNum)
{
	if(bI2cNum == I2C0)
		return &I2C0Regs;
	else
		return NULL;
}

/***************************************************************************
 * Subroutine:	I2C_Init
 * Function:	I2C mode initialization
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0.
 *				I2C_CLK_SRC nClkSrc --
 *									to set I2C clock, this clock for baudrate generator
 *									I2C_CLK_SRC_FCLK, I2C_CLK_SRC_HFROSC, I2C_CLK_SRC_MFROSC, I2C_CLK_SRC_XTAL
 *				I2C_MAP mPortMap --	Select which group of GPIO port as I2C
 *									I2C_MAP_A1_A2	--GPIA[1]&GPIOA[2]
 *									I2C_MAP_A5_A6	--GPIA[5]&GPIOA[6]
 *									I2C_MAP_A16_A21	--GPIA[16]&GPIOA[21]
 * 				STRUCT_I2C_FORMAT sFrame --		I2C frame format;
 * 				STRUCT_I2C_DATA_BUF *pRxBuf --
 * 									I2C receive buffer, to specify the buffer and size for slave receive;
 * 				STRUCT_I2C_DATA_BUF *pTxBuf --
 * 									I2C transmit buffer, to specify the buffer and size for slave transmit;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL or RT_PARAM_ERR;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT8 I2C_Init(UINT8 bI2cNum, I2C_CLK_SRC nClkSrc, I2C_MAP mPortMap, STRUCT_I2C_FORMAT sFrame,\
	STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf)
{			
	volatile struct I2C_REGS * pI2c;
	UINT32 nI2cClk;

	if(bI2cNum == I2C0)
	{
		//open global control
		SCM_DisableModClk(ID_I2C0);// disable I2C0 clock
		SCM_KeepModRst(ID_I2C0);// reset
		SCM_EnableModClk(ID_I2C0);// enable I2C0 clock
		SCM_ReleaseModRst(ID_I2C0);// Relinquish from reset		
	   	//select Pins
		if(I2C_PortInit(mPortMap) != RT_OK) return RT_FAIL;
	}
	else return RT_PARAM_ERR;

	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;

	pI2c->ENABLE.bit.Enable = 0;	 //disable
	pI2c->INTR_MASK.all = 0x0000;	//disable all interrupt

	if(SCM_SetI2cClk(bI2cNum, nClkSrc) != RT_OK) return RT_FAIL; //set I2c clock
	nI2cClk = SCM_GetI2cClk(bI2cNum);

	switch(sFrame.bMasterEn)
	{
		case MASTER:
			pI2c->CON.bit.MasterMode = 1;	 //Master Enable
			pI2c->CON.bit.SlaveDis = 1;	 //Slave Disable

			if(sFrame.bAddrBits == 7)
				pI2c->TAR.all &= ~(1<<12);//I2C0Regs.TAR.bit.BitAddrMaster = 0;
			else if(sFrame.bAddrBits == 10)
				pI2c->TAR.all |= 1<<12;	  //address bit at master mode
			else return RT_FAIL;

			if(sFrame.lSckFreq <= 100000L) 
			{
				pI2c->CON.bit.Speed = 0x1;	  	//standard
				pI2c->SS_SCL_HCNT = nI2cClk/(2*sFrame.lSckFreq);	//minimum value is 6 
				pI2c->SS_SCL_LCNT = nI2cClk/(2*sFrame.lSckFreq);
			}
			else if((sFrame.lSckFreq > 100000L) && (sFrame.lSckFreq <= 1000000L))
			{
				pI2c->CON.bit.Speed = 0x2;		   //fast mode(<=400K) or fast mode plus(<=1M)		
				pI2c->FS_SCL_HCNT = nI2cClk/(2*sFrame.lSckFreq);	//minimum value is 6 
				pI2c->FS_SCL_LCNT = nI2cClk/(2*sFrame.lSckFreq);	//minimum value is 8 
				if(sFrame.lSckFreq > 400000L)
				{
					pI2c->FS_SPKLEN = 1;	 //
					pI2c->SDA_SETUP = 2;	//minimum value is 2
				}
			}
			else if(sFrame.lSckFreq > 1000000L)
			{
				pI2c->CON.bit.Speed = 0x3;		   //high speed mode 		
				pI2c->HS_SCL_HCNT = nI2cClk/(2*sFrame.lSckFreq);	//minimum value is 6 
				pI2c->HS_SCL_LCNT = nI2cClk/(2*sFrame.lSckFreq);
			}
			else return RT_FAIL;
								
			gWaitClk = SystemFrequency/sFrame.lSckFreq;
			pI2c->CON.bit.TxEmptyCtrl = 1;
			pI2c->CON.bit.RestartEn = 1;	//enable restart
//			pI2c->CON.bit.StopDetIfMasterActive = 1;			
			pI2c->RX_TL = 0x00;	    // rxfifo depth is 1
			pI2c->TX_TL = 0x01;     //txfifo depth is 1
			break;
		#if(I2C_SLAVE)
		case SLAVE:
			pI2c->CON.bit.MasterMode = 0;	 //Master Disable
			pI2c->CON.bit.SlaveDis = 0;	 //Slave Enable

			if(sFrame.bAddrBits == 7)
				pI2c->CON.bit.BitAddrSlave = 0;
			else if(sFrame.bAddrBits == 10)
				pI2c->CON.bit.BitAddrSlave = 1;	  //address bit at slave mode
			else return RT_FAIL;

			pI2c->SAR = sFrame.bSlaveAddr;
			
			pI2c->CON.bit.TxEmptyCtrl = 1;
//			pI2c->SDA_SETUP = 121;
			pI2c->RX_TL = 0x00;	    // rxfifo depth is 1
			pI2c->TX_TL = 0x01;     //txfifo depth is 1
			I2C_InitTxFIFO(bI2cNum,pTxBuf);
			I2C_InitRxFIFO(bI2cNum,pRxBuf);
			if(pRxBuf == NULL)		// no buffer, disable receive
			{				
			}
			else
			{
				pI2c->INTR_MASK.bit.RxOver = 1;
				pI2c->INTR_MASK.bit.RxFull = 1;
			}

			if(pTxBuf == NULL)		// no buffer, disable send
			{				
			}
			else
			{
				pI2c->INTR_MASK.bit.TxOver = 1;
				pI2c->INTR_MASK.bit.RdReq = 1;		   
				pI2c->INTR_MASK.bit.RxDone = 1;
			}
			pI2c->ENABLE.bit.Enable = 1;	 //enable
		    NVIC_EnableIRQ(I2C0_IRQn);
			break;
		#endif
		
		default:
			return RT_FAIL;
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	I2C_MasterRead
 * Function:	Read data from I2C device
 * Input:		UINT8 bI2cNum --	I2C number, I2C0
 *				UINT16 bDevAddr --	I2C device address (7 bit address mode);
 * 				UINT16 bIndex -- 	High bit is flag that the index is use  
 *									low byte is the index of I2C device;
 * 				UINT8 *pData -- 	read data buff
 *              UINT32 nLen -- 		to specify the length to be read
 * Output:		UINT8	--	Return the result, RT_OK or RT_FAIL
 * Description:	The operation refer to I2C 2.1 Combined format transfer
 * Date:		2016.11.24
 * ModifyRecord: 
 * *************************************************************************/
UINT8 I2C_MasterRead(UINT8 bI2cNum, UINT16 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT32 nLen)
{
	volatile struct I2C_REGS * pI2c;	
	UINT32 i = 0;
	UINT32 lWaitTime; 
	UINT16 nCmd;

	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;
	lWaitTime = (gWaitClk<<3);

	if(bIndex&0x8000)
	{
		// 1 write device address for write to I2C device
		pI2c->ENABLE.bit.Enable = 0;
		pI2c->TAR.bit.IcTar = (bDevAddr>>1);
		pI2c->ENABLE.bit.Enable = 1;
		// 2 write index
		nCmd = bIndex|(0<<8)|(1<<9);
		pI2c->DATA_CMD.all = nCmd;	 //write index	
		i = 0;							 
		while((!pI2c->RAW_INTR_STAT.bit.StopDet)||(!pI2c->RAW_INTR_STAT.bit.TxEmpty))	 //wait TxEmpty and stop bit 
		{
			if(++i == lWaitTime) 
				return RT_FAIL;
		}
	}
	// 3 write device address for read
	pI2c->ENABLE.bit.Enable = 0;
	pI2c->TAR.bit.IcTar = (bDevAddr>>1);
	pI2c->ENABLE.bit.Enable = 1;

	// 4 wait for data valid, then read data
	while(nLen--)
	{
		if(pI2c->RAW_INTR_STAT.bit.StopDet)
			return RT_FAIL;
		
		if(nLen == 0) pI2c->DATA_CMD.all = (1<<8)|(1<<9);  //read	
		else pI2c->DATA_CMD.all = (1<<8)|(0<<9);  //read	
		
		i = 0;
		while(!pI2c->RAW_INTR_STAT.bit.RxFull)
		{
			if(++i == lWaitTime) 
				return RT_FAIL;
		} 		 
		*pData++ = pI2c->DATA_CMD.all;				
	 }
	// 5 STOP
	i = 0;
	while(!pI2c->RAW_INTR_STAT.bit.StopDet)
	{
		if(++i == lWaitTime) 
			return RT_FAIL;
	}
	// 6 disable
	pI2c->ENABLE.bit.Enable = 0;
	return RT_OK;		
}

/***************************************************************************
 * Subroutine:	I2C_MasterWrite
 * Function:	Write data to I2C device
 * Input:		UINT8 bI2cNum --	I2C number, I2C0
 *				UINT16 bDevAddr --	I2C device address (7 bit address mode or 10 bit address mode);
 * 				UINT16 bIndex -- 	High bit is flag that the index is use  
 *									low byte is the index of I2C device;
 * 				UINT8 *pData -- 	write data buff
 *				UINT32 nLen -- 		the length of data to be written
 * Output:		UINT8	--	Return the result, RT_OK or RT_FAIL
 * Description: The operation refer to I2C 2.1 Combined format transfer
 * Date:		2016.11.24
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_MasterWrite(UINT8 bI2cNum, UINT16 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT32 nLen)
{
	volatile struct I2C_REGS * pI2c;
	UINT32 i = 0;
	UINT32 lWaitTime; 
	UINT16 nCmd;

	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;
	lWaitTime = (gWaitClk<<3);

	// 1 write device address for write to I2C device
	pI2c->ENABLE.bit.Enable = 0;
	pI2c->TAR.bit.IcTar = (bDevAddr>>1);
	pI2c->ENABLE.bit.Enable = 1;
	
	// 2 write index
	if(bIndex&0x8000)
	{
		nCmd = bIndex|(0<<8);
		pI2c->DATA_CMD.all = nCmd;	 //write index
		i = 0;								 
		while(!pI2c->RAW_INTR_STAT.bit.TxEmpty)	 //wait TxEmpty
		{
			if(++i == lWaitTime) 
				return RT_FAIL;
		}
	}
   // 3 send data
	while(nLen--)
	{
		if(!pI2c->RAW_INTR_STAT.bit.TxAbrt)	  //ACK
		{
			if(pI2c->STATUS.bit.Tfnf)
			{
				if(nLen == 0) nCmd = (*pData++)|(0<<8)|(1<<9);
				else nCmd = (*pData++)|(0<<8)|(0<<9);
				pI2c->DATA_CMD.all = nCmd;	 //write data
			}
			i = 0;
			while(!pI2c->RAW_INTR_STAT.bit.TxEmpty)	 //wait TxEmpty
			{
				if(++i == lWaitTime) 
					return RT_FAIL;
			}	
		} 
		else
		{	  
			pI2c->ENABLE.bit.Enable = 0;
			return RT_FAIL;			 //NACK
		}
	}
	// 4 STOP
	i = 0;
	while(!pI2c->RAW_INTR_STAT.bit.StopDet)
	{
		if(++i == lWaitTime) 
			return RT_FAIL;
	}
	// 5 disable
	pI2c->ENABLE.bit.Enable = 0;
	return RT_OK;
}

#if(I2C_SLAVE)
/***************************************************************************
* Subroutine:	I2C_SlaveWriteBuf
* Function:		I2C send buffer
* Input:		UINT8 bI2cNum -- 	I2C number, I2C0
*				UINT8 *pBuf --		The buffer to be sent
* 				UINT16 iLen --		The length of buffer
* Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
* Description:
* Date:		2011.11.24
* ModifyRecord:
* **********************************************************************/
UINT8 I2C_SlaveWriteBuf(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen)
{
	volatile struct I2C_REGS * pI2c;
	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;

	while(iLen--)
	{
		if(I2C_GetTxFIFOValidSize(bI2cNum) < gI2cTxFIFO[bI2cNum].FFDepth)// buffer is not full
		{
			gI2cTxFIFO[bI2cNum].pFFData[gI2cTxFIFO[bI2cNum].FFInOffset++] = *pBuf++;
			if(gI2cTxFIFO[bI2cNum].FFInOffset >= gI2cTxFIFO[bI2cNum].FFDepth)
				gI2cTxFIFO[bI2cNum].FFInOffset = 0;
			I2C_SetTxFIFOValidSize(bI2cNum, 1);	// valid size + 1 with interrupt disabled			
		}
		else
		{			   
			UINT32 times = pI2c->SS_SCL_HCNT * 256 ;// wait
			while((I2C_GetTxFIFOValidSize(bI2cNum) >= gI2cTxFIFO[bI2cNum].FFDepth) && (times--));
			if(times == 0) return RT_FAIL;
		}
	}
	return RT_OK;
}

/**************************************************************************
 * Subroutine:	I2C_SlaveReadBuf
 * Function:	I2C read buffer
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0
 *				UINT8 *pBuf --		The buffer used for storing received data
 * 				UINT16 iLen --		The length will be receive
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL(no enough data)
 * Description: same as UART_ReceBuf
 * Date:		2011.11.24
 * ModifyRecord:
 * ************************************************************************/
UINT8 I2C_SlaveReadBuf(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen)
{
	if(I2C_GetRxFIFOValidSize(bI2cNum) >= iLen)		// have enough data in FIFO
	{
		while(iLen--)
		{
			*pBuf++ = gI2cRxFIFO[bI2cNum].pFFData[gI2cRxFIFO[bI2cNum].FFOutOffset++];
			if(gI2cRxFIFO[bI2cNum].FFOutOffset == gI2cRxFIFO[bI2cNum].FFDepth)
				gI2cRxFIFO[bI2cNum].FFOutOffset = 0;
			I2C_SetRxFIFOValidSize(bI2cNum,1);
		}
		return RT_OK;
	}
	return RT_FAIL;
}

/***************************************************************************
 * Subroutine:	I2C_InitTxFIFO
 * Function:	Initialize I2c transmit buffer
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0;
 * Output:		none
 * Description: clear memory of gI2cTxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and pFFData
 * Date:		2011.11.24
 * ModifyRecord:
 * ************************************************************************/
void I2C_InitTxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pTxBuf)
{
	gI2cTxFIFO[bI2cNum].pFFData = pTxBuf->pBuf;		// to initialize transmit FIFO
	gI2cTxFIFO[bI2cNum].FFDepth = pTxBuf->Size;
	gI2cTxFIFO[bI2cNum].FFValidSize = 0;
	gI2cTxFIFO[bI2cNum].FFInOffset = 0;
	gI2cTxFIFO[bI2cNum].FFOutOffset = 0;
}

/***************************************************************************
 * Subroutine:	I2C_InitRxFIFO
 * Function:	Initialize I2c Receive buffer
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 ;
 * Output:		none
 * Description: clear memory of gI2cRxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and pFFData
 * Date:		2011.11.24
 * ModifyRecord:
 * ************************************************************************/
void I2C_InitRxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pRxBuf)
{
	gI2cRxFIFO[bI2cNum].pFFData = pRxBuf->pBuf;		// to initialize receive FIFO
	gI2cRxFIFO[bI2cNum].FFDepth = pRxBuf->Size;
	gI2cRxFIFO[bI2cNum].FFValidSize = 0;
	gI2cRxFIFO[bI2cNum].FFInOffset = 0;
	gI2cRxFIFO[bI2cNum].FFOutOffset = 0;
}

/***************************************************************************
 * Subroutine:	I2C_GetTxFIFOValidSize
 * Function:	to get I2c transmit buffer valid data number
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT16 I2C_GetTxFIFOValidSize(UINT8 bI2cNum)
{
	UINT32 status32;
	UINT32 size;
	status32 = __get_PRIMASK();
	__disable_irq();
	size = gI2cTxFIFO[bI2cNum].FFValidSize;
	__set_PRIMASK(status32);
	return size;
}

/***************************************************************************
 * Subroutine: I2cSetTxFIFOValidSize
 * Function:	to set I2c transmit buffer valid data number (increase current value)
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0
 *				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void I2C_SetTxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size)
{
	UINT32 status32;
	status32 = __get_PRIMASK();
	__disable_irq();
	gI2cTxFIFO[bI2cNum].FFValidSize += offset_size;
	__set_PRIMASK(status32);
}

/***************************************************************************
 * Subroutine:	I2C_GetRxFIFOValidSize
 * Function:	to get I2c receive buffer valid data number
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0;
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT16 I2C_GetRxFIFOValidSize(UINT8 bI2cNum)
{
	UINT32 status32;
	UINT32 size;
	status32 = __get_PRIMASK();
	__disable_irq();
	size = gI2cRxFIFO[bI2cNum].FFValidSize;
	__set_PRIMASK(status32);
	return size;
}

/***************************************************************************
 * Subroutine: I2C_SetRxFIFOValidSize
 * Function:	to set I2c receive buffer valid data number (increase current value)
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0
 *				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void I2C_SetRxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size)
{
	UINT32 status32;
	status32 = __get_PRIMASK();
	__disable_irq();
	gI2cRxFIFO[bI2cNum].FFValidSize -= offset_size;
	__set_PRIMASK(status32);
}

/***************************************************************************
 * Subroutine:	I2C_IntProc
 * Function:	I2c interrupt process
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 
 * Output:		none
 * Description: to process all I2C interrupt except NackIntf
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void I2C_IntProc(UINT8 bI2cNum)
{	
	union I2C_INTR_STAT_REG IntFlag;
	volatile struct I2C_REGS * pI2c;

	pI2c = I2C_GetRegs(bI2cNum);	// get i2c0 registers address

	IntFlag = pI2c->INTR_STAT;	// get interrupt flags
	if(IntFlag.bit.RxFull || (IntFlag.bit.RxOver && pI2c->INTR_MASK.bit.RxOver))	//2015.11.30
	{
		pI2c->CLR_INTR;
		pI2c->CLR_RX_OVER;
		if(gI2cRxFIFO[bI2cNum].FFValidSize < gI2cRxFIFO[bI2cNum].FFDepth)	// not full
		{
			gI2cRxFIFO[bI2cNum].pFFData[gI2cRxFIFO[bI2cNum].FFInOffset++] = pI2c->DATA_CMD.all;//pI2c->RXREG;
			if(gI2cRxFIFO[bI2cNum].FFInOffset == gI2cRxFIFO[bI2cNum].FFDepth)
				gI2cRxFIFO[bI2cNum].FFInOffset = 0;
			gI2cRxFIFO[bI2cNum].FFValidSize++;
			_nop();
		}
		else	// if I2C RX FIFO is full, dummy read.	added by coins, 2009.09.17
		{
			pI2c->DATA_CMD.all;
		}
	}

	if(IntFlag.bit.RdReq)// | IntFlag.bit.TxOver | IntFlag.bit.RxDone)	
	{
		UINT8 temp;

		if(gI2cTxFIFO[bI2cNum].FFValidSize != 0)	//if TX FIFO is empty, dummy send.
		{
			temp= gI2cTxFIFO[bI2cNum].pFFData[gI2cTxFIFO[bI2cNum].FFOutOffset++];
			while(!pI2c->RAW_INTR_STAT.bit.TxEmpty);
			pI2c->DATA_CMD.bit.Dat = temp;
			if(gI2cTxFIFO[bI2cNum].FFOutOffset == gI2cTxFIFO[bI2cNum].FFDepth)
				gI2cTxFIFO[bI2cNum].FFOutOffset = 0;
			gI2cTxFIFO[bI2cNum].FFValidSize--; 
		}					
	}
}

/***************************************************************************
 * Subroutine:	I2C_Handler
 * Function:	use for I2C ISR call back
 * Input:		None
 * Output:		none
 * Description: I2C0 interrupt processing
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void I2C0_Handler(void)
{
	I2C_IntProc(I2C0);
}

#endif

#if(I2C_DMA)
/***************************************************************************
 * Subroutine:	I2C_InitDMA
 * Function:	I2C DMA mode initialization
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0.
 *				I2C_CLK_SRC nClkSrc --
 *									to set I2C clock, this clock for baudrate generator
 *									I2C_CLK_SRC_FCLK, I2C_CLK_SRC_HFROSC, I2C_CLK_SRC_MFROSC, I2C_CLK_SRC_XTAL
 *				I2C_MAP mPortMap --	Select which group of GPIO port as I2C
 *									I2C_MAP_A1_A2	--GPIA[1]&GPIOA[2]
 *									I2C_MAP_A5_A6	--GPIA[5]&GPIOA[6]
 *									I2C_MAP_A16_A21	--GPIA[16]&GPIOA[21]
 * 				STRUCT_I2C_FORMAT sFrame --		I2C frame format;
 * 				STRUCT_I2C_DATA_BUF *pRxBuf --
 * 									I2C receive buffer, to specify the buffer and size for slave receive;
 * 				STRUCT_I2C_DATA_BUF *pTxBuf --
 * 									I2C transmit buffer, to specify the buffer and size for slave transmit;
 *				UINT8 nDmaTxChn		to specify the DMA channel for Tx
 *									I2C_DMA_NOUSE:do not use DMA Tx
 *				UINT8 nDmaRxChn		to specify the DMA channel for Rx
 *									I2C_DMA_NOUSE:do not use DMA Rx
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL or RT_PARAM_ERR;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT8 I2C_InitDMA(UINT8 bI2cNum, I2C_CLK_SRC nClkSrc, I2C_MAP mPortMap, STRUCT_I2C_FORMAT sFrame,\
	STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf, UINT8 nDmaTxChn, UINT8 nDmaRxChn)
{
	UINT8 nRet;	

	nRet = I2C_Init( bI2cNum, nClkSrc, mPortMap, sFrame, pRxBuf, pTxBuf);
	if(nRet != RT_OK) return nRet;

	if(nDmaTxChn != I2C_DMA_NOUSE)
	{ 
		nRet = I2C_InitTxWithDMA(bI2cNum, nDmaTxChn);
		if(nRet != RT_OK) return nRet;
	}
	if(nDmaRxChn != I2C_DMA_NOUSE) 
	{
		nRet = I2C_InitRxWithDMA(bI2cNum, nDmaRxChn);
		if(nRet != RT_OK) return nRet;
	}
	return RT_OK;	
}

/****************************************************************************
 * Subroutine:	I2C_InitTxWithDMA
 * Function:	I2C initialization for I2C with DMA transmit 
 * Input:		UINT8 bI2cNum --	I2C number, I2C0 
 *				UINT8 nDmaChn --	to specify the DMA channel for Tx
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_InitTxWithDMA(UINT8 bI2cNum, UINT8 nDmaChn)
{
	volatile struct I2C_REGS * pI2c;	
	UINT8 nRet;	
	STRUCT_DMA_DEV sSrcDevSend = {0x20000000, INC, BYTE_1, BURST_1, HARD};	  //BURST_4
	STRUCT_DMA_DEV sDestDevSend = {0x40030010, FIX, BYTE_1, BURST_1, HARD};	

	if(nDmaChn >= DMA_NUM_CHANNELS) return RT_FAIL;
	pI2c = I2C_GetRegs(bI2cNum);	// get i2c0 registers address
	if(pI2c == NULL)  return RT_FAIL;

	pI2c->INTR_MASK.bit.TxOver = 0;
	pI2c->INTR_MASK.bit.RdReq = 0;		   
	pI2c->INTR_MASK.bit.RxDone = 0;
	pI2c->INTR_MASK.bit.TxAbrt = 0;

	pI2c->DMA_TDLR = 1;
	pI2c->DMA_CR.bit.TdMae = 1; 	  	//Transmit DMA Enable
	gI2cTxDmaChn = nDmaChn;				
	nRet = DMA_Init(nDmaChn, nDmaChn, &sSrcDevSend, &sDestDevSend, 0, NULL);
	return nRet;
}

/****************************************************************************
 * Subroutine:	I2C_InitRxWithDMA
 * Function:	I2C initialization for I2C with DMA receive 
 * Input:		UINT8 bI2cNum --	I2C number, I2C0 
 *				UINT8 nDmaChn --	to specify the DMA channel for Rx
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_InitRxWithDMA(UINT8 bI2cNum, UINT8 nDmaChn)
{
	volatile struct I2C_REGS * pI2c;
	UINT8 nRet;
	STRUCT_DMA_DEV sSrcDevRece = {0x40030014, FIX, BYTE_1, BURST_1, HARD};
	STRUCT_DMA_DEV sDestDevRece = {0x20000000, INC, BYTE_1, BURST_1, HARD};	

	if(nDmaChn >= DMA_NUM_CHANNELS) return RT_FAIL;
	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;

	pI2c->INTR_MASK.bit.RxOver = 0;
	pI2c->INTR_MASK.bit.RxFull = 0;

	pI2c->DMA_RDLR = 0;
	pI2c->DMA_CR.bit.RdMae = 1; 	  	//Receive DMA Enable
	gI2cRxDmaChn = nDmaChn;	
	nRet = DMA_Init(nDmaChn, nDmaChn, &sSrcDevRece, &sDestDevRece, 0, NULL);
	return nRet;
}

/****************************************************************************
 * Subroutine:	I2C_MasterWriteWithDma
 * Function:	write data to I2C device with DMA 			
 * Input:		UINT8 bI2cNum --	I2C number, I2C0
 *				UINT16 bDevAddr --	I2C device address (7 bit address mode or 10 bit address mode);
 * 				UINT16 bIndex -- 	High bit is flag that the index is use  
 *									low byte is the index of I2C device;
 * 				UINT8 *pData -- 	write data buff
 *				UINT16 nLen --		the length of data to be written
 * Output:		UINT8	--	Return the result, RT_OK or RT_FAIL 
 * Output:		None	
 * Description:	  
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_MasterWriteWithDma(UINT8 bI2cNum, UINT16 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT16 nLen)
{
	volatile struct I2C_REGS * pI2c;
	UINT16 nDmaTs;	
	UINT16 nCmd;
	UINT8 nRet;

	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;
		
	pI2c->ENABLE.bit.Enable = 0;
	pI2c->TAR.bit.IcTar = (bDevAddr>>1);
	pI2c->ENABLE.bit.Enable = 1;

	if(bIndex&0x8000)
	{
		nCmd = (0<<8)|bIndex;
		pI2c->DATA_CMD.all = nCmd;	 //write index
		while(!pI2c->RAW_INTR_STAT.bit.TxEmpty);
	}

	if(nLen == 0)return RT_OK;

	nLen -= 1;//the last byte send stop bit
	if(nLen)
	{	
		nDmaTs = (nLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : nLen;
		DMA_Start(gI2cTxDmaChn, (UINT32)pData, (UINT32)&pI2c->DATA_CMD.all, nDmaTs);
		nRet = DMA_CheckDone(gI2cTxDmaChn, 1000000); 
		if(nRet!=RT_OK) return nRet;
		nLen -= nDmaTs;	
		pData += nDmaTs;			
	}
	pI2c->DATA_CMD.all = *pData |(1<<9); 
	return RT_OK;		
}

/****************************************************************************
 * Subroutine:	I2C_SlaveWriteWithDMA
 * Function:	write data to I2C device with DMA 			
 * Input:		UINT8 bI2cNum --	I2C number, I2C0
 * 				UINT8 *pBuf -- 	the data write to;
 *				UINT16 iLen --		the length of data
 * Output:		UINT8	--	Return the result, RT_OK or RT_FAIL 
 * Output:		None	
 * Description:	  
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_SlaveWriteWithDMA(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen)
{
	volatile struct I2C_REGS * pI2c;
	UINT16 nDmaTs;	
	UINT8 nRet;

	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;

	while(!I2C0Regs.RAW_INTR_STAT.bit.RdReq);
	if(iLen)
	{	
		nDmaTs = (iLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : iLen;
		DMA_Start(gI2cTxDmaChn, (UINT32)pBuf, (UINT32)&pI2c->DATA_CMD.all, nDmaTs);
		nRet = DMA_CheckDone(gI2cTxDmaChn, 0); 
		if(nRet!=RT_OK) return nRet;
		iLen -= nDmaTs;	
		pBuf += nDmaTs;			
	}

	pI2c->CLR_RD_REQ;

	return RT_OK;
}

/****************************************************************************
 * Subroutine:	I2C_SlaveReadWithDMA
 * Function:	write data to I2C device with DMA 			
 * Input:		UINT8 bI2cNum --	I2C number, I2C0
 * 				UINT8 *pBuf -- 	the data write to;
 *				UINT16 iLen --		the length of data
 * Output:		UINT8	--	Return the result, RT_OK or RT_FAIL 
 * Output:		None	
 * Description:	  
 * Date:		2016.11.07
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_SlaveReadWithDMA(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen)
{
	volatile struct I2C_REGS * pI2c;
	UINT16 nDmaTs;	
	UINT8 nRet;

	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;
		
	if(iLen)
	{	
		nDmaTs = (iLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : iLen;
		DMA_Start(gI2cRxDmaChn, (UINT32)&pI2c->DATA_CMD.all, (UINT32)pBuf, nDmaTs);
		nRet = DMA_CheckDone(gI2cRxDmaChn, 0); 
		if(nRet!=RT_OK) return nRet;
		iLen -= nDmaTs;	
		pBuf += nDmaTs;			
	}
	return RT_OK;	
}

#endif


