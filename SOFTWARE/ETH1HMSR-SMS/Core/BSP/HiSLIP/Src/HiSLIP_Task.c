/*
 * HiSLIP.c
 *
 *  Created on: Jun 3, 2024
 *      Author: grzegorz
 */


#include <string.h>
#include <stdbool.h>
// #include "stddef.h"
#include "cmsis_os.h"
#include "udp.h"
#include "api.h"
#include "def.h"

#include "HiSLIP.h"
#include "SCPI_Def.h"
#include "BSP.h"

extern bsp_t bsp;

#define HISLIP_THREAD_STACKSIZE		1024
#define HISLIP_PORT					4880

#define	NETCONN_ACCEPT_ON			(u8_t)1
#define	NETCONN_ACCEPT_OFF			(u8_t)0


static	u8_t task_count = 0;



// ----------------------------------------------------------------------------

TaskHandle_t hislip_handler;
uint32_t hislip_buffer[HISLIP_THREAD_STACKSIZE];
StaticTask_t hislip_control_block;

TaskHandle_t hislip_sync_handler;
uint32_t hislip_sync_buffer[HISLIP_THREAD_STACKSIZE];
StaticTask_t hislip_sync_control_block;

TaskHandle_t hislip_async_handler;
uint32_t hislip_async_buffer[HISLIP_THREAD_STACKSIZE];
StaticTask_t hislip_async_control_block;


// ----------------------------------------------------------------------------

static hislip_msg_type_t hislip_Recv(hislip_instr_t* hislip_instr)
{
	int8_t err;
	struct netbuf* buf;
	void* data;
	uint16_t len = 0;
	uint16_t offset = 0;
	hislip_msg_t hislip_msg;

	memset(hislip_instr->netbuf.data, 0, sizeof(hislip_msg_t) + HISLIP_BUFFER_SIZE);
	hislip_instr->netbuf.len = 0;

#if LWIP_SO_RCVTIMEO == 1
	netconn_set_recvtimeout(hislip_instr->netconn.newconn, 100);
#endif

	err = netconn_recv(hislip_instr->netconn.newconn, &buf);

	if(ERR_OK == err)
	{
		do
		{
			netbuf_data(buf, &data, &len);
			hislip_instr->netbuf.len +=len;

			if(hislip_instr->netbuf.len <= sizeof(hislip_instr->netbuf.data))
			{
				memcpy(hislip_instr->netbuf.data + offset, data, len);

			}
			else
			{

			}
			offset += len;

		}
		while(netbuf_next(buf) >= 0);

		netbuf_delete(buf);

		hislip_msg = hislip_MsgParser(hislip_instr);
	}
	else
	{
		netbuf_delete(buf);

		netconn_close(hislip_instr->netconn.newconn);
		netconn_delete(hislip_instr->netconn.newconn);


		return HISLIP_CONN_ERR;
	}

	return (hislip_msg_type_t)hislip_msg.msg_type;

}

static struct netconn*  hislip_Bind(uint16_t port)
{

	struct netconn* conn;

	conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, IP_ADDR_ANY, port);
	netconn_listen(conn);

#if LWIP_SO_RCVTIMEO == 1
	netconn_set_recvtimeout(conn, 1000);
#endif

	return conn;
}

// ----------------------------------------------------------------------------

static void hislip_SyncTask(void  *arg)
{
	hislip_instr_t hislip_instr;
	hislip_Init(&hislip_instr);

	hislip_instr.netconn.newconn = (struct netconn*)arg;

    SCPI_Init(&scpi_hislip_context,
            scpi_commands,
            &scpi_hislip_interface,
            scpi_units_def,
            SCPI_IDN1, SCPI_IDN2, bsp.eeprom.structure.info.serial_number, SCPI_IDN4,
            scpi_input_buffer, SCPI_INPUT_BUFFER_LENGTH,
            scpi_error_queue_data, SCPI_ERROR_QUEUE_SIZE);

    scpi_hislip_context.user_context = (void*)&hislip_instr;

	for (;;)
	{
		switch(hislip_Recv(&hislip_instr))
		{
			case Initialize : hislip_InitializeResponse(&hislip_instr);break;
			case DataEnd : hislip_DataEnd(&hislip_instr); break;
			case Data : hislip_Data(&hislip_instr); break;
			case Trigger : hislip_Trigger(&hislip_instr); break;
			case GetDescriptors : hislip_GetDescriptorsResponse(&hislip_instr); break;
			case DeviceClearComplete : hislip_DeviceClearAcknowledge(&hislip_instr); break;

			case HISLIP_CONN_ERR :
				{
					if(task_count)
					{
						task_count--;
					}

					vTaskDelete(NULL);

				}; break;

			default : hislip_Error(&hislip_instr); break;
		}
		vTaskDelay(pdMS_TO_TICKS(2));
	}

}

static void hislip_aSyncTask(void  *arg)
{
	hislip_instr_t hislip_instr;
	hislip_Init(&hislip_instr);

	hislip_instr.netconn.newconn = (struct netconn*)arg;

	for (;;)
	{
		switch(hislip_Recv(&hislip_instr))
		{
			case AsyncInitialize : hislip_AsyncInitializeResponse(&hislip_instr); break;
			case AsyncMaximumMessageSize : hislip_AsyncMaximumMessageSizeResponse(&hislip_instr);break;
			case AsyncStatusQuery : hislip_AsyncStatusQuery(&hislip_instr); break;
			case AsyncDeviceClear : hislip_AsyncDeviceClearAcknowledge(&hislip_instr); break;
			case AsyncLock : hislip_AsyncLockResponse(&hislip_instr); break;
			case AsyncLockInfo : hislip_AsyncLockInfoResponse(&hislip_instr); break;
			case AsyncRemoteLocalControl : hislip_AsyncRemoteLocalResponse(&hislip_instr); break;

			case GetDescriptors : hislip_GetDescriptorsResponse(&hislip_instr); break;


			case HISLIP_CONN_ERR :
				{
					if(task_count)
					{
						task_count--;
					}

					vTaskDelete(NULL);

				}; break;

			default : hislip_Error(&hislip_instr);  break;
		}
		vTaskDelay(pdMS_TO_TICKS(2));
	}

}

static void hislip_ServerTask(void* argument)
{
	int8_t err;
	struct netconn* newconn;
	struct netconn* conn = hislip_Bind(HISLIP_PORT);

	for (;;)
	{
		err = netconn_accept(conn, &newconn);

		if(ERR_OK == err)
		{
			if(0 == task_count)
			{
				hislip_sync_handler = xTaskCreateStatic(hislip_SyncTask,"hislip_SyncTask",
						HISLIP_THREAD_STACKSIZE, (void*)newconn, tskIDLE_PRIORITY + 2,
						hislip_sync_buffer, &hislip_sync_control_block);

				task_count++;

			}

			else if(1 == task_count)
			{
				hislip_async_handler = xTaskCreateStatic(hislip_aSyncTask,"hislip_aSyncTask",
						HISLIP_THREAD_STACKSIZE, (void*)newconn, tskIDLE_PRIORITY + 2,
						hislip_async_buffer, &hislip_async_control_block);

				task_count++;
			}
		}

	}
}

void HISLIP_CreateTask(void)
{
	hislip_handler = xTaskCreateStatic(hislip_ServerTask, "hislip_Task",
			HISLIP_THREAD_STACKSIZE, (void*)1, tskIDLE_PRIORITY + 3,
			hislip_buffer, &hislip_control_block);
}
