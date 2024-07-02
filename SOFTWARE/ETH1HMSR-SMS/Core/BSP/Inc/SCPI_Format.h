/*
 * SCPI_Format.h
 *
 *  Created on: Apr 24, 2024
 *      Author: BehrensG
 */

#ifndef BSP_INC_SCPI_FORMAT_H_
#define BSP_INC_SCPI_FORMAT_H_

#include "scpi/scpi.h"

// --------------------------------------------------------------------------------------------------------------------

scpi_result_t SCPI_FormatData(scpi_t * context);
scpi_result_t SCPI_FormatDataQ(scpi_t * context);

#endif /* BSP_INC_SCPI_FORMAT_H_ */
