/**
 * @file    actions.h
 * @author  Jeremy ROULLAND
 * @date    13 mar. 2018
 * @brief   Actions (Simple, Complex, Control, ...)
 */

/**
 *    CMD     | OPCOD | / | L | DATA0   DATA1   DATA2   DATA3      |    Comments
 *
 *
 */

#ifndef INC_ACTIONS_H_
#define INC_ACTIONS_H_

#include "stdint.h"

void Actions_Process(void);

void EmergencyStop(uint8_t len, uint8_t *buff);



#endif /* INC_ACTIONS_H_ */
