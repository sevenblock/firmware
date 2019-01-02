/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :App_Debug.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define App_Debug_D
#include "App_Macro.h"

/*
****************************************************************************************************
** 名    称 ：API_CB_LogPrintf()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 :
****************************************************************************************************
*/
#if 1
void API_CB_LogPrintf(char *fmt,...)
{
	va_list ap;
	char string[DEBUG_BUF_SIZE];
	
	va_start(ap,fmt);
	
	vsprintf(string,fmt,ap);
	
	UART_SendBuf(UART0, (UINT8 *)string, strlen(string));  //写串口成功  
	
	va_end(ap);
}
#endif
/*
****************************************************************************************************
** 名    称 ：App_Debug_Printf()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 :
****************************************************************************************************
*/
void App_Debug_Printf(const char *tag,const char *fmt,...)
{
	va_list uVa;
	char gaString[DEBUG_BUF_SIZE] = {0x00};
	
	va_start(uVa,fmt);
	
	vsprintf(gaString,fmt,uVa);
		
	va_end(uVa);

	API_CB_LogPrintf("%s %s \r\n",tag,gaString);
}

/*
****************************************************************************************************
** 名    称 ：App_Debug_PrintfLine()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 :
****************************************************************************************************
*/
void App_Debug_PrintfLine(const char *tag,const char *str,const char *func)
{
	char gaString[DEBUG_BUF_SIZE] = {0x00};
	
	sprintf(gaString,str,func);
	
	API_CB_LogPrintf("%s %s \r\n",tag,gaString);
}

/*
****************************************************************************************************
** 名    称 ：App_Debug_PrintfLineExc()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 :
****************************************************************************************************
*/
void App_Debug_PrintfLineExc(const char *tag,const char *str,const char *func,const char *fmt,...)
{
	va_list uVa;
	char gaString[DEBUG_BUF_SIZE] = {0x00};
	unsigned int cLen;
	
	va_start(uVa,fmt);

	sprintf(gaString,str,func);

	cLen = strlen(gaString);
	
	vsprintf(&gaString[cLen],fmt,uVa);

	va_end(uVa);
		
	API_CB_LogPrintf("%s %s \r\n",tag,gaString);
}

/*
****************************************************************************************************
** 名    称 ：App_Debug_PrintfHex()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 :
****************************************************************************************************
*/
void App_Debug_PrintfHex(char *t_pcHead, const unsigned char *t_pcBuf, unsigned int t_rLength)
{
	unsigned int i;
	char szHex[16*3+1] = {0x00};

	if ((t_pcHead != NULL) && (strlen(t_pcHead)>0))
	{
		DDEBUG("%s:(Size=0x%04X, %d Bytes)", t_pcHead, t_rLength, t_rLength);
	}

	for (i = 0; i < t_rLength; i++)
	{
		sprintf(szHex + strlen(szHex), "%02X ", t_pcBuf[i]);

		if (((i + 1) % 16 == 0) ||(i + 1 == t_rLength))
		{
			while (strlen(szHex) < 16 * 3)
			{
				strcat(szHex, " ");
			}

			DDEBUG("%s", szHex);

			szHex[0] = 0x00;
		}
	}
}

/****************************************End Of File************************************************
***************************************************************************************************/
