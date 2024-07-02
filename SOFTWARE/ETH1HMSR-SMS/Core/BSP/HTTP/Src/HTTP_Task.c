/*
 * http_task.c
 *
 *  Created on: Jun 13, 2024
 *      Author: grzegorz
 */
#include <string.h>
#include <stdbool.h>

#include "DEVICE_INFO.h"
#include "EEPROM.h"
#include "LED.h"
#include "HTTP_Task.h"


extern bsp_t bsp;


const char http_valid_response[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 0\r\n\r\n";

const char http_forbidden_response[] =
    "HTTP/1.1 403 Forbidden\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 0\r\n\r\n";

char* http_post_data(char* buf, u16_t buflen, u16_t* post_data_len)
{
	char* post_data_start = strstr(buf, "\r\n\r\n");  // Find the start of POST data
	if (post_data_start != NULL)
	{
	    post_data_start += 4;  // Move past "\r\n\r\n"
	    *post_data_len = buflen - (post_data_start - buf);

	    return post_data_start;
	}

	return NULL;

}

s32_t http_header(const char* buf,const http_cmd_t cmd[], size_t size)
{

	for(u16_t i = 0; i < size; i++)
	{
		if (strncmp((char const *)buf, cmd[i].cmd, strlen(cmd[i].cmd))==0)
		{
			return cmd[i].value;
		}
	}

	return -1;
}


bool http_string_to_ip4(const char *ip_string, uint8_t *ip_array)
{
	const char *start;

	start = ip_string;
	for (u8_t i = 0; i < 4; i++) {
		u8_t c;
		u16_t n = 0;
		while (1) {
			c = *start;
			start++;
			if (c >= '0' && c <= '9') {
				n *= 10;
				n += c - '0';
			}
			else if ((i < 3 && c == '.') || i == 3) {
				break;
			} else {
				return false;
			}
		}
		if (n >= 256) {
			return false;
		}
		ip_array[i] = n;
	}

	return true;
}

err_t http_send(struct netconn *conn, char* pagedata)
{
	return netconn_write(conn, (const unsigned char*)pagedata, strlen(pagedata), NETCONN_NOCOPY);
}


// ----------------------------------------------------------------------------------------------------------------------------------


static void http_server(struct netconn *conn)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char* buf;
	u16_t buflen;
	bool done = false;

	recv_err = netconn_recv(conn, &inbuf);

	if (recv_err == ERR_OK)
	{
		if (netconn_err(conn) == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&buf, &buflen);

			// Order of the functions is important. The page and image load steps should be first.

			if(!done)
			{
				done = http_load_page(conn, buf, buflen);
			}


			if(!done)
			{
				done = http_home_page(conn, buf, buflen);
			}

			if(!done)
			{
				done = http_setup_page(conn, buf, buflen);
			}

			if(!done)
			{
				done = http_setup_page(conn, buf, buflen);
			}

			if(!done)
			{
				done = http_lxi_identification(conn, buf, buflen);
			}

			if(!done)
			{
				done = http_control_page(conn, buf, buflen);
			}

			if(!done)
			{
				done = http_service_page(conn, buf, buflen);
			}
		}
	}

	netconn_close(conn);
	netbuf_delete(inbuf);
}

// ----------------------------------------------------------------------------------------------------------------------------------

static void http_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;


  conn = netconn_new(NETCONN_TCP);

  if (conn!= NULL)
  {

    err = netconn_bind(conn, IP_ADDR_ANY, 80);

    if (err == ERR_OK)
    {

      netconn_listen(conn);

      while(1)
      {
    	 vTaskDelay(pdMS_TO_TICKS(2));
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {

          http_server(newconn);
          netconn_delete(newconn);
        }
      }
    }
  }
}

#define HTTP_TASK_SIZE	1024

TaskHandle_t http_handler;
uint32_t http_buffer[HTTP_TASK_SIZE];
StaticTask_t http_control_block;

void HTTP_CreateTask(void)
{
	http_handler = xTaskCreateStatic(http_thread,"http_Task",
			HTTP_TASK_SIZE, (void*)1, tskIDLE_PRIORITY + 3,
			http_buffer, &http_control_block);
}
