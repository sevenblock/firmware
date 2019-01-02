/****************************************************************************
* File name    :	AS569_Regs.c
* Function     :	the peripheral registers viarible defined
* Author       : 	coins
* Date         :	2015/05/27
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "AS569_Device.h"


/**************************************************************************
* Local Macro Definition
***************************************************************************/

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
#if defined ( __CC_ARM   )      /*------------------RealView Compiler -----------------*/
__NO_INIT volatile struct SCM_REGS ScmRegs 				__AT(SCM_BASE);   
__NO_INIT volatile struct GPIO_REGS GpioRegs			__AT(GPIO_BASE); 
__NO_INIT volatile struct TMR_REGS T0Regs				__AT(T0_BASE);    
                                            
                                            
__NO_INIT volatile struct USART_REGS Spi0Regs			__AT(SPI0_BASE);	

__NO_INIT volatile struct USB_REGS UsbRegs				__AT(USB_BASE);
__NO_INIT volatile struct TMR_REGS T1Regs				__AT(T1_BASE);
__NO_INIT volatile struct TMR_REGS T2Regs				__AT(T2_BASE);
__NO_INIT volatile struct SSI_REGS SsiRegs				__AT(SSI_BASE);
__NO_INIT volatile struct UART_REGS Uart0Regs			__AT(UART0_BASE);
__NO_INIT volatile struct I2C_REGS I2C0Regs				__AT(I2C0_BASE);
__NO_INIT volatile struct SWPS_REGS SwpsRegs			__AT(SWPS_BASE);
__NO_INIT volatile struct FD12M_REGS Fd12mRegs			__AT(FD12M_BASE);
__NO_INIT volatile struct AOSS_REGS AossRegs			__AT(AOSS_BASE);
__NO_INIT volatile struct ISO7816_REGS Iso7816Regs		__AT(ISO7816_BASE);
__NO_INIT volatile struct SCI_REGS SciRegs				__AT(SCI_BASE);
__NO_INIT volatile struct DMA_REGS DmaRegs				__AT(DMA_BASE);
__NO_INIT volatile struct VPWM_REGS VpwmRegs			__AT(VPWM_BASE);
__NO_INIT volatile struct SDSI_REGS SdsiRegs			__AT(SDSI_BASE);

__NO_INIT volatile struct EFC_REGS EfcRegs				__AT(EFC_BASE);
__NO_INIT volatile struct UART_REGS Uart1Regs			__AT(UART1_BASE);
__NO_INIT volatile struct UART_REGS Uart2Regs			__AT(UART2_BASE);
__NO_INIT volatile struct TMR_REGS T3Regs				__AT(T3_BASE);

__NO_INIT volatile struct SDI_REGS SdiRegs				__AT(SDI_BASE);
__NO_INIT volatile struct USART_REGS Spi1Regs			__AT(SPI1_BASE);
__NO_INIT volatile struct TMR_REGS T4Regs				__AT(T4_BASE);
__NO_INIT volatile struct TMR_REGS T5Regs				__AT(T5_BASE);
__NO_INIT volatile struct TMR_REGS T6Regs				__AT(T6_BASE);
__NO_INIT volatile struct TMR_REGS T7Regs				__AT(T7_BASE);
__NO_INIT volatile struct SQI_REGS SqiRegs				__AT(SQI_BASE);
__NO_INIT volatile struct CRC_REGS CrcRegs				__AT(CRC_BASE);
__NO_INIT volatile struct CACHE_REGS CacheRegs			__AT(CACHE_BASE);

__NO_INIT volatile struct APSI_REGS ApsiRegs			__AT(APSI_BASE);
// SDSI ADMA2 Descriptor Table
__NO_INIT volatile struct ADMA2_DESC_TBL SdsiADMA2DescTbl[32]	__AT(SDSI_ADMA2_DESC_TBL_ADDR);

#endif



/**************************************************************************
* Local Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/


