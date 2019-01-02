/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_Timer.h
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_Timer_H
#define Dev_Timer_H

#ifndef Dev_Timer_D
#define EX_Dev_Timer		extern
#else
#define EX_Dev_Timer
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************�궨��****************************************************
***************************************************************************************************/


/****************************************���ܶ���***************************************************
***************************************************************************************************/
EX_Dev_Timer void Dev_Timer_Init(void);
EX_Dev_Timer void SysTick_Handler(void);
EX_Dev_Timer void Dev_Timer_Test(void);
EX_Dev_Timer u32 Dev_Timer_GetTick(void);
EX_Dev_Timer s32 Dev_Timer_IsTimeOut(u32 t_nTick,u32 t_nDelay);
EX_Dev_Timer void Dev_Timer_DelayMs(u32 t_nDelays);

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/
