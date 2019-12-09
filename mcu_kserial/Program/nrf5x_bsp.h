/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_bsp.h
 *  @author  KitSprout
 *  @date    Aug-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5X_BSP_H
#define __NRF5X_BSP_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"
#include "ist_parameter.h"

/* Define ----------------------------------------------------------------------------------*/

#define KS_COORDINATE_NED           (1U)
#define KS_COORDINATE_ENU           (2U)
#define KS_COORDINATE_SYSTEM        KS_COORDINATE_ENU

#define TIMERx_TICK_FREQ            (1000)
#define KS_UPDATE_FREQ              (100)
#define KS_UPDATE_TIME              (1.0 / KS_UPDATE_FREQ)

#define KS_LOG_FREQ                 (20)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      bsp_gpio_init( void );
void      bsp_gpiote_key_init( pFunc event );
void      bsp_rng_init( pFunc event );
void      bsp_timer_init( pFunc event, uint32_t freq );
void      bsp_timer_enable( uint32_t state );
void      bsp_uart_serial_init( pFunc event );
void      bsp_radio_init( void );
void      bsp_sensor_init( void );

void      bsp_get_gyr_acc_rawdata( int16_t gyr[3], int16_t acc[3] );
void      bsp_get_gyr_acc_data( float gyr[3], float acc[3] );
uint32_t  bsp_get_mag_rawdata( int16_t mag[3] );
uint32_t  bsp_get_mag_data( float mag[3] );

void      bsp_flash_loading( ist_parameter_t *param );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
