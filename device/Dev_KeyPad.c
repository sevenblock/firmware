/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_KeyPad.c
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
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
** ��    �� ��Dev_KeyPad_KeyPadInit()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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
** ��    �� ��Dev_KeyPad_CheckPower()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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
** ��    �� ��Dev_KeyPad_CheckChrg()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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
** ��    �� ��Dev_KeyPad_UpKey()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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
** ��    �� ��Dev_KeyPad_DownKey()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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
** ��    �� ��Dev_KeyPad_PowerOff()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
****************************************************************************************************
*/
void Dev_KeyPad_PowerOff(void)
{
	GPIO_BitClr(GPIOA, GPIO_PIN_POWER);
}

/*
****************************************************************************************************
** ��    �� ��Dev_KeyPad_PowerOn()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
****************************************************************************************************
*/
void Dev_KeyPad_PowerOn(void)
{
	GPIO_BitSet(GPIOA, GPIO_PIN_POWER);
}
/*
****************************************************************************************************
** ��    �� ��Dev_KeyPad_Shutdown()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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
	Dev_KeyPad_PowerOff();       //�ϵ�
}

/*
**********************************************************************************************************
** ��    �� ��Dev_KeyPad_CheckPowerKeyOff()
** ��    �� ��
** ��ڲ��� ���� 
** ���ڲ��� ����
** ��    ע :
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
** ��    �� ��Dev_KeyPad_CheckPowerKeyReOn()
** ��    �� ��
** ��ڲ��� ���� 
** ���ڲ��� ����
** ��    ע :
**********************************************************************************************************
*/
void Dev_KeyPad_CheckPowerKeyReOn(void)
{   
    Dev_KeyPad_PowerOff();       //�ϵ�

    if(s_aPowerKeyPressTimes < 8)
    {
        if(!Dev_KeyPad_CheckPower()) //�ɿ���Դ��  100ms * 8
        {   
            s_aPowerKeyPressTimes++;
        }
        else        //�ػ�һֱ���Ų��ſ���Դ��
        {
            s_aPowerKeyPressTimes = 0;
        }       
    }
    else
    {
        if(Dev_KeyPad_CheckPower())  //�ػ��ɿ����ٰ���Դ��������
        {   
            s_aPowerKeyPressTimes++;
        }
        else
        {
            s_aPowerKeyPressTimes = 8; //�ػ��ɿ����ٰ���Դ��
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
** ��    �� ��Dev_KeyPad_CheckPowerOff()
** ��    �� ��
** ��ڲ��� ���� 
** ���ڲ��� ����
** ��    ע :
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
** ��    �� ��Dev_KeyPad_PowerStateCheck()
** ��    �� ��100ms check
** ��ڲ��� ���� 
** ���ڲ��� ����
** ��    ע :
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
** ��    �� ��App_Main_CheckKeyPad()
** ��    �� ��10ms
** ��ڲ��� ���� 
** ���ڲ��� ����
** ��    ע :
**********************************************************************************************************
*/
void Dev_KeyPad_CheckKeyPad(void)
{    
    if(Dev_KeyPad_UpKey())
    {       
		if(s_aUpKeyFlag < KEYPAD_UPKEYDELAY)
		{
			s_aUpKeyFlag++;
			
	        if(s_aUpKeyFlag >= KEYPAD_UPKEYDELAY)		// 100ms��������
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
			
	        if(s_aDownKeyFlag >= KEYPAD_UPKEYDELAY)		// 100ms��������
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
** ��    �� ��App_Main_ReadKeyPad()
** ��    �� ��
** ��ڲ��� ����
** ���ڲ��� ����
** ��    �� ��
** ��    ע ��
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


