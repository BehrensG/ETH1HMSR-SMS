/*
 * utility.h
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_UTILITY_H_
#define BSP_INC_UTILITY_H_

#include <stdbool.h>

#include "main.h"
#include "SCPI_Def.h"
#include "scpi/scpi.h"
#include "api.h"

#define UTIL_ASCII_SIZE	12

bool UTIL_Timeout(uint32_t start, uint32_t timeout);
char* UTIL_FloatArrayToASCII(scpi_t * context, enum netconn_type conn_type, float* float_array, uint32_t num_floats2, size_t* frame_size);
char* UTIL_FloatArrayToREAL(scpi_t * context, enum netconn_type conn_type, float* float_array, uint32_t num_floats, size_t* frame_size);
float UTIL_Average(uint32_t sample_count);
int32_t UTIL_WhiteSpace(const char* string, uint32_t size);

#endif /* BSP_INC_UTILITY_H_ */
