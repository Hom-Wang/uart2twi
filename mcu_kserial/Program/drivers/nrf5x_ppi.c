/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_ppi.c
 *  @author  KitSprout
 *  @date    Jul-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_ppi.h"

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
 *  @brief  PPI_Init
 */
void PPI_Init( PPI_InitTypeDef *hclock )
{
  PPIx->CH[hclock->Channel].EEP = hclock->EventEndPoint;
  PPIx->CH[hclock->Channel].TEP = hclock->TaskEndPoint;
  if (hclock->ForkState == ENABLE) {
    PPIx->FORK[hclock->Channel].TEP = hclock->ForkTaskEndPoint;
  }
}

/**
 *  @brief  PPI_SetChannelGroup
 */
void PPI_SetChannelGroup( PPI_InitTypeDef *hclock, uint32_t state )
{
  if (state != ENABLE) {
    PPIx->CHG[hclock->Group] = PPIx->CHG[hclock->Group] & ~(1UL << hclock->Channel);
  }
  else {
    PPIx->CHG[hclock->Group] = PPIx->CHG[hclock->Group] |  (1UL << hclock->Channel);
  }
}

/**
 *  @brief  PPI_ChannelCmd
 */
void PPI_ChannelCmd( PPI_InitTypeDef *hclock, uint32_t state )
{
  if (state != ENABLE) {
    PPIx->CHENCLR = 1UL << hclock->Channel;
  }
  else {
    PPIx->CHENSET = 1UL << hclock->Channel;
  }
}

/**
 *  @brief  PPI_GroupCmd
 */
void PPI_GroupCmd( PPI_InitTypeDef *hclock, uint32_t state )
{
  if (state != ENABLE) {
    PPI_TASKS_CHG_DIS(hclock->Group) = SET;
  }
  else {
    PPI_TASKS_CHG_EN(hclock->Group) = SET;
  }
}

/*************************************** END OF FILE ****************************************/
