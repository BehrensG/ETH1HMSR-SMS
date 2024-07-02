/*
 * HTTP_Task.h
 *
 *  Created on: Jun 18, 2024
 *      Author: grzegorz
 */

#ifndef BSP_HTTP_INC_HTTP_TASK_H_
#define BSP_HTTP_INC_HTTP_TASK_H_


#include <stdbool.h>
#include "lwip/apps/httpd_opts.h"
#include "lwip/apps/fs.h"

#include "arch.h"
#include "api.h"

typedef struct{
	const char* cmd;
	s32_t value;
}http_cmd_t;

extern const char http_valid_response[];
extern const char http_forbidden_response[];

void HTTP_CreateTask(void);

s32_t http_header(const char* buf, const http_cmd_t cmd[], size_t size);
bool http_string_to_ip4(const char *ip_string, uint8_t *ip_array);
char* http_post_data(char* buf, u16_t buflen, u16_t* post_data_len);
err_t http_send(struct netconn *conn, char* pagedata);

bool http_load_page(struct netconn *conn, char* buf, u16_t buflen);
bool http_lxi_identification(struct netconn *conn, char* buf, u16_t buflen);
bool http_setup_page(struct netconn *conn, char* buf, u16_t buflen);
bool http_home_page(struct netconn *conn, char* buf, u16_t buflen);
bool http_control_page(struct netconn *conn, char* buf, u16_t buflen);
bool http_service_page(struct netconn *conn, char* buf, u16_t buflen);

#endif /* BSP_HTTP_INC_HTTP_TASK_H_ */
