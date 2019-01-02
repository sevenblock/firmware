/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_FIFO.h
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_FIFO_H
#define Dev_FIFO_H

#ifndef Dev_FIFO_D
#define EX_Dev_FIFO		extern
#else
#define EX_Dev_FIFO
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************�궨��****************************************************
***************************************************************************************************/
typedef	struct {
	u8 *m_pData;				// data buffer pointer
	u16 m_Depth;				// FIFO depth
	u16 m_ValidSize;			// valid data size in FIFO
	u16 m_InOffset;			// index of FIFO in
	u16 m_OutOffset;		// index of FIFO out
}STRUCT_COM_FIFO;

/****************************************���ܶ���***************************************************
***************************************************************************************************/
EX_Dev_FIFO void Dev_FIFO_Init(STRUCT_COM_FIFO *t_puFifo, u8 *t_paBuf, u16 t_rSize);
EX_Dev_FIFO s32 Dev_FIFO_Put(STRUCT_COM_FIFO *t_puFifo, u8 *t_paData, u16 t_rDataLen);
EX_Dev_FIFO s32 Dev_FIFO_Get(STRUCT_COM_FIFO *t_puFifo, u8 *t_paData, u16 t_rDataLen);
EX_Dev_FIFO u16 Dev_FIFO_FreeSize(STRUCT_COM_FIFO *t_puFifo);

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

