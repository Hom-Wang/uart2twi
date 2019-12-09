/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_rng.h
 *  @author  KitSprout
 *  @date    Jul-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_RNG_H
#define __NRF5x_RNG_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"

/* Define ----------------------------------------------------------------------------------*/
#define RNGx                  NRF_RNG

/* Macro -----------------------------------------------------------------------------------*/
#define RNG_TASKS_START()     RNGx->TASKS_START      // Task starting the random number generator
#define RNG_TASKS_STOP()      RNGx->TASKS_STOP       // Task stopping the random number generator

#define RNG_EVENTS_VALRDY()   RNGx->EVENTS_VALRDY    // Event being generated for every new random number written to the VALUE register

#define RNG_VALUE()           ((uint8_t)(RNGx->VALUE & RNG_VALUE_VALUE_Msk))

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {
//  NRF_RNG_Type *Instance;

  uint32_t Shortcut;
  // ENABLE / DISABLE, connect VALRDY and STOP
  uint32_t BiasCorrection;
  // ENABLE / DISABLE
  pFunc EventCallback;
  // RNG EVENT CALLBACK
} RNG_InitTypeDef;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      RNG_Init( RNG_InitTypeDef *hrng );
void      RNG_Cmd( uint32_t state );
void      RNG_InterruptCmd( uint32_t state );
uint8_t   RNG_GetRandomValue( RNG_InitTypeDef *hrng );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
