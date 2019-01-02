/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_Lcd.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_Lcd_H
#define Dev_Lcd_H

#ifndef Dev_Lcd_D
#define EX_Dev_Lcd		extern
#else
#define EX_Dev_Lcd
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************宏定义****************************************************
***************************************************************************************************/
#define LCD_INTERFACE_SPI

/****************************************功能定义***************************************************
***************************************************************************************************/
EX_Dev_Lcd void LCD_Refresh(void);
EX_Dev_Lcd s32 LCD_FillLine(u8 t_aFont,u8 t_aLine,u8 t_cData);
EX_Dev_Lcd void LCD_FillScreen(u8 t_cData,u8 t_cPara);
EX_Dev_Lcd s32 LCD_DisplayRow(LCDPARA *t_puLcdPara);
EX_Dev_Lcd void LCD_LightTime(u8 cFlag,u32 t_nDelay);
EX_Dev_Lcd void LCD_FillRow(u8 t_aFont,u8 t_aLine, u8 t_aOffset,u8 t_aColumn, char *t_pcDisp, u8 t_aAtr);
EX_Dev_Lcd void LCD_FillPictrue(u8 cType);
EX_Dev_Lcd void LCD_FillIcon(u8 cType,u8 t_cPara);
EX_Dev_Lcd void LCD_Waiting(void);
EX_Dev_Lcd void LCD_Progress(u16 t_rCur,u16 t_rTotal);
EX_Dev_Lcd void LCD_BackLightAdj(u8 t_cType);
EX_Dev_Lcd void LCD_Close(void);
EX_Dev_Lcd void LCD_init(void);
#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

