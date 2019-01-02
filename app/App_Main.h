/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :App_Main.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2018-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef App_Main_H
#define App_Main_H

#ifndef App_Main_D
#define EX_App_Main		extern
#else
#define EX_App_Main
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************宏定义****************************************************
***************************************************************************************************/

#define Power_On                	0
#define Power_Ready  	   		1
#define Power_Off               	2

#define MAIN_DISP_STYLE_NULL				0
#define MAIN_DISP_STYLE_CHAR				1
#define MAIN_DISP_STYLE_ARROW			2
#define MAIN_DISP_STYLE_TICK				3
#define MAIN_DISP_STYLE_NEXT				4
#define MAIN_DISP_STYLE_ONLYUP			5
#define MAIN_DISP_STYLE_ONLYDOWN	6


/****************************************功能定义***************************************************
***************************************************************************************************/
EX_App_Main void App_Main_Run(void);
EX_App_Main void App_Main_IRQ1Ms(void);
EX_App_Main void App_Main_InitVariable (void);
EX_App_Main void App_Main_EnterWork(void);
EX_App_Main void App_Main_SetSysState(SysState t_aSysState);
EX_App_Main SysState App_Main_GetSysState(void);
EX_App_Main void App_Main_SetOperateTimeOut(u16 t_rTimeout);
EX_App_Main s32 App_Main_WaitingForConfirm(u32 t_nTimeOut,u8 t_aStyle);
EX_App_Main s32 App_Main_DisPlayTradeNum(u8 *t_paCoinType,u8 *t_paNum);
EX_App_Main s32 App_Main_DisPlayPubAddress(u8 *t_paMsg,u16 t_rMsgLen);
EX_App_Main s32 App_Math_MnemonicDisp(char *t_paWord);
EX_App_Main s32 App_Math_MnemonicDispOnce(u8 *t_pnOffSet,u8 *t_pnPage,char *t_paWord);
EX_App_Main s32 App_Math_MnemonicDispByOne(u8 *t_pnOffSet,u8 *t_pnPage,char *t_paWord);
EX_App_Main void App_Main_AgingTest(void);
EX_App_Main void App_Main_BatteryCharging(u8 t_aRefresh);
EX_App_Main s32 App_Main_GetLockTimeState(void);
EX_App_Main void App_Main_CheckLockTime(void);
EX_App_Main s32 App_Main_DispSelectWord(u8 t_aOffset,u8 t_aLine1,char *t_pcSelectWord);
EX_App_Main s32 App_Main_OnWaitting(SENDCOMData *t_puSendData);
EX_App_Main void App_Main_SetLogoType(void);

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

