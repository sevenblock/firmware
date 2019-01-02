/***************************************************************************************
* File name    :	Spi.c
* Function     :	the function of Spi
* Author       : 	Footman
* Date         :	2016/11/04
* Version      :    v1.0
* Description  :    Spi Hardware Abstract Layer for AS569
* ModifyRecord :
*****************************************************************************************/

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
static UINT8 gSpixIRQn[SPI_NUM] = {SPI0_IRQn, SPI1_IRQn};

#if(SPI_SLAVE)
static STRUCT_FIFO gSpiTxFIFO[SPI_NUM];	// the global for SPI transmitting
static STRUCT_FIFO gSpiRxFIFO[SPI_NUM];	// the global for SPI receiving
#endif

#if(SPI_DMA)
static UINT8 gSpiTxDmaChn;
static UINT8 gSpiRxDmaChn;
static UINT8 gSpiDmaDone;
static UINT32 gSrcAddr, gDestAddr, gLen;
static UINT8 gSpiNum;
static UINT8 *gSrcData,*gDestData;
static UINT8 gDummy;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/

/***************************************************************************
 * Subroutine:	SPI_GetRegs
 * Function:	Get SPI register file address
 * Input:		UINT8 nSpiNum -- 	SPI number, should be less than SPI_NUM;
 * 								if it is out of range, dead loops.
 * Output:		volatile struct SPI_REGS * --	Return SPIx register file
 * 												address
 * Description:	Get SPI register file address.
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
volatile struct USART_REGS * SPI_GetRegs(UINT8 nSpiNum)
{
	switch(nSpiNum)
	{
		case SPI0:
			return &Spi0Regs;
		case SPI1:
			return &Spi1Regs;
		default:
			return NULL;
	}
}

#if(SPI_SLAVE)
/***************************************************************************
 * Subroutine:	SpiInitTxFIFO
 * Function:	Initialize Spi transmit buffer
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		none
 * Description: clear memory of gSpiTxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and FIFO
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
static void SpiInitTxFIFO(UINT8 bSpiNum, STRUCT_DATA_BUF *pTxBuf)
{
	gSpiTxFIFO[bSpiNum].pFFData = pTxBuf->pBuf;		// to initialize transmit FIFO
	gSpiTxFIFO[bSpiNum].FFDepth = pTxBuf->Size;
	gSpiTxFIFO[bSpiNum].FFValidSize = 0;
	gSpiTxFIFO[bSpiNum].FFInOffset = 0;
	gSpiTxFIFO[bSpiNum].FFOutOffset = 0;
}
/***************************************************************************
 * Subroutine:	SpiInitRxFIFO
 * Function:	Initialize Spi transmit buffer
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		none
 * Description: clear memory of gSpiRxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and FIFO
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
static void SpiInitRxFIFO(UINT8 bSpiNum, STRUCT_DATA_BUF *pRxBuf)
{
	gSpiRxFIFO[bSpiNum].pFFData = pRxBuf->pBuf;		// to initialize transmit FIFO
	gSpiRxFIFO[bSpiNum].FFDepth = pRxBuf->Size;
	gSpiRxFIFO[bSpiNum].FFValidSize = 0;
	gSpiRxFIFO[bSpiNum].FFInOffset = 0;
	gSpiRxFIFO[bSpiNum].FFOutOffset = 0;
}

/***************************************************************************
 * Subroutine:	SpiGetRxFIFOValidSize
 * Function:	to get Spi receive buffer valid data number
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
static UINT16 SpiGetRxFIFOValidSize(UINT8 bSpiNum)
{
	UINT32 status;
	UINT32 size;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ((IRQn_Type)gSpixIRQn[bSpiNum]); 	// disable SPI0 interrupt
	size = gSpiRxFIFO[bSpiNum].FFValidSize;
	NVIC->ISER[0] = status;		// recover interrupt status
	return size;
}

/***************************************************************************
 * Subroutine: SpiSetRxFIFOValidSize
 * Function:	to set spi receive buffer valid data number (increase current value)
 * Input:		UINT8 bSpiNum -- 	SPI number, Spi 0/1/2
 * 				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
static void SpiSetRxFIFOValidSize(UINT8 bSpiNum, UINT16 offset_size)
{
	UINT32 status;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ((IRQn_Type)gSpixIRQn[bSpiNum]); 	// disable SPI0 interrupt
	gSpiRxFIFO[bSpiNum].FFValidSize -= offset_size;
	NVIC->ISER[0] = status;		// recover interrupt status
}

/***************************************************************************
 * Subroutine:	SpiGetTxFIFOValidSize
 * Function:	to get Spi transmit buffer valid data number
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
static UINT16 SpiGetTxFIFOValidSize(UINT8 bSpiNum)
{
	UINT32 size;
	UINT32 status;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ((IRQn_Type)gSpixIRQn[bSpiNum]); 	// disable SPI0 interrupt
	size = gSpiTxFIFO[bSpiNum].FFValidSize;
	NVIC->ISER[0] = status;		// recover interrupt status
	return size;
}

/***************************************************************************
 * Subroutine: SpiSetTxFIFOValidSize
 * Function:	to set spi transmit buffer valid data number (increase current value)
 * Input:		UINT8 bSpiNum -- 	SPI number, Spi 0/1/2
 * 				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
static void SpiSetTxFIFOValidSize(UINT8 bSpiNum, UINT16 offset_size)
{
	UINT32 status;
	status = NVIC->ISER[0]; 	// Save interrupt status
	NVIC_DisableIRQ((IRQn_Type)gSpixIRQn[bSpiNum]); 	// disable SPI0 interrupt
	gSpiTxFIFO[bSpiNum].FFValidSize += offset_size;
	NVIC->ISER[0] = status;		// recover interrupt status
}
#endif

/***************************************************************************
 * Subroutine:	SPI_Init
 * Function:	SPI initialization
 * Input:		UINT8 bSpiNum -- 	SPI number, Spi0 or Spi1 or Spi2;
 * 				STRUCT_SPI_FORMAT sFrame --
 * 									SPI frame format;
 * 				STRUCT_DATA_BUF sRxBuf --
 * 									SPI receive buffer, to specify the buffer and size for slave receive;
 * 				STRUCT_DATA_BUF sTxBuf --
 * 									SPI transmit buffer, to specify the buffer and size for slave transmit;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_Init(UINT8 bSpiNum, SPI_PORT_MAP nPortMap, STRUCT_SPI_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf)
{
	volatile struct USART_REGS * pSpi;

	switch(bSpiNum)
	{
		case SPI0 :
			// 1. to open global control
			SCM_DisableModClk(ID_SPI0);// disable SPI0 clock
			SCM_KeepModRst(ID_SPI0);// reset
			SCM_EnableModClk(ID_SPI0);// enable SPI0 clock
			SCM_ReleaseModRst(ID_SPI0);// Relinquish from reset
			// 2. select pins
			if(nPortMap != SPI_MAP_A7A8A9) return RT_PARAM_ERR;
			GpioRegs.GPAUSE0.bit.GPA7 = MUX_SPI;	// CLK											    
			GpioRegs.GPAUSE1.bit.GPA8 = MUX_SPI;	// MISO
			GpioRegs.GPAUSE1.bit.GPA9 = MUX_SPI;	// MOSI	
			break;	
		case SPI1 :
			// 1. to open global control
			SCM_DisableModClk(ID_SPI1);// disable SPI0 clock
			SCM_KeepModRst(ID_SPI1);// reset
			SCM_EnableModClk(ID_SPI1);// enable SPI0 clock
			SCM_ReleaseModRst(ID_SPI1);// Relinquish from reset
			// 2. select pins
			if(nPortMap == SPI_MAP_A0A5A6)
			{
				GpioRegs.GPAUSE0.bit.GPA0 = MUX_SPI;	// CLK
				GpioRegs.GPAUSE0.bit.GPA5 = MUX_SPI;	// MISO
				GpioRegs.GPAUSE0.bit.GPA6 = MUX_SPI;	// MOSI	
			}
			else if(nPortMap == SPI_MAP_B2B3B4)
			{
				GpioRegs.GPBUSE0.bit.GPB2 = MUX_SPI;	// CLK
				GpioRegs.GPBUSE0.bit.GPB3 = MUX_SPI;	// MISO
				GpioRegs.GPBUSE0.bit.GPB4 = MUX_SPI;	// MOSI
			}
			else return RT_PARAM_ERR;
			break;
		default:
			return RT_FAIL;
	}

	// 3. select SPI register file
	pSpi = SPI_GetRegs(bSpiNum);

	// 4. disable SPI transfer and receive and set USART to SPI mode
	pSpi->GCTL.all = 0;
	pSpi->GCTL.bit.Mm = 1;	//must be high before change mode	

	// 5. disable all SPI interrupts and clear all interrupt flags
	pSpi->INTEN.all = 0x0000;
	pSpi->INTCLR.all = 0xffff;

	// 6. config character length bit
	if((sFrame.bBits-7)<2) pSpi->CCTL.bit.SpiLen = sFrame.bBits - 7;
	else return RT_FAIL;

	// 7. config data mode
	switch(sFrame.bMode)
	{
		case MODE0:
			pSpi->CCTL.all &= ~(UINT32)(1<<1);	// ckpl
			pSpi->CCTL.all |= (1<<0);	// ckph
			break;
		case MODE1:
			pSpi->CCTL.all &= ~(UINT32)(1<<1);
			pSpi->CCTL.all &= ~(UINT32)(1<<0);
			break;
		case MODE2:
			pSpi->CCTL.all |= (1<<1);
			pSpi->CCTL.all |= (1<<0);
			break;
		case MODE3:
			pSpi->CCTL.all |= (1<<1);
			pSpi->CCTL.all &= ~(UINT32)(1<<0);
			break;
		default:
			return RT_FAIL;
	}
	// 8. config frame format : Lsb or Msb
	pSpi->CCTL.bit.Lsbfe = sFrame.bFrame;

	// 9. Master or Slave Configure
	switch(sFrame.bMasterEn)
	{
		case MASTER:
			// 10. config SPI to master or slave
			pSpi->GCTL.bit.Mm = sFrame.bMasterEn;
			// 11. Config SPI speed in Spi master
			if(sFrame.lSckFreq == 0)
				return RT_FAIL;
			pSpi->SPBREG = SCM_GetSysClk() / sFrame.lSckFreq;
			if(pSpi->SPBREG < 2) pSpi->SPBREG = 2;	// must be >=2
			// 12. enable SPI transmit and receive
			pSpi->GCTL.bit.TxEn = 1;		// enable transmit
			pSpi->GCTL.bit.RxEn = 1;		// enable receive
			break;
#if(SPI_SLAVE)
		case SLAVE:
			// 10. Config SPI Interrupt & FIFO in Spi Slave
			pSpi->GCTL.bit.Mm = sFrame.bMasterEn;

			SpiInitTxFIFO(bSpiNum, pTxBuf);
			SpiInitRxFIFO(bSpiNum, pRxBuf);

			// 12. to initialize RX FIFO, enable RX interrupt and RX
			if(pRxBuf != NULL)		// no buffer, disable receive
			{
				pSpi->INTEN.bit.RxiEn = 1;				// added by coins	2009.09.17
				pSpi->INTEN.bit.RxOerriIEn = 1;
				pSpi->INTEN.bit.UnderRuniEn = 1;
				pSpi->GCTL.bit.Rxtlf = 0;
			}

			// 13. to initial TX FIFO, enable TX interrupt and TX
			if(pTxBuf != NULL)				// no buffer for transmit interrupt
			{
				pSpi->INTEN.bit.TxiEn = 1;
				pSpi->GCTL.bit.TxEn = 1;		// enable transmit
			    pSpi->GCTL.bit.Txtlf = 0;
			}

			pSpi->GCTL.bit.TxEn = 1;				// enable transmit
			pSpi->GCTL.bit.RxEn = 1;				// enable receive
			break;
#endif
		default:
			return RT_FAIL;
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_MasterTransceive
 * Function:	SPI Master transmit byte and receive byte at the same time
 * Input:		UINT8 nSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --		the point of transmit data and
 * 									received data;
 *				UINT32 nLen -- 		to transceive nLen bytes;
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterTransceive(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT32 i = 0;
	UINT32 lWaitTime;
	if(nSpiNum >= SPI_NUM)
		return RT_FAIL;
	pSpi = SPI_GetRegs(nSpiNum);
	lWaitTime = 8 * pSpi->SPBREG;	// 8 SPI clock transmit a byte
	while(nLen--)
	{
		i = 0;
		while(pSpi->CSTAT.bit.TxFull)	// wait for tx_fifo not full
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		pSpi->TXREG = *pData;

		i = 0;
		while(!pSpi->CSTAT.bit.RxAvl)	// receive available
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		*pData = pSpi->RXREG;
		pData++;	// next byte
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_MasterWrite
 * Function:	SPI master write bytes
 * Input:		UINT8 nSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --		The data to be written
 *				UINT32 nLen -- 		the length of data to be written
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterWrite(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT32 i = 0;
	UINT32 lWaitTime;
	if(nSpiNum >= SPI_NUM)
		return RT_FAIL;
	pSpi = SPI_GetRegs(nSpiNum);
	lWaitTime = 8 * pSpi->SPBREG;	// 8 SPI clock transmit a byte
	while(nLen--)
	{
		i = 0;
		while(pSpi->CSTAT.bit.TxFull)	// wait for tx_fifo not full
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		pSpi->TXREG = *pData++;

		i = 0;
		while(!pSpi->CSTAT.bit.RxAvl)	// receive available
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		pSpi->RXREG;
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_MaterRead
 * Function:	SPI master receive bytes (not interrupt)
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --		The value to be received (pointer)
 *				UINT32 nLen -- 		to specify the length to be read
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterRead(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT32 i = 0;
	UINT32 lWaitTime;
	if(nSpiNum >= SPI_NUM)
		return RT_FAIL;
	pSpi = SPI_GetRegs(nSpiNum);
	lWaitTime = 8 * pSpi->SPBREG;	// 8 SPI clock transmit a byte
	while(nLen--)
	{	
		pSpi->TXREG = DUMMY;
		i = 0;
		while(!pSpi->CSTAT.bit.RxAvl)	// receive available
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		*pData++ = pSpi->RXREG;
	}
	return RT_OK;
}

#if(SPI_SLAVE)
/**************************************************************************
 * Subroutine:	SPI_SlaveWrite
 * Function:	SPI Slave send bytes
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pBuf --		The buffer to be sent
 * 				UINT32 nLen --		The length of buffer
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * **********************************************************************/
UINT8 SPI_SlaveWrite(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT16 spi_tx_buf;
	UINT32 status;
	if(bSpiNum >= SPI_NUM) return RT_FAIL;
	
	pSpi = SPI_GetRegs(bSpiNum);
	spi_tx_buf = SpiGetTxFIFOValidSize(bSpiNum);	// to get TX buffer valid size
	if(spi_tx_buf==0)				// TX buffer is empty /
	{
		status = NVIC->ISER[0]; 	// Save interrupt status
		NVIC_DisableIRQ((IRQn_Type)gSpixIRQn[bSpiNum]); 	// disable SPI0 interrupt
		while(!pSpi->CSTAT.bit.TxFull)
		{
			if(nLen--)
				pSpi->TXREG = *pBuf++;
			else
			{
				NVIC->ISER[0] = status;		// recover interrupt status
				return RT_OK;							// iLen == 0, exit
			}
		}
		NVIC->ISER[0] = status;		// recover interrupt status
	}

	while(nLen--)
	{
		if(SpiGetTxFIFOValidSize(bSpiNum) < gSpiTxFIFO[bSpiNum].FFDepth)// buffer is not full
		{
			gSpiTxFIFO[bSpiNum].pFFData[gSpiTxFIFO[bSpiNum].FFInOffset++] = *pBuf++;
			if(gSpiTxFIFO[bSpiNum].FFInOffset >= gSpiTxFIFO[bSpiNum].FFDepth)
				gSpiTxFIFO[bSpiNum].FFInOffset = 0;
			SpiSetTxFIFOValidSize(bSpiNum, 1);	// valid size + 1 with interrupt disabled
		}
		else
		{
			// wait
			UINT32 times = pSpi->SPBREG * 256;
			while((SpiGetTxFIFOValidSize(bSpiNum) >= gSpiTxFIFO[bSpiNum].FFDepth) && (times--));
			if(times == 0) return RT_FAIL;
		}
	}
	return RT_OK;
}

/**************************************************************************
 * Subroutine:	SPI_SlaveRead
 * Function:	SPI slave receive bytes
 * Input:		UINT8 bSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pBuf --		The buffer used for storing received data
 * 				UINT32 nLen --		The length will be receive
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL(no enough data)
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
UINT8 SPI_SlaveRead(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen)
{
	if(SpiGetRxFIFOValidSize(bSpiNum) >= nLen)		// have enough data in receive FIFO
	{
		while(nLen--)
		{
			*pBuf++ = gSpiRxFIFO[bSpiNum].pFFData[gSpiRxFIFO[bSpiNum].FFOutOffset++];
			if(gSpiRxFIFO[bSpiNum].FFOutOffset == gSpiRxFIFO[bSpiNum].FFDepth)
				gSpiRxFIFO[bSpiNum].FFOutOffset = 0;
			SpiSetRxFIFOValidSize(bSpiNum, 1);		// valid size - 1 with interrupt disabled
		}
		return RT_OK;
	}
	return RT_FAIL;
}

/***************************************************************************
 * Subroutine:	SpiIntProc
 * Function:	SPI interrupt process
 * Input:		UINT8 bSpiNum -- SPI number, SPI 0/1/2
 * Output:		none
 * Description: to process all SPI interrupt
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
void SpiIntProc(UINT8 bSpiNum)
{
	union USART_INTSTAT_REG IntFlag;
	volatile struct USART_REGS * pSpi;
	pSpi = SPI_GetRegs(bSpiNum);

	IntFlag = pSpi->INTSTAT;		// get interrupt flags
	pSpi->INTCLR.all = IntFlag.all;	// clear interrupt flags for next interrupt

	if(IntFlag.bit.RxIntf || IntFlag.bit.RxOerrIntf)	// receive interrupt
	{
		while(pSpi->CSTAT.bit.RxAvl)	// added by coins, 2009.09.16
		{
			if(gSpiRxFIFO[bSpiNum].FFValidSize < gSpiRxFIFO[bSpiNum].FFDepth)	// not full
			{
				gSpiRxFIFO[bSpiNum].pFFData[gSpiRxFIFO[bSpiNum].FFInOffset++] = pSpi->RXREG;
				if(gSpiRxFIFO[bSpiNum].FFInOffset == gSpiRxFIFO[bSpiNum].FFDepth)
					gSpiRxFIFO[bSpiNum].FFInOffset = 0;
				gSpiRxFIFO[bSpiNum].FFValidSize++;
			}
			else	// if USART RX FIFO is full, dummy read.	added by coins, 2009.09.17
			{
				pSpi->RXREG;
			}
		}
	}
	if(IntFlag.bit.TxIntf)		// transmit interrupt
	{
		while(!pSpi->CSTAT.bit.TxFull)	// till FIFO is full
		{
			if(gSpiTxFIFO[bSpiNum].FFValidSize != 0)	// not empty
			{
				pSpi->TXREG = gSpiTxFIFO[bSpiNum].pFFData[gSpiTxFIFO[bSpiNum].FFOutOffset++];
				if(gSpiTxFIFO[bSpiNum].FFOutOffset == gSpiTxFIFO[bSpiNum].FFDepth)
					gSpiTxFIFO[bSpiNum].FFOutOffset = 0;
				gSpiTxFIFO[bSpiNum].FFValidSize--;
			}
			else break;
		}
	}
}

/***************************************************************************
 * Subroutine:	Spi0_IntProc
 * Function:	use for SPI0 ISR call back
 * Input:		None
 * Output:		none
 * Description: SPI0 interrupt processing
 * Date:		2016/11/04
 * ModifyRecord:
 * ************************************************************************/
void SPI0_Handler(void)
{
	SpiIntProc(SPI0);
}

void SPI1_Handler(void)
{
	SpiIntProc(SPI1);
}
#endif

#if(SPI_DMA)
/***************************************************************************
 * Subroutine:	SPI_DmaProc
 * Function:	to handle SPI DMA interrupt 
 * Input:		None
 * Output:		None
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
void SPI_DmaProc(UINT8 nIntType)
{
	UINT16 nDmaTs;
	volatile struct USART_REGS * pSpi;	
	pSpi = SPI_GetRegs(gSpiNum);
	if(gLen)
	{
		nDmaTs = (gLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : gLen;
		DMA_Start(gSpiRxDmaChn, (UINT32)&(pSpi->RXREG), gDestAddr, nDmaTs); 
		DMA_Start(gSpiTxDmaChn, gSrcAddr, (UINT32)&(pSpi->TXREG), nDmaTs);
		if(gSrcData!=NULL) gSrcAddr+=nDmaTs; 
		if(gDestData!=NULL) gDestAddr+=nDmaTs; 
		gLen -= nDmaTs;
	}
	else
		gSpiDmaDone = 1;		
}

/***************************************************************************
 * Subroutine:	SPI_WaitIdle
 * Function:	to wait SPI transfer idle 
 * Input:		UINT32 nTimeOut --	to specify the timeout, uint in us.
 * Output:		RT_OK or RT_OVERTIME
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_WaitIdle(UINT32 nTimeOut)
{
	UINT32 nTimeCnt = 0;
	while((gSpiDmaDone==0))	// wait transmit FIFO empty
	{
		if(nTimeOut != 0)	// timeout check
		{
			_delay_us(1);
			if(nTimeCnt++ == nTimeOut) 
			{
				return RT_OVERTIME;
			}
		}
	}
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_InitWithDMA
 * Function:	SPI initialization with DMA
 * Input:		UINT8 bSpiNum -- 	SPI number, Spi0 or Spi1 or Spi2;
 * 				STRUCT_SPI_FORMAT sFrame --
 * 									SPI frame format;
 * 				UINT8 nTxDmaChn --	to specify the DMA channel for transmit;
 * 				UINT8 nRxDmaChn --	to specify the DMA channel for receive;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_InitWithDMA(UINT8 nSpiNum, SPI_PORT_MAP nPortMap, STRUCT_SPI_FORMAT sFrame, UINT8 nTxDmaChn, UINT8 nRxDmaChn)
{
	UINT8 nRet;
	volatile struct USART_REGS * pSpi;
	STRUCT_DMA_DEV sSrcDevTx = {0x20000000, INC, BYTE_1, BURST_8, HARD};
	STRUCT_DMA_DEV sDestDevTx = {0, FIX, BYTE_1, BURST_1, HARD};
	STRUCT_DMA_DEV sSrcDevRx = {0, FIX, BYTE_1, BURST_1, HARD};
	STRUCT_DMA_DEV sDestDevRx = {0x20000000, INC, BYTE_1, BURST_8, HARD};
	nRet = SPI_Init(nSpiNum, nPortMap, sFrame, NULL, NULL);
	if(nRet!=RT_OK) return nRet;
	gSpiNum = nSpiNum;
	pSpi = SPI_GetRegs(nSpiNum);
	sDestDevTx.nAddr = (UINT32)&(pSpi->TXREG);
	sSrcDevRx.nAddr = (UINT32)&(pSpi->RXREG);
	
	gSpiTxDmaChn = nTxDmaChn;
	gSpiRxDmaChn = nRxDmaChn;
	nRet = DMA_Init(gSpiTxDmaChn, gSpiTxDmaChn, &sSrcDevTx, &sDestDevTx, 0, NULL);
	if(nRet!=RT_OK) return nRet;
	nRet = DMA_Init(gSpiRxDmaChn, gSpiRxDmaChn, &sSrcDevRx, &sDestDevRx, DMA_INT_TFR, SPI_DmaProc);
	if(nRet!=RT_OK) return nRet;
	return RT_OK;
}

/***************************************************************************
 * Subroutine:	SPI_MasterTransceiveWithDMA
 * Function:	SPI Master transmit byte and receive byte at the same time with DMA
 * Input:		UINT8 nSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pSrcData --	the point of transmit data;
 *				UINT8 *pDestData -- the point of receive data;
 *				UINT32 nLen -- 		to transceive nLen bytes;
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterTransceiveWithDMA(UINT8 nSpiNum, UINT8 *pSrcData, UINT8 *pDestData, UINT32 nLen)
{
	volatile struct USART_REGS * pSpi;
	UINT32 nSrcAddr, nDestAddr;
	UINT32 nDmaTs;
	if(nSpiNum >= SPI_NUM)
		return RT_FAIL;
	pSpi = SPI_GetRegs(nSpiNum);
	gSrcData = pSrcData;
	gDestData = pDestData;
	if(pSrcData==NULL) 	// no data to transmit, then transmit dummy byte
	{
		gDummy = DUMMY;
		nSrcAddr = (UINT32)&gDummy;
		DMA_ChangeSrcAddrMode(gSpiTxDmaChn, FIX);
	} 
	else
	{
		nSrcAddr = (UINT32)pSrcData;
		DMA_ChangeSrcAddrMode(gSpiTxDmaChn, INC);	
	}

	if(pDestData==NULL) 	// no data need receive
	{
		nDestAddr = (UINT32)&gDummy;
		DMA_ChangeDestAddrMode(gSpiRxDmaChn, FIX);
	} 
	else
	{
		nDestAddr = (UINT32)pDestData;
		DMA_ChangeDestAddrMode(gSpiRxDmaChn, INC);	
	}

	gSpiDmaDone=0;
	gLen = nLen;
	gDestAddr = nDestAddr;
	gSrcAddr = nSrcAddr;
	nDmaTs = (gLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : gLen;
	gLen -= nDmaTs;
	DMA_Start(gSpiRxDmaChn, (UINT32)&(pSpi->RXREG), nDestAddr, nDmaTs); 
	DMA_Start(gSpiTxDmaChn, nSrcAddr, (UINT32)&(pSpi->TXREG), nDmaTs); 
	if(pSrcData!=NULL) gSrcAddr+=nDmaTs; 
	if(pDestData!=NULL) gDestAddr+=nDmaTs; 
	gLen -= nDmaTs;

	return RT_OK;	
}

/***************************************************************************
 * Subroutine:	SPI_MasterWriteWithDMA
 * Function:	SPI Master write with DMA
 * Input:		UINT8 nSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --	the point of transmit data;
 *				UINT32 nLen -- 		to transceive nLen bytes;
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterWriteWithDMA(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	return SPI_MasterTransceiveWithDMA(nSpiNum, pData, NULL, nLen);		
}

/***************************************************************************
 * Subroutine:	SPI_MasterWriteWithDMA
 * Function:	SPI Master read with DMA
 * Input:		UINT8 nSpiNum -- 	SPI number, SPI 0/1/2
 * 				UINT8 *pData --	the point of receive data;
 *				UINT32 nLen -- 		to transceive nLen bytes;
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * *************************************************************************/
UINT8 SPI_MasterReadWithDMA(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen)
{
	return SPI_MasterTransceiveWithDMA(nSpiNum, NULL, pData, nLen);		
}
#endif
