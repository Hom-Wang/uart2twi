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
#define KSERIAL_SEND_ENABLE             (1U)
#define KSERIAL_RECV_ENABLE             (1U)

#define KS_MAX_SEND_BUFF_SIZE           (4096 + 32)
#define KS_MAX_RECV_BUFF_SIZE           (4096 + 1024 + 32)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
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
