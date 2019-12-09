/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    ist83xx.c
 *  @author  KitSprout
 *  @date    Feb-2019
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"
#include "drivers\nrf5x_i2cSimu.h"
#include "modules\ist83xx.h"

/** @addtogroup NRF5x_Module
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
#define IST83_Delay(__MS)         delay_ms(__MS)
#define IST83_MULTI_SENSOR_NUM    1

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
static SimuI2C_InitTypeDef hi2c;
static ist83xx_t ist[IST83_MULTI_SENSOR_NUM] =
{
    { .i2cAddress = IST83XX_I2C_ADDR, .deviceId = 0x00, .sensitivity = 0 },
//    { .i2cAddress = (0x0C << 1), .deviceId = 0x00, .sensitivity = 0 },
//    { .i2cAddress = (0x0D << 1), .deviceId = 0x00, .sensitivity = 0 },
//    { .i2cAddress = (0x0E << 1), .deviceId = 0x00, .sensitivity = 0 },
};

/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  IST83_WriteReg
 */
static void IST83_WriteReg( uint8_t address, uint8_t writeAddr, uint8_t writeData )
{
    SimuI2C_WriteReg(&hi2c, address, writeAddr, writeData);
}

/**
 *  @brief  IST83_WriteRegs
 */
//static void IST83_WriteRegs( uint8_t address, uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
//{
//    SimuI2C_WriteRegs(&hi2c, address, writeAddr, writeData, lens);
//}

/**
 *  @brief  IST83_ReadReg
 */
static uint8_t IST83_ReadReg( uint8_t address, uint8_t readAddr )
{
    uint8_t readData;
    SimuI2C_ReadReg(&hi2c, address, readAddr, &readData);
    return readData;
}

/**
 *  @brief  IST83_ReadRegs
 */
static void IST83_ReadRegs( uint8_t address, uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
    SimuI2C_ReadRegs(&hi2c, address, readAddr, readData, lens);
}

/**
 *  @brief  IST83_Config
 */
void IST83_Config( void )
{
    hi2c.PinSCL    = IMU_SCL_PIN;
    hi2c.PortSCL   = IMU_SCL_PORT;
    hi2c.PinSDA    = IMU_SDA_PIN;
    hi2c.PortSDA   = IMU_SDA_PORT;
    hi2c.Freguency = 6;  // 400kHz
    SimuI2C_Config(&hi2c);
}

/**
 *  @brief  IST83_Init
 */
uint32_t IST83_Init( uint8_t index )
{
    uint8_t res;

    ist[index].deviceId = IST83_ReadReg(ist[index].i2cAddress, IST83XX_WAI);
    if (IST83_DeviceCheck(index) != KS_OK)
    {
        return KS_ERROR;
    }

    switch (ist[index].deviceId)
    {
        case IST8305_DEVICD_ID:
        {
            break;
        }
        case IST8306_DEVICD_ID:
        {
            /* reset */
            IST83_Delay(10);
            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL3, 0x01);     // Soft resert
            IST83_Delay(50);

            IST83_WriteReg(ist[index].i2cAddress, IST8306_CHIP_TEST, 0x41);
            IST83_WriteReg(ist[index].i2cAddress, IST8306_SRC_SEL, 0x00);
            IST83_WriteReg(ist[index].i2cAddress, IST8306_XY_OFFSET, 0x66);
            IST83_WriteReg(ist[index].i2cAddress, IST8306_Z_OFFSET, 0x60);
            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL6, 0x02);

            /* filter setting */
            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL1, 0x00);     // Noise Suppression Filter : Disable
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL1, 0x20);     // Noise Suppression Filter : Low
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL1, 0x40);     // Noise Suppression Filter : Middle
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL1, 0x60);     // Noise Suppression Filter : High

            /* over-sampling setting */
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_OSRCNTL, 0x00);   // OSR = 1, ODR max = 200Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_OSRCNTL, 0x01);   // OSR = 2, ODR max = 200Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_OSRCNTL, 0x02);   // OSR = 4, ODR max = 200Hz
            IST83_WriteReg(ist[index].i2cAddress, IST8306_OSRCNTL, 0x03);   // OSR = 8, 200Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_OSRCNTL, 0x04);   // OSR = 16, ODR max = 100Hz (Default)
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_OSRCNTL, 0x05);   // OSR = 32, ODR max = 50Hz

            /* odr setting */
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x00);     // Stand-by mode
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x01);     // Single Measurement Mode (maximum ODR = 200Hz)
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x02);     // Continuous Measurement Mode with ODR 10Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x04);     // Continuous Measurement Mode with ODR 20Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x06);     // Continuous Measurement Mode with ODR 50Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x08);     // Continuous Measurement Mode with ODR 100Hz
            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x0A);     // Continuous Measurement Mode with ODR 200Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8306_CNTL2, 0x10);     // Single Measurement Self-test Mode

            /* sensitivity */
            ist[index].sensitivity = 1.0 / IST8306_SENS_3000UT_LSB;
            break;
        }
        case IST8308_DEVICD_ID:
        {
            /* reset */
            IST83_Delay(10);
            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL3, 0x01);       // Soft resert
            IST83_Delay(50);

            /* filter setting */
            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL1, 0x00);       // Noise Suppression Filter : Disable
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL1, 0x20);       // Noise Suppression Filter : Low
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL1, 0x40);       // Noise Suppression Filter : Middle
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL1, 0x60);       // Noise Suppression Filter : High

            /* dynamic range setting */
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL4, 0x11);       // Dynamic Range ±100uT
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL4, 0x01);       // Dynamic Range ±200uT, Sensitivity 13.2 LSB/uT
            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL4, 0x00);       // Dynamic Range ±500uT, Sensitivity 6.6 LSB/uT

            /* over-sampling setting */
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_OSRCNTL, 0x00);     // OSR = 1, ODR max = 1000Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_OSRCNTL, 0x09);     // OSR = 2, ODR max = 500Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_OSRCNTL, 0x12);     // OSR = 4, ODR max = 333Hz
            IST83_WriteReg(ist[index].i2cAddress, IST8308_OSRCNTL, 0x1B);     // OSR = 8, 200Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_OSRCNTL, 0x24);     // OSR = 16, ODR max = 100Hz (Default)
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_OSRCNTL, 0x2D);     // OSR = 32, ODR max = 50Hz

            /* odr setting */
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x00);       // Stand-by mode
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x01);       // Single Measurement Mode (maximum ODR = 1000Hz)
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x02);       // Continuous Measurement Mode with ODR 10Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x04);       // Continuous Measurement Mode with ODR 20Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x06);       // Continuous Measurement Mode with ODR 50Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x08);       // Continuous Measurement Mode with ODR 100Hz
            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x0A);       // Continuous Measurement Mode with ODR 200Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x10);       // Single Measurement Self-test Mode
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x11);       // Continuous Measurement Mode with ODR 333H
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x12);       // Continuous Measurement Mode with ODR 500Hz
//            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x13);       // Continuous Measurement Mode with no-sleep

            /* sensitivity */
            res = IST83_ReadReg(ist[index].i2cAddress, IST8308_CNTL4) & 0x03;
            switch (res)
            {
                case 0x00:  ist[index].sensitivity = 1.0 / IST8308_SENS_500UT_LSB;  break;
                case 0x01:  ist[index].sensitivity = 1.0 / IST8308_SENS_200UT_LSB;  break;
                default :   return KS_ERROR;
            }
            break;
        }
#if IST8310_SELECT == IST8310_NUM
        case IST8310_DEVICD_ID:
        {
            IST83_Delay(50);
            IST83_WriteReg(ist[index].i2cAddress, IST8310_AVGCNTL, 0x24);     // x, y, z average by 16 timese
            IST83_WriteReg(ist[index].i2cAddress, IST8310_PDCNTL, 0xC0);      // Power-down mode
            IST83_Delay(10);
            IST83_WriteReg(ist[index].i2cAddress, IST8310_CNTL1, 0x01);       // Fuse ROM access mode

            /* sensitivity */
            ist[index].sensitivity = 1.0 / IST8310_SENS_1600UT_LSB;
            break;
        }
#elif IST8310_SELECT == IST8310X_NUM
        case IST8310X_DEVICD_ID:
        {
            /* reset */
            IST83_Delay(10);
            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL3, 0x01);       // Soft resert
            IST83_Delay(50);

            /* over-sampling setting */
            IST83_WriteReg(ist[index].i2cAddress, IST8308_OSRCNTL, 0x1B);     // OSR = 8, 200Hz

            /* odr setting */
            IST83_WriteReg(ist[index].i2cAddress, IST8308_CNTL2, 0x0A);       // Continuous Measurement Mode with ODR 200Hz

            /* sensitivity */
            ist[index].sensitivity = 1.0 / IST8310X_SENS_1600UT_LSB;
            break;
        }
#endif
        case IST8315_DEVICD_ID:
        {
            /* reset */
            IST83_Delay(10);
            IST83_WriteReg(ist[index].i2cAddress, IST8315_CNTL3, 0x01);       // Soft resert
            IST83_Delay(50);

            res = IST83_ReadReg(ist[index].i2cAddress, IST8315_CNTL6) & ~0x01;
            IST83_WriteReg(ist[index].i2cAddress, IST8315_CNTL6, res);        // Set bit0 for continue read
            IST83_WriteReg(ist[index].i2cAddress, IST8315_CNTL1, 0x20);       // Noise Suppression Filter : Low

            /* dynamic range setting */
//            IST83_WriteReg(ist[index].i2cAddress, IST8315_CNTL4, 0x00);       // Dynamic Range ±1000uT, Sensitivity 3.3 LSB/uT
            IST83_WriteReg(ist[index].i2cAddress, IST8315_CNTL4, 0x01);       // Dynamic Range ±500uT, Sensitivity 6.6 LSB/uT

            IST83_WriteReg(ist[index].i2cAddress, IST8315_OSRCNTL, 0x24);     // OSR = 16, 100Hz
            IST83_WriteReg(ist[index].i2cAddress, IST8315_CNTL2, 0x0A);       // Disable FIFO, Continuous Measurement Mode with ODR 200Hz
            ist[index].sensitivity = 1.0 / IST8315_SENS_500UT_LSB;

            /* sensitivity */
            res = IST83_ReadReg(ist[index].i2cAddress, IST8315_CNTL4) & 0x03;
            switch (res)
            {
                case 0x00:  ist[index].sensitivity = 1.0 / IST8315_SENS_1000UT_LSB; break;
                case 0x01:  ist[index].sensitivity = 1.0 / IST8315_SENS_500UT_LSB;  break;
                default :   return KS_ERROR;
            }

            break;
        }
    }
    IST83_Delay(10);

    return KS_OK;
}

/**
 *  @brief  IST83_DeviceCheck
 */
uint32_t IST83_DeviceCheck( uint8_t index )
{
#if 0
  printf(" ... 0x%02X (%02X) ... ", ist[index].deviceId, ist[index].i2cAddress);
//  return KS_ERROR;
#endif

    switch (ist[index].deviceId)
    {
        case IST8306_DEVICD_ID:
        case IST8308_DEVICD_ID:
#if IST8310_SELECT == IST8310_NUM
        case IST8310_DEVICD_ID:
#elif IST8310_SELECT == IST8310X_NUM
        case IST8310X_DEVICD_ID:
#endif
        case IST8315_DEVICD_ID:
            return KS_OK;

        case IST8305_DEVICD_ID:
        case IST8312_DEVICD_ID:
            printf("\r\n\r\nNOT SUPPORTED!!!\r\n\r\n");
            return KS_ERROR;
    }

    return KS_ERROR;
}

/**
 *  @brief  IST83_GetDeviceAddress
 */
uint32_t IST83_GetDeviceAddress( uint8_t index )
{
    return ist[index].i2cAddress;
}

/**
 *  @brief  IST83_GetDeviceID
 */
uint32_t IST83_GetDeviceID( uint8_t index )
{
    return ist[index].deviceId;
}

/**
 *  @brief  IST83_GetSensitivity
 */
float IST83_GetMagSensitivity( uint8_t index )
{
    return ist[index].sensitivity;
}

/**
 *  @brief  IST83_GetRawData
 */
uint32_t IST83_GetRawData( uint8_t index, int16_t *raw )
{
    uint8_t buff[6] = {0};

    switch (ist[index].deviceId)
    {
        case IST8305_DEVICD_ID:
        {
            break;
        }
        case IST8306_DEVICD_ID:
        {
            if ((IST83_ReadReg(ist[index].i2cAddress, IST8306_STAT) & 0x01) != 0x01)
            {
                return KS_ERROR;
            }
            IST83_ReadRegs(ist[index].i2cAddress, IST8306_DATAXL, buff, 6);
            break;
        }
        case IST8308_DEVICD_ID:
        {
            if ((IST83_ReadReg(ist[index].i2cAddress, IST8308_STAT) & 0x01) != 0x01)
            {
                return KS_ERROR;
            }
            IST83_ReadRegs(ist[index].i2cAddress, IST8308_DATAXL, buff, 6);
            break;
        }
        case IST8310_DEVICD_ID:
        {
#if IST8310_SELECT == IST8310_NUM
            if ((IST83_ReadReg(ist[index].i2cAddress, IST8310_STAT1) & 0x01) != 0x01)
            {
                return KS_ERROR;
            }
            IST83_ReadRegs(ist[index].i2cAddress, IST8310_DATAXL, buff, 6);
            IST83_WriteReg(ist[index].i2cAddress, IST8310_CNTL1, 0x01);
            break;
#elif IST8310_SELECT == IST8310X_NUM
            if ((IST83_ReadReg(ist[index].i2cAddress, IST8308_STAT) & 0x01) != 0x01)
            {
                return KS_ERROR;
            }
            IST83_ReadRegs(ist[index].i2cAddress, IST8308_DATAXL, buff, 6);
            break;
#endif
        }
        case IST8315_DEVICD_ID:
        {
            if ((IST83_ReadReg(ist[index].i2cAddress, IST8315_STAT1) & 0x01) != 0x01)
            {
                return KS_ERROR;
            }
            IST83_ReadRegs(ist[index].i2cAddress, IST8315_DATAXL, buff, 6);
            break;
        }
        default:
        {
            return KS_ERROR;
        }
    }

    raw[0] = (int16_t)(buff[1] << 8) | buff[0];
    raw[1] = (int16_t)(buff[3] << 8) | buff[2];
    raw[2] = (int16_t)(buff[5] << 8) | buff[4];

    return KS_OK;
}

/**
 *  @brief  IST83_GetData
 */
uint32_t IST83_GetData( uint8_t index, float *data )
{
    uint32_t status;
    int16_t raw[3] = {0};

    status = IST83_GetRawData(index, raw);
    if (status == KS_OK)
    {
        data[0] = raw[0] * ist[index].sensitivity;
        data[1] = raw[1] * ist[index].sensitivity;
        data[2] = raw[2] * ist[index].sensitivity;
    }

    return status;
}

/**
 *  @brief  IST83_GetTemprature
 */
int16_t IST83_GetTemprature( uint8_t index )
{
    uint8_t tmp[4];
    int16_t data[3] = {0};

    switch (ist[index].deviceId)
    {
        case IST8305_DEVICD_ID:
        {
            break;
        }
        case IST8308_DEVICD_ID:
        {
            IST83_ReadRegs(ist[index].i2cAddress, 0x58, tmp, 2);
            IST83_ReadRegs(ist[index].i2cAddress, 0x1C, &tmp[2], 2);
            data[0] = (((int16_t)tmp[1]) << 8) | tmp[0];
            data[1] = (((int16_t)tmp[3]) << 8) | tmp[2];
            data[2] = (((float)(28000 - data[1] - data[0]) / 75) + 25) * 100;
            break;
        }
#if IST8310_SELECT == IST8310_NUM
        case IST8310_DEVICD_ID:
        {
            break;
        }
#elif IST8310_SELECT == IST8310X_NUM
        case IST8310X_DEVICD_ID:
        {
            break;
        }
#endif
        case IST8315_DEVICD_ID:
        {
            break;
        }
    }

    return data[2];
}

/*************************************** END OF FILE ****************************************/
