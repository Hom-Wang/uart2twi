/**
 *  @file   ist_parameter.h
 *  @author iSentek
 *  @date   Aug-2018 v2.0.9
 *  @brief  iSentek library
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __IST_PARAMETER_H
#define __IST_PARAMETER_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"
#include "drivers\nrf5x_flash.h"

/* Define ----------------------------------------------------------------------------------*/
#define IST_PARAMETER_SAVE_ADDRESS                  (FLASH_PAGE_SIZE * (FLASH_SIZE - 1))

#define IST_PARAMETER_DATA_LENS                     40
#define IST_PARAMETER_INIT                          \
{                                                   \
    .header         = 0xF52832A0,                   \
    .gyrCali        = {1, 0, 0, 0, 1, 0, 0, 0, 1},  \
    .gyrBias        = {0, 0, 0},                    \
    .accCali        = {1, 0, 0, 0, 1, 0, 0, 0, 1},  \
    .accBias        = {0, 0, 0},                    \
    .accGravity     = 9.8,                          \
    .magCali        = {1, 0, 0, 0, 1, 0, 0, 0, 1},  \
    .magBias        = {0, 0, 0},                    \
    .magGeomagnetic = 50,                           \
    .crc32          = 0                             \
}                                                   \

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct
{
    uint32_t  header;
    float32_t gyrCali[9];
    float32_t gyrBias[3];
    float32_t accCali[9];
    float32_t accBias[3];
    float32_t accGravity;
    float32_t magCali[9];
    float32_t magBias[3];
    float32_t magGeomagnetic;
    uint32_t  crc32;

} __attribute__((aligned(4))) ist_parameter_t;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      istparam_set_default( ist_parameter_t *param );
uint32_t  istparam_load( ist_parameter_t *param );
uint32_t  istparam_save( ist_parameter_t *param );
void      istparam_setting( ist_parameter_t *param );
void      istparam_conv_ned2enu( float cvCali[3][9], float cvBias[3][4], ist_parameter_t *param );
void      istparam_print( ist_parameter_t *param );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
