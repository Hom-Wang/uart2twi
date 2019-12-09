/**
 *  @file   ist_parameter.c
 *  @author iSentek
 *  @date   Nov-2018
 *  @brief  iSentek parameter
 */

/* Includes --------------------------------------------------------------------------------*/
#include "ist_parameter.h"

/** @addtogroup KS_Program
 *  @{
 */

/* Define ----------------------------------------------------------------------------------*/
/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------------------*/

static uint32_t istparam_get_crc32( ist_parameter_t *param )
{
    uint32_t i;
    uint32_t crc32 = 0;
    uint32_t *ptr = (uint32_t *)param;

    for (i = 0; i < IST_PARAMETER_DATA_LENS - 1; i++)
    {
        crc32 += ptr[i];
    }
    return crc32;
}

void istparam_set_default( ist_parameter_t *param )
{
    ist_parameter_t param_default = IST_PARAMETER_INIT;
    memcpy(param, &param_default, sizeof(ist_parameter_t));
}

uint32_t istparam_load( ist_parameter_t *param )
{
    FLASH_ReadDataU32(IST_PARAMETER_SAVE_ADDRESS, (uint32_t *)param, IST_PARAMETER_DATA_LENS);
    return ((param->crc32 != istparam_get_crc32(param)) ? KS_ERROR : KS_OK);
}

uint32_t istparam_save( ist_parameter_t *param )
{
    param->crc32 = istparam_get_crc32(param);
    FLASH_ErasePage(IST_PARAMETER_SAVE_ADDRESS);
    FLASH_WriteDataU32(IST_PARAMETER_SAVE_ADDRESS, (uint32_t *)param, IST_PARAMETER_DATA_LENS);
    return istparam_load(param);
}

void istparam_setting( ist_parameter_t *param )
{
//#if IST_IS_INCLUDE(IST_SENSOR, SENSOR_GYR)
//    ist_set_gyr_bias(param->gyrBias);
//    ist_set_gyr_cali_matrix(param->gyrCali);
//#endif
//#if IST_IS_INCLUDE(IST_SENSOR, SENSOR_ACC)
//    ist_set_acc_bias(param->accBias);
//    ist_set_acc_cali_matrix(param->accCali);
//#endif
//    ist_set_mag_bias(param->magBias);
//    ist_set_mag_cali_matrix(param->magCali);
//    ist_set_mag_geomagnetic(param->magGeomagnetic);
}

// B_ENU = R_NE * B_NED
// S_ENU = R_NE * S_NED * R_EN
void istparam_conv_ned2enu( float cvCali[3][9], float cvBias[3][4], ist_parameter_t *param )
{
    // gyr
    cvBias[0][0] = -param->gyrBias[1];
    cvBias[0][1] = -param->gyrBias[0];
    cvBias[0][2] =  param->gyrBias[2];

    cvCali[0][0] =  param->gyrCali[4];
    cvCali[0][1] =  param->gyrCali[3];
    cvCali[0][2] = -param->gyrCali[5];
    cvCali[0][3] =  param->gyrCali[1];
    cvCali[0][4] =  param->gyrCali[0];
    cvCali[0][5] = -param->gyrCali[2];
    cvCali[0][6] = -param->gyrCali[7];
    cvCali[0][7] = -param->gyrCali[6];
    cvCali[0][8] =  param->gyrCali[8];

    // acc
    cvBias[1][0] = -param->accBias[1];
    cvBias[1][1] = -param->accBias[0];
    cvBias[1][2] =  param->accBias[2];
    cvBias[1][3] =  param->accGravity;

    cvCali[1][0] =  param->accCali[4];
    cvCali[1][1] =  param->accCali[3];
    cvCali[1][2] = -param->accCali[5];
    cvCali[1][3] =  param->accCali[1];
    cvCali[1][4] =  param->accCali[0];
    cvCali[1][5] = -param->accCali[2];
    cvCali[1][6] = -param->accCali[7];
    cvCali[1][7] = -param->accCali[6];
    cvCali[1][8] =  param->accCali[8];

    // mag
    cvBias[2][0] =  param->magBias[1];
    cvBias[2][1] =  param->magBias[0];
    cvBias[2][2] = -param->magBias[2];
    cvBias[2][3] =  param->magGeomagnetic;

    cvCali[2][0] =  param->magCali[4];
    cvCali[2][1] =  param->magCali[3];
    cvCali[2][2] = -param->magCali[5];
    cvCali[2][3] =  param->magCali[1];
    cvCali[2][4] =  param->magCali[0];
    cvCali[2][5] = -param->magCali[2];
    cvCali[2][6] = -param->magCali[7];
    cvCali[2][7] = -param->magCali[6];
    cvCali[2][8] =  param->magCali[8];

#if 0
    istparam_print(param);

    printf("\r\n");
    printf(" > GYR  %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[0][0], cvCali[0][1], cvCali[0][2], cvBias[0][0]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[0][3], cvCali[0][4], cvCali[0][5], cvBias[0][1]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[0][6], cvCali[0][7], cvCali[0][8], cvBias[0][2]);
    printf("\r\n");
    printf(" > ACC  %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[1][0], cvCali[1][1], cvCali[1][2], cvBias[1][0]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[1][3], cvCali[1][4], cvCali[1][5], cvBias[1][1]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[1][6], cvCali[1][7], cvCali[1][8], cvBias[1][2]);
    printf("                                             %11.6f\r\n", cvBias[1][3]);
    printf("\r\n");
    printf(" > MAG  %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[2][0], cvCali[2][1], cvCali[2][2], cvBias[2][0]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[2][3], cvCali[2][4], cvCali[2][5], cvBias[2][1]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", cvCali[2][6], cvCali[2][7], cvCali[2][8], cvBias[2][2]);
    printf("                                             %11.6f\r\n", cvBias[2][3]);
    printf("\r\n");
#endif
}

void istparam_print( ist_parameter_t *param )
{
    printf("\r\n");
    printf(" >>> IST PARAMETER\r\n");
    printf(" > HEADER : %08X\r\n", param->header);
    printf(" > CRC32  : %08X\r\n", param->crc32);
    printf("\r\n");
    printf(" > GYR  %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->gyrCali[0], param->gyrCali[1], param->gyrCali[2], param->gyrBias[0]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->gyrCali[3], param->gyrCali[4], param->gyrCali[5], param->gyrBias[1]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->gyrCali[6], param->gyrCali[7], param->gyrCali[8], param->gyrBias[2]);
    printf("\r\n");
    printf(" > ACC  %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->accCali[0], param->accCali[1], param->accCali[2], param->accBias[0]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->accCali[3], param->accCali[4], param->accCali[5], param->accBias[1]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->accCali[6], param->accCali[7], param->accCali[8], param->accBias[2]);
    printf("                                             %11.6f\r\n", param->accGravity);
    printf("\r\n");
    printf(" > MAG  %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->magCali[0], param->magCali[1], param->magCali[2], param->magBias[0]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->magCali[3], param->magCali[4], param->magCali[5], param->magBias[1]);
    printf("        %9.6f  %9.6f  %9.6f      %11.6f\r\n", param->magCali[6], param->magCali[7], param->magCali[8], param->magBias[2]);
    printf("                                             %11.6f\r\n", param->magGeomagnetic);
    printf("\r\n");
}

/*************************************** END OF FILE ****************************************/
