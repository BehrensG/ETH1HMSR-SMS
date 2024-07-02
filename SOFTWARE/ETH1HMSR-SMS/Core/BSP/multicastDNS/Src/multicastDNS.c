/*
 * test.c
 *
 *  Created on: Jun 7, 2024
 *      Author: grzegorz
 */

#include <DEVICE_INFO.h>
#include <multicastDNS.h>
#include "api.h"
#include "netif.h"
#include "mdns.h"

#include "SCPI_Def.h"
#include "BSP.h"

extern bsp_t bsp;

#if LWIP_MDNS_RESPONDER

static void web_txt(struct mdns_service *service, void *txt_userdata)
{
  err_t res;
  LWIP_UNUSED_ARG(txt_userdata);

  res = mdns_resp_add_service_txtitem(service, "path=/", 6);
  LWIP_ERROR("mdns add service txt failed\n", (res == ERR_OK), return);
}


static void test_txt(struct mdns_service *service, void *txt_userdata)
{
  err_t res;
  LWIP_UNUSED_ARG(txt_userdata);

  char txt[64];

  memset(txt, 0, 64);
  sprintf(txt,"Manufacurer=%s",MANUFACTURER);
  res = mdns_resp_add_service_txtitem(service, txt, strlen(txt));

  memset(txt, 0, 64);
  sprintf(txt,"Model=%s",MODEL);
  res = mdns_resp_add_service_txtitem(service, txt, strlen(txt));

  memset(txt, 0, 64);
  sprintf(txt,"SerialNumber=%s",bsp.eeprom.structure.info.serial_number);
  res = mdns_resp_add_service_txtitem(service, txt, strlen(txt));


  memset(txt, 0, 64);
  sprintf(txt,"FirmwareVersion=%s",SOFTWARE_VERSION);
  res = mdns_resp_add_service_txtitem(service, txt, strlen(txt));

  LWIP_ERROR("mdns add service txt failed\n", (res == ERR_OK), return);
}
#endif

#if LWIP_MDNS_RESPONDER

static void mdns_example_report(struct netif* netif, u8_t result)
{
  LWIP_PLATFORM_DIAG(("mdns status[netif %d]: %d\n", netif->num, result));
}

#endif
extern ip4_addr_t ipaddr;
ip4_addr_t groupaddr;

 void MDNS_Init(void)
{

#if LWIP_MDNS_RESPONDER

	 char hostname[32];
	 memset(hostname, 0, 32);

	 mdns_resp_register_name_result_cb(mdns_example_report);

	 mdns_resp_init();

	 sprintf(hostname,"%s_%s", MODEL, bsp.eeprom.structure.info.serial_number);
	 mdns_resp_add_netif(netif_default, hostname,20);
	 mdns_resp_add_service(netif_default, "web", "_http", DNSSD_PROTO_TCP, 80, 20, web_txt, NULL);
	 mdns_resp_add_service(netif_default, "lxi", "_lxi", DNSSD_PROTO_TCP, 80, 20, test_txt, NULL);
	 mdns_resp_add_service(netif_default, "spi-raw", "_scpi-raw", DNSSD_PROTO_TCP, 5025, 20, NULL, NULL);
	 mdns_resp_add_service(netif_default, "hislip0", "_hislip", DNSSD_PROTO_TCP, 4880, 20, NULL, NULL);
	 mdns_resp_announce(netif_default);

#endif
}


