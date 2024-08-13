/*
 * SCPI_ADC.c
 *
 *  Created on: Apr 7, 2024
 *      Author: grzegorz
 */

// --------------------------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <math.h>

#include "cmsis_os.h"

#include "GPIO.h"
#include "BSP.h"
#include "SCPI_ADC.h"
#include "ADC.h"

// --------------------------------------------------------------------------------------------------------------------

extern bsp_t bsp;
extern ADC_HandleTypeDef hadc3;
extern scpi_choice_def_t scpi_boolean_select[];

// --------------------------------------------------------------------------------------------------------------------

static float SCPI_CycleToPeriod(float cycle, uint8_t bits);


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

	bsp.adc.period = SCPI_CycleToPeriod(bsp.adc.cycles, bsp.adc.bits);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationResolutionQ(scpi_t * context)
{
	SCPI_ResultUInt8(context, bsp.adc.bits);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

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

// --------------------------------------------------------------------------------------------------------------------

static float periods[][8] = {{0.20, 0.22, 0.34, 0.50, 0.82, 1.46, 7.92, 16.38},
							{0.18, 0.20, 0.32, 0.48, 0.80, 1.44, 7.90, 16.36},
							{0.16, 0.18, 0.30, 0.46, 0.78, 1.42, 7.88, 16.34},
							{0.14, 0.16, 0.28, 0.44, 0.76, 1.40, 7.86, 16.32},
							{0.12, 0.14, 0.26, 0.42, 0.74, 1.38, 7.84, 16.30}};

static uint8_t resoluton_bits[] = {16, 14, 12, 10, 8};

static float SCPI_CycleToPeriod(float cycle, uint8_t bits)
{

	uint16_t cycle_u16 = (uint16_t)(cycle * 10);

	uint16_t cycles[] = {15, 25, 85, 165, 325, 645, 3875, 8105};

	uint8_t index_bits = 0, index_period = 0;


	for(uint8_t x = 0; x < 5; x++)
	{
		if(bits == resoluton_bits[x])
		{
			index_bits = x;
			break;
		}

	}

	for(uint8_t x = 0; x < 8; x++)
	{
		if(cycle_u16 == cycles[x])
		{
			index_period = x;
			break;
		}

	}

	return periods[index_bits][index_period];
}

static bool SCPI_CheckPeriodValue(float value, uint8_t bits)
{
	uint8_t index_bits = 0;

	for(uint8_t x = 0; x < 4; x++)
	{
		if(bits == resoluton_bits[x])
		{
			index_bits = x;
			break;
		}

	}

	for(uint8_t x = 0; x < 8; x++)
	{
		if(value == periods[index_bits][x])
		{
			return 1;
		}
	}

	return 0;
}

static float SCPI_PeriodToCycle(float period, uint8_t bits)
{
	uint8_t index_bits = 0, index_cycles = 0;

	float cycles[] = {1.5, 2.5, 8.5, 16.5, 32.5, 64.5, 387.5, 810.5};

	for(uint8_t x = 0; x < 4; x++)
	{
		if(bits == resoluton_bits[x])
		{
			index_bits = x;
			break;
		}

	}

	for(uint8_t x = 0; x < 8; x++)
	{
		if(period == periods[index_bits][x])
		{
			index_cycles = x;
			break;
		}
	}

	return cycles[index_cycles];
}

// --------------------------------------------------------------------------------------------------------------------

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
	bsp.adc.period = SCPI_CycleToPeriod(bsp.adc.cycles, bsp.adc.bits);
	ADC_Reset(sampling_time);
	ADC_AutoCalibration();

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationSamplingTimeCycleQ(scpi_t * context)
{
	SCPI_ResultFloat(context, bsp.adc.cycles);
	return SCPI_RES_OK;
}

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationSamplingTimePeriod(scpi_t * context)
{
	float value;
	uint32_t sampling_time;

	if (!SCPI_ParamFloat(context, &value, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(!SCPI_CheckPeriodValue(value, bsp.adc.bits))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		return SCPI_RES_ERR;
	}

	sampling_time = SCPI_FloatToSamplingTime(SCPI_PeriodToCycle(value, bsp.adc.bits));

	if (UINT32_MAX == sampling_time)
	{
		SCPI_ErrorPush(context, SCPI_ERROR_NUMERIC_DATA_NOT_ALLOWED);
		return SCPI_RES_ERR;
	}

	bsp.adc.sampling_time = sampling_time;
	bsp.adc.cycles = SCPI_PeriodToCycle(value, bsp.adc.bits);
	bsp.adc.period = value;

	ADC_Reset(sampling_time);
	ADC_AutoCalibration();

	return SCPI_RES_OK;
}

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationSamplingTimePeriodQ(scpi_t * context)
{
	SCPI_ResultFloat(context, bsp.adc.period);
	return SCPI_RES_OK;
}

// --------------------------------------------------------------------------------------------------------------------

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


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationOverSamplingEnableQ(scpi_t * context)
{
	SCPI_ResultBool(context, (scpi_bool_t)bsp.adc.oversampling.enable);
	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

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


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationOverSamplingRatioQ(scpi_t * context)
{
	SCPI_ResultUInt16(context, bsp.adc.oversampling.ratio);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

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


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationGainQ(scpi_t * context)
{
	SCPI_ResultUInt8(context, bsp.adc.gain.value);

	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

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


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationSampleCountQ(scpi_t * context)
{
	SCPI_ResultUInt32(context, bsp.adc.sample_count);

	return SCPI_RES_OK;
}
