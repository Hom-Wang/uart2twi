/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_gpiote.c
 *  @author  KitSprout
 *  @date    Jul-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_gpiote.h"

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
 *  @brief  GPIOTE_Init
 */
void GPIOTE_Init( GPIOTE_InitTypeDef *hgpiotex )
{
  hgpiotex->Instance->CONFIG[hgpiotex->Line] = (hgpiotex->Pin      << GPIOTE_CONFIG_PSEL_Pos) | 
                                               (hgpiotex->Mode     << GPIOTE_CONFIG_MODE_Pos) | 
                                               (hgpiotex->Polarity << GPIOTE_CONFIG_POLARITY_Pos) | 
                                               (hgpiotex->OutInit  << GPIOTE_CONFIG_OUTINIT_Pos);
}

/**
 *  @brief  GPIOTE_InterruptCmd
 */
void GPIOTE_InterruptCmd( GPIOTE_InitTypeDef *hgpiotex, uint32_t state )
{
  if (state == ENABLE) {
    hgpiotex->Instance->INTENSET = 1UL << hgpiotex->Line;
  }
  else {
    hgpiotex->Instance->INTENCLR = 1UL << hgpiotex->Line;
  }
}

/*************************************** END OF FILE ****************************************/
