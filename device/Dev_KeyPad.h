/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_KeyPad.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_KeyPad_H
#define Dev_KeyPad_H

#ifndef Dev_KeyPad_D
#define EX_Dev_KeyPad		extern
#else
#define EX_Dev_KeyPad
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************宏定义****************************************************
***************************************************************************************************/

#define Dev_KeyPad_Up								0x01
#define Dev_KeyPad_Down							0x02
#define Dev_KeyPad_DownAndDown			0x03
/****************************************功能定义***************************************************
***************************************************************************************************/
EX_Dev_KeyPad s32 Dev_KeyPad_UpKey(void);
EX_Dev_KeyPad u8 Dev_KeyPad_ReadKeyPad(void);
EX_Dev_KeyPad void Dev_KeyPad_CheckKeyPad(void);
EX_Dev_KeyPad s32 Dev_KeyPad_CheckChrg(void);
EX_Dev_KeyPad void Dev_KeyPad_PowerStateCheck(void);
EX_Dev_KeyPad void Dev_KeyPad_Shutdown(void);
EX_Dev_KeyPad void Dev_KeyPad_KeyPadInit(void);
EX_Dev_KeyPad void Dev_KeyPad_PowerOn(void);

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

