/**
 *      __            ____
 *     / /__ _  __   / __/                      __  
 *    / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
 *   / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
 *  /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
 *                    /_/   github.com/KitSprout    
 * 
 *  @file    kLogger.c
 *  @author  KitSprout
 *  @date    Jan-2020
 *  @brief   
 * 
 */

/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "kLogger.h"

/* Define ----------------------------------------------------------------------------------*/
#define ENABLE_KSLOG    (0U)

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

int klogd( const char *fmt, ... )
{
#if ENABLE_KSLOG
    uint32_t lens = 0;

    printf("[DBG] ");

    va_list args;
    va_start(args, fmt);
    lens = vprintf(fmt, args);
    va_end(args);

    // printf("\n");

    return (lens + 6);
#else
    return 0;
#endif
}

/*************************************** END OF FILE ****************************************/
