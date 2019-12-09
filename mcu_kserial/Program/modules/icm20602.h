/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    icm20602.h
 *  @author  KitSprout
 *  @date    Aug-2018
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __ICM20602_H
#define __ICM20602_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"

/* Define ----------------------------------------------------------------------------------*/

/*
|     |            ACCELEROMETER           |
| LPF |  BandW  |  Delay  | Noise | Sample |
+-----+---------+---------+-------+--------+
|  x  |  1130Hz |  0.75ms | 250Hz |  4kHz  |
|  0  |   460Hz |  1.94ms | 250Hz |  1kHz  |
|  1  |   184Hz |  5.80ms | 250Hz |  1kHz  |
|  2  |    92Hz |  7.80ms | 250Hz |  1kHz  |
|  3  |    41Hz | 11.80ms | 250Hz |  1kHz  |
|  4  |    20Hz | 19.80ms | 250Hz |  1kHz  |
|  5  |    10Hz | 35.70ms | 250Hz |  1kHz  |
|  6  |     5Hz | 66.96ms | 250Hz |  1kHz  |
|  7  |   460Hz |  1.94ms | 250Hz |  1kHz  |
*/
/*
|     |          GYROSCOPE         |    TEMPERATURE    |
| LPF |  BandW  |  Delay  | Sample |  BandW  |  Delay  |
+-----+---------+---------+--------+---------+---------+
|  x  |  8800Hz | 0.064ms |  32kHz |  4000Hz |  0.04ms |
|  x  |  3600Hz |  0.11ms |  32kHz |  4000Hz |  0.04ms |
|  0  |   250Hz |  0.97ms |   8kHz |  4000Hz |  0.04ms |
|  1  |   184Hz |   2.9ms |   1kHz |   188Hz |   1.9ms |
|  2  |    92Hz |   3.9ms |   1kHz |    98Hz |   2.8ms |
|  3  |    41Hz |   5.9ms |   1kHz |    42Hz |   4.8ms |
|  4  |    20Hz |   9.9ms |   1kHz |    20Hz |   8.3ms |
|  5  |    10Hz | 17.85ms |   1kHz |    10Hz |  13.4ms |
|  6  |     5Hz | 33.48ms |   1kHz |     5Hz |  18.6ms |
|  7  |  3600Hz |  0.17ms |   8kHz |  4000Hz |  0.04ms |
*/

#define ICM62_I2C_ADDR            ((uint8_t)0xD0)   // AD0 = 0
//#define ICM62_I2C_ADDR            ((uint8_t)0xD1)   // AD0 = 1
#define ICM62_DEVICE_ID           ((uint8_t)0x12)

#define ICM62_SELF_TEST_X_GYRO    ((uint8_t)0x00)
#define ICM62_SELF_TEST_Y_GYRO    ((uint8_t)0x01)
#define ICM62_SELF_TEST_Z_GYRO    ((uint8_t)0x02)
#define ICM62_SELF_TEST_X_ACCEL   ((uint8_t)0x0D)
#define ICM62_SELF_TEST_Y_ACCEL   ((uint8_t)0x0E)
#define ICM62_SELF_TEST_Z_ACCEL   ((uint8_t)0x0F)
#define ICM62_XG_OFFS_USRH        ((uint8_t)0x13)
#define ICM62_XG_OFFS_USRL        ((uint8_t)0x14)
#define ICM62_YG_OFFS_USRH        ((uint8_t)0x15)
#define ICM62_YG_OFFS_USRL        ((uint8_t)0x16)
#define ICM62_ZG_OFFS_USRH        ((uint8_t)0x17)
#define ICM62_ZG_OFFS_USRL        ((uint8_t)0x18)
#define ICM62_SMPLRT_DIV          ((uint8_t)0x19)
#define ICM62_CONFIG              ((uint8_t)0x1A)
#define ICM62_GYRO_CONFIG         ((uint8_t)0x1B)
#define ICM62_ACCEL_CONFIG        ((uint8_t)0x1C)
#define ICM62_ACCEL_CONFIG2       ((uint8_t)0x1D)
#define ICM62_LP_MODE_CFG         ((uint8_t)0x1E)
#define ICM62_ACCEL_WOM_THR       ((uint8_t)0x1F)
#define ICM62_FIFO_EN             ((uint8_t)0x23)
#define ICM62_FSYNC_INT           ((uint8_t)0x36)
#define ICM62_INT_PIN_CFG         ((uint8_t)0x37)
#define ICM62_INT_ENABLE          ((uint8_t)0x38)
#define ICM62_INT_STATUS          ((uint8_t)0x3A)
#define ICM62_ACCEL_XOUT_H        ((uint8_t)0x3B)
#define ICM62_ACCEL_XOUT_L        ((uint8_t)0x3C)
#define ICM62_ACCEL_YOUT_H        ((uint8_t)0x3D)
#define ICM62_ACCEL_YOUT_L        ((uint8_t)0x3E)
#define ICM62_ACCEL_ZOUT_H        ((uint8_t)0x3F)
#define ICM62_ACCEL_ZOUT_L        ((uint8_t)0x40)
#define ICM62_TEMP_OUT_H          ((uint8_t)0x41)
#define ICM62_TEMP_OUT_L          ((uint8_t)0x42)
#define ICM62_GYRO_XOUT_H         ((uint8_t)0x43)
#define ICM62_GYRO_XOUT_L         ((uint8_t)0x44)
#define ICM62_GYRO_YOUT_H         ((uint8_t)0x45)
#define ICM62_GYRO_YOUT_L         ((uint8_t)0x46)
#define ICM62_GYRO_ZOUT_H         ((uint8_t)0x47)
#define ICM62_GYRO_ZOUT_L         ((uint8_t)0x48)
#define ICM62_SIGNAL_PATH_RESET   ((uint8_t)0x68)
#define ICM62_ACCEL_INTEL_CTRL    ((uint8_t)0x69)
#define ICM62_USER_CTRL           ((uint8_t)0x6A)
#define ICM62_PWR_MGMT_1          ((uint8_t)0x6B)
#define ICM62_PWR_MGMT_2          ((uint8_t)0x6C)
#define ICM62_FIFO_COUNTH         ((uint8_t)0x72)
#define ICM62_FIFO_COUNTL         ((uint8_t)0x73)
#define ICM62_FIFO_R_W            ((uint8_t)0x74)
#define ICM62_WHO_AM_I            ((uint8_t)0x75)   /* ID = 0xAF */
#define ICM62_XA_OFFSET_H         ((uint8_t)0x77)
#define ICM62_XA_OFFSET_L         ((uint8_t)0x78)
#define ICM62_YA_OFFSET_H         ((uint8_t)0x7A)
#define ICM62_YA_OFFSET_L         ((uint8_t)0x7B)
#define ICM62_ZA_OFFSET_H         ((uint8_t)0x7D)
#define ICM62_ZA_OFFSET_L         ((uint8_t)0x7E)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef struct {

  uint8_t i2cAddress;         // 8-bit address
  uint8_t deviceId;
  float32_t sensitivity[3];   // sensor sensitivity (gyr, acc, temp)

} icm2060x_t;

/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      ICM62_WriteReg( uint8_t writeAddr, uint8_t writeData );
void      ICM62_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens );
uint8_t   ICM62_ReadReg( uint8_t readAddr );
void      ICM62_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens );

void      ICM62_Config( void );
uint32_t  ICM62_Init( void );
uint32_t  ICM62_DeviceCheck( void );
uint32_t  ICM62_GetDeviceAddress( void );
uint32_t  ICM62_GetDeviceID( void );
float     ICM62_GetGyrSensitivity( void );
float     ICM62_GetAccSensitivity( void );
void      ICM62_GetRawData( int16_t *data );
void      ICM62_GetData( float *data );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
