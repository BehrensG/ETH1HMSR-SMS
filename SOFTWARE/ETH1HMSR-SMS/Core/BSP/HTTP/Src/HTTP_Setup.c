/*
 * HTTP_Setup.c
 *
 *  Created on: Jun 19, 2024
 *      Author: grzegorz
 */

// --------------------------------------------------------------------------------------------------------------------

#include <string.h>

#include "HTTP_Task.h"
#include "BSP.h"
#include "DEVICE_INFO.h"
#include "EEPROM.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------

#define	HTTP_GET_SETUP_INSTR		0
#define	HTTP_GET_SETUP_NETMASK		1
#define	HTTP_GET_SETUP_GATEWAY		2
#define	HTTP_POST_PASSWORD			3
#define	HTTP_GET_PASSWORD_STATUS	4
#define	HTTP_POST_SETUP_INSTR		5
#define	HTTP_POST_SETUP_NETMASK		6
#define	HTTP_POST_SETUP_GATEWAY		7
#define	HTTP_POST_SAVE_IP_CONFIG	8
#define	HTTP_POST_SETUP_RESET		9

// --------------------------------------------------------------------------------------------------------------------

static const http_cmd_t http_setup[] = {
		{.cmd="GET /setup_ipv4", .value = HTTP_GET_SETUP_INSTR},
		{.cmd="GET /setup_netmask", .value = HTTP_GET_SETUP_NETMASK},
		{.cmd="GET /setup_gateway", .value = HTTP_GET_SETUP_GATEWAY},
		{.cmd="POST /setup_login", .value = HTTP_POST_PASSWORD},
		{.cmd="GET /setup_login_status", .value = HTTP_GET_PASSWORD_STATUS},
		{.cmd="POST /setup_ipv4", .value = HTTP_POST_SETUP_INSTR},
		{.cmd="POST /setup_netmask", .value = HTTP_POST_SETUP_NETMASK},
		{.cmd="POST /setup_gateway", .value = HTTP_POST_SETUP_GATEWAY},
		{.cmd="POST /save_ip_config", .value = HTTP_POST_SAVE_IP_CONFIG},
		{.cmd="POST /setup_reset", .value = HTTP_POST_SETUP_RESET}
};

// --------------------------------------------------------------------------------------------------------------------

static bool valid_password = false;


// --------------------------------------------------------------------------------------------------------------------

bool http_setup_page(struct netconn *conn, char* buf, u16_t buflen)
{
	s32_t header = -1;
	bool status = false;


	u16_t post_data_len = 0;

	char pagedata[128];
	memset(pagedata,0,128);

	char setup[128];
	memset(setup,0,128);

	size_t size = sizeof(http_setup)/sizeof(http_setup[0]);

	header = http_header(buf, http_setup, size);

	switch(header)
	{
		case HTTP_GET_SETUP_INSTR : {

			sprintf(pagedata,"%d.%d.%d.%d", bsp.ip4_current.ip[0], bsp.ip4_current.ip[1],
					bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]);
		}; break;

		case HTTP_GET_SETUP_NETMASK : {

			sprintf(pagedata,"%d.%d.%d.%d", bsp.ip4_current.netmask[0], bsp.ip4_current.netmask[1],
						bsp.ip4_current.netmask[2], bsp.ip4_current.netmask[3]);
		}; break;

		case HTTP_GET_SETUP_GATEWAY : {

			sprintf(pagedata,"%d.%d.%d.%d", bsp.ip4_current.gateway[0], bsp.ip4_current.gateway[1],
					bsp.ip4_current.gateway[2], bsp.ip4_current.gateway[3]);
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

		case HTTP_POST_SETUP_INSTR : {

			if(valid_password)
			{
				char* data = http_post_data(buf, buflen, &post_data_len);
				memcpy(setup, data, post_data_len);
				http_string_to_ip4(setup, bsp.ip4_current.ip);
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_SETUP_NETMASK : {

			if(valid_password)
			{
				char* data = http_post_data(buf, buflen, &post_data_len);
				memcpy(setup, data, post_data_len);
				http_string_to_ip4(setup, bsp.ip4_current.netmask);
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_SETUP_GATEWAY : {

			if(valid_password)
			{
				char* data = http_post_data(buf, buflen, &post_data_len);
				memcpy(setup, data, post_data_len);
				http_string_to_ip4(setup, bsp.ip4_current.gateway);
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_SAVE_IP_CONFIG : {

			if(valid_password)
			{
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

				EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE);
			}

			memcpy(pagedata, http_valid_response, strlen(http_valid_response));

		}; break;

		case HTTP_POST_SETUP_RESET : {

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
