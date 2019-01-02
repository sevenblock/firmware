/***************************************************************************************
* File name    :	Timer.h
* Function     :	Header of Timer.c
* Author       : 	Footman
* Date         :	2016/11/04
* Version      :    v1.0
* Description  :    Header of Timer Hardware Abstract Layer
* ModifyRecord :
*****************************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/		
#define T0	ID_T0
#define T1	ID_T1
#define T2	ID_T2
#define T3	ID_T3
#define T4	ID_T4
#define T5	ID_T5
#define T6	ID_T6
#define T7	ID_T7

// define the function name for old firmware library revision
#define		TimerInit		TMR_Init
#define		StartTimer		TMR_Start
#define		StopTimer		TMR_Stop
#define		ClrTimerCnt		TMR_ClrCnt	

/**************************************************************************
* Global Type Definition
***************************************************************************/
//define timer mode
typedef enum
{
	TMR_CNT,		// mode count without interrupt
	TMR_INT,		// mode count with interrupt
	TMR_WDT,		// WDT
	TMR_CAP,		// cap mode		
}TMR_MODE;

//define timer clock source
typedef enum
{
	TCLK_SRC_FCLK = 0,
	TCLK_SRC_HFROSC = 8,	
	TCLK_SRC_MFROSC = 9,
	TCLK_SRC_LFROSC = 10,
	TCLK_SRC_XTAL = 11,
	TCLK_SRC_CLKOUT = 13,
	TCLK_SRC_RTCCLK = 14,
	TCLK_SRC_TMRX = 15,
}TCLK_SRC;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 TMR_Init(UINT8 nTimerNum, UINT32 nLimit, TCLK_SRC nClkSrc, TMR_MODE nMode);
UINT8 TMR_Start(UINT8 nTimerNum);
UINT8 TMR_Stop(UINT8 nTimerNum);
UINT8 TMR_ClrCnt(UINT8 nTimerNum);
UINT8 TMR_GetCnt(UINT8 nTimerNum, UINT32 *pCnt);
BOOL TMR_GetIntFlag(UINT8 nTimerNum);
UINT8 TMR_ClrIntFlag(UINT8 nTimerNum);

//captrue function
BOOL TMR_GetCapIntFlag(UINT8 nTimerNum);
UINT32 TMR_GetCapData(UINT8 nTimerNum);

#endif /*TIMER_H_*/
