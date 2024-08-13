/*
 * bsp.h
 *
 *  Created on: Feb 10, 2023
 *      Author: grzegorz
 */

#ifndef THIRD_PARTY_BSP_INC_BSP_H_
#define THIRD_PARTY_BSP_INC_BSP_H_

#include <stdbool.h>

#include "main.h"
#include "ip_addr.h"

// --------------------------------------------------------------------------------------------------------------------

#define IP_ADDRESS_0 192
#define IP_ADDRESS_1 168
#define IP_ADDRESS_2 1
#define IP_ADDRESS_3 17

#define NETMASK_ADDRESS_0 255
#define NETMASK_ADDRESS_1 255
#define NETMASK_ADDRESS_2 255
#define NETMASK_ADDRESS_3 0

#define GATEWAY_ADDRESS_0 192
#define GATEWAY_ADDRESS_1 168
#define GATEWAY_ADDRESS_2 1
#define GATEWAY_ADDRESS_3 1

#define MAC_0 0x00
#define MAC_1 0x80
#define MAC_2 0xE1
#define MAC_3 0x00
#define MAC_4 0x00
#define MAC_5 0x00

// --------------------------------------------------------------------------------------------------------------------

#define STRING_LENGTH 		16

#define TCPIP_DEFAULT_PORT 2000

#define SELECT_ADC1		1
#define SELECT_ADC2		2

// --------------------------------------------------------------------------------------------------------------------

typedef enum bsp_result
{
	BSP_OK = 0,
	BSP_COMM_OK = BSP_OK,
	BSP_ERROR = -1,
	BSP_ERROR_TIMEOUT = -2,

	/* I2C Errors */
	BSP_ERROR_I2C_BUS_NOT_READY = -20,
	BSP_ERROR_I2C_BUFFER_OVERFLOW = -22,
	BSP_ERROR_I2C_ARBITRATION_LOST = -23,
	BSP_ERROR_I2C_BUS_ERROR = -24,
	BSP_ERROR_I2C_NACK_RECEIVED = -25,
	BSP_ERROR_I2C_FAIL = -26,

	BSP_EEPROM_WRITE_ERROR = -50,
	BSP_EEPROM_READ_ERROR = -51,
	BSP_EEPROM_NO_CONNECTION = -52,
	BSP_EEPROM_EMPTY = -53

} bsp_result_t;

// --------------------------------------------------------------------------------------------------------------------

typedef enum format_data_enum
{
	FORMAT_DATA_ASCII = 0,
	FORMAT_DATA_REAL32 = 1
}format_data_t;

// --------------------------------------------------------------------------------------------------------------------

#pragma pack(push, 1)


typedef struct
{

	char serial_number[STRING_LENGTH];

}bsp_scpi_info_t;


typedef struct
{
	uint8_t ip[4];
	uint8_t netmask[4];
	uint8_t gateway[4];
	uint8_t MAC[6];

}bsp_ip4_lan_t;


typedef struct
{

	float gain[3];
	uint32_t count;

}bsp_calibration_t;

typedef struct
{
	uint8_t nominal_current;
	float multiplier;

}bsp_setup_t;

typedef struct
{
	uint8_t mdns;
	uint8_t hislip;

}bsp_services_t;

#define EEPROM_CFG_SIZE 	sizeof(bsp_ip4_lan_t) + sizeof(bsp_scpi_info_t) + sizeof(bsp_calibration_t) + sizeof(bsp_setup_t) + sizeof(bsp_services_t)

typedef union
{
	struct data
	{

		bsp_ip4_lan_t ip4_static;
		bsp_scpi_info_t info;
		bsp_calibration_t calibration;
		bsp_setup_t setup;
		bsp_services_t services;

	}structure;
	uint8_t bytes[EEPROM_CFG_SIZE];

}bsp_eeprom_t;

#pragma pack(pop)

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	bool status;

}bsp_security_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	bool enable;
	uint16_t ratio;

}bsp_oversampling_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	bool enable;
	float zero[3];

}bsp_offset_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	uint8_t value;
	uint8_t index;

}bsp_gain_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	bsp_oversampling_t oversampling;
	uint8_t bits;
	uint32_t sampling_time;
	float cycles;
	float period;
	bsp_gain_t gain;
	uint32_t sample_count;
	float vcom;
	bsp_offset_t offset;
	bsp_offset_t math_offset;
	float resolution;
	uint32_t right_bit_shift;

}bsp_adc_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	float measuring_range;
	float resolution;
	float signal_multiplier;

}bsp_hmsr_sms_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	ip_addr_t* addr;
	uint16_t port;
	uint32_t timeout;
	uint32_t tick;
	bool connected;

}udp_client_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	format_data_t data;
}format_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	uint16_t tcp_port;
	uint16_t udp_port;
}scpi_raw_t;

// --------------------------------------------------------------------------------------------------------------------

typedef enum
{
	VISA_SCPI_RAW,
	VISA_HISLIP
}bsp_resource_t;

// --------------------------------------------------------------------------------------------------------------------

typedef struct
{
	bsp_eeprom_t eeprom;
	bsp_security_t security;
	bsp_ip4_lan_t ip4_current;
	bool default_cfg;
	bool led;
	bsp_adc_t adc;
	bsp_hmsr_sms_t hmsr_sms;
	udp_client_t udp_client;
	format_t format;
	scpi_raw_t scpi_raw;
	bsp_resource_t resource;
}bsp_t;

// --------------------------------------------------------------------------------------------------------------------

bsp_result_t BSP_Init();

#endif /* THIRD_PARTY_BSP_INC_BSP_H_ */
