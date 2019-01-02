/***************************************************************************
* File name    :	Swps.h
* Function     :	Header of SWP Slave controller
* Author       : 	hongjz
* Date         :	2016/11/07
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SWPS_H_
#define SWPS_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/

	
/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the return of SWPS operation
typedef	enum	{
	SWPS_OK 			= 0,		// 成功
	SWPS_FAIL			= 1,   		// 失败
	SWPS_PARAM_ERR 		= 2,		//参数错误
	SWPS_TX_INHIBIT 	= 3,		//发送超时，S2_RESUME时S2拉高后长时间没有检测到S1拉低，说明这时主机没有响应S2的Resume请求
	SWPS_TX_UNDERFLOW 	= 4,		//发送FIFO空溢出
	SWPS_TX_FRAME_ERR 	= 5,		//发送FIFO帧错误
	SWPS_TX_TIMEOUT 	= 6,		//发送超时
	SWPS_RX_FRAME_ERR 	= 7, 		//接收帧错误
	SWPS_RX_CRC_ERR 	= 8,		//接收CRC错误
	SWPS_RX_EOF_ABSENT 	= 9,		//未接收到EOF帧错误
	SWPS_RX_OVERFLOW 	= 10,		//接收FIFO满溢出
	SWPS_RX_LEN_UNMATCH = 11,		//接收长度错误
	SWPS_RX_TIMEOUT 	= 0x12,		//接收超时	
}SWPS_ERR;

// define the SWP State
#ifndef	_TYPE_SWP_STATE_
#define	_TYPE_SWP_STATE_
typedef	enum	{
	SWP_DEACTIVATED 	= 0,	// SWPS operation ok
	SWP_CLF_START 		= 1,
	SWP_SUSPENDED 		= 2,
	SWP_ACTIVATED 		= 3,	
}SWP_STATE;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
SWPS_ERR SWPS_Init(void);
SWPS_ERR SWPS_SendFrame(UINT8 *pData, UINT8 nLen, UINT32 nLoops);
SWPS_ERR SWPS_GetFrame(UINT8 *pData, UINT8 *pLen, UINT32 nLoops);
SWP_STATE SWPS_GetState(void);


#endif



