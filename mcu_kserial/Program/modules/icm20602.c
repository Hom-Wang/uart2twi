/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    icm20602.c
 *  @author  KitSprout
 *  @date    Feb-2019
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_i2cSimu.h"
#include "modules\icm20602.h"

/** @addtogroup NRF5x_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
#define ICM62_Delay(__MS)       delay_ms(__MS)

/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static SimuI2C_InitTypeDef hi2c;
static icm2060x_t icm =
{
    .i2cAddress   = ICM62_I2C_ADDR,
    .deviceId     = 0x00,
    .sensitivity  = {0},
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  ICM62_WriteReg
 */
void ICM62_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
    SimuI2C_WriteReg(&hi2c, icm.i2cAddress, writeAddr, writeData);
}

/**
 *  @brief  ICM62_WriteRegs
 */
void ICM62_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
    SimuI2C_WriteRegs(&hi2c, icm.i2cAddress, writeAddr, writeData, lens);
}

/**
 *  @brief  ICM62_ReadReg
 */
uint8_t ICM62_ReadReg( uint8_t readAddr )
{
    uint8_t readData;

    SimuI2C_ReadReg(&hi2c, icm.i2cAddress, readAddr, &readData);

    return readData;
}

/**
 *  @brief  ICM62_ReadRegs
 */
void ICM62_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
    SimuI2C_ReadRegs(&hi2c, icm.i2cAddress, readAddr, readData, lens);
}

/**
 *  @brief  ICM62_Config
 */
void ICM62_Config( void )
{
    hi2c.PinSCL    = IMU_SCL_PIN;
    hi2c.PortSCL   = IMU_SCL_PORT;
    hi2c.PinSDA    = IMU_SDA_PIN;
    hi2c.PortSDA   = IMU_SDA_PORT;
    hi2c.Freguency = 6;
    SimuI2C_Config(&hi2c);
}

/**
 *  @brief  ICM62_Init
 */
uint32_t ICM62_Init( void )
{
    uint8_t res;

    icm.deviceId = ICM62_ReadReg(ICM62_WHO_AM_I);
    if (ICM62_DeviceCheck() != KS_OK)
    {
        return KS_ERROR;
    }

    ICM62_Delay(10);
    ICM62_WriteReg(ICM62_PWR_MGMT_1,    0x80);   // Reset Device
    ICM62_Delay(50);
    ICM62_WriteReg(ICM62_PWR_MGMT_1,    0x04);   // Clock Source
    ICM62_WriteReg(ICM62_PWR_MGMT_2,    0x00);   // Enable Accel & Gyro
    ICM62_WriteReg(ICM62_SMPLRT_DIV,    0x00);   // Sample Rate Divider, INTERNAL_SAMPLE_RATE = 1KHz
    ICM62_WriteReg(ICM62_GYRO_CONFIG,   0x18);   // 0x00:250dps, 0x08:500dps, 0x10:1000dps, 0x18:2000dps
    ICM62_WriteReg(ICM62_ACCEL_CONFIG,  0x00);   // 0x00:2g, 0x08:4g, 0x10:8g, 0x18:16g
    ICM62_WriteReg(ICM62_CONFIG,        0x00);   // gyro low pass filter
    ICM62_WriteReg(ICM62_ACCEL_CONFIG2, 0x05);   // accel low pass filter
    ICM62_Delay(10);

    res = ICM62_ReadReg(ICM62_GYRO_CONFIG) & 0x18;
    switch (res)
    {
        case 0x00:  icm.sensitivity[0] = 1.0 / 131.0;   break;
        case 0x08:  icm.sensitivity[0] = 1.0 / 65.5;    break;
        case 0x10:  icm.sensitivity[0] = 1.0 / 32.8;    break;
        case 0x18:  icm.sensitivity[0] = 1.0 / 16.4;    break;
    }

    res = ICM62_ReadReg(ICM62_ACCEL_CONFIG) & 0x18;
    switch (res)
    {
        case 0x00:  icm.sensitivity[1] = 1.0 / 16384.0; break;
        case 0x08:  icm.sensitivity[1] = 1.0 / 8192.0;  break;
        case 0x10:  icm.sensitivity[1] = 1.0 / 4096.0;  break;
        case 0x18:  icm.sensitivity[1] = 1.0 / 2048.0;  break;
    }

    return KS_OK;
}

/**
 *  @brief  ICM62_DeviceCheck
 */
uint32_t ICM62_DeviceCheck( void )
{
#if 0
    printf("0x%02X (%02X) ... ", icm.deviceId, icm.i2cAddress);
//    return KS_ERROR;
#endif

    switch (icm.deviceId)
    {
        case ICM62_DEVICE_ID:
            return KS_OK;
    }

    return KS_ERROR;
}
/**
 *  @brief  ICM62_GetDeviceAddress
 */
uint32_t ICM62_GetDeviceAddress( void )
{
    return icm.i2cAddress;
}

/**
 *  @brief  ICM62_GetDeviceID
 */
uint32_t ICM62_GetDeviceID( void )
{
    return icm.deviceId;
}

float ICM62_GetGyrSensitivity( void )
{
    return icm.sensitivity[0];
}

float ICM62_GetAccSensitivity( void )
{
    return icm.sensitivity[1];
}

/**
 *  @brief  ICM62_GetRawData
 */
void ICM62_GetRawData( int16_t *raw )
{
    uint8_t buff[14] = {0};

    ICM62_ReadRegs(ICM62_ACCEL_XOUT_H, buff, 14);   /* Read Acc, Temp, Gyr */

    raw[0] = (int16_t)(buff[8]  << 8) | buff[9];    /* Gyr.X */
    raw[1] = (int16_t)(buff[10] << 8) | buff[11];   /* Gyr.Y */
    raw[2] = (int16_t)(buff[12] << 8) | buff[13];   /* Gyr.Z */
    raw[3] = (int16_t)(buff[0]  << 8) | buff[1];    /* Acc.X */
    raw[4] = (int16_t)(buff[2]  << 8) | buff[3];    /* Acc.Y */
    raw[5] = (int16_t)(buff[4]  << 8) | buff[5];    /* Acc.Z */
}

/**
 *  @brief  ICM62_GetData
 */
void ICM62_GetData( float *data )
{
    int16_t raw[6] = {0};

    ICM62_GetRawData(raw);

    data[0] = raw[0] * icm.sensitivity[0];
    data[1] = raw[1] * icm.sensitivity[0];
    data[2] = raw[2] * icm.sensitivity[0];
    data[3] = raw[3] * icm.sensitivity[1];
    data[4] = raw[4] * icm.sensitivity[1];
    data[5] = raw[5] * icm.sensitivity[1];
}

/*************************************** END OF FILE ****************************************/
