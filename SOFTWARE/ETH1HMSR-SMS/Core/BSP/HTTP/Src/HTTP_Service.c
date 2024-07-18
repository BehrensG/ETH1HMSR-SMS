/*
 * HTTP_Service.c
 *
 *  Created on: Jun 23, 2024
 *      Author: grzegorz
 */

// --------------------------------------------------------------------------------------------------------------------

#include <string.h>

#include "HTTP_Task.h"
#include "BSP.h"
#include "DEVICE_INFO.h"
#include "EEPROM.h"
#include "SCPI_System.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------

#define	HTTP_POST_SERVICE_MAC		0
#define	HTTP_POST_SERVICE_SN		1
#define	HTTP_POST_PASSWORD			2
#define HTTP_POST_NOMINAL_CURRENT	3
#define	HTTP_GET_PASSWORD_STATUS	4
#define	HTTP_POST_SAVE_TO_EEPROM	5
#define	HTTP_GET_IMG_FAVICON		6
#define	HTTP_POST_SERVICE_RESET		7

// --------------------------------------------------------------------------------------------------------------------

static const http_cmd_t http_service[] = {
		{.cmd="POST /macAddress", .value = HTTP_POST_SERVICE_MAC},
		{.cmd="POST /SerialNumber", .value = HTTP_POST_SERVICE_SN},
		{.cmd="POST /service_login", .value = HTTP_POST_PASSWORD},
		{.cmd="GET /service_login_status", .value = HTTP_GET_PASSWORD_STATUS},
		{.cmd="POST /nominal_current", .value = HTTP_POST_NOMINAL_CURRENT},
		{.cmd="POST /save_to_eeprom", .value = HTTP_POST_SAVE_TO_EEPROM},
		{.cmd="GET /img/favicon.png", .value = HTTP_GET_IMG_FAVICON},
		{.cmd="POST /service_reset", .value = HTTP_POST_SERVICE_RESET}
};

// --------------------------------------------------------------------------------------------------------------------

static bool valid_password = false;


// --------------------------------------------------------------------------------------------------------------------

bool http_service_page(struct netconn *conn, char* buf, u16_t buflen)
{
	s32_t header = -1;
	bool status = false;

	struct fs_file file;


	u16_t post_data_len = 0;

	char pagedata[128];
	memset(pagedata,0,128);

	char setup[128];
	memset(setup,0,128);

	size_t size = sizeof(http_service)/sizeof(http_service[0]);

	header = http_header(buf, http_service, size);

	switch(header)
	{

		case HTTP_GET_IMG_FAVICON : {

			fs_open(&file, "/img/favicon.png");
			netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
			fs_close(&file);
			status = true;
			header = 0;

		}; break;

		case HTTP_POST_PASSWORD : {

			char* data = http_post_data(buf, buflen, &post_data_len);

			if(!strncmp(data, PASSWORD, strlen(PASSWORD)) && (post_data_len == strlen(PASSWORD)))
			{
				valid_password = true;
				bsp.security.status = false;

			}
			else
			{
				valid_password = false;
				bsp.security.status = true;
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_GET_PASSWORD_STATUS : {

			if(valid_password)
			{
				strcpy(pagedata,"VALID");
			}
			else
			{
				strcpy(pagedata,"INVALID");
			}

		}; break;

		case HTTP_POST_NOMINAL_CURRENT : {

			if(valid_password)
			{
				char* data = http_post_data(buf, buflen, &post_data_len);

				int value = 0;
				sscanf(data, "%d", &value);

				bsp.eeprom.structure.setup.nominal_current = (u8_t)value;

				memcpy(pagedata, http_valid_response, strlen(http_valid_response));
			}

			memcpy(pagedata, http_forbidden_response, strlen(http_forbidden_response));

		}; break;

		case HTTP_POST_SERVICE_SN : {

			if(valid_password)
			{
				char* data = http_post_data(buf, buflen, &post_data_len);

				memset(bsp.eeprom.structure.info.serial_number, 0, STRING_LENGTH);
				memcpy(bsp.eeprom.structure.info.serial_number, data, post_data_len);

				memcpy(pagedata, http_valid_response, strlen(http_valid_response));

			}

			memcpy(pagedata, http_forbidden_response, strlen(http_forbidden_response));

		}; break;

		case HTTP_POST_SAVE_TO_EEPROM : {

			if(valid_password)
			{
				vTaskDelay(pdMS_TO_TICKS(5));
				EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE);

				memcpy(pagedata, http_valid_response, strlen(http_valid_response));
			}

			memcpy(pagedata, http_forbidden_response, strlen(http_forbidden_response));

		};break;

		case HTTP_POST_SERVICE_MAC : {

			if(valid_password)
			{
				char* data = http_post_data(buf, buflen, &post_data_len);

				u8_t numb[6] = {0};

				if (NET_STR_OK == SCPI_StringToMACArray(data, numb))
				{
					bsp.ip4_current.MAC[0] = bsp.eeprom.structure.ip4_static.MAC[0] = numb[0];
					bsp.ip4_current.MAC[1] = bsp.eeprom.structure.ip4_static.MAC[1] = numb[1];
					bsp.ip4_current.MAC[2] = bsp.eeprom.structure.ip4_static.MAC[2] = numb[2];
					bsp.ip4_current.MAC[3] = bsp.eeprom.structure.ip4_static.MAC[3] = numb[3];
					bsp.ip4_current.MAC[4] = bsp.eeprom.structure.ip4_static.MAC[4] = numb[4];
					bsp.ip4_current.MAC[5] = bsp.eeprom.structure.ip4_static.MAC[5] = numb[5];
				}

				memcpy(pagedata, http_valid_response, strlen(http_valid_response));
			}

			memcpy(pagedata, http_forbidden_response, strlen(http_forbidden_response));

		}; break;

		case HTTP_POST_SERVICE_RESET : {

			if(valid_password)
			{
				memcpy(pagedata, http_valid_response, strlen(http_valid_response));
				http_send(conn,pagedata);
				HAL_NVIC_SystemReset();
			}

			memcpy(pagedata, http_forbidden_response, strlen(http_forbidden_response));

		}; break;

		default: /* DO NOTHING */; break;
	}

	if(header >= 0)
	{

		http_send(conn,pagedata);
		status = true;
	}

	return status;
}
