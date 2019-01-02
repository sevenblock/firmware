/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_Uart.c
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
****************************************************************************************************
***************************************************************************************************/
#define Dev_Uart_D
#include "..\block\app\App_Macro.h"

UINT8 TxArray[512];
UINT8 RxArray[256+4*1024];

UINT8 Tx0Array[512];
UINT8 Rx0Array[256+4*1024];

/*
****************************************************************************************************
** ��    �� ��Dev_Uart_Init()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
****************************************************************************************************
*/
void Dev_Uart_Init(void)
{
	STRUCT_UART_FORMAT sFrame;
	STRUCT_DATA_BUF RxBuf;
	STRUCT_DATA_BUF TxBuf;
	
	sFrame.BaudRate = 115200;			// baud rate
	sFrame.CharBit = CHAR_8BIT;		// character bit
	sFrame.StopBit = ONE_STOP;		// stop bit
	sFrame.Parity = NONE;					// parity
	sFrame.Mode = UART_RX_INT;		//send check

	TxBuf.pBuf=TxArray;
	TxBuf.Size=sizeof(TxArray);
	RxBuf.pBuf=RxArray;
	RxBuf.Size=sizeof(RxArray);
	
	UART_Init(UART2, UART_CLK_SRC_FCLK, UART2_MAP_A26_A27, sFrame, &RxBuf, &TxBuf);

	if(sFrame.Mode != UART_POLLING)
	{
		NVIC_EnableIRQ(UART2_IRQn);
	}

	TxBuf.pBuf=Tx0Array;
	TxBuf.Size=sizeof(Tx0Array);
	RxBuf.pBuf=Rx0Array;
	RxBuf.Size=sizeof(Rx0Array);
	
	UART_Init(UART0, UART_CLK_SRC_FCLK, UART0_MAP_A3_A4, sFrame, &RxBuf, &TxBuf);

	if(sFrame.Mode != UART_POLLING)
	{
		NVIC_EnableIRQ(UART0_IRQn);
	}
	
}

/*
****************************************************************************************************
** ��    �� ��Dev_Uart_Test()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
****************************************************************************************************
*/
void Dev_Uart_Test(void)
{
	UINT8 rb;
	
	if(UART_ReceBuf(UART2, &rb, 1) == RT_OK)
	{
		UART_SendBuf(UART2, &rb, 1);
	}
}

/****************************************End Of File************************************************
***************************************************************************************************/

