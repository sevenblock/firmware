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
	SWPS_OK 			= 0,		// �ɹ�
	SWPS_FAIL			= 1,   		// ʧ��
	SWPS_PARAM_ERR 		= 2,		//��������
	SWPS_TX_INHIBIT 	= 3,		//���ͳ�ʱ��S2_RESUMEʱS2���ߺ�ʱ��û�м�⵽S1���ͣ�˵����ʱ����û����ӦS2��Resume����
	SWPS_TX_UNDERFLOW 	= 4,		//����FIFO�����
	SWPS_TX_FRAME_ERR 	= 5,		//����FIFO֡����
	SWPS_TX_TIMEOUT 	= 6,		//���ͳ�ʱ
	SWPS_RX_FRAME_ERR 	= 7, 		//����֡����
	SWPS_RX_CRC_ERR 	= 8,		//����CRC����
	SWPS_RX_EOF_ABSENT 	= 9,		//δ���յ�EOF֡����
	SWPS_RX_OVERFLOW 	= 10,		//����FIFO�����
	SWPS_RX_LEN_UNMATCH = 11,		//���ճ��ȴ���
	SWPS_RX_TIMEOUT 	= 0x12,		//���ճ�ʱ	
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



