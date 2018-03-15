/**
 * @file    smbus2_cmd.h
 * @author  C-Bot
 * @date    02 mar. 2018
 * @brief   Smbus Commands
 */

#ifndef INC_SMBUS2_CMD_H_
#define INC_SMBUS2_CMD_H_

#include <stdint.h>

uint8_t smbus2_cmd_GetRDWR(uint8_t cmd);

uint8_t smbus2_cmd_isRD(uint8_t cmd);
uint8_t smbus2_cmd_isWR(uint8_t cmd);

uint8_t smbus2_cmd_GetLen(uint8_t cmd);

uint8_t smbus2_cmd_GetData(uint8_t cmd, uint8_t *buff);

uint8_t smbus2_cmd_SetData(uint8_t cmd, uint8_t len, uint8_t *buff);

void smbus2_cmd_ExecuteCmd(uint8_t cmd);


#endif /* INC_SMBUS2_CMD_H_ */
