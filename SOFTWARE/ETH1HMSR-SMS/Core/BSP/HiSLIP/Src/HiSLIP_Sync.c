/*
 * HiSLIP_Sync.c
 *
 *  Created on: Jun 7, 2024
 *      Author: BehrensG
 */

#include "api.h"
#include "SCPI_Def.h"
#include "HiSLIP.h"
#include "BSP.h"
#include "LED.h"

extern bsp_t bsp;

int8_t hislip_Trigger(hislip_instr_t* hislip_instr)
{

	//hislip_msg_t msg_rx;

	char scpi_trigger[] = "*TRG";
	char scpi_data[6];

	memset(scpi_data, 0, sizeof(scpi_data));

	memcpy(scpi_data, scpi_trigger, strlen(scpi_trigger));
	memcpy(scpi_data + strlen(scpi_trigger), SCPI_LINE_ENDING, strlen(SCPI_LINE_ENDING));

	//msg_rx = hislip_MsgParser(hislip_instr);

	return SCPI_Input(&scpi_context, scpi_data, strlen(scpi_data));
}



int8_t hislip_Data(hislip_instr_t* hislip_instr)
{
	//TBD
	return 0;
}

#define LED_START	1
#define LED_STOP	2

static void hislip_LED(u8_t status)
{
	if(LED_START == status)
	{
		if(bsp.led)
		{
			LED_osQueue(BLUE);
		}
	}else if(LED_STOP == status)
	{
		if(scpi_hislip_context.cmd_error)
		{
			if(bsp.led)
				LED_osQueue(RED);
		}
		else
		{
			if(bsp.led)
			{
				(bsp.default_cfg) ? LED_osQueue(BLUE) : LED_osQueue(GREEN);
			}
		}
	}

}

int8_t hislip_DataEnd(hislip_instr_t* hislip_instr)
{

	hislip_msg_t msg_rx;
//	hislip_msg_t msg_tx;

	char* buf;
	char* end;
	u8_t index = 0;

	static const char* ends[3] = {LINE_ENDING_CR, LINE_ENDING_LF, LINE_ENDING_CRLF};

	msg_rx = hislip_MsgParser(hislip_instr);

	buf = &hislip_instr->netbuf.data[sizeof(hislip_msg_t)];

	for(u8_t i = 0; i < 3; i++)
	{
		end = strstr(buf, ends[i]);
		if(NULL != end)
		{
			memset(hislip_instr->end, 0, sizeof(hislip_instr->end));
			memcpy(hislip_instr->end,ends[i],strlen(ends[i]));
			index = i;
			break;
		}

	}

	if(NULL != end)
	{
		memcpy(end, SCPI_LINE_ENDING, strlen(SCPI_LINE_ENDING));
		msg_rx.payload_len.lo -= strlen(SCPI_LINE_ENDING);

		memset(&scpi_hislip_context.end[0], 0, sizeof(scpi_hislip_context.end));
		memcpy(&scpi_hislip_context.end[0], ends[index], strlen(ends[index]));

	}
	else
	{
		memcpy(buf + msg_rx.payload_len.lo, SCPI_LINE_ENDING, strlen(SCPI_LINE_ENDING));

		memset(&scpi_hislip_context.end[0], 0, sizeof(scpi_hislip_context.end));
		memcpy(&scpi_hislip_context.end[0], SCPI_LINE_ENDING, strlen(SCPI_LINE_ENDING));
	}



	memcpy(&hislip_instr->msg, &msg_rx, sizeof(msg_rx));

	bsp.resource = VISA_HISLIP;

	hislip_LED(LED_START);

	SCPI_Input(&scpi_hislip_context, buf, msg_rx.payload_len.lo + strlen(SCPI_LINE_ENDING));

	// TO BE DELETED ?
	/*
	if(NULL != strchr(buf,'?')) // is it a query
	{
		 SCPI_Input(&scpi_hislip_context, buf, msg_rx.payload_len.lo + strlen(SCPI_LINE_ENDING));
	}
	else
	{
		msg_tx.prologue = HISLIP_PROLOGUE;
		msg_tx.msg_type = HISLIP_DATAEND;
		msg_tx.control_code = 0x00;
		msg_tx.msg_param = msg_rx.msg_param;
		msg_tx.payload_len.hi = 0;
		msg_tx.payload_len.lo = 0;

		SCPI_Input(&scpi_context, buf, msg_rx.payload_len.lo + strlen(SCPI_LINE_ENDING));
		netconn_write(hislip_instr->netconn.newconn, &msg_tx, sizeof(hislip_msg_t), NETCONN_NOFLAG);
	}

*/

	hislip_LED(LED_STOP);

	return true;
}




int8_t hislip_InitializeResponse(hislip_instr_t* hislip_instr)
{
	int8_t err = ERR_OK;
	char name_ref[] = "hislip0";
	char name_rx[12];
	char* data;
	hislip_msg_t msg_rx, msg_tx;


	memset(name_rx, 0, 12);

	msg_rx = hislip_MsgParser(hislip_instr);

	data = &hislip_instr->netbuf.data[0];

	memcpy(name_rx, data + sizeof(hislip_msg_t), msg_rx.payload_len.lo);

	if(!strcmp(name_ref, name_rx))
	{
		msg_tx.prologue = HISLIP_PROLOGUE;
		msg_tx.msg_type = HISLIP_INITIALIZE_RESPONSE;
		msg_tx.control_code = 0x00;
		msg_tx.msg_param = 0x01000001;
		msg_tx.payload_len.hi = 0;
		msg_tx.payload_len.lo = 0;

		hislip_htonl(&msg_tx);

		vTaskDelay(pdMS_TO_TICKS(1));
		err = netconn_write(hislip_instr->netconn.newconn, &msg_tx, sizeof(hislip_msg_t), NETCONN_NOFLAG);
	}

	return err;
}

int8_t hislip_DeviceClearAcknowledge(hislip_instr_t* hislip_instr)
{
	int8_t err = ERR_OK;
	//hislip_msg_t msg_rx;
	hislip_msg_t msg_tx;

	//msg_rx = hislip_MsgParser(hislip_instr);

	//hislip_instr->session_id = msg_rx.msg_param;

	msg_tx.prologue = HISLIP_PROLOGUE;
	msg_tx.msg_type = HISLIP_DEVICE_CLEAR_ACKNOWLEDGE;
	msg_tx.control_code = 0x00;
	msg_tx.msg_param = 0x00000000;
	msg_tx.payload_len.hi = 0;
	msg_tx.payload_len.lo = 0;

	hislip_htonl(&msg_tx);

	vTaskDelay(pdMS_TO_TICKS(1));
	err = netconn_write(hislip_instr->netconn.newconn, &msg_tx, sizeof(hislip_msg_t), NETCONN_NOFLAG);

	return err;
}


int8_t hislip_Interrupted(hislip_instr_t* hislip_instr)
{
	int8_t err = ERR_OK;
	//TBD
	return err;
}
