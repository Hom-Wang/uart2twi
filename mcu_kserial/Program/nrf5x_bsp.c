/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    nrf5x_bsp.c
 *  @author  KitSprout
 *  @date    Feb-2019
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"
#include "drivers\nrf5x_clock.h"
#include "modules\serial.h"
#include "nrf5x_bsp.h"

#include "ist_parameter.h"

/** @addtogroup KS_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void bsp_gpio_init( void )
{
    CLOCK_Config();

    nrf_gpio_cfg_output(LED_R_PIN);
    nrf_gpio_cfg_output(LED_G_PIN);
    nrf_gpio_cfg_output(LED_B_PIN);
    nrf_gpio_cfg_input(KEY_PIN, NRF_GPIO_PIN_PULLUP);

    LED_R_Off();
    LED_G_Off();
    LED_B_Off();
}

void bsp_uart_serial_init( pFunc event )
{
    hserial.RxEventCallback = event;

    Serial_Config();
    printf("\r\n\f\r\n");
}

/*************************************** END OF FILE ****************************************/
