/*
 * ADC.h
 *
 *  Created on: Apr 10, 2024
 *      Author: BehrensG
 */

#ifndef BSP_INC_ADC_H_
#define BSP_INC_ADC_H_

#include "main.h"

// --------------------------------------------------------------------------------------------------------------------

#define ADC_MEASUREMENT_BUFFER	32000
#define ADC_DEF_SIZE			1000
#define ADC_VREF				3.3f
#define ADC_VCOM				3.3f/2.0f
#define ADC_TIMEOUT_MAX			40000

// --------------------------------------------------------------------------------------------------------------------

bool ADC_CheckGain(uint32_t value);
uint8_t ADC_GainIndex(uint8_t gain);
bool ADC_CheckResolution(uint32_t value);
void ADC_Reset(uint32_t SamplingTime);
uint32_t ADC_SelectResolution(uint8_t value);
void ADC_ConfigureOverSampling(FunctionalState enable, uint32_t ratio);
bool ADC_CheckOverSamplingRation(uint32_t value);
bool ADC_Sample(uint32_t sample_count);
void ADC_AutoCalibration(void);
void ADC_SignalConditioning(uint8_t gain, uint32_t sample_count, float offset, float calib_gain, float math_offset);
void ADC_SignalConditioningZeroOffset(uint8_t gain, uint32_t sample_count);
uint32_t ADC_RightBitShift(uint32_t value);
bool ADC_Measurement(uint32_t sample_count);
void ADC_CalibrationSetup(void);
bool ADC_CalibrationMeasurement(uint32_t sample_count);
void ADC_BspReset(void);
void ADC_InitMemory();

#endif /* BSP_INC_ADC_H_ */
