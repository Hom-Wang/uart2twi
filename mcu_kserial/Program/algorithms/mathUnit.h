/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    mathUnit.h
 *  @author  KitSprout
 *  @date    Aug-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __MATHUNIT_H
#define __MATHUNIT_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#if defined (NRF51)
  #include "core_cm0.h"
#else
  #include "core_cm4.h"
#endif
#include "arm_math.h"

/* Define ----------------------------------------------------------------------------------*/
#define RAD2DEG             57.2957795f
#define DEG2RAD             0.0174532925f

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef enum
{
    KS_U8   = 0x0,  /* 4'b 0000 */
    KS_U16  = 0x1,  /* 4'b 0001 */
    KS_U32  = 0x2,  /* 4'b 0010 */
    KS_U64  = 0x3,  /* 4'b 0011 */
    KS_I8   = 0x4,  /* 4'b 0100 */
    KS_I16  = 0x5,  /* 4'b 0101 */
    KS_I32  = 0x6,  /* 4'b 0110 */
    KS_I64  = 0x7,  /* 4'b 0111 */
    KS_F16  = 0x9,  /* 4'b 1001 */
    KS_F32  = 0xA,  /* 4'b 1010 */
    KS_F64  = 0xB,  /* 4'b 1011 */
    KS_R0   = 0x8,  /* 4'b 1000 */
    KS_R1   = 0xC,  /* 4'b 1100 */
    KS_R2   = 0xD,  /* 4'b 1101 */
    KS_R3   = 0xE,  /* 4'b 1110 */
    KS_R4   = 0xF,  /* 4'b 1111 */

} KSUnit_t;

typedef struct
{
    float x;
    float y;
    float z;
} __attribute__((aligned(4))) vector_t;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
float     invSqrtf_fast( const float __iSq );
uint32_t  compareArray( KSUnit_t type, const void *pArray1, const void *pArray2, uint32_t lens );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
