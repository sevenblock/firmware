/***************************************************************************************
* File name    :	CtrlItem.h
* Function     :	The header of CtrlItem.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _CTRLITEM_H_
#define _CTRLITEM_H_

/*************************************************************************************
 * System Fuctions Declaration														 *
 ************************************************************************************/
void Chap9_GetStatus(SETUPPACK* SetupPack);
void Chap9_SetFeature(SETUPPACK* SetupPack);
void Chap9_ClearFeature(SETUPPACK* SetupPack);
void Chap9_GetConfig(SETUPPACK* SetupPack);
void Chap9_GetInterface(SETUPPACK* SetupPack);
void Chap9_SetInterface(SETUPPACK* SetupPack);

void Chap9_SetAddress( SETUPPACK* SetupPack);
void Chap9_SetConfiguration( SETUPPACK* SetupPack);
void Chap9_GetDescriptor( SETUPPACK* SetupPack) ;
void Chap9_SendACK (SETUPPACK* SetupPack);

void Class_SendMaxLun( SETUPPACK* SetupPack);

#endif
