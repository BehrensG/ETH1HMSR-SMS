/*
 * SCPI_ADC.c
 *
 *  Created on: Apr 7, 2024
 *      Author: grzegorz
 */

#include <stdint.h>
#include <math.h>

#include "cmsis_os.h"

#include "GPIO.h"
#include "BSP.h"
#include "SCPI_ADC.h"
#include "ADC.h"

extern bsp_t bsp;
extern ADC_HandleTypeDef hadc3;
extern scpi_choice_def_t scpi_boolean_select[];


scpi_result_t SCPI_AdcConfigurationResolution(scpi_t * context)
{
	uint32_t value;

	if(!SCPI_ParamUInt32(context, &value, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(!ADC_CheckResolution(value))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		return SCPI_RES_ERR;
	}

	bsp.adc.bits = (uint8_t)value;
	bsp.adc.resolution = ADC_VREF/pow(2,(double)bsp.adc.bits);
	hadc3.Init.Resolution = ADC_SelectResolution((uint8_t)value);

	ADC_Reset(bsp.adc.sampling_time);
	ADC_AutoCalibration();

	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationResolutionQ(scpi_t * context)
{
	SCPI_ResultUInt8(context, bsp.adc.bits);

	return SCPI_RES_OK;
}

static uint32_t SCPI_FloatToSamplingTime(float value)
{
	uint32_t select = (uint32_t)(value * 10);
	switch (select)
	{
		case 15 : return ADC_SAMPLETIME_1CYCLE_5; break;
		case 25 : return ADC_SAMPLETIME_2CYCLES_5; break;
		case 85 : return ADC_SAMPLETIME_8CYCLES_5; break;
		case 165 : return ADC_SAMPLETIME_16CYCLES_5; break;
		case 325 : return ADC_SAMPLETIME_32CYCLES_5; break;
		case 645 : return ADC_SAMPLETIME_64CYCLES_5; break;
		case 3875 : return ADC_SAMPLETIME_387CYCLES_5; break;
		case 8105 : return ADC_SAMPLETIME_810CYCLES_5; break;
		default : return UINT32_MAX; break;
	}

	return UINT32_MAX;
}

scpi_result_t SCPI_AdcConfigurationSamplingTimeCycle(scpi_t * context)
{
	float value;
	uint32_t sampling_time;

	if (!SCPI_ParamFloat(context, &value, TRUE))
	{
		return SCPI_RES_ERR;
	}

	sampling_time = SCPI_FloatToSamplingTime(value);

	if (UINT32_MAX == sampling_time)
	{
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		return SCPI_RES_ERR;
	}

	bsp.adc.sampling_time = sampling_time;
	bsp.adc.cycles = value;
	ADC_Reset(sampling_time);
	ADC_AutoCalibration();

	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationSamplingTimeCycleQ(scpi_t * context)
{
	SCPI_ResultFloat(context, bsp.adc.cycles);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationOverSamplingEnable(scpi_t * context)
{
	int32_t value;
	if (!SCPI_ParamChoice(context, scpi_boolean_select, &value, TRUE))
	{
		return SCPI_RES_ERR;
	}

	bsp.adc.oversampling.enable = (bool)value;

	ADC_ConfigureOverSampling(bsp.adc.oversampling.enable, bsp.adc.oversampling.ratio);
	ADC_Reset(bsp.adc.sampling_time);
	ADC_AutoCalibration();

	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationOverSamplingEnableQ(scpi_t * context)
{
	SCPI_ResultBool(context, (scpi_bool_t)bsp.adc.oversampling.enable);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationOverSamplingRatio(scpi_t * context)
{
	uint32_t value;

	if (!SCPI_ParamUInt32(context, &value, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (!ADC_CheckOverSamplingRation(value))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		return SCPI_RES_ERR;
	}


	if (bsp.adc.oversampling.enable)
	{
		if (bsp.adc.oversampling.ratio != value)
		{
			ADC_ConfigureOverSampling(bsp.adc.oversampling.enable, bsp.adc.oversampling.ratio);
			ADC_Reset(bsp.adc.sampling_time);
			ADC_AutoCalibration();
		}

	}

	bsp.adc.oversampling.ratio = (uint16_t)value;

	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationOverSamplingRatioQ(scpi_t * context)
{
	SCPI_ResultUInt16(context, bsp.adc.oversampling.ratio);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationGain(scpi_t * context)
{
	uint32_t gain;

	if(!SCPI_ParamUInt32(context, &gain, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(!ADC_CheckGain(gain))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		return SCPI_RES_ERR;
	}

	bsp.adc.gain.value = (uint8_t)gain;
	bsp.adc.gain.index = ADC_GainIndex((uint8_t)gain);
	GPIO_SelectGain((uint8_t)gain);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationGainQ(scpi_t * context)
{
	SCPI_ResultUInt8(context, bsp.adc.gain.value);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationSampleCount(scpi_t * context)
{
	scpi_number_t sample_count;

	if(!SCPI_ParamNumber(context, scpi_special_numbers_def, &sample_count, TRUE))
	{
		return SCPI_RES_ERR;
	}

		if(sample_count.special){
			switch(sample_count.content.tag)
			{
				case SCPI_NUM_MIN: bsp.adc.sample_count = 1; break;
				case SCPI_NUM_MAX: bsp.adc.sample_count = ADC_MEASUREMENT_BUFFER; break;
				case SCPI_NUM_DEF: bsp.adc.sample_count = ADC_DEF_SIZE; break;
				default: SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE); return SCPI_RES_ERR;
			}
		}
		else{

			if ((sample_count.content.value > ADC_MEASUREMENT_BUFFER) || (sample_count.content.value < 1))
			{
				SCPI_ErrorPush(context, SCPI_ERROR_DATA_OUT_OF_RANGE);
				return SCPI_RES_ERR;
			}

			bsp.adc.sample_count = sample_count.content.value;

		}
	return SCPI_RES_OK;
}

scpi_result_t SCPI_AdcConfigurationSampleCountQ(scpi_t * context)
{
	SCPI_ResultUInt32(context, bsp.adc.sample_count);

	return SCPI_RES_OK;
}
