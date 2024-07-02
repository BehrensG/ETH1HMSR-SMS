/*
 * GPIO.h
 *
 *  Created on: Apr 10, 2024
 *      Author: BehrensG
 */

#ifndef BSP_INC_GPIO_H_
#define BSP_INC_GPIO_H_

#include "main.h"
#include <stdbool.h>

bool GPIO_SelectGain(uint8_t gain);
void GPIO_DG419(bool state);

#endif /* BSP_INC_GPIO_H_ */
