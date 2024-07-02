/*
 * HTTP_Index.c
 *
 *  Created on: Jun 19, 2024
 *      Author: grzegorz
 */


#include "HTTP_Task.h"

#define	HTTP_GET_SITE_INIT			0
#define	HTTP_GET_SITE_TOP			1
#define	HTTP_GET_SITE_HOME			2
#define	HTTP_GET_SITE_SETUP			3
#define	HTTP_GET_SITE_CONTROL		4
#define	HTTP_GET_SITE_INDEX			5
#define	HTTP_GET_SITE_HELP			6
#define	HTTP_GET_IMG_INSTRUMENT		7
#define	HTTP_GET_IMG_FAVICON		8
#define	HTTP_GET_SITE_SERVICE		9

static const http_cmd_t http_index[] = {
		{.cmd="GET / HTTP/1.1", .value = HTTP_GET_SITE_INIT},
		{.cmd="GET /top.html", .value = HTTP_GET_SITE_TOP},
		{.cmd="GET /home.html", .value = HTTP_GET_SITE_HOME},
		{.cmd="GET /setup.html", .value = HTTP_GET_SITE_SETUP},
		{.cmd="GET /control.html", .value = HTTP_GET_SITE_CONTROL},
		{.cmd="GET /index.html", .value = HTTP_GET_SITE_INDEX},
		{.cmd="GET /help.html", .value = HTTP_GET_SITE_HELP},
		{.cmd="GET /img/instrument.png", .value = HTTP_GET_IMG_INSTRUMENT},
		{.cmd="GET /img/favicon.png", .value = HTTP_GET_IMG_FAVICON},
		{.cmd="GET /service.html", .value = HTTP_GET_SITE_SERVICE}

};


bool http_load_page(struct netconn *conn, char* buf, u16_t buflen)
{
	size_t size = sizeof(http_index)/sizeof(http_index[0]);
	s32_t header = -1;
	struct fs_file file;
	bool status = false;

	header = http_header(buf, http_index, size);

	switch(header)
	{
		case HTTP_GET_SITE_INIT : fs_open(&file, "/index.html"); break;
		case HTTP_GET_SITE_TOP : fs_open(&file, "/top.html"); break;
		case HTTP_GET_SITE_HOME : fs_open(&file, "/home.html"); break;
		case HTTP_GET_SITE_SETUP : fs_open(&file, "/setup.html"); break;
		case HTTP_GET_SITE_CONTROL : fs_open(&file, "/control.html"); break;
		case HTTP_GET_SITE_INDEX : fs_open(&file, "/index.html"); break;
		case HTTP_GET_SITE_HELP : fs_open(&file, "/help.html"); break;
		case HTTP_GET_IMG_INSTRUMENT : fs_open(&file, "/img/instrument.png"); break;
		case HTTP_GET_IMG_FAVICON : fs_open(&file, "/img/favicon.png"); break;
		case HTTP_GET_SITE_SERVICE : fs_open(&file, "/service.html"); break;

		default: /* DO NOTHING */; break;
	}

	if(header >= 0)
	{
		netconn_write(conn, (const unsigned char*)(file.data), (size_t)file.len, NETCONN_NOCOPY);
		fs_close(&file);
		status = true;
	}

	return status;
}
