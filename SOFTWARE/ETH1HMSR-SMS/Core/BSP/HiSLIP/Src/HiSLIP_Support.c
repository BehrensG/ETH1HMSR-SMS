/*
 * HiSLIP_Support.c
 *
 *  Created on: Jun 7, 2024
 *      Author: BehrensG
 */

#include <string.h>

#include "HiSLIP.h"



void hislip_Init(hislip_instr_t* hislip_instr)
{
	memset(hislip_instr->end, 0, sizeof(hislip_instr->end));
}


void hislip_htonl(hislip_msg_t* hislip_msg)
{
	hislip_msg->msg_param = htonl(hislip_msg->msg_param);
	hislip_msg->prologue = htons(hislip_msg->prologue);
	hislip_msg->payload_len.hi = htonl(hislip_msg->payload_len.hi);
	hislip_msg->payload_len.lo = htonl(hislip_msg->payload_len.lo);
}


size_t hislip_SumSize(size_t* sizes, size_t len)
{
	size_t sum = 0;

	for(u_char i = 0; i < len; i++)
	{
		sum += sizes[i];
	}

	return sum;
}

void hislip_CopyMemory(char* destination, void** sources, size_t* sizes, uint32_t num_sources)
{
    size_t offset = 0;
    for (uint32_t i = 0; i < num_sources; i++)
    {
        memcpy(destination + offset, sources[i], sizes[i]);
        offset += sizes[i];
    }
}


hislip_msg_t hislip_MsgParser(hislip_instr_t* hislip_instr)
{
	hislip_msg_t hislip_msg;


	size_t hislip_msg_size = sizeof(hislip_msg_t);

	memcpy(&hislip_msg, hislip_instr->netbuf.data, hislip_msg_size);


	hislip_htonl(&hislip_msg);

	return hislip_msg;
}


void hislip_DataHeader(hislip_instr_t* hislip_instr, hislip_msg_t* msg, uint8_t msg_type, uint32_t size)
{
	msg->prologue = HISLIP_PROLOGUE;
	msg->msg_type = msg_type;
	msg->control_code = hislip_instr->msg.control_code;
	msg->msg_param = hislip_instr->msg.msg_param;
	msg->payload_len.hi = 0;
	msg->payload_len.lo = size;

	hislip_htonl(msg);
}
