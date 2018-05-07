/**
 * @file    smbus2_cmd.c
 * @author  C-Bot
 * @date    02 mar. 2018
 * @brief   Smbus Commands
 */

/*----------------------------------------------------------------------------*/
/* Configuration                                                              */
/*----------------------------------------------------------------------------*/

//#define POWEROUT
#define STEPPER
//#define SERVO
//#define SHIELD_MOM
//#define BARREL	/* Is defined in Config.h */
//#define STEPPER_ENCODER


/*----------------------------------------------------------------------------*/
/* Configuration END                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#include "smbus2_cmd.h"
#include "stm32f0xx_hal.h"


#ifdef POWEROUT
#include "actions.h"
#endif
#ifdef STEPPER
#include "actions.h"
#endif
#ifdef SERVO
#endif
#ifdef SHIELD_MOM
#endif
#ifdef BARREL
#endif
#ifdef STEPPER_ENCODER
#endif

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#define WR (1U)
#define RD (2U)

#define KEY_55	(0x55)

#define DATA_LEN_MAX (4)

/*----------------------------------------------------------------------------*/
/* Commands                                                                   */
/*----------------------------------------------------------------------------*/


static uint8_t TabCmd[][3+DATA_LEN_MAX] = {

	/* General */
	{0x00, WR, 1, KEY_55},   /* Reset */
	{0x01, WR, 1, KEY_55},   /* Emergency Stop */
	{0x02, RD, 1, 0xAA},     /* Ping */
	{0x03, WR, 1, KEY_55},   /* Init */
	{0x04, RD, 1, 0x00},     /* Status */

	#ifdef POWEROUT
	/* PowerOUT */
	{0x05, RD, 4, 0x00, 0x00, 0x00, 0x00},   /* Get Inputs */
	{0x10, WR, 2, 0x00, 0x00},               /* Set Speed */
	{0x11, WR, 3, 0x00, 0x00, 0x00},         /* Set Outputs */
	#endif

	#ifdef STEPPER
	/* Stepper */
	{0x05, RD, 2, 0x00, 0x00},               /* Get Inputs */
	{0x10, WR, 1, 0x00},                     /* Set Mode */
	{0x11, WR, 2, 0x00, 0x00},               /* Set Speed */
	{0x12, WR, 4, 0x00, 0x00, 0x00, 0x00},   /* Set acc/dec */
	{0x13, WR, 3, 0x00, 0x00, 0x00},         /* Do steps */
	{0x14, WR, 1, 0x55},                     /* Go */
	{0x15, WR, 1, 0x55},                     /* Stop */
	#endif

	#ifdef SERVO
	/* Servo */
	{0x05, RD, 4, 0x00, 0x00, 0x00, 0x00},   /* Get Inputs */
	{0x10, WR, 2, 0x00, 0x00},               /* Set Angle */
	#endif

	#ifdef SHIELD_MOM
	/* Shield MOM */
	{0x10, RD, 1, 0x00},     /* State of Charge */
	#endif

	#ifdef BARREL
	/* Barrel */
	{0x50, WR, 1, 0x00},     /* Goto num */
	#endif

	#ifdef STEPPER_ENCODER
	/* Stepper+Encoder */
	{0x50, WR, 1, 0x00},     /* Goto Position */
	#endif

	{0xFF, 0, 0},            /* Tab end */
};

/*----------------------------------------------------------------------------*/
/* Callback                                                                   */
/*----------------------------------------------------------------------------*/
/* General Functions */
void FunctionReset (uint8_t len, uint8_t *buff) { if(buff[0] == KEY_55) HAL_NVIC_SystemReset(); }
void FunctionInit  (uint8_t len, uint8_t *buff) { /* Nothing */ }


typedef struct callback {
    uint8_t cmd;
    void(*pF)(uint8_t len, uint8_t *buff);
} callback_t;

callback_t TabCallback[] = {
	{0x00, FunctionReset},
	{0x01, EmergencyStop},
	{0x03, FunctionInit},

	#ifdef POWEROUT
	/* PowerOUT */
	{0x10, SetSpeed},    /* Set Speed */
	{0x11, SetOutput},   /* Set Outputs */
	#endif

	#ifdef STEPPER
	/* Stepper */
	{0x10, SetMode},     /* Set Mode */
	{0x11, SetSpeed},    /* Set Speed */
	{0x12, SetAccDec},   /* Set acc/dec */
	{0x13, DoSteps},     /* Do steps */
	{0x14, Go},          /* Go */
	{0x15, Stop},        /* Stop */
	#endif

	#ifdef SERVO
	/* Servo */
	{0x10, SetAngle},    /* Set Angle */
	#endif

	#ifdef BARREL
	/* Barrel */
	{0x50, Goto},     /* Goto num */
	#endif

	{0xFF, NULL},        /* Tab end */
};

/*----------------------------------------------------------------------------*/
/* Private                                                                    */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Implementation                                                             */
/*----------------------------------------------------------------------------*/

uint8_t smbus2_cmd_GetRDWR(uint8_t cmd)
{
	uint8_t i = 0;
	uint8_t rdwr = 0;

	while((TabCmd[i][0] != cmd) && (TabCmd[i][0] != 0xFF))
		i += 1U;

	rdwr = TabCmd[i][1];

	return rdwr;
}

uint8_t smbus2_cmd_isRD(uint8_t cmd)
{
	uint8_t rdwr = 0;
	uint8_t ret = 0;

	rdwr = smbus2_cmd_GetRDWR(cmd);

	if(rdwr == RD)
		ret = 1U;
	else
		ret = 0U;

	return ret;
}

uint8_t smbus2_cmd_isWR(uint8_t cmd)
{
	uint8_t rdwr = 0;
	uint8_t ret = 0;

	rdwr = smbus2_cmd_GetRDWR(cmd);

	if(rdwr == WR)
		ret = 1U;
	else
		ret = 0U;

	return ret;
}

uint8_t smbus2_cmd_GetLen(uint8_t cmd)
{
	uint8_t i = 0;
	uint8_t len = 0;

	while((TabCmd[i][0] != cmd) && (TabCmd[i][0] != 0xFF))
		i += 1U;

	len = TabCmd[i][2];

	return len;
}

uint8_t smbus2_cmd_GetData(uint8_t cmd, uint8_t *buff)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t len = 0;

	while((TabCmd[i][0] != cmd) && (TabCmd[i][0] != 0xFF))
		i += 1U;

	len = TabCmd[i][2];

	for(j=0 ; (j < len) || (j<DATA_LEN_MAX) ; j++)
		buff[j] = TabCmd[i][3+j];

	return j;
}

uint8_t smbus2_cmd_SetData(uint8_t cmd, uint8_t len, uint8_t *buff)
{
	uint8_t i = 0;
	uint8_t j = 0;

	while((TabCmd[i][0] != cmd) && (TabCmd[i][0] != 0xFF))
		i += 1U;

	TabCmd[i][2] = len;

	for(j=0 ; (j < len) || (j<DATA_LEN_MAX) ; j++)
		TabCmd[i][3+j] = buff[j];

	return j;
}

void smbus2_cmd_ExecuteCmd(uint8_t cmd)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t len = 0;
	uint8_t *buff;

	callback_t function;

	if(smbus2_cmd_isWR(cmd))
	{
		while((TabCmd[i][0] != cmd) && (TabCmd[i][0] != 0xFF))
			i += 1U;

		while((TabCallback[j].cmd != cmd) && (TabCallback[j].cmd != 0xFF))
			j += 1U;

		len  =  TabCmd[i][2];
		buff = &TabCmd[i][3];

		if(TabCallback[j].pF != NULL)
		{
			function.pF = TabCallback[j].pF;
			function.pF(len, buff);
		}
	}
}



