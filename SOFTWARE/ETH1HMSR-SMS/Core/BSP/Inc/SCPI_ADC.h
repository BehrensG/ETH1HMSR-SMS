/*
 * SCPI_ADC.h
 *
 *  Created on: Apr 7, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_SCPI_ADC_H_
#define BSP_INC_SCPI_ADC_H_

#include "scpi/scpi.h"

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_AdcConfigurationResolution(scpi_t * context);
scpi_result_t SCPI_AdcConfigurationResolutionQ(scpi_t * context);

scpi_result_t SCPI_AdcConfigurationSamplingTimeCycle(scpi_t * context);
scpi_result_t SCPI_AdcConfigurationSamplingTimeCycleQ(scpi_t * context);

scpi_result_t SCPI_AdcConfigurationSamplingTimePeriod(scpi_t * context);
scpi_result_t SCPI_AdcConfigurationSamplingTimePeriodQ(scpi_t * context);

scpi_result_t SCPI_AdcConfigurationOverSamplingEnable(scpi_t * context);
scpi_result_t SCPI_AdcConfigurationOverSamplingEnableQ(scpi_t * context);

scpi_result_t SCPI_AdcConfigurationOverSamplingRatio(scpi_t * context);
scpi_result_t SCPI_AdcConfigurationOverSamplingRatioQ(scpi_t * context);

scpi_result_t SCPI_AdcConfigurationGain(scpi_t * context);
scpi_result_t SCPI_AdcConfigurationGainQ(scpi_t * context);

scpi_result_t SCPI_AdcConfigurationSampleCount(scpi_t * context);
scpi_result_t SCPI_AdcConfigurationSampleCountQ(scpi_t * context);


#endif /* BSP_INC_SCPI_ADC_H_ */
