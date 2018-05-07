/**
 * @file    Config.h
 * @author  C-Bot
 * @date    21 feb. 2018
 * @brief   Configuration file
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

/*----------------------------------------------------------------------------*/
/*                      === CONFIGURATION ====                                */
/*                      === CONFIGURATION ====                                */
/*                      === CONFIGURATION ====                                */
/*----------------------------------------------------------------------------*/

// ## Functional Configuration

#define BARREL
//#define CANNON_BODY
//#define CANNON_HEAD
//#define MIXER

/*----------------------------------------------------------------------------*/
/*                    === CONFIGURATION END ====                              */
/*                    === CONFIGURATION END ====                              */
/*                    === CONFIGURATION END ====                              */
/*----------------------------------------------------------------------------*/





/*----------------------------------------------------------------------------*/
/* Decoding config - Do not modify                                            */
/*----------------------------------------------------------------------------*/

// Configure Imax(n) = 31,25mA + 31,25mA * n
#if defined(BARREL)
//  #define IMAX_N (0x03)    // 125mA
  #define IMAX_N (0x1F)    // 1000mA
#elif defined(CANNON_BODY)
  #define IMAX_N (0x0F)    // 500mA
#elif defined(CANNON_HEAD)
  #define IMAX_N (0x0F)    // 500mA
#elif defined(MIXER)
  #define IMAX_N (0x03)    // 125mA
#else
  #error "Functional Configuration missing for Imax!"
#endif

// Configure I2C Address
#if defined(BARREL)
  #define I2C_ADDRESS (0x11)
#elif defined(CANNON_BODY)
  #define I2C_ADDRESS (0x21)
#elif defined(CANNON_HEAD)
  #define I2C_ADDRESS (0x22)
#elif defined(MIXER)
  #define I2C_ADDRESS (0x61)
#else
  #error "Functional Configuration missing for I2C Address!"
#endif


#endif /* INC_CONFIG_H_ */

