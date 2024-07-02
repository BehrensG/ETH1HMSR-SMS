/*
 * HiSLIP.h
 *
 *  Created on: Jun 3, 2024
 *      Author: grzegorz
 */

#ifndef HISLIP_INC_HISLIP_H_
#define HISLIP_INC_HISLIP_H_

#include "arch.h"
#include "ip_addr.h"
#include "err.h"

#define HISLIP_INITIALIZE									(uint8_t)0
#define HISLIP_INITIALIZE_RESPONSE							(uint8_t)1
#define	HISLIP_FATAL_ERROR									(uint8_t)2
#define	HISLIP_ERROR										(uint8_t)3
#define HISLIP_ASYN_CLOCK									(uint8_t)4
#define HISLIP_ASYN_CLOCK_RESPONSE							(uint8_t)5
#define HISLIP_DATA											(uint8_t)6
#define HISLIP_DATAEND										(uint8_t)7
#define HISLIP_DEVICE_CLEAR_COMPLETE						(uint8_t)8
#define HISLIP_DEVICE_CLEAR_ACKNOWLEDGE						(uint8_t)9
#define HISLIP_ASYNC_REMOTE_LOCAL_CONTROL					(uint8_t)10
#define HISLIP_ASYNC_REMOTE_LOCAL_RESPONSE					(uint8_t)11
#define HISLIP_TRIGGER										(uint8_t)12
#define HISLIP_INTERRUPTED									(uint8_t)13
#define HISLIP_ASYNC_INTERRUPTED							(uint8_t)14
#define HISLIP_ASYNC_MAXIMUM_MESSAGE_SIZE					(uint8_t)15
#define HISLIP_ASYNC_MAXIMUM_MESSAGE_SIZE_RESPONSE			(uint8_t)16
#define HISLIP_ASYNC_INITIALIZE								(uint8_t)17
#define HISLIP_ASYNC_INITIALIZE_RESPONSE					(uint8_t)18
#define HISLIP_ASYNC_DEVICE_CLEAR							(uint8_t)19
#define HISLIP_ASYNC_SERVICE_REQUEST						(uint8_t)20
#define HISLIP_ASYNC_STATUS_QUERY							(uint8_t)21
#define HISLIP_ASYNC_STATUS_RESPONSE						(uint8_t)22
#define HISLIP_ASYNC_DEVICE_CLEAR_ACKNOWLEDGE				(uint8_t)23
#define HISLIP_ASYNC_LOCK_INFO								(uint8_t)24
#define HISLIP_ASYNC_LOCK_INFO_RESPONSE						(uint8_t)25
#define HISLIP_GET_DESCRIPTORS								(uint8_t)26
#define HISLIP_GET_DESCRIPTORS_RESPONSE						(uint8_t)27
#define HISLIP_START_TLS									(uint8_t)28
#define HISLIP_ASYNC_START_TLS								(uint8_t)29
#define HISLIP_ASYNC_START_TLS_RESPONSE						(uint8_t)30
#define HISLIP_END_TLS										(uint8_t)31
#define HISLIP_ASYNC_END_TLS								(uint8_t)32
#define HISLIP_ASYNC_END_TLS_RESPONSE						(uint8_t)33
#define HISLIP_GET_SASL_MECHANISM_LIST						(uint8_t)34
#define HISLIP_GET_SASL_MECHANISM_LIST_RESPONSE				(uint8_t)35
#define HISLIP_AUTHENTICATION_START							(uint8_t)36
#define HISLIP_AUTHENTICATION_EXCHANGE						(uint8_t)37
#define HISLIP_AUTHENTICATION_RESULT						(uint8_t)38

#define HISLIP_PROLOGUE									0x4853
// TBD
#define HISLIP_VENDOR_ID								0x1111
#define HISLIP_MAX_DATA_SIZE							102400
#define HISLIP_BUFFER_SIZE								1024

#define HISLIP_LINE_ENDING								"\n"

typedef enum
{
	Initialize = HISLIP_INITIALIZE,
	InitializeResponse = HISLIP_INITIALIZE_RESPONSE,
	FatalError = HISLIP_FATAL_ERROR,
	Error = HISLIP_ERROR,
	AsyncLock = HISLIP_ASYN_CLOCK,
	AsyncLockResponse = HISLIP_ASYN_CLOCK_RESPONSE,
	Data = HISLIP_DATA,
	DataEnd = HISLIP_DATAEND,
	DeviceClearComplete = HISLIP_DEVICE_CLEAR_COMPLETE,
	DeviceClearAcknowledge = HISLIP_DEVICE_CLEAR_ACKNOWLEDGE,
	AsyncRemoteLocalControl = HISLIP_ASYNC_REMOTE_LOCAL_CONTROL,
	AsyncRemoteLocalResponse = HISLIP_ASYNC_REMOTE_LOCAL_RESPONSE,
	Trigger = HISLIP_TRIGGER,
	Interrupted = HISLIP_INTERRUPTED,
	AsyncInterrupted = HISLIP_ASYNC_INTERRUPTED,
	AsyncMaximumMessageSize = HISLIP_ASYNC_MAXIMUM_MESSAGE_SIZE,
	AsyncMaximumMessageSizeResponse = HISLIP_ASYNC_MAXIMUM_MESSAGE_SIZE_RESPONSE,
	AsyncInitialize = HISLIP_ASYNC_INITIALIZE,
	AsyncInitializeResponse = HISLIP_ASYNC_INITIALIZE_RESPONSE,
	AsyncDeviceClear = HISLIP_ASYNC_DEVICE_CLEAR,
	AsyncServiceRequest = HISLIP_ASYNC_SERVICE_REQUEST,
	AsyncStatusQuery = HISLIP_ASYNC_STATUS_QUERY,
	AsyncStatusResponse = HISLIP_ASYNC_STATUS_RESPONSE,
	AsyncDeviceClearAcknowledge = HISLIP_ASYNC_DEVICE_CLEAR_ACKNOWLEDGE,
	AsyncLockInfo = HISLIP_ASYNC_LOCK_INFO,
	AsyncLockInfoResponse = HISLIP_ASYNC_LOCK_INFO_RESPONSE,
	GetDescriptors = HISLIP_GET_DESCRIPTORS,
	GetDescriptorsResponse = HISLIP_GET_DESCRIPTORS_RESPONSE,
	StartTLS = HISLIP_START_TLS,
	AsyncStartTLS = HISLIP_ASYNC_START_TLS,
	AsyncStartTLSResponse = HISLIP_ASYNC_START_TLS_RESPONSE,
	EndTLS = HISLIP_END_TLS,
	AsyncEndTLS = HISLIP_ASYNC_END_TLS,
	AsyncEndTLSResponse = HISLIP_ASYNC_END_TLS_RESPONSE,
	GetSaslMechanismList = HISLIP_GET_SASL_MECHANISM_LIST,
	GetSaslMechanismListResponse = HISLIP_GET_SASL_MECHANISM_LIST_RESPONSE,
	AuthenticationStart = HISLIP_AUTHENTICATION_START,
	AuthenticationExchange = HISLIP_AUTHENTICATION_EXCHANGE,
	AuthenticationResult = HISLIP_AUTHENTICATION_RESULT,
	HISLIP_CONN_ERR = 256

}hislip_msg_type_t;


typedef enum {

	FERR_UNIDENTIFIED_ERROR,
	FERR_POORLY_FORMED_MSG_HEADER,
	FEER_CONNECTION_WITHOUT_INIT,
	FEER_INVALID_INIT_SEQ,
	FEER_MAX_NUM_OF_CLIENTS,
	FEER_SECURE_CONNECTION_FAILED
}hislip_fatal_error_t;


typedef enum {

	ERR_UNIDENTIFIED_ERROR,
	ERR_UNRECOGNIZED_MSG_TYPE,
	ERR_UNRECOGNIZED_CONTROL_CODE,
	ERR_UNRECOGNIZED_VENDOR_MSG,
	ERR_MSG_TOO_LARGE,
	ERR_AUTH_FAILED

}hislip_error_t;

#pragma pack(push, 1)

typedef struct
{
	uint32_t hi;
	uint32_t lo;
}payload_len_t;

typedef struct
{
	uint16_t prologue;
	uint8_t msg_type;
	uint8_t control_code;
	uint32_t msg_param;
	payload_len_t payload_len;
}hislip_msg_t;

#pragma pack(pop)


typedef struct {
	struct netconn* newconn;
}hislip_netconn_t;

typedef struct {
	char data[sizeof(hislip_msg_t) + HISLIP_BUFFER_SIZE];
	uint16_t len;
}hislip_netbuf_t;


typedef struct {
	hislip_msg_t msg;
	hislip_netbuf_t netbuf;
	hislip_netconn_t netconn;
	uint16_t session_id;
	char end[2];
}hislip_instr_t;



// SUPPORT Functions

void hislip_Init(hislip_instr_t* hislip_instr);
void hislip_htonl(hislip_msg_t* hislip_msg);
size_t hislip_SumSize(size_t* sizes, size_t len);
void hislip_CopyMemory(char* destination, void** sources, size_t* sizes, uint32_t num_sources);
hislip_msg_t hislip_MsgParser(hislip_instr_t* hislip_instr);
void hislip_DataHeader(hislip_instr_t* hislip_instr, hislip_msg_t* msg, uint8_t msg_type, uint32_t size);

// SYNC Commands

int8_t hislip_InitializeResponse(hislip_instr_t* hislip_instr);
int8_t hislip_DataEnd(hislip_instr_t* hislip_instr);
int8_t hislip_Data(hislip_instr_t* hislip_instr);
int8_t hislip_Trigger(hislip_instr_t* hislip_instr);
int8_t hislip_DeviceClearAcknowledge(hislip_instr_t* hislip_instr);
int8_t hislip_Interrupted(hislip_instr_t* hislip_instr);

// ASYNC Commands

int8_t hislip_AsyncMaximumMessageSizeResponse(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncInitializeResponse(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncStatusQuery(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncDeviceClearAcknowledge(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncLockResponse(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncLockInfoResponse(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncRemoteLocalResponse(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncInterrupted(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncServiceRequest(hislip_instr_t* hislip_instr);
int8_t hislip_AsyncStatusResponse(hislip_instr_t* hislip_instr);


// EITHER Sync or aSync

int8_t hislip_Error(hislip_instr_t* hislip_instr);
int8_t hislip_FatalError(hislip_instr_t* hislip_instr);
int8_t hislip_GetDescriptorsResponse(hislip_instr_t* hislip_instr);

#endif /* HISLIP_INC_HISLIP_H_ */
