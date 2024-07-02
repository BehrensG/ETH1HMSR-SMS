/*
 * ee24.h
 *
 *  Created on: Feb 10, 2023
 *      Author: grzegorz
 */

#ifndef THIRD_PARTY_BSP_INC_EE24_H_
#define THIRD_PARTY_BSP_INC_EE24_H_

/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw

  Version:    2.2.1


  Reversion History:

  (2.2.1)
  Fix erase chip bug.

  (2.2.0)
  Add erase chip function.

  (2.1.0)
  Fix write bytes.

  (2.0.0)
  Rewrite again.

*/

#include "EE24Config.h"
#include <stdbool.h>
#include <stdint.h>

bool ee24_isConnected(uint8_t eeprom_address);
bool ee24_write(uint8_t eeprom_address, uint16_t address, uint8_t *data, size_t lenInBytes);
bool ee24_read(uint8_t eeprom_address, uint16_t address, uint8_t *data, size_t lenInBytes);
bool ee24_eraseChip(uint8_t eeprom_address);

#endif
