/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    boardConfig.h
 *  @author  KitSprout
 *  @date    Aug-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
/* Define ----------------------------------------------------------------------------------*/

#define KS_HW_BOARD_NAME              "IST_IMU"
#define KS_HW_MCU_NAME                "NRF52832"

#define KS_HW_USE_CLOCK_SOUCE_INT     (0U)
#define KS_HW_USE_CLOCK_SOUCE_EXT     (1U)
#define KS_HW_HCLOCK_SOUCE            KS_HW_USE_CLOCK_SOUCE_EXT
#define KS_HW_LCLOCK_SOUCE            KS_HW_USE_CLOCK_SOUCE_INT


/* -------- LED and KEY */

#define LED_R_PIN                     6
#define LED_R_PORT                    NRF_P0
#define LED_R_Set()                   __GPIO_SET(LED_R_PORT, LED_R_PIN)
#define LED_R_Reset()                 __GPIO_RST(LED_R_PORT, LED_R_PIN)
#define LED_R_Toggle()                __GPIO_TOG(LED_R_PORT, LED_R_PIN)
#define LED_R_On()                    LED_R_Reset()
#define LED_R_Off()                   LED_R_Set()

#define LED_G_PIN                     7
#define LED_G_PORT                    NRF_P0
#define LED_G_Set()                   __GPIO_SET(LED_G_PORT, LED_G_PIN)
#define LED_G_Reset()                 __GPIO_RST(LED_G_PORT, LED_G_PIN)
#define LED_G_Toggle()                __GPIO_TOG(LED_G_PORT, LED_G_PIN)
#define LED_G_On()                    LED_G_Reset()
#define LED_G_Off()                   LED_G_Set()

#define LED_B_PIN                     8
#define LED_B_PORT                    NRF_P0
#define LED_B_Set()                   __GPIO_SET(LED_B_PORT, LED_B_PIN)
#define LED_B_Reset()                 __GPIO_RST(LED_B_PORT, LED_B_PIN)
#define LED_B_Toggle()                __GPIO_TOG(LED_B_PORT, LED_B_PIN)
#define LED_B_On()                    LED_B_Reset()
#define LED_B_Off()                   LED_B_Set()

#define KEY_PIN                       5
#define KEY_PORT                      NRF_P0
#define KEY_Read()                    (__GPIO_READ(KEY_PORT, KEY_PIN) == RESET)

#define KEY_GPIOTEx                   NRF_GPIOTE
#define KEY_GPIOTEx_LINE              0
#define KEY_GPIOTEx_IRQn              GPIOTE_IRQn
#define KEY_GPIOTEx_IRQn_PRIORITY     1
#define KEY_GPIOTEx_INT_LINE          NRF_GPIOTE_INT_IN0_MASK
#define KEY_GPIOTEx_POLARITY          NRF_GPIOTE_POLARITY_HITOLO
#define KEY_GPIOTEx_MODE              GPIOTE_CONFIG_MODE_Event
#define KEY_GPIOTEx_OUTINIT           NRF_GPIOTE_INITIAL_VALUE_LOW


/* -------- RNG */

#define RNGx                          NRF_RNG
#define RNGx_IRQn                     RNG_IRQn
#define RNGx_IRQn_PRIORITY            1


/* -------- Timer */

#define TIMERx                        NRF_TIMER0
#define TIMERx_IRQn                   TIMER0_IRQn
#define TIMERx_IRQn_PRIORITY          1
#define TIMERx_MODE                   NRF_TIMER_MODE_TIMER
#define TIMERx_PRESCALER              NRF_TIMER_FREQ_1MHz
#define TIMERx_BIT_MODE               NRF_TIMER_BIT_WIDTH_32

#define TIMERx_CHANNEL                NRF_TIMER_CC_CHANNEL0
#define TIMERx_CHANNEL_PERIOD         1000


/* -------- Serial */

#define SERIAL_UARTx                  NRF_UART0
#define SERIAL_UARTx_IRQn             UARTE0_UART0_IRQn
#define SERIAL_UARTx_IRQn_PRIORITY    1

#define SERIAL_TXD_PIN                3
#define SERIAL_RXD_PIN                4
#define SERIAL_RTS_PIN                9
#define SERIAL_CTS_PIN                10

#define SERIAL_BAUDRATE               NRF_UART_BAUDRATE_115200
//#define SERIAL_BAUDRATE               NRF_UART_BAUDRATE_230400
//#define SERIAL_BAUDRATE               NRF_UART_BAUDRATE_250000
//#define SERIAL_BAUDRATE               NRF_UART_BAUDRATE_460800
//#define SERIAL_BAUDRATE               NRF_UART_BAUDRATE_921600
//#define SERIAL_BAUDRATE               NRF_UART_BAUDRATE_1000000
#define SERIAL_PARITY                 NRF_UART_PARITY_EXCLUDED
#define SERIAL_HARDWARECTRL           NRF_UART_HWFC_DISABLED
#define SERIAL_INTERRUPT_MODE         NRF_UART_INT_MASK_RXDRDY


/* -------- IMU Sensor */

#define IMU_SCL_PIN                   17
#define IMU_SCL_PORT                  NRF_P0

#define IMU_SDA_PIN                   16
#define IMU_SDA_PORT                  NRF_P0


/* -------- RADIO */

#define RADIOx                        NRF_RADIO
#define RADIO_RF_FREQ                 22UL   // 2422 MHz
#define RADIO_TX_POWER                RADIO_TXPOWER_TXPOWER_Pos4dBm
#define RADIO_SPEED                   RADIO_MODE_MODE_Nrf_250Kbit
#define RADIO_CRC                     RADIO_CRCCNF_LEN_Two


/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
