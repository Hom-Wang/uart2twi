/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    mathUnit.c
 *  @author  KitSprout
 *  @date    Aug-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"
#include "algorithms\mathUnit.h"

/** @addtogroup KS_Algorithm
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  invSqrtf_fast
 */
float invSqrtf_fast( const float __iSq )
{
  float x2 = 0.5f * __iSq;
  float y = __iSq;
  int32_t i = *(int32_t*)&y;
  i = 0x5F3759DF - (i >> 1);
  y = *(float*)&i;
  y = y * (1.5f - (x2 * y * y));
  return y;
}

/**
 *  @brief  compareArray
 */
uint32_t compareArray( KSUnit_t type, const void *pArray1, const void *pArray2, uint32_t lens )
{
  switch (type) {
    case KS_I8:
    case KS_U8:
      while (lens--) {
        if (((uint8_t *)pArray1)[lens] != ((uint8_t *)pArray2)[lens]) {
          return KS_ERROR;
        }
      }
      break;
    case KS_I16:
    case KS_U16:
    case KS_F16:
      while (lens--) {
        if (((uint16_t *)pArray1)[lens] != ((uint16_t *)pArray2)[lens]) {
          return KS_ERROR;
        }
      }
      break;
    case KS_I32:
    case KS_U32:
    case KS_F32:
      while (lens--) {
        if (((uint32_t *)pArray1)[lens] != ((uint32_t *)pArray2)[lens]) {
          return KS_ERROR;
        }
      }
      break;
    case KS_I64:
    case KS_U64:
    case KS_F64:
      while (lens--) {
        if (((uint64_t *)pArray1)[lens] != ((uint64_t *)pArray2)[lens]) {
          return KS_ERROR;
        }
      }
      break;
    case KS_R0:
    case KS_R1:
    case KS_R2:
    case KS_R3:
    case KS_R4:
      break;
  }
  return KS_OK;
}

/*************************************** END OF FILE ****************************************/
