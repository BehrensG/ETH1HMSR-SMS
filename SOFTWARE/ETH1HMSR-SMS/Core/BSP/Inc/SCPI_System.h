/*
 * SCPI_System.h
 *
 *  Created on: Feb 11, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_SCPI_SYSTEM_H_
#define BSP_INC_SCPI_SYSTEM_H_

#include "scpi/scpi.h"
#include "BSP.h"

// --------------------------------------------------------------------------------------------------------------------

#define SECURITY_ON 1
#define SECURITY_OFF 0

#define DEFAULT 2
#define NET_STR_WRONG_FORMAT 1
#define NET_STR_WRONG_NUMBER 2
#define NET_STR_OK 0
#define ETH_PORT_MAX_VAL 65535

#define SYS_RESET 	1
#define SYS_DEFAULT 2
#define SYS_SETUP 	3

// --------------------------------------------------------------------------------------------------------------------

enum e_eeprom_states
{
	EEPROM_RESET = 1,
	EEPROM_DEFAULT,
};

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanIpAddress(scpi_t * context);
scpi_result_t SCPI_SystemCommunicateLanIpAddressQ(scpi_t * context);
scpi_result_t SCPI_SystemCommunicateLanIpSmask(scpi_t * context);
scpi_result_t SCPI_SystemCommunicateLanIpSmaskQ(scpi_t * context);
scpi_result_t SCPI_SystemCommunicateLanGateway(scpi_t * context);
scpi_result_t SCPI_SystemCommunicateLanGatewayQ(scpi_t * context);
scpi_result_t SCPI_SystemCommunicateLanMac(scpi_t * context);
scpi_result_t SCPI_SystemCommunicateLanMacQ(scpi_t * context);
scpi_result_t SCPI_SystemServiceMDNSEnable(scpi_t * context);
scpi_result_t SCPI_SystemServiceMDNSEnableQ(scpi_t * context);
scpi_result_t SCPI_SystemServiceHISLIPEnable(scpi_t * context);
scpi_result_t SCPI_SystemServiceHISLIPEnableQ(scpi_t * context);
scpi_result_t SCPI_SystemCommunicationLanUpdate(scpi_t * context);
scpi_result_t SCPI_SystemSecureState(scpi_t * context);
scpi_result_t SCPI_SystemSecureStateQ(scpi_t * context);
scpi_result_t SCPI_SystemServiceEeprom(scpi_t * context);
scpi_result_t SCPI_SystemServiceCurrentNominalQ(scpi_t * context);
scpi_result_t SCPI_SystemServiceLEDEnable(scpi_t * context);
scpi_result_t SCPI_SystemServiceLEDEnableQ(scpi_t * context);
scpi_result_t SCPI_SystemServiceLEDPing(scpi_t * context);
scpi_result_t SCPI_SystemServiceReset (scpi_t *context);

uint8_t SCPI_StringToMACArray(const char *MAC_string, uint8_t *MAC_array);

#endif /* BSP_INC_SCPI_SYSTEM_H_ */
