/*
 * HMSR-SMS.h
 *
 *  Created on: Apr 19, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_HMSR_SMS_H_
#define BSP_INC_HMSR_SMS_H_

#include "stdbool.h"
#include "main.h"

void HMSR_SMS_Init();
float HMSR_SMS_MeasuringRange(uint8_t nominal_current);
bool HMSR_SMS_CheckNominalCurrent(uint8_t value);

#endif /* BSP_INC_HMSR_SMS_H_ */
