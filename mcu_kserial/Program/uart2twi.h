/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    uart2twi.h
 *  @author  KitSprout
 *  @date    Dec-2019
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __UART2TWI_H
#define __UART2TWI_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_i2cSimu.h"
#include "modules\kSerial.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

//typedef struct 
//{
//    int (*open)( serial_config_t *cfg );
//    void (*close)( serial_config_t *cfg );
//    int (*sendbyte)( serial_config_t *cfg, uint8_t sendByte );
//    int (*recvbyte)( serial_config_t *cfg );
//    int (*send)( serial_config_t *cfg, uint8_t *sendData, uint32_t lens );
//    int (*recv)( serial_config_t *cfg, uint8_t *recvData, uint32_t lens );
//    void (*delay)( uint32_t delayms );

//} serial_t;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void    uart2twi_init( void );
void    uart2twi_run( uint8_t param[2], uint32_t type, uint32_t lens, uint8_t *rdata );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
