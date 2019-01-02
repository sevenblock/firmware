/***************************************************************************************
* File name    :	Spi.h
* Function     :	Header of Spi.c
* Author       : 	Footman
* Date         :	2016/11/04
* Version      :    v1.0
* Description  :    Header of Spi Hardware Abstract Layer
* ModifyRecord :
*****************************************************************************************/
#ifndef SPI_H_
#define SPI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define		SPI_SLAVE	1
#define		SPI_DMA		1

//--------- Define UART Number -------------
#ifndef		_SPI_
#define		_SPI_
#define 	SPI_NUM		2
#define		SPI0		0
#define		SPI1		1
#endif

//---------- Define SPI Format Parameter ----------
#ifndef		_SPI_FORMAT_
#define		_SPI_FORMAT_
#define		MASTER		1
#define		SLAVE		0
#define		MSB			0
#define		LSB			1

#define		MODE0		0
#define		MODE1		1
#define		MODE2		2
#define		MODE3		3
#endif

#ifndef		DUMMY
#define		DUMMY		0
#endif


#define		GetSpiRegs	SPI_GetRegs
#define		SpiInit		SPI_Init

/**************************************************************************
* Global Type Definition
***************************************************************************/
#ifndef	_DATA_BUF_
#define _DATA_BUF_
typedef	struct	{
	UINT8 *	pBuf;		// pointer to buffer start address
	UINT16	Size;		// buffer size
}STRUCT_DATA_BUF;
#endif

#ifndef	_STRUCT_SPI_FRAME_FORMAT_
#define	_STRUCT_SPI_FRAME_FORMAT_
typedef	struct {
	UINT8	bMasterEn;		// SPI master mode or slave mode
	UINT8 	bBits;			// SPI character length bits
	UINT8 	bMode;			// SPI data mode, Mode 0/1/2/3
	UINT8 	bFrame;			// SPI frame format support, MSB or LSB
	UINT32 	lSckFreq;		// SPI speed
}STRUCT_SPI_FORMAT;
#endif

#ifndef	_STRUCT_FIFO_
#define	_STRUCT_FIFO_
typedef	struct {
	UINT8 *	pFFData;		// data buffer pointer
	UINT16	FFDepth;		// FIFO depth, maximum FIFO size
	UINT16 	FFValidSize;	// valid data size in FIFO
	UINT16 	FFInOffset;		// index of FIFO in
	UINT16 	FFOutOffset;	// index of FIFO out
}STRUCT_FIFO;
#endif

#ifndef	_SPI_PORT_MAP_
#define	_SPI_PORT_MAP_
typedef enum {
	SPI_MAP_A0A5A6,
	SPI_MAP_A7A8A9,
	SPI_MAP_B2B3B4,	
}SPI_PORT_MAP;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 SPI_Init(UINT8 bSpiNum, SPI_PORT_MAP nPortMap, STRUCT_SPI_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf);
UINT8 SPI_MasterTransceive(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);
UINT8 SPI_MasterWrite(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);
UINT8 SPI_MasterRead(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);

#if(SPI_SLAVE)
UINT8 SPI_SlaveWrite(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen);
UINT8 SPI_SlaveRead(UINT8 bSpiNum, UINT8 *pBuf, UINT32 nLen);
#endif

#if(SPI_DMA)
UINT8 SPI_WaitIdle(UINT32 nTimeOut);
UINT8 SPI_InitWithDMA(UINT8 nSpiNum, SPI_PORT_MAP nPortMap, STRUCT_SPI_FORMAT sFrame, UINT8 nTxDmaChn, UINT8 nRxDmaChn);
UINT8 SPI_MasterTransceiveWithDMA(UINT8 nSpiNum, UINT8 *pSrcData, UINT8 *pDestData, UINT32 nLen);
UINT8 SPI_MasterWriteWithDMA(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);
UINT8 SPI_MasterReadWithDMA(UINT8 nSpiNum, UINT8 *pData, UINT32 nLen);
#endif

#endif /*SPI_H_*/
