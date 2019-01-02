/***************************************Copyright(c)************************************************
****************************************************************************************************
***** ��    �� :Dev_BT_KeyStorage.h
***** ��    �� :sevenblock
***** ��    �� :
***** ��    �� :V1.0
***** ʱ    �� :2017-05-03
***** ��    ע :
****************************************************************************************************
***************************************************************************************************/
#ifndef Dev_BT_KeyStorage_H
#define Dev_BT_KeyStorage_H

#ifndef Dev_BT_KeyStorage_D
#define EX_Dev_BT_KeyStorage		extern
#else
#define EX_Dev_BT_KeyStorage
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************�궨��****************************************************
***************************************************************************************************/


/****************************************���ܶ���***************************************************
***************************************************************************************************/

EX_Dev_BT_KeyStorage void bt_key_init(uint8_t store_num);
EX_Dev_BT_KeyStorage void bt_clear_all_keys(void);
EX_Dev_BT_KeyStorage void legacy_save_bond(uint8_t *bd_addr, uint8_t *linkkey, uint8_t key_type);
EX_Dev_BT_KeyStorage bool legacy_get_bond(uint8_t *bd_addr, uint8_t *link_key, uint8_t *key_type);
EX_Dev_BT_KeyStorage bool legacy_delete_bond(uint8_t *bd_addr);
EX_Dev_BT_KeyStorage void le_save_bond(uint8_t *bd_addr, uint8_t addr_type, uint8_t *linkkey,uint8_t key_type);
EX_Dev_BT_KeyStorage bool le_get_bond(uint8_t *bd_addr, uint8_t *link_key, uint8_t *key_type);
EX_Dev_BT_KeyStorage bool le_delete_bond(uint8_t *bd_addr);
EX_Dev_BT_KeyStorage bool le_get_key_pairs(uint8_t num_pair, uint8_t *idx_tbl, uint8_t *buff);
EX_Dev_BT_KeyStorage bool le_get_key_pair_num(uint8_t *num_pair, uint8_t *idx_tbl);
EX_Dev_BT_KeyStorage void le_save_irk(uint8_t *bd_addr, uint8_t addr_type, uint8_t *linkkey,uint8_t key_type);
EX_Dev_BT_KeyStorage void bt_clear_le_keys(void);
EX_Dev_BT_KeyStorage void bt_clear_bredr_keys(void);

#ifdef __cplusplus
}
#endif
#endif
/****************************************End Of File************************************************
***************************************************************************************************/




