/**
 * @file    Config.h
 * @author  C-Bot
 * @date    21 feb. 2018
 * @brief   Configuration file
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

/*----------------------------------------------------------------------------*/
/* MODE definitions - Do not modify                                           */
/*----------------------------------------------------------------------------*/

#define MODE_DIGITAL_INPUT		(1U)
#define MODE_DIGITAL_OUTPUT		(2U)
#define MODE_ANALOG_INPUT		(3U)
#define MODE_ANALOG_OUTPUT		(4U)
#define MODE_PWM				(5U)




/*----------------------------------------------------------------------------*/
/*                      === CONFIGURATION ====                                */
/*                      === CONFIGURATION ====                                */
/*                      === CONFIGURATION ====                                */
/*----------------------------------------------------------------------------*/

// IN1 Configuration
#define FUNCTION_IN1	MODE_DIGITAL_INPUT

// IN2 Configuration
#define FUNCTION_IN2	MODE_DIGITAL_INPUT

/*----------------------------------------------------------------------------*/
/*                    === CONFIGURATION END ====                              */
/*                    === CONFIGURATION END ====                              */
/*                    === CONFIGURATION END ====                              */
/*----------------------------------------------------------------------------*/





/*----------------------------------------------------------------------------*/
/* Decoding config - Do not modify                                            */
/*----------------------------------------------------------------------------*/

// Check IN1
#if FUNCTION_IN1 == MODE_DIGITAL_OUTPUT
  #warning "IN1 and DIGITAL OUTPUT must be set by CubeMx"
#elif FUNCTION_IN1 == MODE_ANALOG_INPUT
  #error "IN1 and ANALOG INPUT is not yet possible"
#elif FUNCTION_IN1 == MODE_ANALOG_OUTPUT
  #error "IN1 and ANALOG OUTPUT is not possible"
#elif FUNCTION_IN1 == MODE_PWM
  #error "IN1 and PWM is not yet possible"
#endif

// Check IN2
#if FUNCTION_IN2 == MODE_DIGITAL_OUTPUT
  #warning "IN2 and DIGITAL OUTPUT must be set by CubeMx"
#elif FUNCTION_IN2 == MODE_ANALOG_INPUT
  #error "IN2 and ANALOG INPUT is not yet possible"
#elif FUNCTION_IN2 == MODE_ANALOG_OUTPUT
  #error "IN2 and ANALOG OUTPUT is not possible"
#elif FUNCTION_IN2 == MODE_PWM
  #error "IN2 and PWM is not yet possible"
#endif

// Configure IN1
#if FUNCTION_IN1 == MODE_DIGITAL_INPUT
 #define FUNCTION_IN1_DIGITAL_INPUT		(11U)
#elif FUNCTION_IN1 == MODE_DIGITAL_OUTPUT
  #define FUNCTION_IN1_DIGITAL_OUTPUT	(12U)
#elif FUNCTION_IN1 == MODE_ANALOG_INPUT
  #define FUNCTION_IN1_ANALOG_INPUT		(13U)
#elif FUNCTION_IN1 == MODE_ANALOG_OUTPUT
  #define FUNCTION_IN1_ANALOG_OUTPUT	(14U)
#elif FUNCTION_IN1 == MODE_PWM
  #define FUNCTION_IN1_PWM				(15U)
#endif

// Configure IN2
#if FUNCTION_IN2 == MODE_DIGITAL_INPUT
 #define FUNCTION_IN2_DIGITAL_INPUT		(21U)
#elif FUNCTION_IN2 == MODE_DIGITAL_OUTPUT
  #define FUNCTION_IN2_DIGITAL_OUTPUT	(22U)
#elif FUNCTION_IN2 == MODE_ANALOG_INPUT
  #define FUNCTION_IN2_ANALOG_INPUT		(23U)
#elif FUNCTION_IN2 == MODE_ANALOG_OUTPUT
  #define FUNCTION_IN2_ANALOG_OUTPUT	(24U)
#elif FUNCTION_IN2 == MODE_PWM
  #define FUNCTION_IN2_PWM				(25U)
#endif

#endif /* INC_CONFIG_H_ */
