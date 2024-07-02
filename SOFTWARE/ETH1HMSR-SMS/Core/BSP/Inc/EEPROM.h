/*
 * eeporm.h
 *
 *  Created on: Feb 10, 2023
 *      Author: grzegorz
 */

#ifndef THIRD_PARTY_BSP_INC_EEPROM_H_
#define THIRD_PARTY_BSP_INC_EEPROM_H_


#include "BSP.h"
#include "EE24.h"

// --------------------------------------------------------------------------------------------------------------------

#define EEPROM_ADDRESS 0xA0U
#define EEPROM_SIZE 0xFFU

// --------------------------------------------------------------------------------------------------------------------

bsp_result_t EEPROM_Erase(void);
bsp_result_t EEPROM_Write(bsp_eeprom_t* union_data, size_t size);
bsp_result_t EEPROM_Read(bsp_eeprom_t* union_data, size_t size);
bsp_result_t EEPROM_Status(void);

#endif /* THIRD_PARTY_BSP_INC_EEPROM_H_ */
