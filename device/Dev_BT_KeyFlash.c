/***************************************Copyright(c)************************************************
****************************************************************************************************
***** 文    件 :Dev_BT_KeyFlash.c
***** 设    计 :sevenblock
***** 描    述 :
***** 版    本 :V1.0
***** 时    间 :2017-05-03
***** 备    注 :
****************************************************************************************************
***************************************************************************************************/
#define Dev_BT_KeyFlash_D
#include "..\block\app\App_Macro.h"

#define FLASH_BOND_PRIO_OFFSET					0

#define FLASH_LINK_INFO_START_OFFSET		(sizeof(T_BOND_PRIORITY))

#define FLASH_LINK_INFO_SIZE \
	  (sizeof(T_REMOTE_BD) + \
	   sizeof(T_KEY_COMMON) + \
	   sizeof(T_REMOTE_IRK))

#define FLASH_LINK_INFO_BD_OFFSET			FLASH_LINK_INFO_START_OFFSET

#define FLASH_LEGACY_LINK_KEY_OFFSET \
	  (FLASH_LINK_INFO_START_OFFSET + sizeof(T_REMOTE_BD))

#define FLASH_LE_REMOTE_LTK_OFFSET  \
	  (FLASH_LINK_INFO_START_OFFSET + sizeof(T_REMOTE_BD))
	  
#define FLASH_LE_REMOTE_IRK_OFFSET  \
	  (FLASH_LE_REMOTE_LTK_OFFSET + sizeof(T_KEY_COMMON))

#define FLASH_LINK_INFO_MAX_SIZE				(sizeof(T_BOND_PRIORITY)+FLASH_LINK_INFO_SIZE*MAX_STORE_NUM)

uint32_t ftl_save_to_storage(void *p_data, uint16_t offset, uint16_t size)
{
	uint8_t flash_temp[FLASH_LINK_INFO_MAX_SIZE];
	
	if (!p_data || (offset & 0x3) || (size == 0) || (size & 0x3))
	{
		return 1;
	}

	if(SF_Read(FLASH_BASE_BT_KEY_ADDR,flash_temp,FLASH_LINK_INFO_MAX_SIZE) == FALSE)
	{
		return 2;
	}

	memcpy(&flash_temp[offset], p_data, size);

	if(SF_Write(FLASH_BASE_BT_KEY_ADDR,0,flash_temp,FLASH_LINK_INFO_MAX_SIZE,1) == FALSE)
	{
		return 4;
	}
	
	return 0;
}

uint32_t ftl_load_from_storage(void *p_data, uint16_t offset, uint16_t size)
{
	uint8_t flash_temp[FLASH_LINK_INFO_MAX_SIZE];
	
	if (!p_data || (offset & 0x3) || (size == 0) || (size & 0x3))
	{
		return 1;
	}

	if(SF_Read(FLASH_BASE_BT_KEY_ADDR,flash_temp,FLASH_LINK_INFO_MAX_SIZE) == FALSE)
	{
		return 2;
	}
	
	memcpy(p_data, &flash_temp[offset], size);

	return 0;
}

uint32_t imp_flash_save(void *p_data, uint16_t start_offset, uint16_t block_size,uint8_t size, uint8_t idx)
{
	uint16_t offset = start_offset + idx * block_size;

	return ftl_save_to_storage(p_data, offset, size);
}

uint32_t imp_flash_load(void *p_data, uint16_t start_offset, uint16_t block_size,uint8_t size, uint8_t idx)
{
	uint16_t offset = start_offset + idx * block_size;
	uint32_t err = ftl_load_from_storage(p_data, offset, size);

	if (err) 
	{
		DDEBUG("imp_flash_load error!");
		memset(p_data, 0, size);
	}

	return err;
}

uint32_t flash_load_bond_priority(T_BOND_PRIORITY *p_data)
{
	uint16_t offset;
	uint16_t size = sizeof(T_BOND_PRIORITY);
	uint32_t err;

	offset = FLASH_BOND_PRIO_OFFSET;

	err = ftl_load_from_storage(p_data, offset, size);
	
	if (err)
	{
		DDEBUG("flash_load_bond_priority error!");
		memset(p_data, 0, size);
	}
	
	return err;
}

uint32_t flash_save_bond_priority(T_BOND_PRIORITY *p_data)
{
	uint16_t offset;
	uint16_t size = sizeof(T_BOND_PRIORITY);

	offset = FLASH_BOND_PRIO_OFFSET;

	return ftl_save_to_storage(p_data, offset, size);
}

uint32_t flash_load_remote_bd(T_REMOTE_BD *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_load(p_data, FLASH_LINK_INFO_BD_OFFSET,FLASH_LINK_INFO_SIZE,sizeof(T_REMOTE_BD), idx);
}

uint32_t flash_save_remote_bd(T_REMOTE_BD *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_save(p_data, FLASH_LINK_INFO_BD_OFFSET,FLASH_LINK_INFO_SIZE,sizeof(T_REMOTE_BD), idx);
}

uint32_t flash_load_legacy_link_key(T_LINK_KEY *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_load(p_data, FLASH_LEGACY_LINK_KEY_OFFSET,FLASH_LINK_INFO_SIZE,sizeof(T_LINK_KEY), idx);

}

uint32_t flash_save_legacy_link_key(T_LINK_KEY *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_save(p_data, FLASH_LEGACY_LINK_KEY_OFFSET,FLASH_LINK_INFO_SIZE,sizeof(T_LINK_KEY), idx);
}

uint32_t flash_load_le_ltk(T_LTK *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_load(p_data, FLASH_LE_REMOTE_LTK_OFFSET,FLASH_LINK_INFO_SIZE, sizeof(T_LTK), idx);
}

uint32_t flash_save_le_ltk(T_LTK *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_save(p_data, FLASH_LE_REMOTE_LTK_OFFSET,FLASH_LINK_INFO_SIZE, sizeof(T_LTK), idx);
}

uint32_t flash_load_remote_irk(T_REMOTE_IRK *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_load(p_data, FLASH_LE_REMOTE_IRK_OFFSET,FLASH_LINK_INFO_SIZE, sizeof(T_REMOTE_IRK), idx);
}

uint32_t flash_save_remote_irk(T_REMOTE_IRK *p_data, uint8_t idx)
{
	if (idx > MAX_STORE_NUM - 1)
	{
		return 1;
	}

	return imp_flash_save(p_data, FLASH_LE_REMOTE_IRK_OFFSET,FLASH_LINK_INFO_SIZE, sizeof(T_REMOTE_IRK), idx);
}


