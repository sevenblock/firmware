/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_BT_KeyFlash.h
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_BT_KeyFlash_H
#define Dev_BT_KeyFlash_H

#ifndef Dev_BT_KeyFlash_D
#define EX_Dev_BT_KeyFlash		extern
#else
#define EX_Dev_BT_KeyFlash
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************宏定义****************************************************
***************************************************************************************************/


/****************************************功能定义***************************************************
***************************************************************************************************/
EX_Dev_BT_KeyFlash int open_flash_device(void);
EX_Dev_BT_KeyFlash void close_flash_device(void);

EX_Dev_BT_KeyFlash uint32_t flash_load_bond_priority(T_BOND_PRIORITY *p_data);
EX_Dev_BT_KeyFlash uint32_t flash_save_bond_priority(T_BOND_PRIORITY *p_data);

EX_Dev_BT_KeyFlash uint32_t flash_load_remote_bd(T_REMOTE_BD *p_data, uint8_t idx);
EX_Dev_BT_KeyFlash uint32_t flash_save_remote_bd(T_REMOTE_BD *p_data, uint8_t idx);

EX_Dev_BT_KeyFlash uint32_t flash_load_legacy_link_key(T_LINK_KEY *p_data, uint8_t idx);
EX_Dev_BT_KeyFlash uint32_t flash_save_legacy_link_key(T_LINK_KEY *p_data, uint8_t idx);

EX_Dev_BT_KeyFlash uint32_t flash_load_le_ltk(T_LTK *p_data, uint8_t idx);
EX_Dev_BT_KeyFlash uint32_t flash_save_le_ltk(T_LTK *p_data, uint8_t idx);

EX_Dev_BT_KeyFlash uint32_t flash_load_remote_irk(T_REMOTE_IRK *p_data, uint8_t idx);
EX_Dev_BT_KeyFlash uint32_t flash_save_remote_irk(T_REMOTE_IRK *p_data, uint8_t idx);

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/

