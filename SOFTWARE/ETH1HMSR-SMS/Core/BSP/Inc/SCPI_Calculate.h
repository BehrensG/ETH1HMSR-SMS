/*
 * SCPI_Calculate.h
 *
 *  Created on: May 21, 2024
 *      Author: BehrensG
 */

#ifndef BSP_INC_SCPI_CALCULATE_H_
#define BSP_INC_SCPI_CALCULATE_H_

#include "scpi/scpi.h"

scpi_result_t SCPI_CalculateOffsetEnable(scpi_t * context);
scpi_result_t SCPI_CalculateOffsetEnableQ(scpi_t * context);
scpi_result_t SCPI_CalculateOffset(scpi_t * context);
scpi_result_t SCPI_CalculateOffsetQ(scpi_t * context);
scpi_result_t SCPI_CalculateAverageQ(scpi_t * context);

#endif /* BSP_INC_SCPI_CALCULATE_H_ */
