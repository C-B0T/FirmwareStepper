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
static uint8_t _autoCalib = false;

static int8_t _position = 0;
static int8_t _target = 0;

static uint8_t _fsm_cmd = 0;

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
	// Le driver se met en standby electroniquement
	//StepperMotor_Stop();
}

void _control_S_AU(void)
{
	static GPIO_PinState stby      = GPIO_PIN_RESET;
	static GPIO_PinState stby_prev = GPIO_PIN_RESET;

	stby_prev = stby;
	stby = HAL_GPIO_ReadPin(STBY_GPIO_Port, STBY_Pin);

	// Front montant
	if( (stby_prev == GPIO_PIN_RESET) && (stby == GPIO_PIN_SET) ) {
		StepperMotor_Init(IMAX_N);
		StepperMotor_Demo();
		_autoCalib = true;
	}

	// Front descendant
	if( (stby_prev == GPIO_PIN_SET) && (stby == GPIO_PIN_RESET) )
		_stop();
}

void _auto_calibration(void)
{
	static GPIO_PinState top      = GPIO_PIN_RESET;
	static GPIO_PinState top_prev = GPIO_PIN_RESET;

	top_prev = top;
	top = HAL_GPIO_ReadPin(DIN_1_GPIO_Port, DIN_1_Pin);

	if(_autoCalib == true)
	{
		// Front montant
		if( (top_prev == GPIO_PIN_RESET) && (top == GPIO_PIN_SET) ) {
			StepperMotor_Stop();
			_autoCalib = false;
		}
	}
}


/*----------------------------------------------------------------------------*/
/* Implementation                                                             */
/*----------------------------------------------------------------------------*/

void Update_Process(void)
{
	// Read Inputs
	_readInputs();
	
	// Control S_AU
	_control_S_AU();

	// Execute Command
	_auto_calibration();	// Auto-Calibration if startup

}

void EmergencyStop(uint8_t len, uint8_t *buff)
{
	if(buff[0] == 0xFF) {
		_emergencyStop = true;
		_stop();
	}
}

void SetMode(uint8_t len, uint8_t *buff)
{

}

void SetSpeed(uint8_t len, uint8_t *buff)
{

}

void SetAccDec(uint8_t len, uint8_t *buff)
{

}

void DoSteps(uint8_t len, uint8_t *buff)
{
	StepperMotor_DoStep(-200/5);
}

void Go(uint8_t len, uint8_t *buff)
{

}

void Stop(uint8_t len, uint8_t *buff)
{
	StepperMotor_Stop();
}

void Goto(uint8_t len, uint8_t *buff)
{
	int8_t val = buff[0];

	_target = val;
}


/*----------------------------------------------------------------------------*/
/* Callbacks                                                                  */
/*----------------------------------------------------------------------------*/

