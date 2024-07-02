/*
 * SCPI_Measure.h
 *
 *  Created on: Apr 7, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_SCPI_MEASURE_H_
#define BSP_INC_SCPI_MEASURE_H_

#include "scpi/scpi.h"

scpi_result_t SCPI_MeasureQ(scpi_t * context);
scpi_result_t SCPI_FetchQ(scpi_t * context);
scpi_result_t SCPI_Initiate(scpi_t * context);
scpi_result_t SCPI_NullOffsetEnable(scpi_t * context);
scpi_result_t SCPI_NullOffsetEnableQ(scpi_t * context);
scpi_result_t SCPI_NullOffset(scpi_t * context);
scpi_result_t SCPI_NullOffsetQ(scpi_t * context);
scpi_result_t SCPI_DataDataQ(scpi_t * context);

#endif /* BSP_INC_SCPI_MEASURE_H_ */
