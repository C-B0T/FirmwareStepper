/**
 * @file    actions.h
 * @author  Jeremy ROULLAND
 * @date    13 mar. 2018
 * @brief   Actions (Simple, Complex, Control, ...)
 */

/**
 *    CMD     | OPCOD | / | L | DATA0   DATA1   DATA2   DATA3      |    Comments
 * Get Inputs	0x05	R	2	Input 1	Input 2					        Ana : 0 à 255	TOR : 0 ou 255
 * Set mode	    0x10	W	1	Mode						            1 2 4 8 16
 * Set speed	0x11	W	2	speed_l	speed_h					        step / s
 * Set acc/dec	0x12	W	4	acc_l	acc_h	dec_l	dec_h			step / s²
 * Do steps	    0x13	W	3	step_ll	step_l	step_h	step_hh
 * Go	        0x14	W	1	0x55
 * Stop	        0x15	W	1	0x55
 *
 */

#ifndef INC_ACTIONS_H_
#define INC_ACTIONS_H_

#include "stdint.h"

void Actions_Process(void);

void EmergencyStop(uint8_t len, uint8_t *buff);



#endif /* INC_ACTIONS_H_ */
