/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_delay.c
 *  @author  KitSprout
 *  @date    28-May-2018
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_system.h"
#include "drivers\nrf5x_spim.h"

/** @addtogroup NRF5x_Driver
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  SPIM_Init
 */
void SPIM_Init( SPI_InitTypeDef *hspi )
{
  uint32_t config = 0;

  nrf_gpio_cfg_output(hspi->PinCSN);
  nrf_gpio_pin_set(hspi->PinCSN);

  // sck
  if ((hspi->Mode == NRF_SPIM_MODE_0) || (hspi->Mode == NRF_SPIM_MODE_1)) {
    nrf_gpio_pin_clear(hspi->PinSCK);
  }
  else {
    nrf_gpio_pin_set(hspi->PinSCK);
  }
  nrf_gpio_cfg(
    hspi->PinSCK,
    NRF_GPIO_PIN_DIR_OUTPUT,
    NRF_GPIO_PIN_INPUT_CONNECT,
    NRF_GPIO_PIN_NOPULL,
    NRF_GPIO_PIN_S0S1,
    NRF_GPIO_PIN_NOSENSE
  );

  // mosi
  nrf_gpio_pin_clear(hspi->PinMOSI);
  nrf_gpio_cfg_output(hspi->PinMOSI);

  // miso
//  nrf_gpio_cfg_input(hspi->PinMISO, NRF_GPIO_PIN_NOPULL);
  nrf_gpio_cfg_input(hspi->PinMISO, NRF_GPIO_PIN_PULLDOWN);
//  nrf_gpio_cfg_input(hspi->PinMISO, NRF_GPIO_PIN_PULLUP);

  hspi->Instance->PSELSCK   = hspi->PinSCK;
  hspi->Instance->PSELMOSI  = hspi->PinMOSI;
  hspi->Instance->PSELMISO  = hspi->PinMISO;
  hspi->Instance->FREQUENCY = hspi->Freguency;

  config = hspi->BitOrder << SPIM_CONFIG_ORDER_Pos;
  switch (hspi->Mode) {
    case NRF_SPIM_MODE_0: {
      config |= (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) |
                (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos);
      break;
    }
    case NRF_SPIM_MODE_1: {
      config |= (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) |
                (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos);
      break;
    }
    case NRF_SPIM_MODE_2: {
      config |= (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) |
                (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos);
      break;
    }
    case NRF_SPIM_MODE_3: {
      config |= (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) |
                (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos);
      break;
    }
  }
  hspi->Instance->CONFIG    = config;
}

/**
 *  @brief  SPIM_Cmd
 */
void SPIM_Cmd( SPI_InitTypeDef *hspi, uint32_t state )
{
  if (state == ENABLE) {
    hspi->Instance->ENABLE = (SPIM_ENABLE_ENABLE_Enabled << SPIM_ENABLE_ENABLE_Pos);
  }
  else {
    hspi->Instance->ENABLE = (SPIM_ENABLE_ENABLE_Disabled << SPIM_ENABLE_ENABLE_Pos);
  }
}

/**
 *  @brief  SPIM_InterruptCmd
 */
void SPIM_InterruptCmd( SPI_InitTypeDef *hspi, uint32_t mode, uint32_t state )
{
  if (state == ENABLE) {
    hspi->Instance->INTENSET = mode;
  }
  else {
    hspi->Instance->INTENCLR = mode;
  }
}

/**
 *  @brief  SPIM_Set_TxBuffer
 */
void SPIM_Set_TxBuffer( SPI_InitTypeDef *hspi, uint8_t *buffer, uint32_t lens )
{
  hspi->Instance->TXD.PTR = (uint32_t)buffer;
  hspi->Instance->TXD.MAXCNT = lens;
}

/**
 *  @brief  SPIM_Set_RxBuffer
 */
void SPIM_Set_RxBuffer( SPI_InitTypeDef *hspi, uint8_t *buffer, uint32_t lens )
{
  hspi->Instance->RXD.PTR = (uint32_t)buffer;
  hspi->Instance->RXD.MAXCNT = lens;
}

/**
 *  @brief  SPIM_SendByte
 */
void SPIM_SendByte( SPI_InitTypeDef *hspi, uint8_t *sendByte )
{
  SPIM_EVENTS_ENDTX(hspi->Instance) = RESET;
  hspi->Instance->TXD.PTR    = (uint32_t)sendByte;
  hspi->Instance->TXD.MAXCNT = 1;
  SPIM_TASKS_START(hspi->Instance);
  while (SPIM_EVENTS_ENDTX(hspi->Instance) != SET);
}

/**
 *  @brief  SPIM_RecvByte
 */
void SPIM_RecvByte( SPI_InitTypeDef *hspi, uint8_t *recvByte )
{
  SPIM_EVENTS_ENDRX(hspi->Instance) = RESET;
  hspi->Instance->RXD.PTR    = (uint32_t)recvByte;
  hspi->Instance->RXD.MAXCNT = 1;
  SPIM_TASKS_START(hspi->Instance);
  while (SPIM_EVENTS_ENDRX(hspi->Instance) != SET);
}

/**
 *  @brief  SPI_RW
 */
uint8_t SPI_RW( SPI_InitTypeDef *hspi, uint8_t byte )
{
  uint8_t readByte;

  SPIM_EVENTS_ENDTX(hspi->Instance) = RESET;
  SPIM_EVENTS_ENDRX(hspi->Instance) = RESET;
  hspi->Instance->TXD.PTR    = (uint32_t)&byte;
  hspi->Instance->TXD.MAXCNT = 1;
  hspi->Instance->RXD.PTR    = (uint32_t)&readByte;
  hspi->Instance->RXD.MAXCNT = 1;
  SPIM_TASKS_START(hspi->Instance);
  while ((SPIM_EVENTS_ENDTX(hspi->Instance) != SET) || (SPIM_EVENTS_ENDRX(hspi->Instance) != SET) || (SPIM_EVENTS_END(hspi->Instance) != SET));

  return readByte;
}


/*************************************** END OF FILE ****************************************/
