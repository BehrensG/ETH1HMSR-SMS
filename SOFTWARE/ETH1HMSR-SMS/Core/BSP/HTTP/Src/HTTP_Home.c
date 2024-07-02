/*
 * HTTP_Home.c
 *
 *  Created on: Jun 20, 2024
 *      Author: BehrensG
 */

// --------------------------------------------------------------------------------------------------------------------

#include "HTTP_Task.h"
#include "SCPI_Def.h"
#include "BSP.h"
#include "DEVICE_INFO.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------

#define HTTP_GET_HOME_INSTR					0
#define HTTP_GET_HOME_SN					1
#define HTTP_GET_HOME_DESCR					2
#define HTTP_GET_HOME_IPV4					3
#define HTTP_GET_HOME_NETMASK				4
#define HTTP_GET_HOME_GATEWAY				5
#define HTTP_GET_HOME_MAC					6
#define HTTP_GET_HOME_VISA					7
#define HTTP_GET_HOME_MDIX					8
#define HTTP_GET_HOME_MDNS					9
#define HTTP_GET_HOME_UDP					10
#define HTTP_GET_HOME_NOMINAL_CURRENT		11
#define HTTP_GET_HOME_MEAS_RANGE			12

// --------------------------------------------------------------------------------------------------------------------

static const http_cmd_t http_home[] = {
		{.cmd="GET /info_instr", .value = HTTP_GET_HOME_INSTR},
		{.cmd="GET /info_sn", .value = HTTP_GET_HOME_SN},
		{.cmd="GET /info_descr", .value = HTTP_GET_HOME_DESCR},
		{.cmd="GET /info_ipv4", .value = HTTP_GET_HOME_IPV4},
		{.cmd="GET /info_netmask", .value = HTTP_GET_HOME_NETMASK},
		{.cmd="GET /info_gateway", .value = HTTP_GET_HOME_GATEWAY},
		{.cmd="GET /info_mac", .value = HTTP_GET_HOME_MAC},
		{.cmd="GET /info_visa", .value = HTTP_GET_HOME_VISA},
		{.cmd="GET /info_mdix", .value = HTTP_GET_HOME_MDIX},
		{.cmd="GET /info_mdns", .value = HTTP_GET_HOME_MDNS},
		{.cmd="GET /info_udp", .value = HTTP_GET_HOME_UDP},
		{.cmd="GET /info_nom_current", .value = HTTP_GET_HOME_NOMINAL_CURRENT},
		{.cmd="GET /info_meas_range", .value = HTTP_GET_HOME_MEAS_RANGE}
};


// --------------------------------------------------------------------------------------------------------------------

bool http_home_page(struct netconn *conn, char* buf, u16_t buflen)
{
	s32_t header = -1;
	bool status = false;

	char pagedata[128];
	memset(pagedata,0,128);

	size_t size = sizeof(http_home)/sizeof(http_home[0]);
	header = http_header(buf, http_home, size);

	switch(header)
	{
		case HTTP_GET_HOME_INSTR: strcpy(pagedata, MODEL); break;
		case HTTP_GET_HOME_SN: strcpy(pagedata,bsp.eeprom.structure.info.serial_number); break;
		case HTTP_GET_HOME_DESCR: strcpy(pagedata,MANUFACTURER_DESCRIPTION); break;

		case HTTP_GET_HOME_IPV4: sprintf(pagedata,"%d.%d.%d.%d", bsp.ip4_current.ip[0], bsp.ip4_current.ip[1],
										bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]); break;

		case HTTP_GET_HOME_NETMASK: sprintf(pagedata,"%d.%d.%d.%d", bsp.ip4_current.netmask[0], bsp.ip4_current.netmask[1],
											bsp.ip4_current.netmask[2], bsp.ip4_current.netmask[3]); break;

		case HTTP_GET_HOME_GATEWAY: sprintf(pagedata,"%d.%d.%d.%d", bsp.ip4_current.gateway[0], bsp.ip4_current.gateway[1],
											bsp.ip4_current.gateway[2], bsp.ip4_current.gateway[3]); break;

		case HTTP_GET_HOME_MAC: sprintf(pagedata,"%02X:%02X:%02X:%02X:%02X:%02X", bsp.ip4_current.MAC[0], bsp.ip4_current.MAC[1],
										bsp.ip4_current.MAC[2], bsp.ip4_current.MAC[3], bsp.ip4_current.MAC[4], bsp.ip4_current.MAC[5]); break;

		case HTTP_GET_HOME_VISA:

			if(bsp.eeprom.structure.services.hislip)
			{
				sprintf(pagedata,"TCPIP0::%d.%d.%d.%d::5025::SOCKET\r\nTCPIP0::%d.%d.%d.%d::hislip0::INSTR",
						bsp.ip4_current.ip[0], bsp.ip4_current.ip[1], bsp.ip4_current.ip[2], bsp.ip4_current.ip[3],
						bsp.ip4_current.ip[0], bsp.ip4_current.ip[1], bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]); break;
			}
			else
			{
				sprintf(pagedata,"TCPIP0::%d.%d.%d.%d::5025::SOCKET",
						bsp.ip4_current.ip[0], bsp.ip4_current.ip[1], bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]); break;
			}


		case HTTP_GET_HOME_MDIX: strcpy(pagedata,"YES"); break;
		case HTTP_GET_HOME_MDNS: (bsp.eeprom.structure.services.hislip) ? strcpy(pagedata,"YES") : strcpy(pagedata,"NO"); break;
		case HTTP_GET_HOME_UDP: strcpy(pagedata,"5025"); break;
		case HTTP_GET_HOME_NOMINAL_CURRENT: sprintf(pagedata,"%d", bsp.eeprom.structure.setup.nominal_current); break;

		case HTTP_GET_HOME_MEAS_RANGE : {

			switch (bsp.eeprom.structure.setup.nominal_current)
			{
				case 6: strcpy(pagedata,"-15 / +15"); break;
				case 8: strcpy(pagedata,"-20 / +20"); break;
				case 10: strcpy(pagedata,"-25 / +25"); break;
				case 15: strcpy(pagedata,"-37.5 / +37.5"); break;
				case 20: strcpy(pagedata,"-50 / +50"); break;
				case 30: strcpy(pagedata,"-75 / +75"); break;
				default : strcpy(pagedata,"INVALID !"); break;
			}
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
