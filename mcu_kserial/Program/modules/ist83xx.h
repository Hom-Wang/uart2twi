/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    ist83xx.h
 *  @author  KitSprout
 *  @date    Feb-2019
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __IST83XX_H
#define __IST83XX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

/* Define ----------------------------------------------------------------------------------*/
#define IST83XX_I2C_ADDR          ((uint8_t)0x18)   /* 0Ch, CADx = [0, 0] */
//#define IST83XX_I2C_ADDR          ((uint8_t)0x1A)   /* 0Dh, CADx = [1, 0] */
//#define IST83XX_I2C_ADDR          ((uint8_t)0x1C)   /* 0Eh, CADx = [0, 1] */
//#define IST83XX_I2C_ADDR          ((uint8_t)0x1E)   /* 0Fh, CADx = [1, 1] */

#define IST8305_DEVICD_ID         ((uint8_t)0x05)
#define IST8306_DEVICD_ID         ((uint8_t)0x06)
#define IST8308_DEVICD_ID         ((uint8_t)0x08)
#define IST8310_DEVICD_ID         ((uint8_t)0x10)
#define IST8310X_DEVICD_ID        IST8310_DEVICD_ID   // ist8308
#define IST8312_DEVICD_ID         ((uint8_t)0x12)
#define IST8315_DEVICD_ID         ((uint8_t)0x15)

#define IST8310_NUM               (0U)
#define IST8310X_NUM              (1U)
#define IST8310_SELECT            IST8310_NUM

/* register map : general */
#define IST83XX_WAI               ((uint8_t)0x00)   /* Device ID */


/* ist8306 */
#define IST8306_STR               ((uint8_t)0x0C)   /* For self-test mode */
#define IST8306_STAT              ((uint8_t)0x10)   /* Data status information */
#define IST8306_DATAXL            ((uint8_t)0x11)   /* Low  byte for X-axis data */
#define IST8306_DATAXH            ((uint8_t)0x12)   /* High byte for X-axis data */
#define IST8306_DATAYL            ((uint8_t)0x13)   /* Low  byte for Y-axis data */
#define IST8306_DATAYH            ((uint8_t)0x14)   /* High byte for Y-axis data */
#define IST8306_DATAZL            ((uint8_t)0x15)   /* Low  byte for Z-axis data */
#define IST8306_DATAZH            ((uint8_t)0x16)   /* High byte for Z-axis data */
#define IST8306_ACTR              ((uint8_t)0x20)   /* Controls the entrance and exit of Suspend Mode */
#define IST8306_CNTL1             ((uint8_t)0x30)   /* Chip control settings 1 */
#define IST8306_CNTL2             ((uint8_t)0x31)   /* Chip control settings 2 */
#define IST8306_CNTL3             ((uint8_t)0x32)   /* Chip control settings 3 */
#define IST8306_CNTL6             ((uint8_t)0x35)   /* Chip control settings 6 */
#define IST8306_CHIP_TEST         ((uint8_t)0x40)   /* Chip test */
#define IST8306_OSRCNTL           ((uint8_t)0x41)   /* Oversampling ratio setting */

#define IST8306_SENSOR_EN_2       ((uint8_t)0x43)   /* Sensor selection register 2 */
#define IST8306_SRC_SEL           ((uint8_t)0x45)   /* Source select */
#define IST8306_X_GAIN            ((uint8_t)0x50)   /* Gain setting register of X channel */
#define IST8306_Y_GAIN            ((uint8_t)0x52)   /* Gain setting register of Y channel */
#define IST8306_XY_OFFSET         ((uint8_t)0x53)   /* Offset setting register of XY channel */
#define IST8306_Z_GAIN            ((uint8_t)0x54)   /* Gain setting register of Z channel */
#define IST8306_Z_OFFSET          ((uint8_t)0x55)   /* Offset setting register of Z channel */
#define IST8306_1MHZ_TUNING       ((uint8_t)0x55)   /* 1 Mhz clock tuning register */

#define IST8306_SENS_3000UT_LSB   (3.0)             /* ±3000 uT, LSB/uT */
#define IST8306_SENS_3000_LSB2UT  (0.33)


/* ist8308 */
#define IST8308_STR               ((uint8_t)0x0C)   /* Self-Test Register */
#define IST8308_STAT              ((uint8_t)0x10)   /* Data status information */
#define IST8308_DATAXL            ((uint8_t)0x11)   /* Low  byte for X-axis data */
#define IST8308_DATAXH            ((uint8_t)0x12)   /* High byte for X-axis data */
#define IST8308_DATAYL            ((uint8_t)0x13)   /* Low  byte for Y-axis data */
#define IST8308_DATAYH            ((uint8_t)0x14)   /* High byte for Y-axis data */
#define IST8308_DATAZL            ((uint8_t)0x15)   /* Low  byte for Z-axis data */
#define IST8308_DATAZH            ((uint8_t)0x16)   /* High byte for Z-axis data */
#define IST8308_ACTR              ((uint8_t)0x20)   /* Suspend Mode control */
#define IST8308_CNTL1             ((uint8_t)0x30)   /* Chip control settings 1 */
#define IST8308_CNTL2             ((uint8_t)0x31)   /* Chip control settings 2 */
#define IST8308_CNTL3             ((uint8_t)0x32)   /* Chip control settings 3 */
#define IST8308_CNTL4             ((uint8_t)0x34)   /* Chip control settings 4 */
#define IST8308_OSRCNTL           ((uint8_t)0x41)   /* Oversampling ratio setting */

#define IST8308_SENS_500UT_LSB    (6.6)             /* ±500 uT, LSB/uT */
#define IST8308_SENS_200UT_LSB    (13.2)            /* ±200 uT, LSB/uT */
#define IST8310_SENS_500_LSB2UT   (0.15)
#define IST8310_SENS_200_LSB2UT   (0.075)


/* ist8310 */
#define IST8310_STAT1             ((uint8_t)0x02)   /* Data Status 1 */
#define IST8310_DATAXL            ((uint8_t)0x03)   /* Low  byte for X-axis data */
#define IST8310_DATAXH            ((uint8_t)0x04)   /* High byte for X-axis data */
#define IST8310_DATAYL            ((uint8_t)0x05)   /* Low  byte for Y-axis data */
#define IST8310_DATAYH            ((uint8_t)0x06)   /* High byte for Y-axis data */
#define IST8310_DATAZL            ((uint8_t)0x07)   /* Low  byte for Z-axis data */
#define IST8310_DATAZH            ((uint8_t)0x08)   /* High byte for Z-axis data */
#define IST8310_STAT2             ((uint8_t)0x09)   /* Data Status 2 */
#define IST8310_CNTL1             ((uint8_t)0x0A)   /* Control Register 1 */
#define IST8310_CNTL2             ((uint8_t)0x0B)   /* Control Register 2 */
#define IST8310_STR               ((uint8_t)0x0C)   /* Self-Test Register */
#define IST8310_TEMPL             ((uint8_t)0x1C)   /* Low  byte for Temperature data */
#define IST8310_TEMPH             ((uint8_t)0x1D)   /* High byte for Temperature data */
#define IST8310_AVGCNTL           ((uint8_t)0x41)   /* Average Control Register */
#define IST8310_PDCNTL            ((uint8_t)0x42)   /* Pulse Duration Control Register */

#define IST8310_SENS_1600UT_LSB   (3.3)             /* XY ±1600 uT, Z ±2500 uT, LSB/uT */
#define IST8310_SENS_1600_LSB2UT  (0.3)


/* ist8310x */
#define IST8310X_SENS_1600UT_LSB  IST8310_SENS_1600UT_LSB
#define IST8310X_SENS_1600_LSB2UT (0.3)


/* ist8315 */
#define IST8315_STR               ((uint8_t)0x0C)   /* Self-Test Register */
#define IST8315_STAT1             ((uint8_t)0x10)   /* Data status information */
#define IST8315_DATAXL            ((uint8_t)0x11)   /* Low  byte for X-axis data */
#define IST8315_DATAXH            ((uint8_t)0x12)   /* High byte for X-axis data */
#define IST8315_DATAYL            ((uint8_t)0x13)   /* Low  byte for Y-axis data */
#define IST8315_DATAYH            ((uint8_t)0x14)   /* High byte for Y-axis data */
#define IST8315_DATAZL            ((uint8_t)0x15)   /* Low  byte for Z-axis data */
#define IST8315_DATAZH            ((uint8_t)0x16)   /* High byte for Z-axis data */
#define IST8315_STAT2             ((uint8_t)0x18)   /* Data status information */
#define IST8315_ACTR              ((uint8_t)0x20)   /* Suspend Mode control */
#define IST8315_CNTL1             ((uint8_t)0x30)   /* Chip control settings 1 */
#define IST8315_CNTL2             ((uint8_t)0x31)   /* Chip control settings 2 */
#define IST8315_CNTL3             ((uint8_t)0x32)   /* Chip control settings 3 */
#define IST8315_CNTL4             ((uint8_t)0x34)   /* Chip control settings 4 */
#define IST8315_CNTL6             ((uint8_t)0x35)   /* Chip control settings 5 */
#define IST8315_CHIPTEST          ((uint8_t)0x40)   /* Chip test register */
#define IST8315_OSRCNTL           ((uint8_t)0x41)   /* Oversampling ratio setting */

#define IST8315_SENS_1000UT_LSB   (3.3)             /* ±1000 uT, LSB/uT */
#define IST8315_SENS_500UT_LSB    (6.6)             /* ±500 uT, LSB/uT */
#define IST8315_SENS_1000_LSB2UT  (0.3)
#define IST8315_SENS_500_LSB2UT   (0.15)


/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
typedef struct
{
  uint8_t i2cAddress;   // 8-bit address
  uint8_t deviceId;
  float   sensitivity;  // sensor sensitivity (LSB to uT)

} ist83xx_t;

/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
void      IST83_Config( void );
uint32_t  IST83_Init( uint8_t index );
uint32_t  IST83_DeviceCheck( uint8_t index );
uint32_t  IST83_GetDeviceAddress( uint8_t index );
uint32_t  IST83_GetDeviceID( uint8_t index );
float     IST83_GetMagSensitivity( uint8_t index );
uint32_t  IST83_GetRawData( uint8_t index, int16_t *raw );
uint32_t  IST83_GetData( uint8_t index, float *data );
int16_t   IST83_GetTemprature( uint8_t index );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
