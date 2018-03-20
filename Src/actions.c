/**
 * @file    actions.c
 * @author  Jeremy ROULLAND
 * @date    13 mar. 2018
 * @brief   Actions (Simple, Complex, Control, ...)
 */

#include "actions.h"

#include "stm32f0xx_hal.h"
#include "smbus2_cmd.h"
#include "status.h"

#include "StepperMotorDriver.h"

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#define false (0U)
#define true  (1U)

/*----------------------------------------------------------------------------*/
/* Private Members                                                            */
/*----------------------------------------------------------------------------*/

static uint8_t _emergencyStop = false;

/*----------------------------------------------------------------------------*/
/* Private Functions                                                          */
/*----------------------------------------------------------------------------*/

void _readInputs(void)
{
	GPIO_PinState input[2] = {GPIO_PIN_RESET};
	uint8_t buff[2] = {0};
	int16_t i = 0;

	input[0] = HAL_GPIO_ReadPin(DIN_1_GPIO_Port, DIN_1_Pin);
	input[1] = HAL_GPIO_ReadPin(DIN_2_GPIO_Port, DIN_2_Pin);

	for(i = 0 ; i < 2 ; i++) {
		if(input[i] == GPIO_PIN_RESET)
			buff[i] = 0U;
		else
			buff[i] = 255U;
	}
	
	smbus2_cmd_SetData(0x05, 2, buff);
}

void _stop(void)
{
	//StepperMotor_Stop();
}

void _control_S_AU(void)
{

}

/*----------------------------------------------------------------------------*/
/* Implementation                                                             */
/*----------------------------------------------------------------------------*/

void Update_Process(void)
{
	// Read Inputs
	_readInputs();
	
	// Control S_AU

	// Execute Command
	/* PowerOut : Nothing to do */
}

void EmergencyStop(uint8_t len, uint8_t *buff)
{
	if(buff[0] == 0xFF) {
		_emergencyStop = true;
		_stop();
	}
}



/*----------------------------------------------------------------------------*/
/* Callbacks                                                                  */
/*----------------------------------------------------------------------------*/

