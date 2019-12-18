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
#include <stdint.h>

#include "serial.h"
#include "kSerial.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void      i2c_delay( serial_t *serial, uint32_t delay );
uint32_t  i2c_write( serial_t *serial, const uint8_t address, const uint8_t reg, uint8_t wdata );
uint32_t  i2c_read( serial_t *serial, const uint8_t address, const uint8_t reg, uint8_t *rdata, uint8_t lenght, const uint32_t timeout );
uint32_t  i2c_scandevice( serial_t *serial, uint8_t *address, uint32_t delay );
uint32_t  i2c_scanregister( serial_t *serial, uint8_t address, uint8_t reg[256], uint32_t delay );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
