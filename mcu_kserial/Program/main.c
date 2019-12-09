/**
 *  @file   main.c
 *  @author KitSprout
 *  @date   Dec-2019
 *  @brief  main function
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"
#include "drivers\nrf5x_i2cSimu.h"
#include "modules\serial.h"
#include "modules\kSerial.h"
#include "nrf5x_bsp.h"

#include "uart2twi.h"

/** @addtogroup KS_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
void ist_event_command( void );

/* Functions -------------------------------------------------------------------------------*/

void bsp_init( void )
{
    bsp_gpio_init();
    bsp_uart_serial_init(ist_event_command);

    uart2twi_init();
}

int main( void )
{
    bsp_init();

    while (1)
    {
//        LED_G_Toggle();
//        delay_ms(100);
        if (KEY_Read())
        {
//            SimuI2C_ScanAll(IMU_SCL_PIN, IMU_SDA_PIN);
            LED_G_Toggle();
            delay_ms(100);
        }
    }
}

uint32_t status;
uint8_t param[2] = {0};
uint8_t rdata[KS_MAX_SEND_BUFF_SIZE] = {0};
uint32_t lens;
uint32_t type;

void ist_event_command( void )
{
#if 0
    Serial_SendByte(Serial_RecvByte());
//    status = kSerial_RecvPacket(param, rdata, &lens, &type);
//    if (status == KS_OK)
//    {
//        LED_R_Toggle();
//        kSerial_SendPacket(param, rdata, lens, type);
//    }
#else
    status = kSerial_RecvPacket(param, rdata, &lens, &type);
    if (status == KS_OK)
    {
        uart2twi_run(param, type, lens, rdata);
    }
#endif
}

/*************************************** END OF FILE ****************************************/
