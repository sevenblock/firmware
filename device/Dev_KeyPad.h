/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_KeyPad.h
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
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
/*****************************************�궨��****************************************************
***************************************************************************************************/

#define Dev_KeyPad_Up								0x01
#define Dev_KeyPad_Down							0x02
#define Dev_KeyPad_DownAndDown			0x03
/****************************************���ܶ���***************************************************
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

