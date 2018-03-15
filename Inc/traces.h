/**
 * @file    traces.h
 * @author  Jeremy ROULLAND
 * @date    12 mar. 2018
 * @brief   Diagnosis
 */

#ifndef INC_TRACES_H_
#define INC_TRACES_H_

#include "stm32f0xx_hal.h"

void Traces_Init(UART_HandleTypeDef* huart);

void Traces_Process(void);

#endif /* INC_TRACES_H_ */
