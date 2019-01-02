/***************************************************************************
* File name    :	I2C.h
* Function     :	Header of I2C.c
* Author       : 	
* Date         :	2012/06/28
* Version      :    v1.0
* Description  :    Header of I2C Hardware Abstract Layer
* ModifyRecord :
****************************************************************************/
#ifndef I2C_H_
#define I2C_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	I2C_SLAVE	1
#define I2C_DMA		1

// Define I2c Number
#ifndef	_I2C_
#define	_I2C_
#define 	I2C_NUM		1
#define		I2C0		0
#endif

// Define E2PROM Address
#ifndef	_AT24C02_
#define _AT24C02_
#define 	AT24C02		0xA0
#endif

// Define GC0303 Address
#ifndef	_GC0303_
#define _GC0303_
#define 	GC0303		0x30
#endif

// Define Transmit Mode
#ifndef	_TX_MODE_
#define _TX_MODE_
#define		I2C_TX_POLLING		0
#define		I2C_TX_INT			1
#endif	 

// Define I2C Format
#ifndef	_I2C_FORMAT_
#define	_I2C_FORMAT_
#define		I2C_MODE	3
#define		MASTER		1
#define		SLAVE		0
#endif	  

#define I2C_DMA_NOUSE	0xff

/**************************************************************************
* Global Type Definition
***************************************************************************/
// Define I2c Frame Format Structure Type 
#ifndef	_STRUCT_I2C_FRAME_FORMAT_
#define	_STRUCT_I2C_FRAME_FORMAT_
typedef	struct {
	UINT8	bMasterEn;		// I2C master mode or slave mode
	UINT8 	bAddrBits;		// I2C addressing mode, set 7, 10 for 7, 10 bits
	UINT16 	bSlaveAddr;		// I2C slave device address, 7/10
	UINT32 	lSckFreq;		// I2C baud rate, uint in bps, 100/400K
//	UINT8	bI2cMode;		// I2C mode,Standard/Fast
}STRUCT_I2C_FORMAT;
#endif

// Define Data Buffer Structure Type 
#ifndef	_DATA_BUF_
#define _DATA_BUF_
typedef	struct	{
	UINT8 *	pBuf;		// pointer to buffer start address
	UINT16	Size;		// buffer size
}STRUCT_DATA_BUF;
#endif

// Define FIFO Structure Type 
#ifndef	_STRUCT_I2C_FIFO_
#define	_STRUCT_I2C_FIFO_
typedef	struct {
	UINT8 *	pFFData;		// data buffer pointer
	UINT16	FFDepth;		// FIFO depth
	UINT16 	FFValidSize;	// valid data size in FIFO
	UINT16 	FFInOffset;		// index of FIFO in
	UINT16 	FFOutOffset;	// index of FIFO out
}STRUCT_I2C_FIFO;
#endif

// Define the I2C Port Map to GPIO Port
typedef	enum {
	I2C_MAP_A1_A2 = 0,
			//GPA1 --  I2C_SCL	
			//GPA2 --  I2C_SDA	
	I2C_MAP_A5_A6 = 1,
			//GPA5 --  I2C_SCL	
			//GPA6 --  I2C_SDA	
	I2C_MAP_A16_A21 = 2,
			//GPA16 --  I2C_SCL	
			//GPA21 --  I2C_SDA
}I2C_MAP;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

#define I2C_MasterRd(bI2cNum,bDevAddr,bIndex,pData,nLen)    I2C_MasterRead((UINT8)bI2cNum,(UINT16)bDevAddr,(UINT16)(bIndex|0x8000), (UINT8*)pData,(UINT32)nLen)
#define I2C_MasterWr(bI2cNum,bDevAddr,bIndex,pData,nLen)    I2C_MasterWrite((UINT8)bI2cNum,(UINT16)bDevAddr,(UINT16)(bIndex|0x8000), (UINT8*)pData,(UINT32)nLen)
/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 I2C_Init(UINT8 bI2cNum, I2C_CLK_SRC nClkSrc, I2C_MAP mPortMap, STRUCT_I2C_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf);
UINT8 I2C_MasterRead(UINT8 bI2cNum, UINT16 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT32 nLen);
UINT8 I2C_MasterWrite(UINT8 bI2cNum, UINT16 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT32 nLen);

#if(I2C_SLAVE)
UINT8 I2C_SlaveWriteBuf(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen);
UINT8 I2C_SlaveReadBuf(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen);
#endif

#if(I2C_DMA)
UINT8 I2C_InitDMA(UINT8 bI2cNum, I2C_CLK_SRC nClkSrc, I2C_MAP mPortMap, STRUCT_I2C_FORMAT sFrame,\
	STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf, UINT8 nDmaTxChn, UINT8 nDmaRxChn);
UINT8 I2C_InitTxWithDMA(UINT8 bI2cNum, UINT8 nDmaChn);
UINT8 I2C_InitRxWithDMA(UINT8 bI2cNum, UINT8 nDmaChn);
UINT8 I2C_MasterWriteWithDma(UINT8 bI2cNum, UINT16 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT16 nLen);
UINT8 I2C_SlaveWriteWithDMA(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen);
UINT8 I2C_SlaveReadWithDMA(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen);
#endif

/**************************************************************************
* Functon
***************************************************************************/
void I2C_SetRxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size);
UINT16 I2C_GetRxFIFOValidSize(UINT8 bI2cNum);
void I2C_SetTxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size);
UINT16 I2C_GetTxFIFOValidSize(UINT8 bI2cNum);
void I2C_InitTxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pTxBuf);
void I2C_InitRxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pRxBuf);

#endif	/* I2C_H_ */

