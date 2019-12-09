/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kSerial.h
 *  @author  KitSprout
 *  @date    Apr-2019
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __SERIAL_H
#define __SERIAL_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <windows.h>
#include <stdint.h>
#include "rs232.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct 
{
    int port;
    int baudrate;
    char mode[4];

} serial_config_t;

typedef struct 
{
    serial_config_t cfg;

    int (*open)( serial_config_t *cfg );
    void (*close)( serial_config_t *cfg );
    int (*sendbyte)( serial_config_t *cfg, uint8_t sendByte );
    int (*recvbyte)( serial_config_t *cfg );
    int (*send)( serial_config_t *cfg, uint8_t *sendData, uint32_t lens );
    int (*recv)( serial_config_t *cfg, uint8_t *recvData, uint32_t lens );
    void (*delay)( uint32_t delayms );

} serial_t;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void      Serial_Config( serial_t *serial );
int       Serial_Open( serial_config_t *cfg );
void      Serial_Close( serial_config_t *cfg );
void      Serial_flush( serial_config_t *cfg );

int       Serial_SendByte( serial_config_t *cfg, uint8_t sendByte );
int       Serial_RecvByte( serial_config_t *cfg );
int       Serial_SendData( serial_config_t *cfg, uint8_t *sendData, uint32_t lens );
int       Serial_RecvData( serial_config_t *cfg, uint8_t *recvData, uint32_t lens );

void      Serial_DelayMs( uint32_t delayms );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
