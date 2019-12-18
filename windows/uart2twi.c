/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    uart2twi.c
 *  @author  KitSprout
 *  @date    Dec-2019
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdio.h>

#include "uart2twi.h"

/** @addtogroup KS_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define KSERIAL_RW_BUFFER_SIZE  4096

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
uint8_t i2cbuff[2][KSERIAL_RW_BUFFER_SIZE] = {0};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

void i2c_delay( serial_t *serial, uint32_t delay )
{
    serial->delay(delay);
}

uint32_t i2c_write( serial_t *serial, const uint8_t address, const uint8_t reg, uint8_t wdata )
{
    uint8_t param[2] = {address << 1, reg};
    uint32_t type = KS_R1;
    uint32_t lens;

    lens = kSerial_Pack(i2cbuff[0], param, type, 1, &wdata);
    serial->send(&serial->cfg, i2cbuff[0], lens);
#if 0
    printf("[W] param = %02X, %02X, type = %d, bytes = %d, data = %02X\n", param[0], param[1], type, lens, wdata);
#endif

    return lens;
}

#define READ_WAIT_TIME 100
uint32_t i2c_read( serial_t *serial, const uint8_t address, const uint8_t reg, uint8_t *rdata, uint8_t lenght, const uint32_t timeout )
{
    uint8_t param[2] = {(address << 1) + 1, reg};
    uint32_t type = KS_R1;
    uint32_t lens;
    uint32_t status;
    uint32_t timecount = 0;

    Serial_flush(&serial->cfg);

    lens = kSerial_Pack(i2cbuff[0], param, type, 1, &lenght);
    serial->send(&serial->cfg, i2cbuff[0], lens);

    lens = 0;
    while (lens == 0)
    {
        i2c_delay(serial, READ_WAIT_TIME);
        lens = serial->recv(&serial->cfg, i2cbuff[0], KSERIAL_RW_BUFFER_SIZE);
        if (timecount > timeout)
        {
            return KS_TIMEOUT;
        }
        timecount += READ_WAIT_TIME;
    }

    // TODO: check i2cbuff first 'KS'
    status = kSerial_Unpack(i2cbuff[0], param, &type, &lens, i2cbuff[1]);
    if (status == KS_OK)
    {
        for (uint32_t i = 0; i < lens; i++)
        {
            rdata[i] = i2cbuff[1][i];
        }
#if 0
        printf("[R] param = %02X, %02X, type = %d, bytes = %d, data =", param[0], param[1], type, lens + 8);
        for (uint32_t i = 0; i < lens; i++)
        {
            printf(" %02X", i2cbuff[1][i]);
        }
        printf("\n");
#endif
    }

    return status;
}

uint32_t i2c_scandevice( serial_t *serial, uint8_t *address, uint32_t delay )
{
    uint8_t param[2] = {0xAB, 0};
    uint32_t type = KS_R2;
    uint32_t lens;
    uint32_t status;

    Serial_flush(&serial->cfg);

    lens = kSerial_Pack(i2cbuff[0], param, type, 0, NULL);
    serial->send(&serial->cfg, i2cbuff[0], lens);
    i2c_delay(serial, delay);
    lens = serial->recv(&serial->cfg, i2cbuff[0], KSERIAL_RW_BUFFER_SIZE);

    // TODO: check i2cbuff first 'KS'
    status = kSerial_Unpack(i2cbuff[0], param, &type, &lens, i2cbuff[1]);
    if (status == KS_OK)
    {
        for (uint32_t i = 0; i < lens; i++)
        {
            address[i] = i2cbuff[1][i];
        }
#if 0
        printf("\n");
        printf(" >> i2c device list (found %d device)\n\n", lens);
        printf("    ");
        for (uint32_t i = 0; i < lens; i++)
        {
            printf(" %02X", i2cbuff[1][i]);
        }
        printf("\n\n");
#endif
    }
    else
    {
        lens = 255;
    }

    return lens;
}

uint32_t i2c_scanregister( serial_t *serial, uint8_t address, uint8_t reg[256], uint32_t delay )
{
    uint8_t param[2] = {0xCB, address << 1};
    uint32_t type = KS_R2;
    uint32_t lens;
    uint32_t status;

    Serial_flush(&serial->cfg);

    lens = kSerial_Pack(i2cbuff[0], param, type, 0, NULL);
    serial->send(&serial->cfg, i2cbuff[0], lens);
    i2c_delay(serial, delay);
    lens = serial->recv(&serial->cfg, i2cbuff[0], KSERIAL_RW_BUFFER_SIZE);

    // TODO: check i2cbuff first 'KS'
    status = kSerial_Unpack(i2cbuff[0], param, &type, &lens, i2cbuff[1]);
    if (status == KS_OK)
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            reg[i] = i2cbuff[1][i];
        }
#if 0
        printf("\n");
        printf(" >> i2c device register (address 0x%02X)\n\n", address);
        printf("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
        for (uint32_t i = 0; i < 256; i += 16)
        {
            printf(" %02X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                i,
                i2cbuff[1][i +  0], i2cbuff[1][i +  1], i2cbuff[1][i +  2], i2cbuff[1][i +  3],
                i2cbuff[1][i +  4], i2cbuff[1][i +  5], i2cbuff[1][i +  6], i2cbuff[1][i +  7],
                i2cbuff[1][i +  8], i2cbuff[1][i +  9], i2cbuff[1][i + 10], i2cbuff[1][i + 11],
                i2cbuff[1][i + 12], i2cbuff[1][i + 13], i2cbuff[1][i + 14], i2cbuff[1][i + 15]
            );
        }
        printf("\n\n");
#endif
    }

    return status;
}

/*************************************** END OF FILE ****************************************/
