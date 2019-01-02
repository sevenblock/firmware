/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_KeyPad.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_KeyPad_D
#include "..\block\app\App_Macro.h"


#define GPIO_PIN_CHECK			24
#define GPIO_PIN_UP				16
#define GPIO_PIN_DOWN			17
#define GPIO_PIN_POWER		18
#define GPIO_PIN_CHRG			19

#define KEYPAD_UPKEYDELAY						5
#define KEYPAD_DOWNKEYDELAY				10

static u8 s_aPowerKeyPressTimes = 0;
static u8 s_aKeyPrevKey = 0;
static u8 s_aUpKeyFlag = 0;
static u8 s_aDownKeyFlag = 0;

/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_KeyPadInit()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_KeyPad_KeyPadInit(void)
{
	GPIO_Init(GPIOA, GPIO_PIN_CHECK, INPUT);	
	GPIO_Init(GPIOA, GPIO_PIN_UP, INPUT);
	GPIO_Init(GPIOA, GPIO_PIN_DOWN, INPUT);
	GPIO_Init(GPIOA, GPIO_PIN_CHRG, INPUT);

	GPIO_BitSet(GPIOA, GPIO_PIN_POWER);
	GPIO_Init(GPIOA, GPIO_PIN_POWER, OUTPUT);
}

/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_CheckPower()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_KeyPad_CheckPower(void)
{	
	if(GPIO_BitIn(GPIOA,GPIO_PIN_CHECK))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_CheckChrg()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_KeyPad_CheckChrg(void)
{	
	if(GPIO_BitIn(GPIOA,GPIO_PIN_CHRG))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_UpKey()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_KeyPad_UpKey(void)
{
	if(GPIO_BitIn(GPIOA,GPIO_PIN_UP))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_DownKey()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
s32 Dev_KeyPad_DownKey(void)
{
	if(GPIO_BitIn(GPIOA,GPIO_PIN_DOWN))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_PowerOff()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_KeyPad_PowerOff(void)
{
	GPIO_BitClr(GPIOA, GPIO_PIN_POWER);
}

/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_PowerOn()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_KeyPad_PowerOn(void)
{
	GPIO_BitSet(GPIOA, GPIO_PIN_POWER);
}
/*
****************************************************************************************************
** 名    称 ：Dev_KeyPad_Shutdown()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
void Dev_KeyPad_Shutdown(void)
{
	s_aPowerKeyPressTimes = 0;
	ValSystemInfo.cCheckEnable = 2;
	ValSystemInfo.cCloseDev= Power_Off;
	ValSystemInfo.cSysState = SysState_Ide;
	
	API_CB_ClearScreen(0x00,0);

	LCD_Close();

	//while(1)
	Dev_KeyPad_PowerOff();       //断电
}

/*
**********************************************************************************************************
** 名    称 ：Dev_KeyPad_CheckPowerKeyOff()
** 功    能 ：
** 入口参数 ：无 
** 出口参数 ：无
** 备    注 :
**********************************************************************************************************
*/
void Dev_KeyPad_CheckPowerKeyOff(void)
{  
    if(!Dev_KeyPad_CheckPower())
    {
        if(s_aPowerKeyPressTimes < 15)
        {
            s_aPowerKeyPressTimes++;
        }
        else
        {
            ValSystemInfo.cCheckEnable = 1;
        }
    }
}
/*
**********************************************************************************************************
** 名    称 ：Dev_KeyPad_CheckPowerKeyReOn()
** 功    能 ：
** 入口参数 ：无 
** 出口参数 ：无
** 备    注 :
**********************************************************************************************************
*/
void Dev_KeyPad_CheckPowerKeyReOn(void)
{   
    Dev_KeyPad_PowerOff();       //断电

    if(s_aPowerKeyPressTimes < 8)
    {
        if(!Dev_KeyPad_CheckPower()) //松开电源键  100ms * 8
        {   
            s_aPowerKeyPressTimes++;
        }
        else        //关机一直按着不放开电源键
        {
            s_aPowerKeyPressTimes = 0;
        }       
    }
    else
    {
        if(Dev_KeyPad_CheckPower())  //关机松开后再按电源键，重启
        {   
            s_aPowerKeyPressTimes++;
        }
        else
        {
            s_aPowerKeyPressTimes = 8; //关机松开后不再按电源键
        }       

        if(s_aPowerKeyPressTimes >= 12)
        {
			u8 cTemp[2];

			Dev_KeyPad_PowerOn();

			cTemp[0] = ValSystemInfo.m_BtState;
			cTemp[1] = ValSystemInfo.m_BlePinComple;

			DDEBUG("ValSystemInfo.m_BlePinComple = %d",ValSystemInfo.m_BlePinComple);

			App_Main_InitVariable();

			App_Main_SetLogoType();

			ValSystemInfo.m_BtState = cTemp[0];
			ValSystemInfo.m_BlePinComple = cTemp[1];

			LCD_init();

			App_Main_EnterWork();
        }
    }
}
/*
**********************************************************************************************************
** 名    称 ：Dev_KeyPad_CheckPowerOff()
** 功    能 ：
** 入口参数 ：无 
** 出口参数 ：无
** 备    注 :
**********************************************************************************************************
*/
void Dev_KeyPad_CheckPowerOff(void)
{
    static u8 cCheckFlg = 0;
    
    if(Dev_KeyPad_CheckPower())
    {       
        if(++cCheckFlg >= 10)
        {
            if(ValSystemInfo.cCloseDev== Power_On)
            {
                ValSystemInfo.cCloseDev= Power_Ready;
                ValSystemInfo.cTimeStep = 0;
            }
            else if(cCheckFlg >= 30)
            {
                cCheckFlg = 0;
                Dev_KeyPad_Shutdown();
            }
        }
    }
    else
    {
        cCheckFlg = 0;
    }
}

/*
**********************************************************************************************************
** 名    称 ：Dev_KeyPad_PowerStateCheck()
** 功    能 ：100ms check
** 入口参数 ：无 
** 出口参数 ：无
** 备    注 :
**********************************************************************************************************
*/
void Dev_KeyPad_PowerStateCheck(void)
{
	if(ValSystemInfo.cCheckEnable == 1)
	{
		Dev_KeyPad_CheckPowerOff();
	}
	else if (ValSystemInfo.cCheckEnable == 2)
	{
		Dev_KeyPad_CheckPowerKeyReOn();
	}
	else
	{
		Dev_KeyPad_CheckPowerKeyOff();
	}
}

/*
**********************************************************************************************************
** 名    称 ：App_Main_CheckKeyPad()
** 功    能 ：10ms
** 入口参数 ：无 
** 出口参数 ：无
** 备    注 :
**********************************************************************************************************
*/
void Dev_KeyPad_CheckKeyPad(void)
{    
    if(Dev_KeyPad_UpKey())
    {       
		if(s_aUpKeyFlag < KEYPAD_UPKEYDELAY)
		{
			s_aUpKeyFlag++;
			
	        if(s_aUpKeyFlag >= KEYPAD_UPKEYDELAY)		// 100ms防抖计数
	        {
				if(Dev_KeyPad_DownKey() && s_aDownKeyFlag >= KEYPAD_DOWNKEYDELAY)
				{
					s_aKeyPrevKey |= Dev_KeyPad_DownAndDown;
				}
				else
				{
					s_aKeyPrevKey |= Dev_KeyPad_Up;
				}

				s_aUpKeyFlag = KEYPAD_DOWNKEYDELAY;
	        }
		}
    }
	else
	{
		if(s_aUpKeyFlag)
		{
			s_aUpKeyFlag--;
			
			if(s_aUpKeyFlag <= KEYPAD_UPKEYDELAY)
			{
				s_aUpKeyFlag = 0;
			}
		}
	}

    if(Dev_KeyPad_DownKey())
    {       
		if(s_aDownKeyFlag < KEYPAD_UPKEYDELAY)
		{
			s_aDownKeyFlag++;
			
	        if(s_aDownKeyFlag >= KEYPAD_UPKEYDELAY)		// 100ms防抖计数
	        {
				if(Dev_KeyPad_UpKey() && s_aUpKeyFlag >= KEYPAD_DOWNKEYDELAY)
				{
					s_aKeyPrevKey |= Dev_KeyPad_DownAndDown;
				}
				else
				{
					s_aKeyPrevKey |= Dev_KeyPad_Down;
				}

				s_aDownKeyFlag = KEYPAD_DOWNKEYDELAY;
	        }
		}
    }
	else
	{
		if(s_aDownKeyFlag)
		{
			s_aDownKeyFlag--;
			
			if(s_aDownKeyFlag <= KEYPAD_UPKEYDELAY)
			{
				s_aDownKeyFlag = 0;
			}
		}
	}
}

/*
****************************************************************************************************
** 名    称 ：App_Main_ReadKeyPad()
** 功    能 ：
** 入口参数 ：无
** 出口参数 ：无
** 返    回 ：
** 备    注 ：
****************************************************************************************************
*/
u8 Dev_KeyPad_ReadKeyPad(void)
{
	u8 aKey = 0;
	
	if(s_aKeyPrevKey)
	{
		aKey = s_aKeyPrevKey;

		s_aKeyPrevKey = 0;
	}

	return aKey;
}

/****************************************End Of File************************************************
***************************************************************************************************/


