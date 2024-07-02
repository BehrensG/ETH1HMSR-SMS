/*
 * ee24Config.h
 *
 *  Created on: Feb 10, 2023
 *      Author: grzegorz
 */

#ifndef THIRD_PARTY_BSP_INC_EE24CONFIG_H_
#define THIRD_PARTY_BSP_INC_EE24CONFIG_H_

#include "main.h"

extern I2C_HandleTypeDef hi2c1;
#define		_EEPROM_SIZE_KBIT			8
#define		_EEPROM_USE_FREERTOS        0
#define		_EEPROM_USE_WP_PIN          0
#define		_EEPROM_I2C   				hi2c1

#define EEPROM_WRITE_DONE 0x44U
#define EEPROM_END 255U

#if (_EEPROM_USE_WP_PIN==1)
#define		_EEPROM_WP_GPIO				EEPROM_nWP_GPIO_Port
#define		_EEPROM_WP_PIN				EEPROM_nWP_Pin
#endif

#endif /* THIRD_PARTY_BSP_INC_EE24CONFIG_H_ */
