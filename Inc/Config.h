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
  #define IMAX_N (0x03)    // 125mA
#elif defined(CANNON_BODY)
  #define IMAX_N (0x0F)    // 500mA
#elif defined(CANNON_HEAD)
  #define IMAX_N (0x0F)    // 500mA
#else
  #error "Functional Configuration missing!"
#endif



#endif /* INC_CONFIG_H_ */

