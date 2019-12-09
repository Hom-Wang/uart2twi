/**
 *  @file   ist_command.c
 *  @author iSentek
 *  @date   Nov-2018 v1.0.0
 *  @brief  iSentek command
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"
#include "drivers\nrf5x_radio.h"
#include "modules\serial.h"
#include "nrf5x_bsp.h"

#include "ist_parameter.h"
#include "ist_command.h"

/** @addtogroup KS_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/

#define CMD_NULL                          NULL

//#define CMD_SET_FREQ_5Hz                  '1'
//#define CMD_SET_FREQ_10Hz                 '2'
//#define CMD_SET_FREQ_20Hz                 '3'
//#define CMD_SET_FREQ_40Hz                 '4'
//#define CMD_SET_FREQ_50Hz                 '5'
//#define CMD_SET_FREQ_100Hz                '6'

//#define CMD_LOG                           'L'
//#define CMD_KSERIAL                       'K'
#define CMD_PARAM                         'P'
#define CMD_STOP                          ' '

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
extern uint8_t logcmd;
extern ist_parameter_t istparam;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t istcmd_run( uint8_t *cmd )
{
    uint8_t command;

    command = ((*cmd < 'a') || (*cmd > 'z')) ? *cmd : *cmd - 32;
    *cmd = CMD_NULL;

    switch (command)
    {
        case CMD_STOP:
        {
            logcmd = !logcmd;
            break;
        }
        case CMD_PARAM:
        {
            istparam_print(&istparam);
            delay_ms(1000);
            break;
        }
    }

    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
