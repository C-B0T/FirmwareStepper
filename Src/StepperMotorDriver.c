/**
 * @file    StepperMotorDriver.c
 * @author  C-Bot
 * @date    06 jan. 2018
 * @brief   L6474 Stepper Motor Driver
 */

#include "StepperMotorDriver.h"

#include "stm32f0xx_hal.h"
#include "main.h"
#include "spi.h"
#include "tim.h"

/**
 * L6474
 *
 * Device Power-up end:
 *  - Registers are set to default 
 *  - Internal oscillator at 16MHz
 *  - Bridges are disabled (High-Z)
 * 
 * Device SPI:
 *  - Maximum SPI clock frequency   fCK,MAX    = 5   Mhz
 *  - SPI clock rise and fall time  trCK, tfCK = 25  ns
 *  - SPI clock high and low time   thCK, tlCK = 75  ns
 *  - Chip select setup time        tsetCS     = 350 ns
 *  - Chip select hold time         tholCS     = 10  ns
 *  - ...
 *
 */


/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#define MOT_SPI           (&hspi1)

#define MOT_TIMER         (&htim2)
#define MOT_TIMER_CHANNEL (TIM_CHANNEL_1)

// SPI Commands
#define SPI_CMD_NOP        (0x00)
#define SPI_CMD_SETPARAM   (0x00)
#define SPI_CMD_GETPARAM   (0x20)
#define SPI_CMD_ENABLE     (0xB8)
#define SPI_CMD_DISABLE    (0xA8)
#define SPI_CMD_STATUS     (0xD0)

// SPI Command Parameters (example: SPI_CMD_SETPARAM | SPI_CMD_PARAM_ABS_POS)
#define SPI_CMD_PARAM_ABS_POS   (0x01)
#define SPI_CMD_PARAM_EL_POS    (0x02)
#define SPI_CMD_PARAM_MARK      (0x03)
#define SPI_CMD_PARAM_TVAL      (0x09)
#define SPI_CMD_PARAM_ADC_OUT   (0x12)
#define SPI_CMD_PARAM_OCD_TH    (0x13)
#define SPI_CMD_PARAM_STEP_MODE (0x16)
#define SPI_CMD_PARAM_ALARM_EN  (0x17)
#define SPI_CMD_PARAM_CONFIG    (0x18)
#define SPI_CMD_PARAM_STATUS    (0x19)

/*----------------------------------------------------------------------------*/
/* Private members                                                            */
/*----------------------------------------------------------------------------*/

static enum Direction direction = FORWARD;

static int32_t stepCounter = 0;
static int32_t stepTarget  = 0;

/*----------------------------------------------------------------------------*/
/* Private function                                                           */
/*----------------------------------------------------------------------------*/

static void _spi_send(uint8_t *cmd, uint8_t *res, uint8_t len)
{
	uint8_t i = 0;
	for(i=0 ; i<len ; i++)
	{
		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(MOT_SPI, &cmd[i], &res[i], 1U, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
		asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	}

}

static void _direction(enum Direction dir)
{
	if (dir == FORWARD)
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);	// Forward
	else
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);	// Backward

}

static void _enable(void)
{
	uint8_t cmd[1] = {SPI_CMD_ENABLE};
	uint8_t res[1] = {0};

	_spi_send(cmd, res, 1);
	HAL_TIM_PWM_Start_IT(MOT_TIMER, MOT_TIMER_CHANNEL);
}

static void _disable(void)
{
	uint8_t cmd[1] = {SPI_CMD_DISABLE};
	uint8_t res[1] = {0};

	_spi_send(cmd, res, 1);
	HAL_TIM_PWM_Stop_IT(MOT_TIMER, MOT_TIMER_CHANNEL);
}

static void _disableIT(void)
{
	_disable();
}


/*----------------------------------------------------------------------------*/
/* Implementation                                                             */
/*----------------------------------------------------------------------------*/

void StepperMotor_Init (uint8_t imax_n)
{
	uint8_t cmd[3] = {SPI_CMD_STATUS, SPI_CMD_NOP, SPI_CMD_NOP};
	uint8_t res[3] = {0};

	// Init
	stepCounter = 0;
	stepTarget  = 0;

    // Reset
	HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_RESET);  // Standby
	HAL_Delay(1);
	HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);	  // Running
	HAL_Delay(1);
    
    // Direction
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_SET);	// Forward

	// Get Status
	_spi_send(cmd, res, 3);
	// res should have data

	// Config
	// > FullStep
	cmd[0] = SPI_CMD_SETPARAM | SPI_CMD_PARAM_STEP_MODE;
	cmd[1] = 0x88;
	_spi_send(cmd, res, 2);
	// > Imax(n) = 31,25mA + 31,25mA * n
	cmd[0] = SPI_CMD_SETPARAM | SPI_CMD_PARAM_TVAL;
	cmd[1] = imax_n;
	_spi_send(cmd, res, 2);

    // Stop Step clock
	HAL_TIM_PWM_Stop_IT(MOT_TIMER, MOT_TIMER_CHANNEL);

	__HAL_TIM_ENABLE_IT(MOT_TIMER, TIM_IT_UPDATE);
}

void StepperMotor_Demo (void)
{
	/*_enable();
	HAL_Delay(5000);
	_disable();*/
	StepperMotor_DoStep(2048);
}

void StepperMotor_DoStep (int32_t step)
{
	stepTarget = stepCounter + step;

	if(step < 0)
		direction = REVERSE;
	else
		direction = FORWARD;

	_direction(direction);
	_enable();
}

void StepperMotor_TimCallback ()
{
	// Count steps
	if(direction == FORWARD)
		stepCounter++;
	else
		stepCounter--;

	if(stepCounter == stepTarget)
		_disableIT();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2) {
		StepperMotor_TimCallback();
	}
}
