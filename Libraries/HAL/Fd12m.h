/***************************************************************************************
* File name    :	Fd12m.h
* Function     :	The header of Fd12m.c
* Author       : 	Footman
* Date         :	2016/11/07
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef _FD12M_H_
#define _FD12M_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum
{
	FD12M_SRAM = 1,
	FD12M_RESET = (1<<1),
	FD12M_INT = (1<<2),			
}FD12M_MODE;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 Fd12m_Init(UINT32 nMin, UINT32 nMax, FD12M_MODE nMode);
void Fd12m_Start(void);
void Fd12m_Stop(void);
void Fd12m_ClrInt(void);
UINT16 Fd12m_GetData(void);


/***************************************************************************
* Subroutine:	Fd12m_Start
* Function:		
* Input:		None
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE void Fd12m_Start(void)
{
	Fd12mRegs.CTRL.bit.FdEn = 1;
}

/***************************************************************************
* Subroutine:	Fd12m_Stop
* Function:		
* Input:		None
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE void Fd12m_Stop(void)
{
	Fd12mRegs.CTRL.bit.FdEn = 0;
	Fd12mRegs.CTRL.bit.IntEn = 0;
}

/***************************************************************************
* Subroutine:	Fd12m_ClrInt
* Function:		
* Input:		None
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE void Fd12m_ClrInt(void)
{
	Fd12mRegs.CTRL.bit.IntFlag = 1;
}

/***************************************************************************
* Subroutine:	Fd12m_GetData
* Function:		
* Input:		None
* Output:		Fd12m output 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
__INLINE UINT16 Fd12m_GetData(void)
{
	return Fd12mRegs.FDOUT;
}

#endif
