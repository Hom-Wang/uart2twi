/**
 *       __            ____
 *      / /__ _  __   / __/                      __  
 *     / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *    / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *   /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                     /_/   github.com/KitSprout    
 *  
 *  @file    nrf5x_i2cSimu.c
 *  @author  KitSprout
 *  @date    Dec-2019
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\nrf5x_i2cSimu.h"

/** @addtogroup KS_Driver
  * @{
  */

/* Define ----------------------------------------------------------------------------------*/
#define SIMUI2C_DEFAULT_FREQ        (6U)

/* Macro -----------------------------------------------------------------------------------*/

#define I2CxS_SCL_L()               __GPIO_RST(NRF_GPIO, pSimuI2c->PinSCL)
#define I2CxS_SCL_H()               __GPIO_SET(NRF_GPIO, pSimuI2c->PinSCL)
#define I2CxS_SCL_READ()            (__GPIO_READ(NRF_GPIO, pSimuI2c->PinSCL) == SET)

#define I2CxS_SDA_L()               __GPIO_RST(NRF_GPIO, pSimuI2c->PinSDA)
#define I2CxS_SDA_H()               __GPIO_SET(NRF_GPIO, pSimuI2c->PinSDA)
#define I2CxS_SDA_READ()            (__GPIO_READ(NRF_GPIO, pSimuI2c->PinSDA) == SET)

/* Typedef ---------------------------------------------------------------------------------*/

enum
{
    ACK = 0,
    NACK = 1
};

/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

/**
 *  @brief  SimuI2C_Config
 */
void SimuI2C_Config( SimuI2C_InitTypeDef *pSimuI2c )
{
    nrf_gpio_cfg(
        pSimuI2c->PinSCL,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_CONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0S1,
        NRF_GPIO_PIN_NOSENSE
    );

    nrf_gpio_cfg(
        pSimuI2c->PinSDA,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_CONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0S1,
        NRF_GPIO_PIN_NOSENSE
    );

    I2CxS_SCL_H();
    I2CxS_SDA_H();
}

/**
 *  @brief  SimuI2C_Delay
 */
static void SimuI2C_Delay( SimuI2C_InitTypeDef *pSimuI2c )
{
    uint32_t count = pSimuI2c->Frequency;

    while (count--)
    {
        __nop();
    }
}

/**
 *  @brief  SimuI2C_Start
 */
void SimuI2C_Start( SimuI2C_InitTypeDef *pSimuI2c )
{
    I2CxS_SDA_H();
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SDA_L();
    SimuI2C_Delay(pSimuI2c);
}

/**
 *  @brief  SimuI2C_Stop
 */
void SimuI2C_Stop( SimuI2C_InitTypeDef *pSimuI2c )
{
    I2CxS_SDA_L();
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SDA_H();
    SimuI2C_Delay(pSimuI2c);
}

/**
 *  @brief  SimuI2C_SendACK
 */
void SimuI2C_SendACK( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t sendACK )
{
    I2CxS_SCL_L();
    if (sendACK)
    {
        I2CxS_SDA_H();
    }
    else
    {
        I2CxS_SDA_L();
    }
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_L();
    SimuI2C_Delay(pSimuI2c);
}

/**
 *  @brief  SimuI2C_RecvACK
 */
uint8_t SimuI2C_RecvACK( SimuI2C_InitTypeDef *pSimuI2c )
{
    uint8_t ack;

    I2CxS_SCL_L();
    I2CxS_SDA_H();
    SimuI2C_Delay(pSimuI2c);
    I2CxS_SCL_H();
    SimuI2C_Delay(pSimuI2c);
    ack = I2CxS_SDA_READ();
    I2CxS_SCL_L();
    SimuI2C_Delay(pSimuI2c);

    return ack;
}

/**
 *  @brief  SimuI2C_SendByte
 */
void SimuI2C_SendByte( SimuI2C_InitTypeDef *pSimuI2c, uint8_t sendByte )
{
    for (uint32_t i = 8; i > 0; i--)
    {
        I2CxS_SCL_L();
        SimuI2C_Delay(pSimuI2c);
        if (sendByte & 0x80)  // MSB First
        {
            I2CxS_SDA_H();
        }
        else
        {
            I2CxS_SDA_L();
        }
        sendByte = sendByte << 1;
        SimuI2C_Delay(pSimuI2c);
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
    }
    I2CxS_SCL_L();
}

/**
 *  @brief  SimuI2C_RecvByte
 */
uint8_t SimuI2C_RecvByte( SimuI2C_InitTypeDef *pSimuI2c )
{
    uint8_t recvByte = 0;

    I2CxS_SDA_H();
    for (uint32_t i = 8; i > 0; i--)
    {
        recvByte = recvByte << 1;
        I2CxS_SCL_L();
        SimuI2C_Delay(pSimuI2c);
        I2CxS_SCL_H();
        SimuI2C_Delay(pSimuI2c);
        if (I2CxS_SDA_READ())
        {
            recvByte |= 0x01;
        }
    }
    I2CxS_SCL_L();

    return recvByte;
}

/**
 *  @brief  SimuI2C_WriteData
 */
void SimuI2C_WriteData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t *writeData, const uint8_t lens )
{
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, slaveAddr << 1);
    SimuI2C_RecvACK(pSimuI2c);
    for (uint32_t i = 0; i < lens; i++)
    {
        SimuI2C_SendByte(pSimuI2c, writeData[i]);
        SimuI2C_RecvACK(pSimuI2c);
    }
    SimuI2C_Stop(pSimuI2c);
}

/**
 *  @brief  SimuI2C_ReadData
 */
uint8_t SimuI2C_ReadData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *readData, const uint8_t lens )
{
    uint8_t ack;

    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, (slaveAddr << 1) | 0x01);
    ack = SimuI2C_RecvACK(pSimuI2c);
    for (uint32_t i = 0; i < lens; i++)
    {
        readData[i] = SimuI2C_RecvByte(pSimuI2c);
        if (i == (lens - 1))
        {
            SimuI2C_SendACK(pSimuI2c, NACK);
        }
        else
        {
            SimuI2C_SendACK(pSimuI2c, ACK);
        }
    }
    SimuI2C_Stop(pSimuI2c);

    return ack;
}

/**
 *  @brief  SimuI2C_WriteReg
 */
void SimuI2C_WriteReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t regData )
{
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, slaveAddr << 1);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regData);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Stop(pSimuI2c);
}

/**
 *  @brief  SimuI2C_ReadReg
 */
void SimuI2C_ReadReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData )
{
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, slaveAddr << 1);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, (slaveAddr << 1) | 0x01);
    SimuI2C_RecvACK(pSimuI2c);
    *regData = SimuI2C_RecvByte(pSimuI2c);
    SimuI2C_SendACK(pSimuI2c, 1);
    SimuI2C_Stop(pSimuI2c);
}

/**
 *  @brief  SimuI2C_WriteRegs
 */
void SimuI2C_WriteRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t *regData, const uint8_t lens )
{
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, slaveAddr << 1);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    for (uint32_t i = 0; i < lens; i++)
    {
        SimuI2C_SendByte(pSimuI2c, regData[i]);
        SimuI2C_RecvACK(pSimuI2c);
    }
    SimuI2C_Stop(pSimuI2c);
}

/**
 *  @brief  SimuI2C_ReadRegs
 */
void SimuI2C_ReadRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData, const uint8_t lens )
{
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, slaveAddr << 1);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, regAddr);
    SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, (slaveAddr << 1) | 0x01);
    SimuI2C_RecvACK(pSimuI2c);
    for (uint32_t i = 0; i < lens; i++)
    {
        regData[i] = SimuI2C_RecvByte(pSimuI2c);
        if (i == lens - 1)
        {
            SimuI2C_SendACK(pSimuI2c, NACK);
        }
        else
        {
            SimuI2C_SendACK(pSimuI2c, ACK);
        }
    }
    SimuI2C_Stop(pSimuI2c);
}

/**
 *  @brief  SimuI2C_ScanRegs
 */
#define MULIBYTEREAD    (8U)
uint32_t SimuI2C_ScanRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *regData )
{
    uint8_t ack;

    SimuI2C_Start(pSimuI2c);
    SimuI2C_SendByte(pSimuI2c, slaveAddr << 1);
    ack = SimuI2C_RecvACK(pSimuI2c);
    SimuI2C_Stop(pSimuI2c);

    if (!ack)
    {
        for (uint32_t i = 0; i < 256; i = i + MULIBYTEREAD)
        {
            SimuI2C_ReadRegs(pSimuI2c, slaveAddr, i, &regData[i], MULIBYTEREAD);
        }
    }

#if 0
    printf("\r\n");
    printf(" >>  i2c device register (address 0x%02X)\r\n\r\n", slaveAddr);
    printf("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    for (uint32_t i = 0; i < 256; i++)
    {
        if (i % 16 == 0)
        {
            printf("\r\n %02X:", i);
        }
        printf(" %02X", regData[i]);
    }
    printf("\r\n\r\n");
#endif

    return ((!ack) ? KS_OK : KS_ERROR);
}

/**
 *  @brief  SimuI2C_ScanRegsGPIO
 */
uint32_t SimuI2C_ScanRegsGPIO( const uint8_t sclpin, const uint8_t sdapin, const uint8_t slaveAddr, uint8_t *regData )
{
    uint8_t status;
    SimuI2C_InitTypeDef i2c =
    {
        .PinSCL  = sclpin,
        .PortSCL = NRF_P0,
        .PinSDA  = sdapin,
        .PortSDA = NRF_P0,
        .Frequency = SIMUI2C_DEFAULT_FREQ
    };
    SimuI2C_Config(&i2c);
    status = SimuI2C_ScanRegs(&i2c, slaveAddr, regData);

    return status;
}

/**
 *  @brief  SimuI2C_ScanDevice
 */
uint32_t SimuI2C_ScanDevice( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *devAddress )
{
    uint8_t ack;
    uint32_t idx = 0;

    for (uint32_t i = 0; i < 128; i++)
    {
        devAddress[i] = 0;

        SimuI2C_Start(pSimuI2c);
        SimuI2C_SendByte(pSimuI2c, i << 1);
        ack = SimuI2C_RecvACK(pSimuI2c);
        SimuI2C_Stop(pSimuI2c);

        if (!ack)
        {
            devAddress[idx] = i;
            idx++;
        }
    }

#if 0
    printf("\r\n");
    printf(" >>  i2c device scanner (found %d)\r\n\r\n", idx);
    printf("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    idx = 0;
    for (uint32_t i = 0; i < 128; i++)
    {
        if (i % 16 == 0)
        {
           printf("\r\n %02X:", i);
        }
        if (i == devAddress[idx])
        {
            idx++;
            printf(" %02X", i);
        }
        else
        {
            printf(" --");
        }
    }
    printf("\r\n\r\n");
#endif

    return idx;
}

/**
 *  @brief  SimuI2C_ScanDeviceByGPIO
 */
uint32_t SimuI2C_ScanDeviceByGPIO( const uint8_t sclpin, const uint8_t sdapin, uint8_t *devAddress )
{
    uint8_t status;
    SimuI2C_InitTypeDef i2c =
    {
        .PinSCL  = sclpin,
        .PortSCL = NRF_P0,
        .PinSDA  = sdapin,
        .PortSDA = NRF_P0,
        .Frequency = SIMUI2C_DEFAULT_FREQ
    };

    SimuI2C_Config(&i2c);
    status = SimuI2C_ScanDevice(&i2c, devAddress);

    return status;
}

/**
 *  @brief  SimuI2C_ScanAll
 */
void SimuI2C_ScanAll( const uint8_t sclpin, const uint8_t sdapin )
{
    uint8_t slaveAddr[128] = {0};
    uint8_t regData[256] = {0};
    uint8_t devicenum;
    devicenum = SimuI2C_ScanDeviceByGPIO(IMU_SCL_PIN, IMU_SDA_PIN, slaveAddr);
    for (uint32_t i = 0; i < devicenum; i++)
    {
        SimuI2C_ScanRegsGPIO(IMU_SCL_PIN, IMU_SDA_PIN, slaveAddr[i], regData);
    }
}

/*************************************** END OF FILE ****************************************/
