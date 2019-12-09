/**
 *       __            ____
 *      / /__ _  __   / __/                      __  
 *     / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *    / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *   /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                     /_/   github.com/KitSprout    
 *  
 *  @file    nrf5x_i2cSimu.h
 *  @author  KitSprout
 *  @date    Dec-2019
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __NRF5X_I2CSIMU_H
#define __NRF5X_I2CSIMU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"

/* Constants -------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct
{
    uint32_t        PinSDA;
    NRF_GPIO_Type   *PortSDA;
    uint32_t        PinSCL;
    NRF_GPIO_Type   *PortSCL;
    uint32_t        Frequency;

} SimuI2C_InitTypeDef;

/* Functions -------------------------------------------------------------------------------*/  

void        SimuI2C_Config( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_Start( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_Stop( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_SendACK( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t sendACK );
uint8_t     SimuI2C_RecvACK( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_SendByte( SimuI2C_InitTypeDef *pSimuI2c, uint8_t sendByte );
uint8_t     SimuI2C_RecvByte( SimuI2C_InitTypeDef *pSimuI2c );
void        SimuI2C_WriteData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t *writeData, const uint8_t lens );
uint8_t     SimuI2C_ReadData( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *readData, const uint8_t lens );
void        SimuI2C_WriteReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t regData );
void        SimuI2C_ReadReg( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData );
void        SimuI2C_WriteRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, const uint8_t *regData, const uint8_t lens );
void        SimuI2C_ReadRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, const uint8_t regAddr, uint8_t *regData, const uint8_t lens );

uint32_t    SimuI2C_ScanRegs( SimuI2C_InitTypeDef *pSimuI2c, const uint8_t slaveAddr, uint8_t *regData );
uint32_t    SimuI2C_ScanRegsGPIO( const uint8_t sclpin, const uint8_t sdapin, const uint8_t slaveAddr, uint8_t *regData );
uint32_t    SimuI2C_ScanDevice( SimuI2C_InitTypeDef *pSimuI2c, uint8_t *devAddress );
uint32_t    SimuI2C_ScanDeviceByGPIO( const uint8_t sclpin, const uint8_t sdapin, uint8_t *devAddress );
void        SimuI2C_ScanAll( const uint8_t sclpin, const uint8_t sdapin );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
