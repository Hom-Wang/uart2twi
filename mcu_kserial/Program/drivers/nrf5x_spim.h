/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_spim.h
 *  @author  KitSprout
 *  @date    28-May-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5X_SPIM_H
#define __NRF5X_SPIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define SPIM_TASKS_START(__INS)       (__INS->TASKS_START = SET)      // Start SPI transaction
#define SPIM_TASKS_STOP(__INS)        (__INS->TASKS_STOP = SET)       // Stop SPI transaction
#define SPIM_TASKS_SUSPEND(__INS)     (__INS->TASKS_SUSPEND = SET)    // Suspend SPI transaction
#define SPIM_TASKS_RESUME(__INS)      (__INS->TASKS_RESUME = SET)     // Resume SPI transaction

#define SPIM_EVENTS_STOPPED(__INS)    __INS->EVENTS_STOPPED           // SPI transaction has stopped
#define SPIM_EVENTS_ENDRX(__INS)      __INS->EVENTS_ENDRX             // End of RXD buffer reached
#define SPIM_EVENTS_END(__INS)        __INS->EVENTS_END               // End of RXD buffer and TXD buffer reached
#define SPIM_EVENTS_ENDTX(__INS)      __INS->EVENTS_ENDTX             // End of TXD buffer reached
#define SPIM_EVENTS_STARTED(__INS)    __INS->EVENTS_STARTED           // Transaction started

/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {
  NRF_SPIM_Type *Instance;

  uint8_t  PinCSN;
  uint8_t  PinSCK;
  uint8_t  PinMOSI;
  uint8_t  PinMISO;
  uint8_t BitOrder;
  // NRF_SPIM_BIT_ORDER_MSB_FIRST - Most significant bit shifted out first.
  // NRF_SPIM_BIT_ORDER_LSB_FIRST - Least significant bit shifted out first.
  uint8_t Mode;
  // NRF_SPIM_MODE_0 - SCK active high (CPOL), sample on leading  edge of clock (CPHA).
  // NRF_SPIM_MODE_1 - SCK active high (CPOL), sample on trailing edge of clock (CPHA).
  // NRF_SPIM_MODE_2 - SCK active low  (CPOL), sample on leading  edge of clock (CPHA).
  // NRF_SPIM_MODE_3 - SCK active low  (CPOL), sample on trailing edge of clock (CPHA).
  uint32_t Freguency;
  // NRF_SPIM_FREQ_125K - 125kbps.
  // NRF_SPIM_FREQ_250K - 250kbps.
  // NRF_SPIM_FREQ_500K - 500kbps.
  // NRF_SPIM_FREQ_1M   - 1Mbps.
  // NRF_SPIM_FREQ_2M   - 2Mbps.
  // NRF_SPIM_FREQ_4M   - 4Mbps.
  // NRF_SPIM_FREQ_8M   - 8Mbps.
} SPI_InitTypeDef;

/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      SPIM_Init( SPI_InitTypeDef *hspi );
void      SPIM_Cmd( SPI_InitTypeDef *hspi, uint32_t state );
void      SPIM_InterruptCmd( SPI_InitTypeDef *hspi, uint32_t mode, uint32_t state );
void      SPIM_Set_TxBuffer( SPI_InitTypeDef *hspi, uint8_t *buffer, uint32_t lens );
void      SPIM_Set_RxBuffer( SPI_InitTypeDef *hspi, uint8_t *buffer, uint32_t lens );
void      SPIM_SendByte( SPI_InitTypeDef *hspi, uint8_t *sendByte );
void      SPIM_RecvByte( SPI_InitTypeDef *hspi, uint8_t *recvByte );
uint8_t   SPI_RW( SPI_InitTypeDef *hspi, uint8_t byte );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
