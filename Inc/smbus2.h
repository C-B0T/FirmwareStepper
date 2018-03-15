/**
 * @file    smbus2.h
 * @author  C-Bot
 * @date    24 feb. 2018
 * @brief   Smbus 2.0 Protocol manager
 */

#ifndef INC_SMBUS2_H_
#define INC_SMBUS2_H_

#include "stm32f0xx_hal.h"


void smbus2_Init(SMBUS_HandleTypeDef *h);

void smbus2_Process(void);

#endif /* INC_SMBUS2_H_ */
