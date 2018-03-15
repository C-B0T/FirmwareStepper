/**
 * @file    status.h
 * @author  Jeremy ROULLAND
 * @date    12 mar. 2018
 * @brief   Status Code on GPIO
 */

#include "stm32f0xx_hal.h"

// status falgs :
// b7 to b2 : always 0
// b1 : error
// b0 : busy


void Status_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void Status_Process(uint32_t time);


uint8_t Status_GetStatus(void);

void Status_SetBusy(uint8_t busyState);

uint8_t Status_isBusy(void);

