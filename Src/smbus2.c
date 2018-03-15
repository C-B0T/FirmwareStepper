/**
 * @file    smbus2.h
 * @author  C-Bot
 * @date    24 feb. 2018
 * @brief   Stack Smbus 2.0
 */

#include "smbus2.h"

#include "smbus2_cmd.h"

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

//#define SMBUS_BUFFER_SIZE	0xFF
#define SMBUS_BUFFER_SIZE	10

#define DIR_WRITE (0U)
#define DIR_READ  (1U)

#define CMD_WR (1U)
#define CMD_RD (2U)

#define STATE_0_IDLE	(0)
#define STATE_1			(1)
#define STATE_2			(2)
#define STATE_31		(31)
#define STATE_32		(32)
#define STATE_41		(41)
#define STATE_42		(42)


#define PIPE_SIZE	(5)

/*----------------------------------------------------------------------------*/
/* Private members                                                            */
/*----------------------------------------------------------------------------*/

static SMBUS_HandleTypeDef *hsmbus;

static uint8_t buff[SMBUS_BUFFER_SIZE] = {0};

static int16_t state = STATE_0_IDLE;

/*----------------------------------------------------------------------------*/
/* Private function                                                           */
/*----------------------------------------------------------------------------*/

void _error(uint8_t flag)
{
	printf("smbus2=%xh\n", flag);

	state = STATE_0_IDLE;
	HAL_SMBUS_EnableListen_IT(hsmbus);
}

/*----------------------------------------------------------------------------*/
/* Implementation                                                             */
/*----------------------------------------------------------------------------*/

void smbus2_Init(SMBUS_HandleTypeDef *h)
{
	hsmbus = h;

	state = STATE_0_IDLE;
	HAL_SMBUS_EnableListen_IT(hsmbus);
}

/*----------------------------------------------------------------------------*/
/* Callbacks                                                                  */
/*----------------------------------------------------------------------------*/

void HAL_SMBUS_AddrCallback(SMBUS_HandleTypeDef *hsmbus, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	uint8_t cmd  = 0;
	uint8_t size = 0;

	if (state == STATE_0_IDLE)
	{
		if (TransferDirection == DIR_WRITE)
		{
			state = STATE_1;
			HAL_SMBUS_Slave_Receive_IT(hsmbus, &buff[0], 1U, SMBUS_FIRST_FRAME);	// Wait CMD
		}
		else
		{
			// Error 2.1
			_error(0x21);
		}
	}
	else if (state == STATE_2)
	{
		if (TransferDirection == DIR_READ)
		{
			state = STATE_32;
			cmd = buff[0];
			size = smbus2_cmd_GetLen(cmd);
			buff[1] = size;
			smbus2_cmd_GetData(cmd, &buff[2]);
			HAL_SMBUS_Slave_Transmit_IT(hsmbus, &buff[1], 1U + size, SMBUS_LAST_FRAME_NO_PEC);
		}
		else
		{
			// Error 2.2
			_error(0x22);
		}
	}
	else
	{
		// Error 1
		_error(0x10);
	}
}


void HAL_SMBUS_SlaveRxCpltCallback(SMBUS_HandleTypeDef *hsmbus)
{
	uint8_t cmd  = 0;
	uint8_t size = 0;

	if (state == STATE_1)
	{
		//Decode Command
		state = STATE_2;
		cmd = buff[0];
		if(smbus2_cmd_isWR(cmd)) {
			HAL_SMBUS_Slave_Receive_IT(hsmbus, &buff[1], 1U, SMBUS_NEXT_FRAME);		// Wait SIZE
		}
	}
	else if (state == STATE_2)
	{
		state = STATE_31;
		size = buff[1];
		HAL_SMBUS_Slave_Receive_IT(hsmbus, &buff[2], size, SMBUS_LAST_FRAME_NO_PEC);		// Wait DATAs
	}
	else if (state == STATE_31)
	{
		// Reception End
		state = STATE_41;
		cmd  = buff[0];
		size = buff[1];
		smbus2_cmd_SetData(cmd, size, &buff[2]);
		smbus2_cmd_ExecuteCmd(cmd);
		state = STATE_0_IDLE;
	}
	else
	{
		// Error 3
		_error(0x30);
	}
}


void HAL_SMBUS_SlaveTxCpltCallback(SMBUS_HandleTypeDef *hsmbus)
{
	if (state == STATE_32)
	{
		// Transmission End
		state = STATE_42;
		state = STATE_0_IDLE;
	}
	else
	{
		// Error 4
		_error(0x40);
	}
}


void HAL_SMBUS_ListenCpltCallback(SMBUS_HandleTypeDef *hsmbus)
{
	// Restart listening
	HAL_SMBUS_EnableListen_IT(hsmbus);
}
