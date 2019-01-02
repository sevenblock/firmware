/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :App_Debug.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef App_Debug_H
#define App_Debug_H

#ifndef App_Debug_D
#define EX_App_Debug		extern
#else
#define EX_App_Debug
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************宏定义****************************************************
***************************************************************************************************/

#define DEBUG_BUF_SIZE			256
#define LOG_TAG 						"[SevenBlock]"

/****************************************功能定义***************************************************
***************************************************************************************************/

EX_App_Debug void App_Debug_Printf(const char *tag,const char *fmt,...);
EX_App_Debug void App_Debug_PrintfLine(const char *tag,const char *str,const char *func);
EX_App_Debug void App_Debug_PrintfLineExc(const char *tag,const char *str,const char *func,const char *fmt,...);
EX_App_Debug void App_Debug_PrintfHex(char *t_pcHead, const unsigned char *t_pcBuf, unsigned int t_tLength) ;


#ifdef DEBUG_OPEN

#define APP_Macro_PrintfStart() 		
#define APP_Macro_PrintfEnd() 		
#define APP_Macro_PrintfStartExc(fmt,...) 		
#define APP_Macro_PrintfEndExc(fmt,...) 		
#define DDEBUG(fmt,...)											//不打印log
#define DDEBUGHEX(fmt,...)									//不打印log

#endif

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/
