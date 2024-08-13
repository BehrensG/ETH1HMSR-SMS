/*
 * BSP.c
 *
 *  Created on: Apr 10, 2024
 *      Author: BehrensG
 */

// --------------------------------------------------------------------------------------------------------------------

#include "BSP.h"
#include "ADC.h"
#include "SCPI_Def.h"
#include "EEPROM.h"
#include "HMSR-SMS.h"

// --------------------------------------------------------------------------------------------------------------------

bsp_t bsp;
bsp_eeprom_t eeprom_default;

// --------------------------------------------------------------------------------------------------------------------

static void BSP_Init_Common()
{

	bsp.security.status = true;

	bsp.adc.gain.value = 1;
	bsp.adc.gain.index = 0;
	bsp.adc.oversampling.enable = false;
	bsp.adc.oversampling.ratio = 1;
	bsp.adc.bits = 16;
	bsp.adc.sample_count = ADC_DEF_SIZE;
	bsp.adc.vcom = ADC_VCOM;
	bsp.adc.resolution = (float)(ADC_VREF/pow(2.0,(double)bsp.adc.bits));

	bsp.adc.offset.zero[0] = 0.0f;
	bsp.adc.offset.zero[1] = 0.0f;
	bsp.adc.offset.zero[2] = 0.0f;
	bsp.adc.offset.enable = false;

	bsp.adc.math_offset.zero[0] = 0.0f;
	bsp.adc.math_offset.zero[1] = 0.0f;
	bsp.adc.math_offset.zero[2] = 0.0f;
	bsp.adc.math_offset.enable = false;


	bsp.adc.sampling_time = ADC_SAMPLETIME_1CYCLE_5;
	bsp.adc.cycles = 1.5;
	bsp.adc.period = 0.2;
	bsp.led = true;

	bsp.format.data = FORMAT_DATA_ASCII;

	bsp.resource = VISA_SCPI_RAW;


}


// --------------------------------------------------------------------------------------------------------------------

void BSP_Init_DefualtEEPROM()
{
	if(bsp.default_cfg)
	{
		bsp.eeprom.structure.ip4_static.gateway[0] = GATEWAY_ADDRESS_0;
		bsp.eeprom.structure.ip4_static.gateway[1] = GATEWAY_ADDRESS_1;
		bsp.eeprom.structure.ip4_static.gateway[2] = GATEWAY_ADDRESS_2;
		bsp.eeprom.structure.ip4_static.gateway[3] = GATEWAY_ADDRESS_3;

		bsp.eeprom.structure.ip4_static.ip[0] = IP_ADDRESS_0;
		bsp.eeprom.structure.ip4_static.ip[1] = IP_ADDRESS_1;
		bsp.eeprom.structure.ip4_static.ip[2] = IP_ADDRESS_2;
		bsp.eeprom.structure.ip4_static.ip[3] = IP_ADDRESS_3;

		bsp.eeprom.structure.ip4_static.netmask[0] = NETMASK_ADDRESS_0;
		bsp.eeprom.structure.ip4_static.netmask[1] = NETMASK_ADDRESS_1;
		bsp.eeprom.structure.ip4_static.netmask[2] = NETMASK_ADDRESS_2;
		bsp.eeprom.structure.ip4_static.netmask[3] = NETMASK_ADDRESS_3;
	}
	else
	{
		bsp.eeprom.structure.ip4_static.gateway[0] = GATEWAY_ADDRESS_0;
		bsp.eeprom.structure.ip4_static.gateway[1] = GATEWAY_ADDRESS_1;
		bsp.eeprom.structure.ip4_static.gateway[2] = GATEWAY_ADDRESS_2;
		bsp.eeprom.structure.ip4_static.gateway[3] = GATEWAY_ADDRESS_3;

		bsp.eeprom.structure.ip4_static.ip[0] = IP_ADDRESS_0;
		bsp.eeprom.structure.ip4_static.ip[1] = IP_ADDRESS_1;
		bsp.eeprom.structure.ip4_static.ip[2] = IP_ADDRESS_2;
		bsp.eeprom.structure.ip4_static.ip[3] = IP_ADDRESS_3;

		bsp.eeprom.structure.ip4_static.netmask[0] = NETMASK_ADDRESS_0;
		bsp.eeprom.structure.ip4_static.netmask[1] = NETMASK_ADDRESS_1;
		bsp.eeprom.structure.ip4_static.netmask[2] = NETMASK_ADDRESS_2;
		bsp.eeprom.structure.ip4_static.netmask[3] = NETMASK_ADDRESS_3;

		bsp.eeprom.structure.ip4_static.MAC[0] = MAC_0;
		bsp.eeprom.structure.ip4_static.MAC[1] = MAC_1;
		bsp.eeprom.structure.ip4_static.MAC[2] = MAC_2;
		bsp.eeprom.structure.ip4_static.MAC[3] = MAC_3;
		bsp.eeprom.structure.ip4_static.MAC[4] = MAC_4;
		bsp.eeprom.structure.ip4_static.MAC[5] = MAC_5;


		bsp.scpi_raw.tcp_port = 5025;
		bsp.scpi_raw.udp_port = 5025;

		bsp.eeprom.structure.setup.multiplier = 2.0;
		bsp.eeprom.structure.setup.nominal_current = 6;

		bsp.eeprom.structure.calibration.gain[0] = 1.0f;
		bsp.eeprom.structure.calibration.gain[1] = 1.0f;
		bsp.eeprom.structure.calibration.gain[2] = 1.0f;
		bsp.eeprom.structure.calibration.count = 0u;

		bsp.eeprom.structure.services.hislip = true;
		bsp.eeprom.structure.services.mdns = true;

		strncpy(bsp.eeprom.structure.info.serial_number, SCPI_IDN3, STRING_LENGTH);

		eeprom_default = bsp.eeprom;
	}

}


// --------------------------------------------------------------------------------------------------------------------

static void BSP_Init_IP4Current()
{
	if(bsp.default_cfg)
	{
		bsp.ip4_current.MAC[0] = bsp.eeprom.structure.ip4_static.MAC[0];
		bsp.ip4_current.MAC[1] = bsp.eeprom.structure.ip4_static.MAC[1];
		bsp.ip4_current.MAC[2] = bsp.eeprom.structure.ip4_static.MAC[2];
		bsp.ip4_current.MAC[3] = bsp.eeprom.structure.ip4_static.MAC[3];
		bsp.ip4_current.MAC[4] = bsp.eeprom.structure.ip4_static.MAC[4];
		bsp.ip4_current.MAC[5] = bsp.eeprom.structure.ip4_static.MAC[5];

		bsp.ip4_current.gateway[0] = GATEWAY_ADDRESS_0;
		bsp.ip4_current.gateway[1] = GATEWAY_ADDRESS_1;
		bsp.ip4_current.gateway[2] = GATEWAY_ADDRESS_2;
		bsp.ip4_current.gateway[3] = GATEWAY_ADDRESS_3;

		bsp.ip4_current.ip[0] = IP_ADDRESS_0;
		bsp.ip4_current.ip[1] = IP_ADDRESS_1;
		bsp.ip4_current.ip[2] = IP_ADDRESS_2;
		bsp.ip4_current.ip[3] = IP_ADDRESS_3;

		bsp.ip4_current.netmask[0] = NETMASK_ADDRESS_0;
		bsp.ip4_current.netmask[1] = NETMASK_ADDRESS_1;
		bsp.ip4_current.netmask[2] = NETMASK_ADDRESS_2;
		bsp.ip4_current.netmask[3] = NETMASK_ADDRESS_3;
	}
	else
	{
		bsp.ip4_current.MAC[0] = bsp.eeprom.structure.ip4_static.MAC[0];
		bsp.ip4_current.MAC[1] = bsp.eeprom.structure.ip4_static.MAC[1];
		bsp.ip4_current.MAC[2] = bsp.eeprom.structure.ip4_static.MAC[2];
		bsp.ip4_current.MAC[3] = bsp.eeprom.structure.ip4_static.MAC[3];
		bsp.ip4_current.MAC[4] = bsp.eeprom.structure.ip4_static.MAC[4];
		bsp.ip4_current.MAC[5] = bsp.eeprom.structure.ip4_static.MAC[5];

		bsp.ip4_current.gateway[0] = bsp.eeprom.structure.ip4_static.gateway[0];
		bsp.ip4_current.gateway[1] = bsp.eeprom.structure.ip4_static.gateway[1];
		bsp.ip4_current.gateway[2] = bsp.eeprom.structure.ip4_static.gateway[2];
		bsp.ip4_current.gateway[3] = bsp.eeprom.structure.ip4_static.gateway[3];

		bsp.ip4_current.ip[0] = bsp.eeprom.structure.ip4_static.ip[0];
		bsp.ip4_current.ip[1] = bsp.eeprom.structure.ip4_static.ip[1];
		bsp.ip4_current.ip[2] = bsp.eeprom.structure.ip4_static.ip[2];
		bsp.ip4_current.ip[3] = bsp.eeprom.structure.ip4_static.ip[3];

		bsp.ip4_current.netmask[0] = bsp.eeprom.structure.ip4_static.netmask[0];
		bsp.ip4_current.netmask[1] = bsp.eeprom.structure.ip4_static.netmask[1];
		bsp.ip4_current.netmask[2] = bsp.eeprom.structure.ip4_static.netmask[2];
		bsp.ip4_current.netmask[3] = bsp.eeprom.structure.ip4_static.netmask[3];
	}

}


// --------------------------------------------------------------------------------------------------------------------

bsp_result_t BSP_Init()
{
	bsp_result_t status;

	BSP_Init_DefualtEEPROM();

	if(!LL_GPIO_IsInputPinSet(MCU_DEFAULT_GPIO_Port, MCU_DEFAULT_Pin))
	{
		bsp.default_cfg = true;

	}
	else
	{
		bsp.default_cfg = false;
	}


	status = EEPROM_Status();

	switch (status)
	{
		case BSP_EEPROM_EMPTY:
		{
			BSP_Init_DefualtEEPROM();
			status = EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE);
			status = EEPROM_Read(&bsp.eeprom, EEPROM_CFG_SIZE);
			if(BSP_OK == status)
			{
				BSP_Init_IP4Current();
			}; break;
		}
		case BSP_OK:
		{
			status = EEPROM_Read(&bsp.eeprom, EEPROM_CFG_SIZE);
			if(BSP_OK == status)
			{
				BSP_Init_IP4Current();
			}; break;
		}
		default:
		{
			bsp.default_cfg = true;
			BSP_Init_DefualtEEPROM();
			BSP_Init_IP4Current();

		}; break;
	}

	BSP_Init_Common();
	HMSR_SMS_Init();

	return status;
}
