/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_rng.c
 *  @author  KitSprout
 *  @date    Jul-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_rng.h"

/** @addtogroup KS_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/


/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  RNG_Init
 */
void RNG_Init( RNG_InitTypeDef *hrng )
{
  if (hrng->Shortcut != ENABLE) {
    RNGx->SHORTS &= ~NRF_RNG_SHORT_VALRDY_STOP_MASK;
  }
  else {
    RNGx->SHORTS |= NRF_RNG_SHORT_VALRDY_STOP_MASK;
  }

  if (hrng->BiasCorrection != ENABLE) {
    RNGx->CONFIG &= ~RNG_CONFIG_DERCEN_Msk;
  }
  else {
    RNGx->CONFIG |= RNG_CONFIG_DERCEN_Msk;
  }
}

/**
 *  @brief  RNG_Cmd
 */
void RNG_Cmd( uint32_t state )
{
  if (state != ENABLE) {
    RNG_TASKS_STOP() = SET;
  }
  else {
    RNG_TASKS_START() = SET;
  }
}

/**
 *  @brief  RNG_InterruptCmd
 */
void RNG_InterruptCmd( uint32_t state )
{
  if (state != ENABLE) {
    RNGx->INTENCLR = NRF_RNG_INT_VALRDY_MASK;
  }
  else {
    RNGx->INTENSET = NRF_RNG_INT_VALRDY_MASK;
  }
}

/**
 *  @brief  RNG_GetRandomValue
 */
uint8_t RNG_GetRandomValue( RNG_InitTypeDef *hrng )
{
  while (RNG_EVENTS_VALRDY() != SET);
  RNG_EVENTS_VALRDY() = RESET;
  return RNG_VALUE();
}

/*************************************** END OF FILE ****************************************/
