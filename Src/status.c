/**
 * @file    status.c
 * @author  Jeremy ROULLAND
 * @date    12 mar. 2018
 * @brief   Status Code on GPIO
 */

#include "status.h"
#include "smbus2_cmd.h"

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#define false	(0U)
#define true	(1U)

// Period (ms)
#define STATUS_PERIOD_OKAY       (1000U)
#define STATUS_PERIOD_ERROR      (200U)


/*----------------------------------------------------------------------------*/
/* Private Members                                                            */
/*----------------------------------------------------------------------------*/

static GPIO_TypeDef* _GPIOx    = NULL;
static uint16_t      _GPIO_Pin = 0U;

// General status
static uint8_t _error  = false;
static uint8_t _busy   = false;

/*----------------------------------------------------------------------------*/
/* Private Functions                                                          */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Implementation                                                             */
/*----------------------------------------------------------------------------*/

void Status_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    _GPIOx    = GPIOx;
    _GPIO_Pin = GPIO_Pin;
}

void Status_Process(uint32_t time)
{
    static uint32_t t0 = 0;
    
    uint16_t local_time = 0;
    uint16_t period = 0;
	uint8_t status = 0;
    
    local_time = time - t0;
    
    // GPIO Blink
    if(_error >= 1U)
        period = STATUS_PERIOD_ERROR;
    else
        period = STATUS_PERIOD_OKAY;

	if(local_time <= period/2U)
		HAL_GPIO_WritePin(_GPIOx, _GPIO_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(_GPIOx, _GPIO_Pin, GPIO_PIN_RESET);

    if(local_time >= period)
        t0 = time;

	// Update status in smbus tab
	status = Status_GetStatus();
	smbus2_cmd_SetData(0x04, 1U, &status);
}

uint8_t Status_GetStatus(void)
{
	uint8_t status = 0;

	if(_busy >= 1U)
		status &= (0x01<<0);
	
	if(_error >= 1U)
		status &= (0x01<<1);
	
	return status;
}

uint8_t Status_isBusy(void)
{
	uint8_t busy = false;

	if(_busy >= 1U)
		busy = true;
	else
		busy = false;

	return busy;
}

void Status_SetBusy(uint8_t busyState)
{
	uint8_t status = 0;

	if(busyState >= 1U)
		_busy = 1U;
	else
		_busy = 0U;
	
	// Update status in smbus tab
	status = Status_GetStatus();
	smbus2_cmd_SetData(0x04, 1U, &status);
}

/*----------------------------------------------------------------------------*/
/* Callbacks                                                                  */
/*----------------------------------------------------------------------------*/

