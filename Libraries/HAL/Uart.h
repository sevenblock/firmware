/***************************************************************************
* File name    :	Uart.h
* Function     :	Header of Uart.c
* Author       : 	coins
* Date         :	2009/03/31
* Version      :    v1.0
* Description  :    Header of UART Hardware Abstract Layer
* ModifyRecord :
****************************************************************************/
#ifndef UART_H_
#define UART_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/															 
#define 	UART_DMA 		1
#define		UART_AFCE		0//1// Auto Flow Control Enable


// Define UART Number
#ifndef	_UART_
#define	_UART_
#define 	UART_NUM	3
#define		UART0		0
#define		UART1		1
#define		UART2		2
#endif

// Define UART Frame Format 
#ifndef	_STOP_BIT_
#define	_STOP_BIT_
#define		ONE_STOP	1
#define		TWO_STOP	2
#endif

// Define UART Char Bit 
#ifndef	_CHAR_BIT_
#define	_CHAR_BIT_
#define		CHAR_5BIT	5
#define		CHAR_6BIT	6
#define		CHAR_7BIT	7
#define		CHAR_8BIT	8
#endif

// Define UART Parity
#ifndef	_PARITY_
#define _PARITY_
#define		NONE		0
#define		ODD			1    
#define		EVEN		2 
#define		MARK		3    
#define		SPACE		4    
#endif

// Define UART Transmit Mode
#ifndef	_UART_MODE_
#define _UART_MODE_
#define		UART_POLLING	0
#define		UART_TX_INT		1
#define		UART_RX_INT		2
#define		UART_BOTH_INT	3
#endif


/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef	enum {
	UART0_MAP_A3_A4 = 0, //A3--RXD, A4--TXD, A1--CTS, A2--RTS 
	UART1_MAP_A1_A2 = 1, //A1--RXD, A2--TXD, A5--CTS, A6--RTS 
	UART1_MAP_A22_A23 = 2, //A22--RXD, A23--TXD, A26--CTS, A27--RTS 
	UART2_MAP_A26_A27 = 3, //A26--RXD, A27--TXD, A24--CTS, A25--RTS	
}UART_MAP;


// Define Data Buffer Structure Type 
#ifndef	_DATA_BUF_
#define	_DATA_BUF_
typedef	struct	{
	UINT8 *	pBuf;		// pointer to buffer start address
	UINT16	Size;		// buffer size
}STRUCT_DATA_BUF;
#endif

// Define Uart Frame Structure Type
#ifndef	_STRUCT_UART_FRAME_FORMAT_
#define	_STRUCT_UART_FRAME_FORMAT_
typedef	struct {
	UINT32	BaudRate;		// baud rate
	UINT8 	CharBit;		// character bit
	UINT8 	StopBit;		// stop bit
	UINT8 	Parity;			// parity
	UINT8 	Mode;			// transmit mode, UART_TX_INT or UART_TX_POLLING				
	UINT8	ShadowEn;	
}STRUCT_UART_FORMAT;
#endif
//
// Define FIFO Structure Type 
#ifndef	_STRUCT_UART_FIFO_
#define	_STRUCT_UART_FIFO_
typedef	struct {
	UINT8 *	pFFData;		// data buffer pointer
	UINT8 	gUartxIntMode;
	UINT16	FFDepth;		// FIFO depth
	UINT16 	FFValidSize;	// valid data size in FIFO
	UINT16 	FFInOffset;		// index of FIFO in
	UINT16 	FFOutOffset;	// index of FIFO out
}STRUCT_UART_FIFO;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
// Function Declare 
volatile struct UART_REGS * UART_GetRegs(UINT8 UARTx);
UINT8 UART_Init(UINT8 UARTx, UART_CLK_SRC mSrc, UART_MAP mPortMap, STRUCT_UART_FORMAT sFrame,\
	STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf);
UINT16 UART_GetRxFFValidSize(UINT8 UARTx);
UINT8 UART_SendByte(UINT8 UARTx, UINT8 bData);
UINT8 UART_ReceByte(UINT8 UARTx, UINT8 *pData);
UINT8 UART_SendBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen);
UINT8 UART_ReceBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen);
UINT8 UART_DmaInit(UINT8 UARTx, UART_CLK_SRC mSrc, UART_MAP mPortMap, STRUCT_UART_FORMAT sFrame,\
	UINT8 nTxDmaChn, UINT8 nRxDmaChn);
UINT8 UART_DmaSend(UINT8 UARTx, UINT8 *pData, UINT32 nLen);
UINT8 UART_DmaRece(UINT8 UARTx, UINT8 *pData, UINT32 nLen);
void UARTx_IntProc(UINT8 UARTx);
//
#endif /*UART_H_*/

