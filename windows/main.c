
/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "uart2twi.h"

/* Define ----------------------------------------------------------------------------------*/
#define FILENAME    "setting.txt"

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
uint8_t uart2twibuff[4096] = {0};
serial_t ser = {.cfg = {0, 115200, {'8', 'N', '1', 0}}};

/* Prototypes ------------------------------------------------------------------------------*/
static uint32_t read_setting( char *filename, serial_config_t *cfg );
static uint32_t write_setting( char *filename, uint32_t port, uint32_t baudrate );

static uint32_t command_help( void );
static uint32_t command_uart( uint32_t port, uint32_t baudrate );
static uint32_t command_i2c_delay( uint32_t times );
static uint32_t command_i2c_check( void );
static uint32_t command_i2c_scandevice( void );
static uint32_t command_i2c_scanregister( uint32_t address );
static uint32_t command_i2c_write_single( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t sdata );
static uint32_t command_i2c_write_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t *writeData );
static uint32_t command_i2c_read_single( uint32_t slaveAddress, uint32_t deviceRegister );
static uint32_t command_i2c_read_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t readLength );

/* Functions -------------------------------------------------------------------------------*/

int main( int argc, char **argv )
{
    uint32_t status;

    // load setting
    status = read_setting(FILENAME, &ser.cfg);
    if (status != KS_OK)
    {
        return KS_ERROR;
    }

    // command (without serial)
    switch (argc)
    {
        case 1:
        {
            // >> i2c
            printf("  COM%d, %d,", ser.cfg.port + 1, ser.cfg.baudrate);
            break;
        }
        case 2:
        {
            // >> i2c -help
            if ((strcmp("-h", argv[1]) == 0) || (strcmp("-H", argv[1]) == 0) ||
                (strcmp("-help", argv[1]) == 0) || (strcmp("-HELP", argv[1]) == 0))
            {
                status = command_help();
                return status;
            }
            break;
        }
        case 4:
        {
            // >> i2c -uart port baudrate
            if ((strcmp("-uart", argv[1]) == 0) || (strcmp("-UART", argv[1]) == 0))
            {
                uint32_t port = strtoul(argv[2], NULL, 0);
                uint32_t baudrate = strtoul(argv[3], NULL, 0);
                status = command_uart(port, baudrate);
                return status;
            }
            break;
        }
        default:
        {
            break;
        }
    }

    // open serial port
    Serial_Config(&ser);
    if (!ser.open(&ser.cfg))
    {
        return KS_ERROR;
    }

    // command (with serial)
    switch (argc)
    {
        case 1:
        {
            // >> i2c
            status = command_i2c_check();
            if (status == KS_OK)
            {
                status = command_i2c_scandevice();
            }
            break;
        }
        case 2:
        {
            // >> i2c -scan
            if ((strcmp("-check", argv[1]) == 0) || (strcmp("-CHECK", argv[1]) == 0))
            {
                status = command_i2c_check();
                if (status == KS_OK)
                {
                    printf("  device ok\r\n");
                }
            }
            // >> i2c -scan
            else if ((strcmp("-s", argv[1]) == 0) || (strcmp("-S", argv[1]) == 0) ||
                     (strcmp("-scan", argv[1]) == 0) || (strcmp("-SCAN", argv[1]) == 0))
            {
                status = command_i2c_scandevice();
            }
            break;
        }
        case 3:
        {
            // >> i2c -d time
            if ((strcmp("-d", argv[1]) == 0) || (strcmp("-D", argv[1]) == 0) ||
                (strcmp("-delay", argv[1]) == 0) || (strcmp("-DELAY", argv[1]) == 0))
            {
                uint32_t times = strtoul(argv[2], NULL, 0);
                status = command_i2c_delay(times);
                break;
            }
            // >> i2c -reg address
            else if ((strcmp("-reg", argv[1]) == 0) || (strcmp("-REG", argv[1]) == 0))
            {
                uint32_t address = strtoul(argv[2], NULL, 0);
                status = command_i2c_scanregister(address);
                break;
            }
            break;
        }
        case 4:
        {
            // >> i2c -read address register
            if ((strcmp("r", argv[1]) == 0) || (strcmp("-r", argv[1]) == 0) || (strcmp("-R", argv[1]) == 0) ||
                (strcmp("-read", argv[1]) == 0) || (strcmp("-READ", argv[1]) == 0))
            {
                uint32_t slaveAddress = strtoul(argv[2], NULL, 0);
                uint32_t deviceRegister = strtoul(argv[3], NULL, 0);
                status = command_i2c_read_single(slaveAddress, deviceRegister);
                return status;
            }
            break;
        }
        case 5:
        {
            // >> i2c -read address register lenght
            if ((strcmp("r", argv[1]) == 0) || (strcmp("-r", argv[1]) == 0) || (strcmp("-R", argv[1]) == 0) ||
                (strcmp("-read", argv[1]) == 0) || (strcmp("-READ", argv[1]) == 0))
            {
                uint32_t slaveAddress = strtoul(argv[2], NULL, 0);
                uint32_t deviceRegister = strtoul(argv[3], NULL, 0);
                uint32_t readLength = strtoul(argv[4], NULL, 0);        
                status = command_i2c_read_multiple(slaveAddress, deviceRegister, readLength);   
            }
            // >> i2c -write address register data
            else if ((strcmp("w", argv[1]) == 0) || (strcmp("-w", argv[1]) == 0) || (strcmp("-W", argv[1]) == 0) ||
                     (strcmp("-write", argv[1]) == 0) || (strcmp("-WRITE", argv[1]) == 0))
            {
                    uint32_t slaveAddress = strtoul(argv[2], NULL, 0);
                    uint32_t deviceRegister = strtoul(argv[3], NULL, 0);
                    uint32_t writeData = strtoul(argv[4], NULL, 0);
                    status =  command_i2c_write_single(slaveAddress, deviceRegister, writeData);
            }
            break;
        }
        case 6:
        {
            // TODO: multiple write "case n"
            // >> i2c -write address register data1 data2 ...
            if ((strcmp("w", argv[1]) == 0) || (strcmp("-w", argv[1]) == 0) || (strcmp("-W", argv[1]) == 0) ||
                (strcmp("-write", argv[1]) == 0) || (strcmp("-WRITE", argv[1]) == 0))
            {
                    uint32_t slaveAddress = strtoul(argv[2], NULL, 0);
                    uint32_t deviceRegister = strtoul(argv[3], NULL, 0);
                    uint32_t writeData[2] = {0};
                    for (uint32_t i = 0; i < 2; i++)
                    {
                        writeData[i] = strtoul(argv[4 + i], NULL, 0);
                    }
                    status =  command_i2c_write_multiple(slaveAddress, deviceRegister, writeData);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    // Close COM Port
    ser.close(&ser.cfg);

    return KS_OK;
}

static uint32_t read_setting( char *filename, serial_config_t *cfg )
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("  open setting file error ... create new\n");
        fp = fopen(filename, "ab");
        fprintf(fp, "0,115200,8,N,1,0");    /// default setting
        fclose(fp);
        return KS_ERROR;
    }
    else
    {
        int port;
        int baudrate;
        char mode[4] = {0};
        fscanf(fp, "%d,%d,%c,%c,%c,%c", &port, &baudrate, &mode[0], &mode[1], &mode[2], &mode[3]);
        cfg->port = port;
        cfg->baudrate = baudrate;
        cfg->mode[0] = mode[0];
        cfg->mode[1] = mode[1];
        cfg->mode[2] = mode[2];
        cfg->mode[3] = mode[3] - 48;
        fclose(fp);
    }
    return KS_OK;
}

static uint32_t write_setting( char *filename, uint32_t port, uint32_t baudrate )
{
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        return KS_ERROR;
    }
    else
    {
        fprintf(fp, "%d,%d,8,N,1,0", port, baudrate);    /// default setting
        fclose(fp);
    }
    return KS_OK;
}

static uint32_t command_help( void )
{
    printf("\n");
    // printf("  i2c                               ... i2c and serial port information\n");
    // printf("  i2c -help                         ... command help\n");
    printf("  -CHECK                            ... check uart to i2c device ok\n");
    printf("  -SCAN                             ... i2c scan device\n");
    printf("  -REG [ADDRESS]                    ... i2c get all register data\n");
    printf("  -UART [PORT] [BAUDRATE]           ... serial port setting\n");
    printf("\n");
    printf("  -R [ADDRESS] [REGISTER]           ... i2c single read data\n");
    printf("  -R [ADDRESS] [REGISTER] [LENS]    ... i2c multiple write data\n");
    printf("  -W [ADDRESS] [REGISTER] [DATA]    ... i2c single write data\n");
    return KS_OK;
}

static uint32_t command_uart( uint32_t port, uint32_t baudrate )
{
    uint32_t status;

    printf("  COM%d, baudrate %d\n", port, baudrate);
    ser.cfg.port = port - 1;
    ser.cfg.baudrate = baudrate;
    status = write_setting(FILENAME, ser.cfg.port, ser.cfg.baudrate);
    if (status != KS_OK)
    {
        printf("  write setting file error\n");
        return KS_ERROR;
    }
    return KS_OK;
}

static uint32_t command_i2c_delay( uint32_t times )
{
    printf("delay %d ms\n", times);
    i2c_delay(&ser, times);
    return KS_OK;
}
static uint32_t command_i2c_check( void )
{
    uint32_t status;
    status = i2c_read(&ser, 0x00, 0x00, uart2twibuff, 1, 50);
    if (status != KS_OK)
    {
        printf("  no response ... please check your device\n");
        return KS_ERROR;
    }
    return KS_OK;
}

static uint32_t command_i2c_scandevice( void )
{
    // scan device address
    uint32_t devicenum;
    devicenum = i2c_scandevice(&ser, uart2twibuff, 100);
    if (devicenum > 128)
    {
        printf("  i2c secn device error\n");
        return KS_ERROR;
    }
    else
    {
        printf("  found %d device ...", devicenum);
        for (uint32_t i = 0; i < devicenum; i++)
        {
            printf(" %02X", uart2twibuff[i]);
        }
        printf("\n");
    }
    return KS_OK;
}

static uint32_t command_i2c_scanregister( uint32_t address )
{
    uint32_t status;

    // scan device register
    status = i2c_scanregister(&ser, address, uart2twibuff, 100);
    if (status != KS_OK)
    {
        printf("  i2c secn register error\n");
        return KS_ERROR;
    }
    else
    {
        printf("\n");
        printf("       0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
        for (uint32_t i = 0; i < 256; i += 16)
        {
            printf("  %02X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                i,
                uart2twibuff[i +  0], uart2twibuff[i +  1], uart2twibuff[i +  2], uart2twibuff[i +  3],
                uart2twibuff[i +  4], uart2twibuff[i +  5], uart2twibuff[i +  6], uart2twibuff[i +  7],
                uart2twibuff[i +  8], uart2twibuff[i +  9], uart2twibuff[i + 10], uart2twibuff[i + 11],
                uart2twibuff[i + 12], uart2twibuff[i + 13], uart2twibuff[i + 14], uart2twibuff[i + 15]
            );
        }
    }
    return KS_OK;
}

static uint32_t command_i2c_write_single( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t writeData )
{
    // >> i2c -w 0x0c 0x00 32
    i2c_write(&ser, slaveAddress, deviceRegister, writeData);
    printf("  %02X\n", writeData);
    return KS_OK;
}

static uint32_t command_i2c_write_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t *writeData )
{
#if 1
    printf("  i2c multiple write not ready\n");
    return KS_ERROR;
#else
    // multiple write
    uint8_t slaveAddress = strtoul(argv[2], NULL, 0);
    uint8_t deviceRegister = strtoul(argv[3], NULL, 0);
    uint8_t writeLength = argc - 4;
    uint8_t lens;
    for (uint32_t i = 0; i < writeLength; i++)
    {
        uart2twibuff[i] = strtoul(argv[i + 4], NULL, 0);
    }
    // lens = i2c_write(&s, slaveAddress, deviceRegister, uart2twibuff, writeLength);
    if (lens < 2)
    {
        printf("  i2c multiple write error\n");
    }
    else
    {
        printf("  %d\n", writeLength);
    }
#endif
}

static uint32_t command_i2c_read_single( uint32_t slaveAddress, uint32_t deviceRegister )
{
    uint32_t status;

    // single read
    status = i2c_read(&ser, slaveAddress, deviceRegister, uart2twibuff, 1, 50);
    if (status != KS_OK)
    {
        printf("  i2c single read error\n");
        return KS_ERROR;
    }
    else
    {
        printf("  %02X\n", uart2twibuff[0]);
    }

    return KS_OK;
}

static uint32_t command_i2c_read_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t readLength )
{
    uint32_t status;

    status = i2c_read(&ser, slaveAddress, deviceRegister, uart2twibuff, readLength, 50);
    if (status != KS_OK)
    {
        printf("  i2c multiple read error\n");
        return KS_ERROR;
    }
    else
    {
        printf(" ");
        for (uint32_t i = 0; i < readLength; i++)
        {
            printf(" %02X", uart2twibuff[i]);
        }
        printf("\n");
    }
    // printf("address = %02X, register = %02X\n", slaveAddress, deviceRegister);

    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
