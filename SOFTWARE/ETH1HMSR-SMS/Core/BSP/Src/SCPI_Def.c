/*-
 * Copyright (c) 2012-2013 Jan Breuer,
 *
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   scpi-def.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 *
 * @brief  SCPI parser test
 *
 *
 */

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmsis_os.h>


#include "main.h"
#include "scpi/scpi.h"

#include "SCPI_Def.h"
#include "SCPI_System.h"
#include "SCPI_ADC.h"
#include "SCPI_Measure.h"
#include "SCPI_Calibration.h"
#include "SCPI_Format.h"
#include "SCPI_Calculate.h"
#include "printf.h"
#include "FloatToString.h"
#include "BSP.h"
#include "GPIO.h"
#include "ADC.h"

extern bsp_t bsp;

scpi_choice_def_t scpi_boolean_select[] =
{
    {"OFF", 0},
    {"ON", 1},
	{"0", 0},
	{"1", 1},
    SCPI_CHOICE_LIST_END
};



static scpi_result_t SCPI_Rst(scpi_t * context)
{
	// DO NOT MAKE A HARD RESET !
	BSP_Init();
	ADC_InitMemory();
	ADC_Reset(bsp.adc.sampling_time);
	ADC_AutoCalibration();
	GPIO_SelectGain(bsp.adc.gain.value);
	return SCPI_RES_OK;
}

static scpi_result_t SCPI_IdnQ(scpi_t * context)
{
	int32_t ptr = 0;
	char info[46];
	memset(info, 0, 46);

    for (uint8_t i = 0; i < 4; i++)
    {
        if (context->idn[i])
        {
        	if(3 == i)
        	{
        		ptr += snprintf(info + ptr, sizeof(info) - ptr, "%s", context->idn[i] );
        	}
        	else
        	{
        		ptr += snprintf(info + ptr, sizeof(info) - ptr, "%s,", context->idn[i] );
        	}

        }
    }

    SCPI_ResultCharacters(context, info, strlen(info));
    return SCPI_RES_OK;
}


#define TEX_SIZE	256

scpi_result_t SCPI_SystemErrorQ(scpi_t * context)
{
	char text[TEX_SIZE];
	const char * data;
	scpi_error_t error;
	int len;

	SCPI_ErrorPop(context, &error);
	data = SCPI_ErrorTranslate(error.error_code);
	len = snprintf(text, TEX_SIZE,"%d, %s",error.error_code, data);

	SCPI_ResultCharacters(context, text, len);

	return SCPI_RES_OK;
}


extern bsp_t bsp;

scpi_result_t SCPI_TS(scpi_t * context)
{
    return SCPI_RES_OK;
}

const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    { .pattern = "*CLS", .callback = SCPI_CoreCls,},
    { .pattern = "*ESE", .callback = SCPI_CoreEse,},
    { .pattern = "*ESE?", .callback = SCPI_CoreEseQ,},
    { .pattern = "*ESR?", .callback = SCPI_CoreEsrQ,},
    { .pattern = "*IDN?", .callback = SCPI_IdnQ,},
    { .pattern = "*OPC", .callback = SCPI_CoreOpc,},
    { .pattern = "*OPC?", .callback = SCPI_CoreOpcQ,},
    { .pattern = "*RST", .callback = SCPI_Rst,},
    { .pattern = "*SRE", .callback = SCPI_CoreSre,},
    { .pattern = "*SRE?", .callback = SCPI_CoreSreQ,},
    { .pattern = "*STB?", .callback = SCPI_CoreStbQ,},
    { .pattern = "*TST?", .callback = SCPI_CoreTstQ,},
    { .pattern = "*WAI", .callback = SCPI_CoreWai,},

    {.pattern = "STATus:QUEStionable[:EVENt]?", .callback = SCPI_StatusQuestionableEventQ,},
    /* {.pattern = "STATus:QUEStionable:CONDition?", .callback = scpi_stub_callback,}, */
    {.pattern = "STATus:QUEStionable:ENABle", .callback = SCPI_StatusQuestionableEnable,},
    {.pattern = "STATus:QUEStionable:ENABle?", .callback = SCPI_StatusQuestionableEnableQ,},

    {.pattern = "STATus:PRESet", .callback = SCPI_StatusPreset,},

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {.pattern = "SYSTem:ERRor[:NEXT]?", .callback = SCPI_SystemErrorQ,},
    {.pattern = "SYSTem:ERRor:COUNt?", .callback = SCPI_SystemErrorCountQ,},
    {.pattern = "SYSTem:VERSion?", .callback = SCPI_SystemVersionQ,},

	{.pattern = "SYSTem:COMMunicate:LAN:IPADdress", .callback = SCPI_SystemCommunicateLanIpAddress,},
	{.pattern = "SYSTem:COMMunicate:LAN:IPADdress?", .callback = SCPI_SystemCommunicateLanIpAddressQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:SMASk", .callback = SCPI_SystemCommunicateLanIpSmask,},
	{.pattern = "SYSTem:COMMunicate:LAN:SMASk?", .callback = SCPI_SystemCommunicateLanIpSmaskQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:GATEway", .callback = SCPI_SystemCommunicateLanGateway,},
	{.pattern = "SYSTem:COMMunicate:LAN:GATEway?", .callback = SCPI_SystemCommunicateLanGatewayQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:MAC", .callback = SCPI_SystemCommunicateLanMac,},
	{.pattern = "SYSTem:COMMunicate:LAN:MAC?", .callback = SCPI_SystemCommunicateLanMacQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:UPDate", .callback = SCPI_SystemCommunicationLanUpdate,},
	{.pattern = "SYSTem:SECure:STATe", .callback = SCPI_SystemSecureState,},
	{.pattern = "SYSTem:SECure:STATe?", .callback = SCPI_SystemSecureStateQ,},
	{.pattern = "SYSTem:SERVice:MDNS[:ENAble]", .callback = SCPI_SystemServiceMDNSEnable,},
	{.pattern = "SYSTem:SERVice:MDNS[:ENAble]?", .callback = SCPI_SystemServiceMDNSEnableQ,},
	{.pattern = "SYSTem:SERVice:HISLIP[:ENAble]", .callback = SCPI_SystemServiceHISLIPEnable,},
	{.pattern = "SYSTem:SERVice:HISLIP[:ENAble]?", .callback = SCPI_SystemServiceHISLIPEnableQ,},
	{.pattern = "SYSTem:SERVice:EEPROM", .callback = SCPI_SystemServiceEeprom,},
	{.pattern = "SYSTem:SERVice:LED[:ENAble]", .callback = SCPI_SystemServiceLEDEnable,},
	{.pattern = "SYSTem:SERVice:LED[:ENAble]?", .callback = SCPI_SystemServiceLEDEnableQ,},
	{.pattern = "SYSTem:SERVice:LED:PING", .callback = SCPI_SystemServiceLEDPing,},
	{.pattern = "SYSTem:SERVice:RESET", .callback = SCPI_SystemServiceReset,},

	//{.pattern = "TS", .callback = SCPI_TS,},


	{.pattern = "CONFiguration:RESOLution", .callback = SCPI_AdcConfigurationResolution,},
	{.pattern = "CONFiguration:RESOLution?", .callback = SCPI_AdcConfigurationResolutionQ,},
	{.pattern = "CONFiguration:SAMPling:TIME[:CYCLe]", .callback = SCPI_AdcConfigurationSamplingTimeCycle,},
	{.pattern = "CONFiguration:SAMPling:TIME[:CYCLe]?", .callback = SCPI_AdcConfigurationSamplingTimeCycleQ,},
	{.pattern = "CONFiguration:OVERSAMPling[:ENAble]", .callback = SCPI_AdcConfigurationOverSamplingEnable,},
	{.pattern = "CONFiguration:OVERSAMPling[:ENAble]?", .callback = SCPI_AdcConfigurationOverSamplingEnableQ,},
	{.pattern = "CONFiguration:OVERSAMPling:RATIO", .callback = SCPI_AdcConfigurationOverSamplingRatio,},
	{.pattern = "CONFiguration:OVERSAMPling:RATIO?", .callback = SCPI_AdcConfigurationOverSamplingRatioQ,},

	{.pattern = "CONFiguration:GAIN", .callback = SCPI_AdcConfigurationGain,},
	{.pattern = "CONFiguration:GAIN?", .callback = SCPI_AdcConfigurationGainQ,},
	{.pattern = "SAMPle:COUNt", .callback = SCPI_AdcConfigurationSampleCount,},
	{.pattern = "SAMPle:COUNt?", .callback = SCPI_AdcConfigurationSampleCountQ,},

	{.pattern = "MEASure?", .callback = SCPI_MeasureQ,},
	{.pattern = "READ?", .callback = SCPI_MeasureQ,},
	{.pattern = "R?", .callback = SCPI_MeasureQ,},
	{.pattern = "FETCh?", .callback = SCPI_FetchQ,},
	{.pattern = "DATA[:DATA]?", .callback = SCPI_DataDataQ,},

	{.pattern = "INITiate[:IMMediate]", .callback = SCPI_Initiate,},
	{.pattern = "*TRG", .callback = SCPI_Initiate,},

	{.pattern = "ADC:NULL:OFFSet:ENAble", .callback = SCPI_NullOffsetEnable,},
	{.pattern = "ADC:NULL:OFFSet:ENAble?", .callback = SCPI_NullOffsetEnableQ,},
	{.pattern = "ADC:NULL:OFFSet", .callback = SCPI_NullOffset,},
	{.pattern = "ADC:NULL:OFFSet?", .callback = SCPI_NullOffsetQ,},

	{.pattern = "CALibration:COUNt?", .callback = SCPI_CalibrationCountQ,},
	{.pattern = "CALibration:STORe", .callback = SCPI_CalibrationStore,},
	{.pattern = "CALibration:VALue", .callback = SCPI_CalibrationValue,},
	{.pattern = "CALibration:VALue?", .callback = SCPI_CalibrationValueQ,},
	{.pattern = "CALIBration:ADC:AUTO[:MODE]", .callback = SCPI_CalibrationAdcAutoMode,},

	{.pattern = "FORMat[:DATA]", .callback = SCPI_FormatData,},
	{.pattern = "FORMat[:DATA]?", .callback = SCPI_FormatDataQ,},

	{.pattern = "CALCulate:AVERage?", .callback = SCPI_CalculateAverageQ,},
	{.pattern = "CALCulate:OFFSet:ENAble", .callback = SCPI_CalculateOffsetEnable,},
	{.pattern = "CALCulate:OFFSet:ENAble?", .callback = SCPI_CalculateOffsetEnableQ,},
	{.pattern = "CALCulate:OFFSet", .callback = SCPI_CalculateOffset,},
	{.pattern = "CALCulate:OFFSet?", .callback = SCPI_CalculateOffsetQ,},

	SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};

scpi_interface_t scpi_hislip_interface = {
    .error = SCPI_Error,
    .write = SCPI_WriteHiSLIP,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};


char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];

scpi_t scpi_context;
scpi_t scpi_hislip_context;
