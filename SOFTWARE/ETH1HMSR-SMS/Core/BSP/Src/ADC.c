/*
 * ADC.c
 *
 *  Created on: Apr 10, 2024
 *      Author: BehrensG
 */

#include <stdbool.h>
#include <Utility.h>

#include "cmsis_os.h"
#include "ADC.h"
#include "BSP.h"
#include "LED.h"

// --------------------------------------------------------------------------------------------------------------------

extern ADC_HandleTypeDef hadc3;
extern bsp_t bsp;

// --------------------------------------------------------------------------------------------------------------------

__attribute__ ((section(".MEAS_BUFF"), used)) float measurements[ADC_MEASUREMENT_BUFFER];

// --------------------------------------------------------------------------------------------------------------------

ALIGN_32BYTES (uint16_t adc_data[ADC_MEASUREMENT_BUFFER]);

// --------------------------------------------------------------------------------------------------------------------

volatile bool adc_convertion_done = false;


// --------------------------------------------------------------------------------------------------------------------

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes */
	SCB_InvalidateDCache_by_Addr((uint32_t *) &adc_data[0], ADC_MEASUREMENT_BUFFER);
}


// --------------------------------------------------------------------------------------------------------------------

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
	SCB_InvalidateDCache_by_Addr((uint32_t *) &adc_data[ADC_MEASUREMENT_BUFFER/2], ADC_MEASUREMENT_BUFFER);

	adc_convertion_done = true;
}


// --------------------------------------------------------------------------------------------------------------------

void ADC_AutoCalibration(void)
{
	if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET_LINEARITY, ADC_DIFFERENTIAL_ENDED) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_DIFFERENTIAL_ENDED) != HAL_OK)
	{
		Error_Handler();
	}
}


// --------------------------------------------------------------------------------------------------------------------

bool ADC_Sample(uint32_t sample_count)
{
	uint32_t start = HAL_GetTick();

	adc_convertion_done = false;

	if (HAL_OK != HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc_data, sample_count))
	{
		return false;
	}

	while(!adc_convertion_done)
	{
		if(!UTIL_Timeout(start, ADC_TIMEOUT_MAX))
		{
			return false;
		}
	}

	return true;
}


// --------------------------------------------------------------------------------------------------------------------

void ADC_SignalConditioning(uint8_t gain, uint32_t sample_count, float offset, float calib_gain, float math_offset)
{
	float adc = 0.0f;
	float inv_gain = 1.0f/(float)gain;

	for(uint32_t x = 0; x < sample_count; x++)
	{
		adc = inv_gain * (bsp.adc.resolution * adc_data[x] - bsp.adc.vcom) + offset;
		measurements[x] = calib_gain * bsp.hmsr_sms.resolution * bsp.hmsr_sms.signal_multiplier * adc + math_offset;
	}


}


// --------------------------------------------------------------------------------------------------------------------

void ADC_SignalConditioningZeroOffset(uint8_t gain, uint32_t sample_count)
{
	float inv_gain = 1.0f/(float)gain;

	for(uint32_t x = 0; x < sample_count; x++)
	{
		measurements[x] = inv_gain * (bsp.adc.resolution * adc_data[x] - bsp.adc.vcom);

	}
}


// --------------------------------------------------------------------------------------------------------------------

uint32_t ADC_SelectResolution(uint8_t value)
{
	switch (value)
	{
		case 8 : return ADC_RESOLUTION_8B;
		case 10 : return ADC_RESOLUTION_10B;
		case 12 : return ADC_RESOLUTION_12B;
		case 14 : return ADC_RESOLUTION_14B;
		case 16 : return ADC_RESOLUTION_16B;
		default : return ADC_RESOLUTION_16B;
	}

	return ADC_RESOLUTION_16B;
}


// --------------------------------------------------------------------------------------------------------------------

bool ADC_CheckOverSamplingRation(uint32_t value)
{
	uint32_t valid[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
	uint32_t size = sizeof(valid)/sizeof(valid[0]);

	for (uint8_t x = 0; x < size; x++)
	{
		if (value == valid[x])
			return true;
	}

	return false;
}


// --------------------------------------------------------------------------------------------------------------------

uint8_t ADC_GetRightShiftIndex(uint32_t value)
{
	uint32_t valid[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
	uint32_t size = sizeof(valid)/sizeof(valid[0]);

	for (uint8_t x = 0; x < size; x++)
	{
		if (value == valid[x])
			return x;
	}

	return 0;
}


// --------------------------------------------------------------------------------------------------------------------

uint32_t ADC_RightBitShift(uint32_t value)
{
	uint32_t right_bit_shift[] = {
			ADC_RIGHTBITSHIFT_NONE,
			ADC_RIGHTBITSHIFT_1,
			ADC_RIGHTBITSHIFT_2,
			ADC_RIGHTBITSHIFT_3,
			ADC_RIGHTBITSHIFT_4,
			ADC_RIGHTBITSHIFT_5,
			ADC_RIGHTBITSHIFT_6,
			ADC_RIGHTBITSHIFT_7,
			ADC_RIGHTBITSHIFT_8,
			ADC_RIGHTBITSHIFT_9,
			ADC_RIGHTBITSHIFT_10
			};

	return right_bit_shift[ADC_GetRightShiftIndex(value)];

}

// --------------------------------------------------------------------------------------------------------------------

void ADC_ConfigureOverSampling( FunctionalState enable, uint32_t ratio)
{
	hadc3.Init.OversamplingMode = enable;
	hadc3.Init.Oversampling.Ratio = ratio;
	hadc3.Init.Oversampling.RightBitShift = ADC_RightBitShift(ratio);
	hadc3.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	hadc3.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;
}


// --------------------------------------------------------------------------------------------------------------------

bool ADC_CheckResolution(uint32_t value)
{
	uint8_t valid[5] = {8, 10, 12, 14, 16};
	bool status = false;
	size_t size = sizeof(valid)/sizeof(valid[0]);

	for (uint8_t x = 0; x < size; x++)
	{
		if (value == valid[x])
		{
			status = true;
			break;
		}
	}

	return status;
}


// --------------------------------------------------------------------------------------------------------------------

bool ADC_CheckGain(uint32_t value)
{
	uint8_t valid[3] = {1, 10, 100};

	for (uint8_t x = 0; x < 3; x++)
	{
		if (value == valid[x])
			return true;
	}

	return false;
}


// --------------------------------------------------------------------------------------------------------------------

uint8_t ADC_GainIndex(uint8_t gain)
{
	uint8_t valid[] = {1, 10, 100};

	for (uint8_t x = 0; x < 3; x++)
	{
		if (gain == valid[x])
			return x;
	}

	return 0;
}


// --------------------------------------------------------------------------------------------------------------------

void ADC_Reset(uint32_t SamplingTime)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	if (HAL_ADC_DeInit(&hadc3) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_ADC_Init(&hadc3) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = SamplingTime;
	sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;

	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}


// --------------------------------------------------------------------------------------------------------------------

void ADC_BspReset(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	if (HAL_ADC_DeInit(&hadc3) != HAL_OK)
	{
		Error_Handler();
	}

	  hadc3.Instance = ADC3;
	  hadc3.Init.Resolution = bsp.adc.resolution;
	  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
	  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	  hadc3.Init.LowPowerAutoWait = DISABLE;
	  hadc3.Init.ContinuousConvMode = ENABLE;
	  hadc3.Init.NbrOfConversion = 1;
	  hadc3.Init.DiscontinuousConvMode = DISABLE;
	  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_ONESHOT;
	  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	  hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;

	  hadc3.Init.OversamplingMode = bsp.adc.oversampling.enable;
	  hadc3.Init.Oversampling.Ratio = bsp.adc.oversampling.ratio;
	  hadc3.Init.Oversampling.RightBitShift =ADC_RightBitShift(bsp.adc.oversampling.ratio);
	  hadc3.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	  hadc3.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;

	  if (HAL_ADC_Init(&hadc3) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Regular Channel
	  */
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	  sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  sConfig.OffsetSignedSaturation = DISABLE;
	  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}


// --------------------------------------------------------------------------------------------------------------------

bool ADC_Measurement(uint32_t sample_count)
{
	float zero_offset = 0.0f;
	float math_offset = 0.0f;

	float cal_gain = bsp.eeprom.structure.calibration.gain[bsp.adc.gain.index];

	if(ADC_Sample(sample_count))
	{
		(bsp.adc.offset.enable) ? (zero_offset = bsp.adc.offset.zero[bsp.adc.gain.index]) : (zero_offset = 0.0f);
		(bsp.adc.math_offset.enable) ? (math_offset = bsp.adc.math_offset.zero[bsp.adc.gain.index]) : (math_offset = 0.0f);

		ADC_SignalConditioning(bsp.adc.gain.value, sample_count, zero_offset, cal_gain, math_offset);

		return true;
	}

	return false;
}


// --------------------------------------------------------------------------------------------------------------------

bool ADC_CalibrationMeasurement(uint32_t sample_count)
{

	if(ADC_Sample(sample_count))
	{

		ADC_SignalConditioning(bsp.adc.gain.value, sample_count, 0.0f, 1.0f, 0.0f);

		return true;
	}

	return false;
}


// --------------------------------------------------------------------------------------------------------------------

void ADC_CalibrationSetup(void)
{
	  ADC_ChannelConfTypeDef sConfig = {0};


		if (HAL_ADC_DeInit(&hadc3) != HAL_OK)
		{
			Error_Handler();
		}


	  hadc3.Init.Resolution = ADC_RESOLUTION_16B;
	  hadc3.Init.OversamplingMode = ENABLE;
	  hadc3.Init.Oversampling.Ratio = 1024;
	  hadc3.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_10;
	  hadc3.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	  hadc3.Init.Oversampling.OversamplingStopReset = ADC_REGOVERSAMPLING_CONTINUED_MODE;

		if (HAL_ADC_Init(&hadc3) != HAL_OK)
		{
			Error_Handler();
		}

	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
	  sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  sConfig.OffsetSignedSaturation = DISABLE;

	  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}


// --------------------------------------------------------------------------------------------------------------------

void ADC_InitMemory()
{
	memset(measurements, 0, ADC_MEASUREMENT_BUFFER*sizeof(float));
}
