/**
 * @file    traces.c
 * @author  Jeremy ROULLAND
 * @date    12 mar. 2018
 * @brief   Diagnosis
 */

#include "traces.h"

/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#define STDOUT_BUFFER_SIZE  128


/*----------------------------------------------------------------------------*/
/* Private Members                                                            */
/*----------------------------------------------------------------------------*/

static UART_HandleTypeDef* _huart = NULL;

static uint8_t  _TxBuffer[STDOUT_BUFFER_SIZE] = {0};
static uint16_t _TxHead = 0U;
static uint16_t _TxTail = 0U;

/*----------------------------------------------------------------------------*/
/* Private Functions                                                          */
/*----------------------------------------------------------------------------*/

int __io_putchar(int ch)
{
    _TxBuffer[_TxHead++] = (uint8_t)ch;
    _TxHead %= STDOUT_BUFFER_SIZE;
    return ch;
}

int __io_getchar(void)
{
    int ch = 0;
    return ch;
}


/*----------------------------------------------------------------------------*/
/* Implementation                                                             */
/*----------------------------------------------------------------------------*/

void Traces_Init(UART_HandleTypeDef* huart)
{
    _huart = huart;
}

void Traces_Process(void)
{
    uint8_t c = 0U;
    
    if(_TxTail != _TxHead)
    {
        c = _TxBuffer[_TxTail++];
        _TxTail %= STDOUT_BUFFER_SIZE;
        HAL_UART_Transmit(_huart, (uint8_t*) &c, 1U, HAL_MAX_DELAY);
    }
}

/*----------------------------------------------------------------------------*/
/* Callbacks                                                                  */
/*----------------------------------------------------------------------------*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == _huart)
    {

    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == _huart)
    {

    }
}
