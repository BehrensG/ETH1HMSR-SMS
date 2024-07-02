//#include <stdio.h>

// --------------------------------------------------------------------------------------------------------------------

#include "SCPI_Def.h"
#include "SCPI_System.h"
#include "types.h"
#include "scpi.h"
#include "EEPROM.h"
#include "HMSR-SMS.h"
#include "printf.h"
#include "LED.h"
#include "DEVICE_INFO.h"

// --------------------------------------------------------------------------------------------------------------------

extern TIM_HandleTypeDef htim3;
extern bsp_t bsp;
extern bsp_eeprom_t eeprom_default;
extern scpi_choice_def_t scpi_boolean_select[];

// --------------------------------------------------------------------------------------------------------------------

#define LAN_CURRENT 1
#define LAN_STATIC 2

// --------------------------------------------------------------------------------------------------------------------

scpi_choice_def_t Lan_state_select[] = {
		{ "CURRent", LAN_CURRENT },
		{ "STATic", LAN_STATIC },
SCPI_CHOICE_LIST_END };

/* --------------------------------------------------------------------------------------------------------------------
 *
 * @INFO:
 * Private function to convert a IP string (format nnnn.nnnn.nnnn.nnnn) to a array of uint8_t. The conversion is need
 * for the lwIP Ethernet function.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

static uint8_t SCPI_StringToIp4Array(const char *ip_string, uint8_t *ip_array) {

	/* A pointer to the next digit to process. */
	const char *start;

	start = ip_string;
	for (uint8_t i = 0; i < 4; i++) {
		/* The digit being processed. */
		uint8_t c;
		/* The value of this byte. */
		uint16_t n = 0;
		while (1) {
			c = *start;
			start++;
			if (c >= '0' && c <= '9') {
				n *= 10;
				n += c - '0';
			}
			/* We insist on stopping at "." if we are still parsing
			 the first, second, or third numbers. If we have reached
			 the end of the numbers, we will allow any character. */
			else if ((i < 3 && c == '.') || i == 3) {
				break;
			} else {
				return NET_STR_WRONG_FORMAT;
			}
		}
		if (n >= 256) {
			return NET_STR_WRONG_NUMBER;
		}
		ip_array[i] = n;
	}

	return NET_STR_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * @INFO:
 * Private function to convert a MAC string (format xx:xx:xx:xx:xx:xx or xx-xx-xx-xx-xx-xx) to a array of uint8_t.
 * The conversion is need for the lwIP Ethernet function.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

uint8_t SCPI_StringToMACArray(const char *MAC_string, uint8_t *MAC_array) {
	unsigned int values[6];

	if (6 == sscanf(MAC_string, "%x:%x:%x:%x:%x:%x%*c", &values[0],
					&values[1], &values[2], &values[3], &values[4],
					&values[5])) {
		for (uint8_t i = 0; i < 6; i++)
			MAC_array[i] = (uint8_t) values[i];
	} else if (6 == sscanf(MAC_string, "%x-%x-%x-%x-%x-%x%*c", &values[0],
					&values[1], &values[2], &values[3], &values[4],
					&values[5])) {
		for (uint8_t i = 0; i < 6; i++)
			MAC_array[i] = (uint8_t) values[i];
	} else {
	}

	return NET_STR_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanIpAddress(scpi_t *context) {
	char str[16] = { 0 };
	uint8_t numb[4] = { 0 };
	size_t len = 0;
	uint8_t conv_result = 0;

	if (!SCPI_ParamCopyText(context, (char*) str, 16, &len, TRUE)) {
		return SCPI_RES_ERR;
	}

	conv_result = SCPI_StringToIp4Array(str, numb);

	switch (conv_result) {
	case NET_STR_OK: {
		bsp.ip4_current.ip[0] = numb[0];
		bsp.ip4_current.ip[1] = numb[1];
		bsp.ip4_current.ip[2] = numb[2];
		bsp.ip4_current.ip[3] = numb[3];
	}
		break;
	case NET_STR_WRONG_FORMAT:
		SCPI_ErrorPush(context, SCPI_ERROR_DATA_TYPE_ERROR);
		break;
	case NET_STR_WRONG_NUMBER:
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		break;
	default:
		return SCPI_RES_ERR;
		break;
	}

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanIpAddressQ(scpi_t *context) {
	int32_t value = 0;
	char str[16] = { 0 };

	if (!SCPI_ParamChoice(context, Lan_state_select, &value, FALSE)) {
		return SCPI_RES_ERR;
	}

	if (LAN_CURRENT == value) {
		sprintf(str, "%d.%d.%d.%d", bsp.ip4_current.ip[0],
				bsp.ip4_current.ip[1], bsp.ip4_current.ip[2],
				bsp.ip4_current.ip[3]);
	} else if (LAN_STATIC == value) {
		sprintf(str, "%d.%d.%d.%d", bsp.eeprom.structure.ip4_static.ip[0],
				bsp.eeprom.structure.ip4_static.ip[1],
				bsp.eeprom.structure.ip4_static.ip[2],
				bsp.eeprom.structure.ip4_static.ip[3]);
	}

	SCPI_ResultMnemonic(context, (char* )str);
	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanIpSmask(scpi_t *context) {
	char str[16] = { 0 };
	uint8_t numb[4] = { 0 };
	size_t len = 0;
	uint8_t conv_result = 0;

	if (!SCPI_ParamCopyText(context, (char*) str, 16, &len, TRUE)) {
		return SCPI_RES_ERR;
	}

	conv_result = SCPI_StringToIp4Array(str, numb);

	switch (conv_result) {
	case NET_STR_OK: {
		bsp.ip4_current.netmask[0] = numb[0];
		bsp.ip4_current.netmask[1] = numb[1];
		bsp.ip4_current.netmask[2] = numb[2];
		bsp.ip4_current.netmask[3] = numb[3];
	}
		break;
	case NET_STR_WRONG_FORMAT:
		SCPI_ErrorPush(context, SCPI_ERROR_DATA_TYPE_ERROR);
		break;
	case NET_STR_WRONG_NUMBER:
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		break;
	default:
		return SCPI_RES_ERR;
		break;
	}

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanIpSmaskQ(scpi_t *context) {
	int32_t value = 0;
	char str[16] = { 0 };

	if (!SCPI_ParamChoice(context, Lan_state_select, &value, FALSE)) {
		value = LAN_CURRENT;
	}
	if (LAN_CURRENT == value) {
		sprintf(str, "%d.%d.%d.%d", bsp.ip4_current.netmask[0],
				bsp.ip4_current.netmask[1], bsp.ip4_current.netmask[2],
				bsp.ip4_current.netmask[3]);
	} else if (LAN_STATIC == value) {
		sprintf(str, "%d.%d.%d.%d", bsp.eeprom.structure.ip4_static.netmask[0],
				bsp.eeprom.structure.ip4_static.netmask[1],
				bsp.eeprom.structure.ip4_static.netmask[2],
				bsp.eeprom.structure.ip4_static.netmask[3]);
	}
	SCPI_ResultMnemonic(context, (char* )str);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanGateway(scpi_t *context) {
	char gateway_str[16] = { 0 };
	uint8_t gateway_numb[4] = { 0 };
	size_t len = 0;
	uint8_t conv_result = 0;

	if (!SCPI_ParamCopyText(context, gateway_str, 16, &len, TRUE)) {
		return SCPI_RES_ERR;
	}

	conv_result = SCPI_StringToIp4Array(gateway_str, gateway_numb);

	switch (conv_result) {
	case NET_STR_OK: {
		bsp.ip4_current.gateway[0] = gateway_numb[0];
		bsp.ip4_current.gateway[1] = gateway_numb[1];
		bsp.ip4_current.gateway[2] = gateway_numb[2];
		bsp.ip4_current.gateway[3] = gateway_numb[3];
	}
		break;
	case NET_STR_WRONG_FORMAT:
		SCPI_ErrorPush(context, SCPI_ERROR_DATA_TYPE_ERROR);
		break;
	case NET_STR_WRONG_NUMBER:
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		break;
	default:
		return SCPI_RES_ERR;
		break;
	}

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanGatewayQ(scpi_t *context) {
	int32_t value = 0;
	char str[16] = { 0 };

	if (!SCPI_ParamChoice(context, Lan_state_select, &value, FALSE)) {
		value = LAN_CURRENT;
	}
	if (LAN_CURRENT == value) {

		sprintf(str, "%d.%d.%d.%d", bsp.ip4_current.gateway[0],
				bsp.ip4_current.gateway[1], bsp.ip4_current.gateway[2],
				bsp.ip4_current.gateway[3]);
	} else if (LAN_STATIC == value) {
		sprintf(str, "%d.%d.%d.%d", bsp.eeprom.structure.ip4_static.gateway[0],
				bsp.eeprom.structure.ip4_static.gateway[1],
				bsp.eeprom.structure.ip4_static.gateway[2],
				bsp.eeprom.structure.ip4_static.gateway[3]);
	}

	SCPI_ResultMnemonic(context, (char* )str);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanMac(scpi_t *context) {
	char str[18] = { 0 };
	uint8_t numb[6] = { 0 };
	size_t len = 0;
	uint8_t conv_result = 0;

	if (bsp.security.status) {
		SCPI_ErrorPush(context, SCPI_ERROR_SERVICE_MODE_SECURE);
		return SCPI_RES_ERR;
	}

	if (!SCPI_ParamCopyText(context, (char*) str, 18, &len, TRUE)) {
		return SCPI_RES_ERR;
	}

	conv_result = SCPI_StringToMACArray(str, numb);

	switch (conv_result) {
	case NET_STR_OK: {
		bsp.ip4_current.MAC[0] = numb[0];
		bsp.ip4_current.MAC[1] = numb[1];
		bsp.ip4_current.MAC[2] = numb[2];
		bsp.ip4_current.MAC[3] = numb[3];
		bsp.ip4_current.MAC[4] = numb[4];
		bsp.ip4_current.MAC[5] = numb[5];
	}
		break;
	case NET_STR_WRONG_FORMAT:
		SCPI_ErrorPush(context, SCPI_ERROR_DATA_TYPE_ERROR);
		break;
	case NET_STR_WRONG_NUMBER:
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		break;
	default:
		return SCPI_RES_ERR;
		break;
	}

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicateLanMacQ(scpi_t *context) {
	char str[18] = { 0 };

	if (!bsp.default_cfg) {
		sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", bsp.ip4_current.MAC[0],
				bsp.ip4_current.MAC[1], bsp.ip4_current.MAC[2],
				bsp.ip4_current.MAC[3], bsp.ip4_current.MAC[4],
				bsp.ip4_current.MAC[5]);
	} else {
		sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
				bsp.eeprom.structure.ip4_static.MAC[0],
				bsp.eeprom.structure.ip4_static.MAC[1],
				bsp.eeprom.structure.ip4_static.MAC[2],
				bsp.eeprom.structure.ip4_static.MAC[3],
				bsp.eeprom.structure.ip4_static.MAC[4],
				bsp.eeprom.structure.ip4_static.MAC[5]);
	}

	SCPI_ResultMnemonic(context, (char* )str);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemCommunicationLanUpdate(scpi_t *context) {

	if (bsp.security.status) {
		SCPI_ErrorPush(context, SCPI_ERROR_SERVICE_MODE_SECURE);
		return SCPI_RES_ERR;
	}

	bsp.eeprom.structure.ip4_static.MAC[0] = bsp.ip4_current.MAC[0];
	bsp.eeprom.structure.ip4_static.MAC[1] = bsp.ip4_current.MAC[1];
	bsp.eeprom.structure.ip4_static.MAC[2] = bsp.ip4_current.MAC[2];
	bsp.eeprom.structure.ip4_static.MAC[3] = bsp.ip4_current.MAC[3];
	bsp.eeprom.structure.ip4_static.MAC[4] = bsp.ip4_current.MAC[4];
	bsp.eeprom.structure.ip4_static.MAC[5] = bsp.ip4_current.MAC[5];

	bsp.eeprom.structure.ip4_static.gateway[0] = bsp.ip4_current.gateway[0];
	bsp.eeprom.structure.ip4_static.gateway[1] = bsp.ip4_current.gateway[1];
	bsp.eeprom.structure.ip4_static.gateway[2] = bsp.ip4_current.gateway[2];
	bsp.eeprom.structure.ip4_static.gateway[3] = bsp.ip4_current.gateway[3];

	bsp.eeprom.structure.ip4_static.ip[0] = bsp.ip4_current.ip[0];
	bsp.eeprom.structure.ip4_static.ip[1] = bsp.ip4_current.ip[1];
	bsp.eeprom.structure.ip4_static.ip[2] = bsp.ip4_current.ip[2];
	bsp.eeprom.structure.ip4_static.ip[3] = bsp.ip4_current.ip[3];

	bsp.eeprom.structure.ip4_static.netmask[0] = bsp.ip4_current.netmask[0];
	bsp.eeprom.structure.ip4_static.netmask[1] = bsp.ip4_current.netmask[1];
	bsp.eeprom.structure.ip4_static.netmask[2] = bsp.ip4_current.netmask[2];
	bsp.eeprom.structure.ip4_static.netmask[3] = bsp.ip4_current.netmask[3];


	if (BSP_OK == EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE)) {
		return SCPI_RES_OK;
	} else {
		SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
		return SCPI_RES_ERR;
	}

}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemSecureState(scpi_t *context) {
	int32_t state = 0;
	int8_t password_read[STRING_LENGTH] = { 0 };
	size_t length = 0;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &state, TRUE)) {
		return SCPI_RES_ERR;
	}

	if (!SCPI_ParamCopyText(context, (char*) password_read, STRING_LENGTH,
			&length, TRUE)) {
		return SCPI_RES_ERR;
	}

	if (!strcmp((const char*) password_read, (const char*) PASSWORD)) {
		bsp.security.status = state;
		return SCPI_RES_OK;
	} else {
		SCPI_ErrorPush(context, SCPI_ERROR_SERVICE_INVALID_PASSWORD);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemSecureStateQ(scpi_t *context) {
	SCPI_ResultUInt8(context, bsp.security.status);
	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_choice_def_t EEPROM_state_select[] = {
		{ "RESET", SYS_RESET },
		{ "DEFault", SYS_DEFAULT },
		{ "SETup", SYS_SETUP },
		SCPI_CHOICE_LIST_END };

scpi_result_t SCPI_SystemServiceEeprom(scpi_t *context) {
	int32_t select = 0;
	uint32_t nominal_current = 6;
	float multiplier = 2.0;

	char str[STRING_LENGTH] = { 0 };
	size_t len = 0;

	if (bsp.security.status) {
		SCPI_ErrorPush(context, SCPI_ERROR_SERVICE_MODE_SECURE);
		return SCPI_RES_ERR;
	}

	if (!SCPI_ParamChoice(context, EEPROM_state_select, &select, TRUE)) {
		return SCPI_RES_ERR;
	}

	if (SYS_RESET == select) {
		if (BSP_OK != EEPROM_Erase()) {
			SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
			return SCPI_RES_ERR;
		}
	} else if (SYS_DEFAULT == select) {

		if (BSP_OK != EEPROM_Write(&eeprom_default, EEPROM_CFG_SIZE)) {
			SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
			return SCPI_RES_ERR;
		}
	} else if (SYS_SETUP == select) {
		if (SCPI_ParamCopyText(context, (char*) str, 18, &len, FALSE)) {
			strncpy(bsp.eeprom.structure.info.serial_number, str,
					STRING_LENGTH);
		} else {
			return SCPI_RES_ERR;
		}

		if (SCPI_ParamUInt32(context, &nominal_current, FALSE)) {
			if (!HMSR_SMS_CheckNominalCurrent(nominal_current)) {
				SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
				return SCPI_RES_ERR;
			} else {
				bsp.eeprom.structure.setup.nominal_current = nominal_current;
			}
		} else {
			return SCPI_RES_ERR;
		}

		if (SCPI_ParamFloat(context, &multiplier, FALSE)) {
			bsp.eeprom.structure.setup.multiplier = multiplier;
		} else {
			return SCPI_RES_ERR;
		}

		if (BSP_OK != EEPROM_Write(&eeprom_default, EEPROM_CFG_SIZE)) {
			SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
			return SCPI_RES_ERR;
		}
	}

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemServiceCurrentNominalQ(scpi_t *context) {
	SCPI_ResultUInt8(context, bsp.eeprom.structure.setup.nominal_current);
	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemServiceLEDEnable(scpi_t *context) {
	int32_t state;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &state, TRUE)) {
		return SCPI_RES_ERR;
	}

	bsp.led = (bool) state;

	if(state)
	{
		HAL_TIM_Base_Start_IT(&htim3);
	}
	else
	{
		HAL_TIM_Base_Stop_IT(&htim3);
	}

	return SCPI_RES_OK;
}


scpi_result_t SCPI_SystemServiceLEDEnableQ(scpi_t *context) {
	SCPI_ResultBool(context, bsp.led);
	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemServiceLEDPing(scpi_t *context) {
	LED_Toggle((GREEN | RED | BLUE), 100, 900);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemServiceMDNSEnable(scpi_t *context) {

	int32_t state;


	if (bsp.security.status) {

		SCPI_ErrorPush(context, SCPI_ERROR_SERVICE_MODE_SECURE);
		return SCPI_RES_ERR;
	}

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &state, TRUE)) {

		return SCPI_RES_ERR;
	}

		bsp.eeprom.structure.services.mdns = (bool) state;

		if (BSP_OK == EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE)) {
			return SCPI_RES_OK;
		} else {
			SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
			return SCPI_RES_ERR;
		}

	return SCPI_RES_OK;
}

scpi_result_t SCPI_SystemServiceMDNSEnableQ(scpi_t *context) {
	SCPI_ResultBool(context, bsp.eeprom.structure.services.mdns);
	return SCPI_RES_OK;
}

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemServiceHISLIPEnable(scpi_t *context) {

	int32_t state;


	if (bsp.security.status) {

		SCPI_ErrorPush(context, SCPI_ERROR_SERVICE_MODE_SECURE);
		return SCPI_RES_ERR;
	}

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &state, TRUE)) {

		return SCPI_RES_ERR;
	}

		bsp.eeprom.structure.services.hislip = (bool) state;

		if (BSP_OK == EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE)) {
			return SCPI_RES_OK;
		} else {
			SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
			return SCPI_RES_ERR;
		}

	return SCPI_RES_OK;
}

scpi_result_t SCPI_SystemServiceHISLIPEnableQ(scpi_t *context) {
	SCPI_ResultBool(context, bsp.eeprom.structure.services.hislip);
	return SCPI_RES_OK;
}

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_SystemServiceReset (scpi_t *context)
{
	HAL_NVIC_SystemReset();
	return SCPI_RES_OK;
}
