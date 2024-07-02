/*
 * GPIO.c
 *
 *  Created on: Apr 10, 2024
 *      Author: BehrensG
 */


#include "GPIO.h"


bool GPIO_SelectGain(uint8_t gain)
{
	switch (gain)
	{
		case 1:
			{
				LL_GPIO_SetOutputPin(MCU_G1_GPIO_Port, MCU_G1_Pin);
				LL_GPIO_ResetOutputPin(MCU_G10_GPIO_Port, MCU_G10_Pin);
				LL_GPIO_ResetOutputPin(MCU_G100_GPIO_Port, MCU_G100_Pin);
			};break;
		case 10:
			{
				LL_GPIO_SetOutputPin(MCU_G10_GPIO_Port, MCU_G10_Pin);
				LL_GPIO_ResetOutputPin(MCU_G1_GPIO_Port, MCU_G1_Pin);
				LL_GPIO_ResetOutputPin(MCU_G100_GPIO_Port, MCU_G100_Pin);
			};break;
		case 100:
			{
				LL_GPIO_SetOutputPin(MCU_G100_GPIO_Port, MCU_G100_Pin);
				LL_GPIO_ResetOutputPin(MCU_G1_GPIO_Port, MCU_G1_Pin);
				LL_GPIO_ResetOutputPin(MCU_G10_GPIO_Port, MCU_G10_Pin);
			};break;

		default: return false;
	}

	return true;

}

void GPIO_DG419(bool state)
{
	(state) ? LL_GPIO_SetOutputPin(MCU_ZOFFS_GPIO_Port, MCU_ZOFFS_Pin) : LL_GPIO_ResetOutputPin(MCU_ZOFFS_GPIO_Port, MCU_ZOFFS_Pin);
}
