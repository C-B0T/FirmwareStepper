/**
 * @file    StepperMotorDriver.h
 * @author  C-Bot
 * @date    06 jan. 2018
 * @brief   L6474 Stepper Motor Driver
 */

#ifndef INC_STEPPERMOTORDRIVER_H_
#define INC_STEPPERMOTORDRIVER_H_

#include "common.h"

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Declaration                                                                */
/*----------------------------------------------------------------------------*/

/**
 * @brief Motor Direction
 */
enum Direction
{
    FORWARD,
    REVERSE
};

/**
 * @brief
 * @param
 */
void StepperMotor_Init (uint8_t imax_n);

/**
 * @brief
 * @param
 */
void StepperMotor_Demo ();

/**
 * @brief Do n step motor
 * @param step or microstep
 */
void StepperMotor_DoStep (int32_t step);

/**
 * @brief Disable motor drv
 */
void StepperMotor_Disable (void);

/**
 * @brief Stop step motor
 */
void StepperMotor_Stop (void);

/**
 * @brief StepperMotor TIM Callback
 */
void StepperMotor_TimCallback ();

#endif /* INC_STEPPERMOTORDRIVER_H_ */
