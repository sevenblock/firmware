/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_BT_KeyStorage.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_BT_KeyStorage_D
#include "..\block\app\App_Macro.h"

uint8_t  bond_store_num = 0;
T_REMOTE_BD bond_store_table[MAX_STORE_NUM];

static uint8_t bond_flag_addr(uint8_t *bdaddr, uint8_t addr_type)
{
	switch (addr_type) 
	{
		case BTIF_REMOTE_ADDR_CLASSIC:
			return BOND_FLAG_ADDR_BREDR;
			
		case BTIF_REMOTE_ADDR_LE_PUBLIC:
			return BOND_FLAG_ADDR_LE_PUBLIC;
			
		case BTIF_REMOTE_ADDR_LE_RANDOM:
		case BTIF_REMOTE_ADDR_LE_RANDOM_IDENTITY:
			return BOND_FLAG_ADDR_LE_RANDOM;
			
		default:
			return 0;
	}
}

void bt_key_init(uint8_t store_num)
{
	uint8_t i;

	if (store_num <= MAX_STORE_NUM)
	{
		bond_store_num = store_num;
	} 
	else 
	{
		bond_store_num = MAX_STORE_NUM;
	}

	bond_priority_queue_init(bond_store_num);

	for (i = 0; i < bond_store_num; i++) 
	{
		flash_load_remote_bd(&bond_store_table[i], i);
	}
}

void bt_clear_all_keys(void)
{
	uint8_t i;
	
	memset(bond_store_table, 0x00,MAX_STORE_NUM * sizeof(T_REMOTE_BD));
	
	bond_priority_queue_clear();
	
	for (i = 0; i < MAX_STORE_NUM; i++) 
	{
		flash_save_remote_bd(&bond_store_table[i], i);
	}
}

void bt_clear_le_keys(void)
{
	uint8_t i;

	for (i = 0; i < bond_store_num; i++) 
	{
		if (!((bond_store_table[i].bond_flag & 0x07) ==BOND_FLAG_ADDR_BREDR)) 
		{
			bond_priority_queue_delete(i);
			
			memset(&bond_store_table[i], 0x00, sizeof(T_REMOTE_BD));
			
			flash_save_remote_bd(&bond_store_table[i], i);
		}
	}
}

void bt_clear_bredr_keys(void)
{
	uint8_t i;

	for (i = 0; i < bond_store_num; i++)
	{
		if ((bond_store_table[i].bond_flag & 0x07) ==BOND_FLAG_ADDR_BREDR) 
		{
			bond_priority_queue_delete(i);
			
			memset(&bond_store_table[i], 0x00, sizeof(T_REMOTE_BD));
			
			flash_save_remote_bd(&bond_store_table[i], i);
		}
	}
}

static uint8_t bt_search_paired_idx(uint8_t *bd_addr, T_DEV_TYPE dev_type)
{
	uint8_t i;
	
	for (i = 0; i < bond_store_num; i++) 
	{
		DDEBUGHEX("bond_store_table = ", bond_store_table[i].remote_bd,sizeof(T_REMOTE_BD));
		
		if ((bond_store_table[i].bond_flag & BOND_FLAG_PAIRED) && memcmp(bond_store_table[i].remote_bd, bd_addr, 6) == 0) 
		{
			if (dev_type == LEGACY_BT_DEV)
			{
				if ((bond_store_table[i].bond_flag & 0x0f) == BOND_FLAG_ADDR_BREDR)
				{
					break;
				}
			} 
			else 
			{
				if ((bond_store_table[i].bond_flag & 0x0f) !=BOND_FLAG_ADDR_BREDR)
				{
					break;
				}
			}
		}
	}
	
	return i;
}

uint8_t bt_search_empty_dev(void)
{
	uint8_t i;
	
	for (i = 0; i < bond_store_num; i++) 
	{
		if (!(bond_store_table[i].bond_flag & BOND_FLAG_PAIRED)) 
		{
			break;
		}
	}
	
	return i;
}

void legacy_save_bond(uint8_t *bd_addr, uint8_t *linkkey, uint8_t key_type)
{
	uint8_t index;
	T_LINK_KEY link_key;

	APP_Macro_PrintfStart();

	index = bt_search_paired_idx(bd_addr, LEGACY_BT_DEV);

	DDEBUG("bt_search_paired_idx index = %d",index);
	
	if (bond_store_num == index) 
	{
		/* no paired info matched */
		index = bt_search_empty_dev();

		DDEBUG("bt_search_empty_dev index = %d",index);
		
		if (bond_store_num == index) 
		{
			/* If there is no empty slot, get low priority of
			 * bond info first.
			 */
			index = bond_get_low_priority();
			
			if (index == 0xff) 
			{
				return;
			}
		}

		memset(&bond_store_table[index], 0x00, sizeof(T_REMOTE_BD));
		memcpy(bond_store_table[index].remote_bd, bd_addr, 6);
		memcpy(link_key.key, linkkey, sizeof(link_key.key));
		bond_store_table[index].bond_flag = BOND_FLAG_PAIRED |BOND_FLAG_ADDR_BREDR;
		bond_store_table[index].key_type = key_type;

		if(flash_save_remote_bd(&bond_store_table[index], index))
		{
			DDEBUG("flash_save_remote_bd error");
		}
		
		if(flash_save_legacy_link_key(&link_key, index))
		{
			DDEBUG("flash_save_legacy_link_key error");
		}

		/* If the index already exists in prio table, the function
		 * would improve its prio. Otherwise add index to the end.
		 *
		 * If the table is full, nothing to add.
		 * */
		bond_priority_queue_add(index);
		
	} 
	else 
	{
		memset(&bond_store_table[index], 0x00, sizeof(T_REMOTE_BD));
		memcpy(bond_store_table[index].remote_bd, bd_addr, 6);
		memcpy(link_key.key, linkkey, sizeof(link_key.key));
		
		bond_store_table[index].bond_flag = BOND_FLAG_PAIRED |BOND_FLAG_ADDR_BREDR;
		bond_store_table[index].key_type = key_type;

		flash_save_remote_bd(&bond_store_table[index], index);
		flash_save_legacy_link_key(&link_key, index);
		bond_set_high_priority(index);
	}
}

bool legacy_get_bond(uint8_t *bd_addr, uint8_t *link_key, uint8_t *key_type)
{
	uint8_t index;

	index = bt_search_paired_idx(bd_addr, LEGACY_BT_DEV);
	
	if (index < bond_store_num) 
	{
		if (!flash_load_legacy_link_key((T_LINK_KEY *)link_key, index)) 
		{
			*key_type = bond_store_table[index].key_type;
			return true;
		} 
		else 
		{
			return false;
		}
	} 
	else 
	{
		return false;
	}
}

bool legacy_delete_bond(uint8_t *bd_addr)
{
	uint8_t index;

	index = bt_search_paired_idx(bd_addr, LEGACY_BT_DEV);
	
	if (index < bond_store_num) 
	{
		bond_priority_queue_delete(index);
		memset(&bond_store_table[index], 0x00, sizeof(T_REMOTE_BD));
		flash_save_remote_bd(&bond_store_table[index], index);
		
		return true;
	}
	else
	{
		return false;
	}
}

void le_save_bond(uint8_t *bd_addr, uint8_t addr_type, uint8_t *linkkey,uint8_t key_type)
{
	uint8_t index;
	T_LTK ltk;

	APP_Macro_PrintfStart();

	index = bt_search_paired_idx(bd_addr, LE_BT_DEV);
	
	if (bond_store_num == index)
	{
		index = bt_search_empty_dev();
		
		if (bond_store_num == index)
		{
			/* If there is no empty slot, get low priority of
			 * bond info first.
			 */
			index = bond_get_low_priority();
			
			if (index == 0xff) 
			{
				return;
			}
		}

		memset(&bond_store_table[index], 0x00, sizeof(T_REMOTE_BD));
		memcpy(bond_store_table[index].remote_bd, bd_addr, 6);
		memcpy(ltk.key, linkkey, sizeof(ltk.key));

		bond_store_table[index].bond_flag = bond_flag_addr(bd_addr,addr_type);
		bond_store_table[index].bond_flag |= BOND_FLAG_PAIRED;

		bond_store_table[index].key_type = key_type;

		flash_save_remote_bd(&bond_store_table[index], index);
		flash_save_le_ltk(&ltk, index);

		/* If the index already exists in prio table, the function
		 * would improve its prio. Otherwise add index to the end.
		 *
		 * If the table is full, nothing to add.
		 * */
		bond_priority_queue_add(index);
		
	}
	else 
	{
		uint8_t irk_flag;

		irk_flag = bond_store_table[index].bond_flag & BOND_FLAG_IRK;

		memset(&bond_store_table[index], 0x00, sizeof(T_REMOTE_BD));
		memcpy(bond_store_table[index].remote_bd, bd_addr, 6);
		memcpy(ltk.key, linkkey, sizeof(ltk.key));

		bond_store_table[index].bond_flag = bond_flag_addr(bd_addr,addr_type);
		bond_store_table[index].bond_flag |= BOND_FLAG_PAIRED;
		bond_store_table[index].bond_flag |= irk_flag;

		bond_store_table[index].key_type = key_type;

		flash_save_remote_bd(&bond_store_table[index], index);
		flash_save_le_ltk(&ltk, index);

		bond_set_high_priority(index);
	}
}

void le_save_irk(uint8_t *bd_addr, uint8_t addr_type, uint8_t *linkkey,uint8_t key_type)
{
	uint8_t i;
	uint8_t index;
	T_REMOTE_IRK irk;
	uint32_t ret;

	APP_Macro_PrintfStart();

	index = bt_search_paired_idx(bd_addr, LE_BT_DEV);
	
	if (bond_store_num == index) 
	{
		return;
	}

	for (i = 0; i < bond_store_num; i++) 
	{
		if ((bond_store_table[i].bond_flag & BOND_FLAG_PAIRED) &&
		    (bond_store_table[i].bond_flag & BOND_FLAG_IRK))
		{
			ret = flash_load_remote_irk(&irk, i);
			
			if (ret)
			{
				continue;
			}
			
			if (!memcmp(irk.key, linkkey, sizeof(irk.key)) &&index != i)
			{
				le_delete_bond(bond_store_table[i].remote_bd);
			}
		}
	}


	if (addr_type != BTIF_REMOTE_ADDR_LE_RANDOM ||(bd_addr[5] >> 6) != 0x01) 
	{
		//pr_warn("bdaddr not resolvable private address");
	}

	memcpy(irk.key, linkkey, sizeof(irk.key));

	bond_store_table[index].bond_flag |= BOND_FLAG_PAIRED;
	bond_store_table[index].bond_flag |= BOND_FLAG_IRK;
	/* Remain the key type */

	flash_save_remote_bd(&bond_store_table[index], index);
	flash_save_remote_irk(&irk, index);
}

bool le_get_bond(uint8_t *bd_addr, uint8_t *link_key, uint8_t *key_type)
{
	uint8_t index;

	index = bt_search_paired_idx(bd_addr, LE_BT_DEV);
	
	if (index < bond_store_num) 
	{
		if (!flash_load_le_ltk((T_LTK *)link_key, index)) 
		{
			*key_type = bond_store_table[index].key_type;
			return true;
		} 
		else 
		{
			return false;
		}
	} 
	else
	{
		return false;
	}
}

bool le_delete_bond(uint8_t *bd_addr)
{
	uint8_t index;

	index = bt_search_paired_idx(bd_addr, LE_BT_DEV);
	
	if (index < bond_store_num) 
	{
		bond_priority_queue_delete(index);
		memset(&bond_store_table[index], 0x00, sizeof(T_REMOTE_BD));
		flash_save_remote_bd(&bond_store_table[index], index);
		
		return true;
	}
	else
	{
		return false;
	}
}

bool le_get_key_pairs(uint8_t num_pair, uint8_t *idx_tbl, uint8_t *buff)
{
	int k;
	uint8_t i;
	uint16_t blk_size = 6 + 2 * LE_KEY_LEN;
	uint8_t ret;
	uint8_t j = 0;
	uint8_t num;
	uint8_t *bond_prio;

	if (!num_pair || !idx_tbl || !buff)
	{
		return false;
	}

	bond_prio = bond_priority_get_table(&num);
	
	if (!num || num > MAX_STORE_NUM) 
	{
		return false;
	}

	/* for (i = 0; i < MAX_STORE_NUM; i++) { */
	for (k = num - 1; k >= 0; k--) 
	{
		i = bond_prio[k];
		
		if (i >= MAX_STORE_NUM)
		{
			return false;
		}

		if (!idx_tbl[i])
		{
			continue;
		}
		
		memcpy(buff + j * blk_size, bond_store_table[i].remote_bd, 6);
		
		ret = flash_load_le_ltk((T_LTK *)(buff + j * blk_size + 6), i);
		
		if (ret)
		{
			//printf("load ltk error\n");
		}
		
		ret = flash_load_remote_irk((T_REMOTE_IRK *)(buff + j * blk_size + 6 + LE_KEY_LEN), i);
		
		if (ret)
		{
			//printf("load remote irk error\n");
		}
		
		j++;
		
		if (j >= MAX_STORE_NUM)
		{
			break;
		}
	}

	return true;

}

bool le_get_key_pair_num(uint8_t *num_pair, uint8_t *idx_tbl)
{
	uint8_t i;
	uint8_t num = 0;

	if (!num_pair || !idx_tbl)
	{
		return false;
	}

	for (i = 0; i < MAX_STORE_NUM; i++) 
	{
		if ((bond_store_table[i].bond_flag & 0x0f) != BOND_FLAG_ADDR_BREDR && (bond_store_table[i].bond_flag & BOND_FLAG_IRK)) 
		{
			idx_tbl[i] = 1;
			num++;
		}
		else
		{
			idx_tbl[i] = 0;
		}

	}

	*num_pair = num;
	
	if (num)
	{
		return true;
	}
	else
	{
		return false;
	}
}

