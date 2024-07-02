/*
 * HMSR-SMS.c
 *
 *  Created on: Apr 19, 2024
 *      Author: grzegorz
 */

// --------------------------------------------------------------------------------------------------------------------

#include "BSP.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------


bool HMSR_SMS_CheckNominalCurrent(uint8_t value)
{
	uint8_t valid[6] = {6, 8, 10, 15, 20, 30};
	bool status = false;

	for (uint8_t x = 0; x < 6; x++)
	{
		if (value == valid[x])
		{
			status = true;
			break;
		}
	}

	return status;
}


// --------------------------------------------------------------------------------------------------------------------

float HMSR_SMS_MeasuringRange(uint8_t nominal_current)
{
	switch (nominal_current)
	{
		case 6 : return 15;
		case 8 : return 20;
		case 10 : return 25;
		case 15 : return 37.5;
		case 20 : return 50;
		case 30 : return 75;
		default : return 15;
	}
}


// --------------------------------------------------------------------------------------------------------------------

void HMSR_SMS_Init()
{
	bsp.hmsr_sms.signal_multiplier = bsp.eeprom.structure.setup.multiplier;
	bsp.hmsr_sms.measuring_range = HMSR_SMS_MeasuringRange(bsp.eeprom.structure.setup.nominal_current);
	bsp.hmsr_sms.resolution = (float)bsp.hmsr_sms.measuring_range/2.0;
}
