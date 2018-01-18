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
void BrushlessMotor_Init ();

/**
 * @brief Do n step motor
 * @param step or microstep
 */
void StepperMotor_DoStep (int32_t step);

/**
 * @brief StepperMotor TIM Callback
 */
void StepperMotor_TimCallback ();

#endif /* INC_STEPPERMOTORDRIVER_H_ */
