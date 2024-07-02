/*
 * UDP.c
 *
 *  Created on: Apr 21, 2024
 *      Author: grzegorz
 */

#include <stdbool.h>

#include "stddef.h"
#include "cmsis_os.h"
#include "err.h"
#include "ip_addr.h"
#include "udp.h"
#include "api.h"
#include "scpi/parser.h"
#include "utils.h"

#include "BSP.h"
#include "Utility.h"
#include "LED.h"
#include "ADC.h"

extern bsp_t bsp;
extern float measurements[];
extern SemaphoreHandle_t MeasMutex;

/*
osThreadId UDPTaskHandle;
uint32_t UDPTaskBuffer[1024];
osStaticThreadDef_t UDPTaskControlBlock;
*/

#define UDP_THREAD_STACKSIZE	1024

TaskHandle_t udp_handler;
uint32_t udp_buffer[UDP_THREAD_STACKSIZE];
StaticTask_t udp_control_block;

xQueueHandle QueueUDPHandle;

static struct netconn *conn;

volatile bool rcv_udp = false;

typedef enum {
	UDP_STATE_OK = 0,
	UDP_STATE_ERROR,
	UDP_STATE_CREATE,
	UDP_STATE_LISTEN,
	UDP_STATE_FETCHQ,
	UDP_STATE_MEASQ,
	UDP_STATE_INIT,
	UDP_STATE_CONNECT,
	UDP_STATE_IDLE

} udp_task_state_t;

typedef struct {
	const char *pattern;
	udp_task_state_t state;
} _scpi_udp_command_t;

#define UDP_CMD_LIST_END       {NULL, UDP_STATE_IDLE}

_scpi_udp_command_t udp_commands[] = {
		{ .pattern = "UDP:FETCh?", .state =	UDP_STATE_FETCHQ },
		{ .pattern = "UDP:INITiate[:IMMediate]", .state = UDP_STATE_INIT },
		{ .pattern = "UDP:READ?", .state = UDP_STATE_MEASQ },
		{ .pattern = "UDP:MEASure?", .state = UDP_STATE_MEASQ },
		{ .pattern = "UDP:R?", .state = UDP_STATE_MEASQ },
		UDP_CMD_LIST_END };

static void StartUDPTask(void* argument);

void UDP_CreateTask(void) {


	udp_handler = xTaskCreateStatic(StartUDPTask, "udp_Task",
			UDP_THREAD_STACKSIZE, (void*)1, tskIDLE_PRIORITY + 3,
			udp_buffer, &udp_control_block);
	/*
	osThreadStaticDef(UDPTask, StartUDPTask, osPriorityAboveNormal, 0,
			DEFAULT_THREAD_STACKSIZE, UDPTaskBuffer, &UDPTaskControlBlock);
	UDPTaskHandle = osThreadCreate(osThread(UDPTask), NULL);
*/
}

static udp_task_state_t UDP_Match(const char *data, uint16_t len) {
	udp_task_state_t state = UDP_STATE_ERROR;
	uint16_t cmd_len;
	int32_t ws_index;
	size_t array_size;
	char cmd[64];

	if (len >= 64) {
		return UDP_STATE_ERROR;
	}

	memcpy(cmd, data, len);

	cmd_len = len;
	ws_index = UTIL_WhiteSpace(cmd, cmd_len);
	array_size = sizeof(udp_commands) / sizeof(udp_commands[0]);

	if (-1 < ws_index) {
		cmd[ws_index] = '\0';
		cmd_len = strlen(cmd);
	}

	for (size_t x = 0; x < array_size; x++) {
		if (TRUE == SCPI_Match(udp_commands[x].pattern, cmd, cmd_len)) {
			state = udp_commands[x].state;
			break;
		}
	}

	return state;
}

void UDP_NetconnCallback(struct netconn *conn, enum netconn_evt even, u16_t len) {
	struct netbuf *buf;
	void *data;
	err_t status;
	uint16_t udp_len;
	static udp_task_state_t state;

	if (NETCONN_EVT_RCVPLUS == even) {

		status = netconn_recv(conn, &buf);

		if (ERR_OK != status) {
			netbuf_delete(buf);
		} else {

			bsp.udp_client.addr = netbuf_fromaddr(buf); // get the address of the client
			bsp.udp_client.port = netbuf_fromport(buf); // get the Port of the client

			netbuf_data(buf, &data, &udp_len);

			state = UDP_Match(data, udp_len);

			netbuf_delete(buf);

			xQueueSend(QueueUDPHandle, &state, portMAX_DELAY);
		}

	}

}

static err_t UDP_Create() {
	err_t err;
	conn = netconn_new_with_callback(NETCONN_UDP, UDP_NetconnCallback);
	err = netconn_bind(conn, IP_ADDR_ANY, bsp.scpi_raw.udp_port);

	return err;
}

static err_t UDP_Connect() {
	return netconn_connect(conn, bsp.udp_client.addr, bsp.udp_client.port);

}

static err_t UDP_Close() {

	err_t err;

	err = netconn_close(conn);
	err = netconn_delete(conn);

	return err;
}

#define UDP_PACKAGE_SIZE 20000
#define UDP_PACKAGE_MAX_SIZE (UDP_PACKAGE_SIZE + 10)

typedef enum {
	ADD_NONE, ADD_HEAD, ADD_TAIL, ADD_HEAD_AND_TAIL

} addon_t;

static err_t UDP_Send(char *data, uint32_t bytes_count, addon_t addon) {
	struct netbuf *buf;
	err_t err = ERR_OK;
	void *payload;
	char end[2] = { '\n', 0 };

	buf = netbuf_new();

	if (NULL != buf) {
		if (ADD_TAIL == addon) {
			payload = netbuf_alloc(buf, bytes_count + strlen(end));
		} else {
			payload = netbuf_alloc(buf, bytes_count);
		}

		if (NULL != payload) {
			memcpy(payload, data, bytes_count);

			if (ADD_TAIL == addon)
				memcpy(payload + bytes_count, end, strlen(end));

			//vTaskDelay(pdMS_TO_TICKS(1));
			err = netconn_send(conn, buf);

			netbuf_delete(buf);
		}
	}

	return err;
}

/****************** NEW TEST CODE START *********************/

typedef struct {
	char data[12];
	uint32_t len;

} udp_scpi_header_t;

udp_scpi_header_t UDP_ResultArbitraryBlockHeader(uint32_t len) {
	udp_scpi_header_t header;

	memset(&header.data[0], 0, 12);

	header.data[0] = '#';
	uint32_t tmp_len;
	SCPI_UInt32ToStrBase(len, header.data + 2, 10, 10);

	tmp_len = strlen(header.data + 2);

	header.data[1] = (char) (tmp_len + '0');
	header.len = strlen(header.data);

	return header;
}

static err_t UDP_SendV2(char *data, uint32_t bytes_count, addon_t addon) {
	struct netbuf *buf;
	err_t err = ERR_OK;
	void *payload;
	udp_scpi_header_t start;
	char end[2] = { '\n', 0 };

	uint32_t size = bsp.adc.sample_count * sizeof(float);

	buf = netbuf_new();

	if (NULL != buf) {

		start = UDP_ResultArbitraryBlockHeader(size);

		switch (addon) {
		case ADD_TAIL:
			payload = netbuf_alloc(buf, bytes_count + strlen(end));
			break;
		case ADD_HEAD:
			payload = netbuf_alloc(buf, bytes_count + start.len);
			break;
		case (ADD_HEAD_AND_TAIL):
			payload = netbuf_alloc(buf, bytes_count + start.len + strlen(end));
			break;
		default:
			payload = netbuf_alloc(buf, bytes_count);
			break;
		}

		if (NULL != payload) {

			switch (addon) {
			case ADD_TAIL:
				memcpy(payload + bytes_count, end, strlen(end));
				break;
			case ADD_HEAD: {
				memcpy(payload, start.data, start.len);
				memcpy(payload + start.len, data, bytes_count);

			}
				;
				break;
			case (ADD_HEAD_AND_TAIL): {
				memcpy(payload, start.data, start.len);
				memcpy(payload + start.len, data, bytes_count);
				memcpy(payload + start.len + bytes_count, end, strlen(end));

			}
				;
				break;
			default:
				memcpy(payload, data, bytes_count);
				break;
			}

			//vTaskDelay(pdMS_TO_TICKS(1));
			err = netconn_send(conn, buf);

			netbuf_delete(buf);
		}
	}

	return err;
}

static err_t UDP_SendREAL() {
	char *tx;
	uint32_t sample_count = bsp.adc.sample_count;
	uint32_t size, loop, rest, index;
	addon_t addon = ADD_NONE;
	err_t err;

	tx = UTIL_FloatArrayToREAL(NULL, NETCONN_UDP, &measurements[0], sample_count, NULL);
	size = sample_count * sizeof(float);
	loop = size / UDP_PACKAGE_SIZE;
	rest = size % UDP_PACKAGE_SIZE;
	index = 0;

	for (uint32_t x = 0; x < loop; x++) {
		if (0 == x) {
			addon += ADD_HEAD;

		} else if (!rest) {
			if ((loop - 1) == x)
				addon += ADD_TAIL;
		} else {
			addon = ADD_NONE;
		}

		err = UDP_SendV2(tx + index, UDP_PACKAGE_SIZE, addon);

		index += UDP_PACKAGE_SIZE;

		if (ERR_OK != err)
			break;
	}

	if (rest) {
		if (!loop) {
			addon = ADD_HEAD_AND_TAIL;
		} else {
			addon = ADD_TAIL;
		}
		err = UDP_SendV2(tx + index, rest, addon);
	}

	return err;
}

/****************** NEW TEST CODE END *********************/

static err_t UDP_SendASCII() {
	char *tx;
	uint32_t sample_count = bsp.adc.sample_count;
	uint32_t size, loop, rest, index;
	addon_t addon = ADD_NONE;
	err_t err;

	tx = UTIL_FloatArrayToASCII(NULL, NETCONN_UDP, &measurements[0], sample_count, NULL);
	size = strlen(tx);
	loop = size / UDP_PACKAGE_SIZE;
	rest = size % UDP_PACKAGE_SIZE;
	index = 0;

	for (uint32_t x = 0; x < loop; x++) {
		if (!rest) {
			if ((loop - 1) == x)
				addon = ADD_TAIL;
		}

		err = UDP_Send(tx + index, UDP_PACKAGE_SIZE, addon);

		index += UDP_PACKAGE_SIZE;

		if (ERR_OK != err)
			break;
	}

	if (rest) {
		err = UDP_Send(tx + index, rest, ADD_TAIL);
	}

	return err;
}

static void UDP_INITiate(void) {
	if (!ADC_Measurement(bsp.adc.sample_count)) {
		LED_osQueue(RED);
	} else {
		(bsp.default_cfg) ? LED_osQueue(BLUE) : LED_osQueue(GREEN);
	}
}

static bool UDP_Measure(udp_task_state_t state) {
	LED_osQueue(BLUE);

	if (UDP_STATE_FETCHQ != state) {
		if (!ADC_Measurement(bsp.adc.sample_count)) {
			LED_osQueue(RED);
			return false;
		}
	}

	if (ERR_OK == UDP_Connect()) {
		if (FORMAT_DATA_ASCII == bsp.format.data) {
			if (ERR_OK != UDP_SendASCII()) {
				LED_osQueue(RED);
			} else {
				(bsp.default_cfg) ? LED_osQueue(BLUE) : LED_osQueue(GREEN);
			}
		} else {

			if (ERR_OK != UDP_SendREAL()) {
				LED_osQueue(RED);
			} else {
				(bsp.default_cfg) ? LED_osQueue(BLUE) : LED_osQueue(GREEN);
			}
		}
	}

	if (ERR_OK != UDP_Close()) {
		LED_osQueue(RED);
		return false;
	}

	if (ERR_OK != UDP_Create()) {
		LED_osQueue(RED);
		return false;
	} else {
		(bsp.default_cfg) ? LED_osQueue(BLUE) : LED_osQueue(GREEN);
	}

	return true;
}

static void StartUDPTask(void* argument) {

	udp_task_state_t state = UDP_STATE_IDLE;

	QueueUDPHandle = xQueueCreate(1, sizeof(udp_task_state_t));

	UDP_Create();

	for (;;) {
		if (pdTRUE == xQueueReceive(QueueUDPHandle, &state, portMAX_DELAY)) {

		} else {
			state = UDP_STATE_IDLE;
		}

		switch (state) {
		case UDP_STATE_FETCHQ: {
			if (pdTRUE == xSemaphoreTake(MeasMutex, pdMS_TO_TICKS(20000))) {
				UDP_Measure(UDP_STATE_FETCHQ);
				xSemaphoreGive(MeasMutex);
			} else {
				LED_osQueue(RED);
			}
		}
			;
			break;
		case UDP_STATE_MEASQ: {
			if (pdTRUE == xSemaphoreTake(MeasMutex, pdMS_TO_TICKS(20000))) {
				UDP_Measure(UDP_STATE_MEASQ);
				xSemaphoreGive(MeasMutex);
			} else {
				LED_osQueue(RED);
			}
		}
			;
			break;
		case UDP_STATE_INIT: {
			if (pdTRUE == xSemaphoreTake(MeasMutex, pdMS_TO_TICKS(20000))) {
				UDP_INITiate();
				xSemaphoreGive(MeasMutex);
			} else {
				LED_osQueue(RED);
			}
		}
			;
			break;
		case UDP_STATE_ERROR:
			LED_osQueue(RED);
			break;
		case UDP_STATE_IDLE: vTaskDelay(pdMS_TO_TICKS(1)); break;
		default: vTaskDelay(pdMS_TO_TICKS(1)); break;
		}
	}
}
