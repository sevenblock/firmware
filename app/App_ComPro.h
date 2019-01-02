/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :App_ComPro.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef App_ComPro_H
#define App_ComPro_H

#ifndef App_ComPro_D
#define EX_App_ComPro		extern
#else
#define EX_App_ComPro
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************宏定义****************************************************
***************************************************************************************************/


/****************************************功能定义***************************************************
***************************************************************************************************/
EX_App_ComPro void App_ComPro_SetResErr(SENDCOMData *t_puSendData,u16 t_rResponse);
EX_App_ComPro void App_ComPro_SetResSuccess(SENDCOMData *t_puSendData,u16 t_rDataLen);
EX_App_ComPro void App_ComPro_EncryptResSuccess(SENDCOMData *t_puSendData,u16 t_rDataLen,u8 *t_paRand);
EX_App_ComPro void App_ComPro_MacResSuccess(SENDCOMData *t_puSendData,u16 t_rDataLen,u8 *t_paRand);
EX_App_ComPro void App_ComPro_ComDeal(void);
EX_App_ComPro void App_ComPro_SendData(STRPROTOCOL *t_puProtocolData,SENDCOMData *t_puSendData);
EX_App_ComPro void App_ComPro_SendResponseData(u8 *t_paData,u16 t_rDataLen,u16 t_rResponse,u8 t_MacType);
EX_App_ComPro s32 App_ComPro_SaveSendDataBac(u8 *t_paData,u16 t_rDataLen);
#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

