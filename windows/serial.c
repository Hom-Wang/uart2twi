/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    serial.c
 *  @author  KitSprout
 *  @date    Dec-2019
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "serial.h"

/** @addtogroup KS_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void Serial_Config( serial_t *serial )
{
    serial->open = Serial_Open;
    serial->close = Serial_Close;
    serial->sendbyte = Serial_SendByte;
    serial->recvbyte = Serial_RecvByte;
    serial->send = Serial_SendData;
    serial->recv = Serial_RecvData;
    serial->delay = Serial_DelayMs;

    RS232_flushRXTX(serial->cfg.port);
}

int Serial_Open( serial_config_t *cfg )
{
    return !RS232_OpenComport(cfg->port, cfg->baudrate, cfg->mode);
}

void Serial_Close( serial_config_t *cfg )
{
    RS232_CloseComport(cfg->port);
}

void Serial_flush( serial_config_t *cfg )
{
    RS232_flushRXTX(cfg->port);
}

int Serial_SendByte( serial_config_t *cfg, uint8_t sendByte )
{
    return RS232_SendByte(cfg->port, sendByte);
}

int Serial_RecvByte( serial_config_t *cfg )
{
    uint8_t recv;
    RS232_PollComport(cfg->port, &recv, 1);
    return recv;
}

int Serial_SendData( serial_config_t *cfg, uint8_t *sendData, uint32_t lens )
{
    return RS232_SendBuf(cfg->port, sendData, lens);
}

int Serial_RecvData( serial_config_t *cfg, uint8_t *recvData, uint32_t lens )
{
    return RS232_PollComport(cfg->port, recvData, lens);
}

void Serial_DelayMs( uint32_t delayms )
{
    Sleep(delayms);
}

/*************************************** END OF FILE ****************************************/
