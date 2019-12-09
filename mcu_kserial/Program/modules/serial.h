/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    serial.h
 *  @author  KitSprout
 *  @date    Aug-2018
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
#include "drivers\ks_system.h"
#include "drivers\nrf5x_uart.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define Serial_Start()      UART_Start(&hserial)
#define Serial_Stop()       UART_Stop(&hserial)

/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
extern    UART_InitTypeDef hserial;

/* Functions -------------------------------------------------------------------------------*/
void      Serial_Config( void );

void      Serial_SendByte( uint8_t sendByte );
uint8_t   Serial_RecvByte( void );
uint32_t  Serial_SendData( uint8_t *sendData, uint32_t lens );
uint32_t  Serial_RecvData( uint8_t *recvData, uint32_t lens );
uint32_t  Serial_SendDataWaitTimeout( uint8_t *sendData, uint32_t lens, uint32_t timuout );
uint32_t  Serial_RecvDataWaitTimeout( uint8_t *recvData, uint32_t lens, uint32_t timeout );
uint32_t  Serial_RecvDataContinuous( uint8_t *recvData );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
