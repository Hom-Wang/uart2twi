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

#include "uart2twi.h"

/** @addtogroup KS_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/

SimuI2C_InitTypeDef i2c;

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  kSerial_GetTypeSize
 */
static void i2c_read( uint8_t slaveAddr, uint8_t reg, uint8_t length, uint8_t *data )
{
    SimuI2C_ReadRegs(&i2c, slaveAddr >> 1, reg, data, length);
}

static void i2c_write( uint8_t slaveAddr, uint8_t reg, uint8_t data )
{
    SimuI2C_WriteReg(&i2c, slaveAddr >> 1, reg, data);
}

void uart2twi_init( void )
{
    i2c.PinSCL    = IMU_SCL_PIN;
    i2c.PortSCL   = IMU_SCL_PORT;
    i2c.PinSDA    = IMU_SDA_PIN;
    i2c.PortSDA   = IMU_SDA_PORT;
    i2c.Frequency = 6;  // 400kHz
    SimuI2C_Config(&i2c);
}

void uart2twi_run( uint8_t param[2], uint32_t type, uint32_t lens, uint8_t *rdata )
{
LED_R_Toggle();
    switch (type)
    {
        case KS_R0:
        {
            break;
        }

        // i2c write / read
        case KS_R1:
        {
            uint32_t slaveAddress;
            uint32_t regAddress;
            uint32_t lenght;

            slaveAddress = param[0];
            regAddress = param[1];

            if ((slaveAddress & 0x01) && (lens == 1))  //read
            {
LED_G_Toggle();
                lenght = rdata[0];
                i2c_read(slaveAddress, regAddress, lenght, rdata);
                kSerial_SendPacket(param, rdata, lenght, type);
            }
            else  // write
            {
                i2c_write(slaveAddress, regAddress, rdata[0]);
            }
            break;
        }

        // i2c device command
        case KS_R2:
        {
LED_B_Toggle();
            if (param[0] == 0xAB)
            {
                uint8_t address[128];
                uint8_t deviceNum;
                deviceNum = SimuI2C_ScanDevice(&i2c, address);
                kSerial_SendPacket(param, address, deviceNum, type);
            }
            else if (param[0] == 0xCB)
            {
                uint8_t address = param[1] >> 1;
                uint8_t regread[256] = {0};
                SimuI2C_ScanRegs(&i2c, address, regread);
                kSerial_SendPacket(param, regread, 256, KS_R2);
            }
            break;
        }

        // i2c continuous read
        case KS_R3:
        {
            break;
        }

        // system setting
        case KS_R4:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}

/*************************************** END OF FILE ****************************************/
