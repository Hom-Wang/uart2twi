/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kSerial.h
 *  @author  KitSprout
 *  @date    Dec-2019
 *  @brief   
 * 
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __KSERIAL_H
#define __KSERIAL_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include <stdint.h>

/* Define ----------------------------------------------------------------------------------*/
#define KSERIAL_SEND_ENABLE             (0U)
#define KSERIAL_RECV_ENABLE             (0U)

#define KS_MAX_SEND_BUFF_SIZE           (4096 + 32)
#define KS_MAX_RECV_BUFF_SIZE           (4096 + 1024 + 32)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/

typedef enum
{
    KS_OK       = 0U,
    KS_ERROR    = 1U,
    KS_BUSY     = 2U,
    KS_TIMEOUT  = 3U

} KSStatus_t;

typedef enum
{
    KS_U8     = 0x0,  /* 4'b 0000 */
    KS_U16    = 0x1,  /* 4'b 0001 */
    KS_U32    = 0x2,  /* 4'b 0010 */
    KS_U64    = 0x3,  /* 4'b 0011 */
    KS_I8     = 0x4,  /* 4'b 0100 */
    KS_I16    = 0x5,  /* 4'b 0101 */
    KS_I32    = 0x6,  /* 4'b 0110 */
    KS_I64    = 0x7,  /* 4'b 0111 */
    KS_F16    = 0x9,  /* 4'b 1001 */
    KS_F32    = 0xA,  /* 4'b 1010 */
    KS_F64    = 0xB,  /* 4'b 1011 */

    KS_R0     = 0x8,  /* 4'b 1000 */
    KS_R1     = 0xC,  /* 4'b 1100 */
    KS_R2     = 0xD,  /* 4'b 1101 */
    KS_R3     = 0xE,  /* 4'b 1110 */
    KS_R4     = 0xF,  /* 4'b 1111 */

} KSUnit_t;

/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

uint32_t    kSerial_GetTypeSize( uint32_t type );

uint32_t    kSerial_CheckHeader( uint8_t *packet, void *param, uint32_t *type, uint32_t *lens );
uint32_t    kSerial_CheckEnd( uint8_t *packet, const uint32_t lens );
uint32_t    kSerial_Check( uint8_t *packet, void *param, uint32_t *type, uint32_t *lens );

uint32_t    kSerial_Pack( uint8_t *packet, const void *param, const uint32_t type, const uint32_t lens, void *pdata );
uint32_t    kSerial_Unpack( uint8_t *packet, void *param, uint32_t *type, uint32_t *lens, void *pdata );

uint32_t    kSerial_SendPacket( void *param, void *sdata, const uint32_t lens, const uint32_t type );
uint32_t    kSerial_RecvPacket( void *param, void *rdata, uint32_t *lens, uint32_t *type );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
