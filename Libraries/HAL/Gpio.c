/****************************************************************************
* File name    :	Gpio.c
* Function     :
* Author       : 	Footman
* Date         :	2016/11/04
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "AS569.h"

/**********************************************************************************
* Local Macro Definition
**********************************************************************************/


/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/


/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/


/**********************************************************************************
* Functon
**********************************************************************************/

/***********************************************************************************
 * Subroutine:	GPIO_Init
 * Function:	Write GPIO bit use value
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * 				UINT8 nMode --		GPIO bit value;
 * Output:		
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_Init(UINT8 nPort, UINT8 nBitNum, UINT8 nMode)
{
	UINT8 temp;
	UINT32 lRegStatus;
	volatile UINT32 *pUse;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pUse = (volatile UINT32 *)&(GpioRegs.GPAUSE0);
	else pUse = (volatile UINT32 *)&(GpioRegs.GPBUSE0);

	temp  = nBitNum/8;
	pUse += temp;
	temp = (nBitNum&7)*4;
	lRegStatus = __get_PRIMASK();	// save PRIMASK
	__disable_irq();						// disable interrupt
	*pUse = (*pUse & (~((UINT32)0xf << temp))) | ((UINT32)nMode << temp);
	__set_PRIMASK(lRegStatus);		// recover
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitIn
 * Function:	Read GPIO bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB, GPIOD, GPIOE supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return GPIOXn bit value 0 or 1
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitIn(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPADATA);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBDATA);
	return (*pData >> nBitNum) & 0x1;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitOut
 * Function:	output GPIO bit
 * Input:		PORT_TYPE nPort -- 	GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * 				UINT8 nBitVal --	the value of GPIO bit output, 0 or 1
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:	to set the value of specified bit
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitOut(UINT8 nPort, UINT8 nBitNum, UINT8 nBitVal)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPADATA);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBDATA);

	if(nBitVal) (*pData) |= (1<<nBitNum);
	else (*pData) &= ~(UINT32)(1<<nBitNum);
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitTog
 * Function:	Toggle GPIO bit
 * Input:		PORT_TYPE nPort -- 	GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitTog(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPATOG);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBTOG);

	(*pData) = (1<<nBitNum);
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitSet
 * Function:	Set GPIO bit
 * Input:		PORT_TYPE nPort -- 	GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitSet(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPASET);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBSET);
							  
	(*pData) = (1<<nBitNum);
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitClr
 * Function:	Clear GPIO bit
 * Input:		PORT_TYPE nPort -- 	GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitClr(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPACLR);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBCLR);

	(*pData) = (1<<nBitNum);
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitIntClr
 * Function:	Clear GPIO interrupt flag bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitIntClr(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPAINT);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBINT);

	(*pData) |= (1<<nBitNum);	
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_GetBitUse
 * Function:	Get GPIO bit use
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_GetBitUse(UINT8 nPort, UINT8 nBitNum)
{
	UINT8 temp;
	volatile UINT32 *pUse;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pUse = (volatile UINT32 *)&(GpioRegs.GPAUSE0);
	else pUse = (volatile UINT32 *)&(GpioRegs.GPBUSE0);

	temp  = nBitNum/8;
	pUse += temp;
	temp = (nBitNum&7)*4;
	return (*pUse>>temp)&0xf;	
}

/***********************************************************************************
 * Subroutine:	GPIO_BitPullSet
 * Function:	Set the bit of GPIOxPE Register 
 * Input:		UINT8 bPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 bBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitPullSet(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPAPS);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBPS);

	(*pData) |= (1<<nBitNum);		
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitPullClr
 * Function:	Clear the bit of GPIOxPE register 
 * Input:		UINT8 bPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 bBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitPullClr(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPAPS);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBPS);
	
	(*pData) &= ~(1<<nBitNum);		
	return RT_OK;	
}

///***********************************************************************************
// * Subroutine:	GPIO_BitODSet
// * Function:	Set the bit of GPxOD Register 
// * Input:		UINT8 bPort -- 		GPIO port, GPIOA, GPIOB supported;
// * 				UINT8 bBitNum -- 	GPIO bit number of each port, according to each port;
// * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
// * Description:
// * Date:		2016/11/04
// * ModifyRecord:
// * ********************************************************************************/
//UINT8 GPIO_BitODSet(UINT8 nPort, UINT8 nBitNum)
//{
//	volatile UINT32 *pData;
//
//	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
//	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPAOD);
//	else pData = (volatile UINT32 *)&(GpioRegs.GPBOD);
//
//	(*pData) |= (1<<nBitNum);		
//	return RT_OK;
//}

///***********************************************************************************
// * Subroutine:	GPIO_BitODClr
// * Function:	Clear the bit of GPxOD register 
// * Input:		UINT8 bPort -- 		GPIO port, GPIOA, GPIOB supported;
// * 				UINT8 bBitNum -- 	GPIO bit number of each port, according to each port;
// * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
// * Description:
// * Date:		2016/11/04
// * ModifyRecord:
// * ********************************************************************************/
//UINT8 GPIO_BitODClr(UINT8 nPort, UINT8 nBitNum)
//{
//	volatile UINT32 *pData;
//
//	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
//	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPAOD);
//	else pData = (volatile UINT32 *)&(GpioRegs.GPBOD);
//	
//	(*pData) &= ~(1<<nBitNum);		
//	return RT_OK;	
//}

///***********************************************************************************
// * Subroutine:	GPIO_BitDriveConfig
// * Function:	Config GPIO bit Drive
// * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
// * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
// *				UINT8 nBitVal --	the value of GPIO Driver Register bit 0~3
// * Output:		UINT8	--			RT_OK or RT_FAIL
// * Description:
// * Date:		2016/11/04
// * ModifyRecord:
// * ********************************************************************************/
//UINT8 GPIO_BitDriveConfig(UINT8 nPort, UINT8 nBitNum, UINT8 nBitVal)
//{
//	UINT8 temp;
//	UINT32 lRegStatus;
//	volatile UINT32 *pDrive;
//
//	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
//	if(nPort == GPIOA) pDrive = (volatile UINT32 *)&(GpioRegs.GPADRIVE0);
//	else pDrive = (volatile UINT32 *)&(GpioRegs.GPBDRIVE0);
//	
//	temp  = (nBitNum/16);
//	pDrive += temp;
//	temp = (nBitNum&0xf)*2;
//	lRegStatus = __get_PRIMASK();	  // save STATUS32					
//	__disable_irq();			  	// disable interrupt
//	*pDrive = (*pDrive & (~((UINT32)0x3 << temp))) | ((UINT32)nBitVal << temp);
//	__set_PRIMASK(lRegStatus);		// recover
//	return RT_OK;
//}

/***********************************************************************************
 * Subroutine:	GPIO_PortIn
 * Function:	read whole GPIO port
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * Output:		UINT32	--			Return specific port value
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT32 GPIO_PortIn(UINT8 nPort)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPADATA);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBDATA);
	return *pData;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortOut
 * Function:	output whole GPIO port
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nPortVal --  the value of GPIO port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortOut(UINT8 nPort, UINT32 nPortVal)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPADATA);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBDATA);	 
 	(*pData) = nPortVal;
 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortTog
 * Function:	Toggle GPIO port with specified bits
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nPortVal -- to specify GPIO bits to be toggled;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortTog(UINT8 nPort, UINT32 nPortVal)
{
 	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPATOG);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBTOG);
 	(*pData) = nPortVal;
 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortSet
 * Function:	Set GPIO port with specified bits
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nPortVal -- to specify GPIO bits to be set;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortSet(UINT8 nPort, UINT32 nPortVal)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPASET);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBSET);
 	(*pData) = nPortVal;

 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortClr
 * Function:	Clear GPIO port with specified bits
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nPortVal -- to specify GPIO bits to be cleared;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortClr(UINT8 nPort, UINT32 nPortVal)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPACLR);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBCLR);
 	(*pData) = nPortVal;

 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortIntStat
 * Function:	read GPIO port interrupt state
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * Output:		UINT32	--			Return specific port value
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT32 GPIO_PortIntStat(UINT8 nPort)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPAINT);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBINT);
	return *pData;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortIntClr
 * Function:	Clear GPIO interrupt flag bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2016/11/04
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortIntClr(UINT8 nPort, UINT32 nSpecBits)
{
	volatile UINT32 *pData;

	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	if(nPort == GPIOA) pData = (volatile UINT32 *)&(GpioRegs.GPAINT);
	else pData = (volatile UINT32 *)&(GpioRegs.GPBINT);
	(*pData) = nSpecBits;	
	return RT_OK;
}
