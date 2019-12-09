/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_gpiote.h
 *  @author  KitSprout
 *  @date    Jul-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5x_GPIOE_H
#define __NRF5x_GPIOE_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define GPIOTE_TASKS_OUT(__INS, __CH)   __INS->TASKS_OUT[__CH]    // Task for writing to pin specified in CONFIG[x].PSEL. Action on pin is configured in CONFIG[7].POLARITY.
#define GPIOTE_TASKS_SET(__INS, __CH)   __INS->TASKS_SET[__CH]    // Task for writing to pin specified in CONFIG[x].PSEL. Action on pin is to set it high.
#define GPIOTE_TASKS_CLR(__INS, __CH)   __INS->TASKS_CLR[__CH]    // Task for writing to pin specified in CONFIG[x].PSEL. Action on pin is to set it low.
#define GPIOTE_TASKS_CLEAR(__INS)       __INS->TASKS_CLEAR        // Clear time

#define GPIOTE_EVENTS_IN(__INS, __CH)   __INS->EVENTS_IN[__CH]    // Event generated from pin specified in CONFIG[x].PSEL

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {
  NRF_GPIOTE_Type *Instance;

  uint8_t Line;
  // Line 0-3
  uint32_t Pin;
  // Pin 0-31
  uint32_t Mode;
  // GPIOTE_CONFIG_MODE_Disabled - Disabled.
  // GPIOTE_CONFIG_MODE_Event    - Channel configure in event mode.
  // GPIOTE_CONFIG_MODE_Task     - Channel configure in task mode.
  uint32_t Polarity;
  // NRF_GPIOTE_POLARITY_LOTOHI - Low to high.
  // NRF_GPIOTE_POLARITY_HITOLO - High to low.
  // NRF_GPIOTE_POLARITY_TOGGLE - Toggle.
  uint32_t OutInit;
  // NRF_GPIOTE_INITIAL_VALUE_LOW  - Initial low output when in task mode.
  // NRF_GPIOTE_INITIAL_VALUE_HIGH - Initial high output when in task mode.
  pFunc EventCallback;
  // GPIOTE EVENT CALLBACK
} GPIOTE_InitTypeDef;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void GPIOTE_Init( GPIOTE_InitTypeDef *hgpiotex );
void GPIOTE_InterruptCmd( GPIOTE_InitTypeDef *hgpiotex, uint32_t state );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
