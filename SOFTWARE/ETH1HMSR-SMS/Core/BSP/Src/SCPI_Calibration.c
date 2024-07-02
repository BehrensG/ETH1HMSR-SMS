/*
 * SCPI_Calibration.c
 *
 *  Created on: Apr 21, 2024
 *      Author: grzegorz
 */

// --------------------------------------------------------------------------------------------------------------------

#include <math.h>
#include "cmsis_os.h"

#include "SCPI_Calibration.h"
#include "ADC.h"
#include "BSP.h"
#include "EEPROM.h"

// --------------------------------------------------------------------------------------------------------------------

extern ADC_HandleTypeDef hadc3;
extern bsp_t bsp;
extern SemaphoreHandle_t MeasMutex;

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_CalibrationCountQ(scpi_t * context)
{
	SCPI_ResultUInt32(context, bsp.eeprom.structure.calibration.count);
	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_CalibrationStore(scpi_t * context)
{

	if (bsp.security.status)
	{
		SCPI_ErrorPush(context, SCPI_ERROR_CALIBRATION_MEMORY_SECURE);
		return SCPI_RES_ERR;
	}

	bsp.eeprom.structure.calibration.count += 1;

	if (BSP_OK != EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
		return SCPI_RES_ERR;
	}



	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

#define CAL_LIMIT_HIGH	1.1f
#define CAL_LIMIT_LOW	0.9f
#define CAL_SAMP_COUNT	1000

extern float measurements[];

static float CALIB_Average(uint32_t sample_count)
{
	float average = 0.0f;
	float tmp = 0.0f;

	for (uint32_t i = 0; i < sample_count ; i++)
	{
		tmp += measurements[i];
	}

	average = (float)(tmp/sample_count);

	return average;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_CalibrationValue(scpi_t * context)
{

	float calib = 1.0f;
	float average = 1.0f;
	float vref = 1.0f;

	if (bsp.security.status)
	{
		SCPI_ErrorPush(context, SCPI_ERROR_CALIBRATION_MEMORY_SECURE);
		return SCPI_RES_ERR;
	}

	if(!SCPI_ParamFloat(context, &vref, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(pdTRUE == xSemaphoreTake(MeasMutex, pdMS_TO_TICKS(20000)))
	{
		ADC_CalibrationSetup();
		ADC_AutoCalibration();

		if(!ADC_CalibrationMeasurement(CAL_SAMP_COUNT))
		{
			xSemaphoreGive(MeasMutex);
			SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
			return SCPI_RES_ERR;
		}

		average = fabs(CALIB_Average(CAL_SAMP_COUNT));

		calib = vref/average;

		ADC_BspReset();
		ADC_AutoCalibration();

		if((calib >= CAL_LIMIT_LOW) && (calib <= CAL_LIMIT_HIGH))
		{
			bsp.eeprom.structure.calibration.gain[bsp.adc.gain.index] = calib;
		}
		else
		{
			xSemaphoreGive(MeasMutex);
			SCPI_ErrorPush(context, SCPI_ERROR_CALIBRATION_MEAS_OUT_OF_RANGE);
			return SCPI_RES_ERR;
		}

		xSemaphoreGive(MeasMutex);
		return SCPI_RES_OK;
	}
	else
	{
		return SCPI_RES_ERR;
	}
}


// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_CalibrationValueQ(scpi_t * context)
{
	uint32_t gain;
	uint8_t index;

	if(!SCPI_ParamUInt32(context, &gain, FALSE))
	{
		index = bsp.adc.gain.index;
	}
	else
	{
		(ADC_CheckGain(gain)) ? (index = ADC_GainIndex(gain)) :  (index = bsp.adc.gain.index);

	}

	float calib =  bsp.eeprom.structure.calibration.gain[index];
	SCPI_ResultFloat(context, calib);
	return SCPI_RES_OK;
}


// --------------------------------------------------------------------------------------------------------------------

scpi_choice_def_t ADC_CALIB_Mode_state_select[] =
{
		{ "OFFSet", ADC_CALIB_OFFSET },
		{ "LINEARity", ADC_CALIB_OFFSET_LINEARITY },
		SCPI_CHOICE_LIST_END
};

scpi_result_t SCPI_CalibrationAdcAutoMode(scpi_t * context)
{
	int32_t select = 0;

	if (!SCPI_ParamChoice(context, ADC_CALIB_Mode_state_select, &select, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(pdTRUE == xSemaphoreTake(MeasMutex, pdMS_TO_TICKS(20000)))
	{
		if( ADC_CALIB_OFFSET == select)
		{
			if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_DIFFERENTIAL_ENDED) != HAL_OK)
			{
				xSemaphoreGive(MeasMutex);
				SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
				return SCPI_RES_ERR;
			}
		}
		else if (ADC_CALIB_OFFSET_LINEARITY == select)
		{
			if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET_LINEARITY, ADC_DIFFERENTIAL_ENDED) != HAL_OK)
			{
				xSemaphoreGive(MeasMutex);
				SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
				return SCPI_RES_ERR;
			}
		}

		xSemaphoreGive(MeasMutex);
		return SCPI_RES_OK;
	}
	else
	{
		return SCPI_RES_ERR;
	}
}
