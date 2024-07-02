/*
 * SCPI_Calibration.h
 *
 *  Created on: Apr 21, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_SCPI_CALIBRATION_H_
#define BSP_INC_SCPI_CALIBRATION_H_

#include "scpi/scpi.h"

scpi_result_t SCPI_CalibrationCountQ(scpi_t * context);
scpi_result_t SCPI_CalibrationStore(scpi_t * context);
scpi_result_t SCPI_CalibrationValue(scpi_t * context);
scpi_result_t SCPI_CalibrationValueQ(scpi_t * context);
scpi_result_t SCPI_CalibrationAdcAutoMode(scpi_t * context);

#endif /* BSP_INC_SCPI_CALIBRATION_H_ */
