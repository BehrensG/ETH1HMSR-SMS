/*
 * HTTP_LXI_Idn.c
 *
 *  Created on: Jun 19, 2024
 *      Author: grzegorz
 */

// --------------------------------------------------------------------------------------------------------------------

#include "HTTP_Task.h"
#include "SCPI_Def.h"
#include "BSP.h"
#include "DEVICE_INFO.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------

#define PAGE_BODY_SIZE	2048
#define FORMATED_SIZE	256
#define HEADER_SIZE		128

// --------------------------------------------------------------------------------------------------------------------

static err_t http_dynamic_web_page(struct netconn *conn)
{

	err_t err;


	char HTTP_HEADER[HEADER_SIZE];
	char PAGE_BODY[PAGE_BODY_SIZE];
	char formated[FORMATED_SIZE];

	memset(PAGE_BODY, 0, PAGE_BODY_SIZE);
	memset(HTTP_HEADER, 0, HEADER_SIZE);


	strcat((char *)PAGE_BODY, "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\r\n");

	strcat((char *)PAGE_BODY, "<LXIDevice xmlns=\"http://www.lxistandard.org/InstrumentIdentification/1.0\""
		" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.lxistandard.org/InstrumentIdentification/1.0 LXIIdentification.xsd\">\r\n");


		// Manufacturer
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<Manufacturer>%s</Manufacturer>\r\n",MANUFACTURER);
		strcat((char *)PAGE_BODY, formated);

		// Model
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<Model>%s</Model>\r\n",MODEL);
		strcat((char *)PAGE_BODY, formated);

		// SerialNumber
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<SerialNumber>%s</SerialNumber>\r\n", bsp.eeprom.structure.info.serial_number); // Change this
		strcat((char *)PAGE_BODY, formated);

		// FirmwareRevision
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<FirmwareRevision>%s</FirmwareRevision>\r\n",SOFTWARE_VERSION);
		strcat((char *)PAGE_BODY, formated);

		// ManufacturerDescription
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<ManufacturerDescription>%s</ManufacturerDescription>\r\n",MANUFACTURER_DESCRIPTION);
		strcat((char *)PAGE_BODY, formated);

		// HomepageURL
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<HomepageURL></HomepageURL>\r\n");
		strcat((char *)PAGE_BODY, formated);

		// DriverURL
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<DriverURL></DriverURL>\r\n");
		strcat((char *)PAGE_BODY, formated);

		// UserDescription
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<UserDescription></UserDescription>\r\n");
		strcat((char *)PAGE_BODY, formated);

		// IdentificationURL
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<IdentificationURL>%d.%d.%d.%d/lxi/identification</IdentificationURL>\r\n",
							bsp.ip4_current.ip[0], bsp.ip4_current.ip[1], bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]);
		strcat((char *)PAGE_BODY, formated);

		// Interface
		strcat((char *)PAGE_BODY, "<Interface xsi:type=\"NetworkInformation\" InterfaceType=\"LXI\" IPType=\"IPv4\">\r\n");

			// InstrumentAddressString SOCKET
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<InstrumentAddressString>TCPIP::%d.%d.%d.%d::5025::SOCKET</InstrumentAddressString>\r\n",
								bsp.ip4_current.ip[0], bsp.ip4_current.ip[1], bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]);
			strcat((char *)PAGE_BODY, formated);

			// InstrumentAddressString HISLIP
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<InstrumentAddressString>TCPIP::%d.%d.%d.%d::hislip0::INSTR</InstrumentAddressString>\r\n",
								bsp.ip4_current.ip[0], bsp.ip4_current.ip[1], bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]);
			strcat((char *)PAGE_BODY, formated);

			// Hostname
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<Hostname></Hostname>\r\n");
			strcat((char *)PAGE_BODY, formated);

			// IPAddress
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<IPAddress>%d.%d.%d.%d</IPAddress>\r\n",
					bsp.ip4_current.ip[0], bsp.ip4_current.ip[1], bsp.ip4_current.ip[2], bsp.ip4_current.ip[3]);
			strcat((char *)PAGE_BODY, formated);

			// SubnetMask
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<SubnetMask>%d.%d.%d.%d</SubnetMask>\r\n",
								bsp.ip4_current.netmask[0], bsp.ip4_current.netmask[1], bsp.ip4_current.netmask[2], bsp.ip4_current.netmask[3]);
			strcat((char *)PAGE_BODY, formated);

			// MACAddress
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<MACAddress>%02X-%02X-%02X-%02X-%02X-%02X</MACAddress>\r\n",
								bsp.ip4_current.MAC[0], bsp.ip4_current.MAC[1], bsp.ip4_current.MAC[2],
								bsp.ip4_current.MAC[3], bsp.ip4_current.MAC[4], bsp.ip4_current.MAC[5]);
			strcat((char *)PAGE_BODY, formated);

			// Gateway
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<Gateway>%d.%d.%d.%d</Gateway>\r\n",
					bsp.ip4_current.gateway[0], bsp.ip4_current.gateway[1], bsp.ip4_current.gateway[2], bsp.ip4_current.gateway[3]);
			strcat((char *)PAGE_BODY, formated);

			// DHCPEnabled
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<DHCPEnabled>false</DHCPEnabled>\r\n");
			strcat((char *)PAGE_BODY, formated);

			// AutoIPEnabled
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<AutoIPEnabled>false</AutoIPEnabled>\r\n");
			strcat((char *)PAGE_BODY, formated);

		// Interface
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "</Interface>\r\n");
		strcat((char *)PAGE_BODY, formated);

		// IVISoftwareModuleName
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<IVISoftwareModuleName>unknown</IVISoftwareModuleName>\r\n");
		strcat((char *)PAGE_BODY, formated);

		// LXIVersion
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<LXIVersion>1.4</LXIVersion>\r\n");
		strcat((char *)PAGE_BODY, formated);

		// LXIExtendedFunctions
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "<LXIExtendedFunctions>\r\n");
		strcat((char *)PAGE_BODY, formated);

			// Function
			memset(formated,0, FORMATED_SIZE);
			sprintf(formated, "<Function FunctionName=\"LXI HiSLIP\" Version=\"1.0\"></Function>");
			strcat((char *)PAGE_BODY, formated);

		// LXIExtendedFunctions
		memset(formated,0, FORMATED_SIZE);
		sprintf(formated, "</LXIExtendedFunctions>\r\n");
		strcat((char *)PAGE_BODY, formated);

	// LXIDevice
	memset(formated,0, FORMATED_SIZE);
	sprintf(formated, "</LXIDevice>\r\n");
	strcat((char *)PAGE_BODY, formated);



	sprintf(HTTP_HEADER,"HTTP/1.1 200 OK\r\n"
    "Content-Type: text/xml\r\n"
    "Content-Length: %d\r\n\r\n",strlen(PAGE_BODY));

	err = netconn_write(conn, HTTP_HEADER, strlen(HTTP_HEADER), NETCONN_NOCOPY);
	err = netconn_write(conn, PAGE_BODY, strlen(PAGE_BODY), NETCONN_NOCOPY);

	return err;
}


// --------------------------------------------------------------------------------------------------------------------

const char http_lxi_idn[] = "GET /lxi/identification";

bool http_lxi_identification(struct netconn *conn, char* buf, u16_t buflen)
{
	if (strncmp((char const *)buf, http_lxi_idn, strlen(http_lxi_idn))==0)
	{
		http_dynamic_web_page(conn);

		return true;

	}

	return false;
}

