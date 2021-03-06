/**
 *  @file   ist_command.h
 *  @author iSentek
 *  @date   Nov-2018 v1.0.0
 *  @brief  iSentek command
 */

/* Define to prevent recursive inclusion ---------------------------------------------------*/
#ifndef __IST_COMMAND_H
#define __IST_COMMAND_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes --------------------------------------------------------------------------------*/
#include "drivers\ks_system.h"

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Extern ----------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/
uint32_t  istcmd_run( uint8_t *cmd );
void      istcmd_print_parameter( void );

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ****************************************/
