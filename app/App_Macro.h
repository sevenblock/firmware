/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :App_Macro.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef App_Macro_H
#define App_Macro_H
/*****************************************宏定义****************************************************
***************************************************************************************************/
#ifndef STD_HEAD
#ifndef NULL
#define NULL 			(void*)0
#endif

#ifndef FALSE
#define FALSE			(0)
#endif

#ifndef TRUE
#define TRUE				(1)
#endif

//指针:p   结构:u   g_   s_  t_

typedef unsigned char 				bool;			/*  8 bits b			*/

typedef unsigned char 				u8;				/*  8 bits a			*/
typedef unsigned short 			u16;			/* 16 bits r			*/
typedef unsigned int 				u32;			/* 32 bits n			*/
typedef unsigned long long 		s64;			/* 32 bits o			*/

typedef signed char 					s8;				/*  8 bits c			*/
typedef signed short 				s16;			/* 16 bits s			*/
typedef signed int 					s32;			/* 32 bits i			*/

typedef unsigned int            	uint32_t;
typedef unsigned short          	uint16_t;
typedef unsigned char           	uint8_t;

typedef signed short 				int16_t;
typedef signed int 					int32_t;
typedef signed char 					int8_t;

#endif


/****************************************配置文件***************************************************
***************************************************************************************************/


#endif
/****************************************End Of File************************************************
***************************************************************************************************/
