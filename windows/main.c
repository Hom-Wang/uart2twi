
/* Includes --------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <conio.h>

#include "kStatus.h"
#include "kSerial.h"
#include "serial.h"
// #include "uart2twi.h"

/* Define ----------------------------------------------------------------------------------*/
#define FILENAME    "setting.txt"

/* Macro -----------------------------------------------------------------------------------*/
/* Typedef ---------------------------------------------------------------------------------*/
/* Variables -------------------------------------------------------------------------------*/
/* Prototypes ------------------------------------------------------------------------------*/
static uint32_t load_setting( char *filename, int *port, int *baudrate );
static uint32_t update_setting( char *filename, int port, int baudrate );

static uint32_t command_help( void );
static uint32_t command_uart( uint32_t port, uint32_t baudrate );
static uint32_t command_i2c_delay( uint32_t times );
static uint32_t command_i2c_check( void );
static uint32_t command_i2c_scandevice( void );
static uint32_t command_i2c_scanregister( uint32_t address );
static uint32_t command_i2c_write_single( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t sdata );
// static uint32_t command_i2c_write_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t *writeData );
static uint32_t command_i2c_read_single( uint32_t slaveAddress, uint32_t deviceRegister );
static uint32_t command_i2c_read_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t readLength );

/* Functions -------------------------------------------------------------------------------*/

int main( int argc, char **argv )
{
    uint32_t status;

    // scan comport
    if (Serial_GetComportList(&cplist))
    {
        printf("\n  scan comport error\n");
        return KS_ERROR;
    }

    // load setting
    status = load_setting(FILENAME, &s.port, &s.cfg.baudrate);
    if (status != KS_OK)
    {
        if (cplist.num > 0)
        {
            s.port = cplist.port[0];
            update_setting(FILENAME, s.port, s.cfg.baudrate);
        }
    }

    // command (without serial)
    switch (argc)
    {
        case 1:
        {
            // >> i2c
            printf("  COM%d, %d", s.port, s.cfg.baudrate);
            break;
        }
        case 2:
        {
            // >> i2c -help
            if ((strcmp("-h", argv[1]) == 0) || (strcmp("-H", argv[1]) == 0) || (strcmp("-help", argv[1]) == 0) || (strcmp("-HELP", argv[1]) == 0))
            {
                status = command_help();
                return status;
            }
             // >> i2c -uart
            else if ((strcmp("-uart", argv[1]) == 0) || (strcmp("-UART", argv[1]) == 0))
            {
                if (cplist.num > 0)
                {
                    s.port = cplist.port[0];
                    update_setting(FILENAME, s.port, s.cfg.baudrate);
                    printf("  select COM%d, baudrate %d\n", s.port, s.cfg.baudrate);
                }
                else
                {
                    printf("  not available port\n");
                }
            }
            break;
        }
        case 3:
        {
            // >> i2c -uart list
            if ((strcmp("-uart", argv[1]) == 0) || (strcmp("-UART", argv[1]) == 0))
            {
                if ((strcmp("list", argv[2]) == 0) || (strcmp("LIST", argv[2]) == 0))
                {
                    printf("\n");
                    for (uint32_t i = 0; i < cplist.num; i++)
                    {
                        printf("  [%d] COM%d\t(%s)\n", i + 1, cplist.port[i], cplist.info[i]);
                    }
                    if (cplist.num > 0)
                    {
                        printf("\n  ");
                        char portSelect = getch();
                        if ((portSelect < 48) || (portSelect > 57))
                        {
                            printf("wrong input ... %c\n", portSelect);
                            return KS_ERROR;
                        }
                        portSelect -= 48;
                        if (portSelect == 0)
                        {
                            printf("exit\n");
                            return KS_OK;
                        }
                        else if (portSelect > cplist.num)
                        {
                            printf("not in list ... %d\n", portSelect);
                            return KS_ERROR;
                        }
                        s.port = cplist.port[portSelect - 1];
                        update_setting(FILENAME, s.port, s.cfg.baudrate);
                        printf("select COM%d, baudrate %d\n", s.port, s.cfg.baudrate);
                    }
                }
                return KS_OK;
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
    if (Serial_OpenComport(&s))
    {
        printf("\n  open serial error\n");
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
                // printf(", connect success,");
                printf(",");
                status = command_i2c_scandevice();
            }
            else
            {
                printf(", connect failed\n");
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
                else
                {
                    printf("  no response ... please check your device (COM%d, baudrate %d)\n", s.port, s.cfg.baudrate);
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
        // case 6:
        // {
        //     // TODO: multiple write "case n"
        //     // >> i2c -write address register data1 data2 ...
        //     if ((strcmp("w", argv[1]) == 0) || (strcmp("-w", argv[1]) == 0) || (strcmp("-W", argv[1]) == 0) ||
        //         (strcmp("-write", argv[1]) == 0) || (strcmp("-WRITE", argv[1]) == 0))
        //     {
        //             uint32_t slaveAddress = strtoul(argv[2], NULL, 0);
        //             uint32_t deviceRegister = strtoul(argv[3], NULL, 0);
        //             uint32_t writeData[2] = {0};
        //             for (uint32_t i = 0; i < 2; i++)
        //             {
        //                 writeData[i] = strtoul(argv[4 + i], NULL, 0);
        //             }
        //             status =  command_i2c_write_multiple(slaveAddress, deviceRegister, writeData);
        //     }
        //     break;
        // }
        default:
        {
            break;
        }
    }

    // Close COM Port
    Serial_CloseComport(&s);
    Serial_FreeComportList(&cplist);

    return KS_OK;
}

#define DEFAULT_COMPORT     1
#define DEFAULT_BAUDRATE    115200

static uint32_t load_setting( char *filename, int *port, int *baudrate )
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("  open setting file error ... create new\n");
        fp = fopen(filename, "ab");
        *port = DEFAULT_COMPORT;
        *baudrate = DEFAULT_BAUDRATE;
        fprintf(fp, "%d,%d,8,N,1,0", *port, *baudrate);    // default setting
        fclose(fp);
        return KS_ERROR;
    }
    else
    {
        char mode[4] = {0};
        fscanf(fp, "%d,%d,%c,%c,%c,%c", port, baudrate, &mode[0], &mode[1], &mode[2], &mode[3]);
        fclose(fp);
    }
    return KS_OK;
}

static uint32_t update_setting( char *filename, int port, int baudrate )
{
    FILE *fp;
    fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        return KS_ERROR;
    }
    else
    {
        fprintf(fp, "%d,%d,8,N,1,0", port, baudrate);   // default setting
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
    s.port = port;
    s.cfg.baudrate = baudrate;
    status = update_setting(FILENAME, s.port, s.cfg.baudrate);
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
    // i2c_delay(&ser, times);
    return KS_OK;
}

static uint32_t command_i2c_check( void )
{
    return kSerial_TwiCheck();
}

static uint32_t command_i2c_scandevice( void )
{
    // scan device address
    uint8_t slaveAddr[128];
    uint32_t devicenum;
    devicenum = kSerial_TwiScanDevice(slaveAddr);
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
            printf(" %02X", slaveAddr[i]);
        }
        printf("\n");
    }
    return KS_OK;
}

static uint32_t command_i2c_scanregister( uint32_t address )
{
    uint8_t reg[256];
    uint32_t status;

    // scan device register
    status = kSerial_TwiScanRegister(address, reg);
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
                reg[i +  0], reg[i +  1], reg[i +  2], reg[i +  3],
                reg[i +  4], reg[i +  5], reg[i +  6], reg[i +  7],
                reg[i +  8], reg[i +  9], reg[i + 10], reg[i + 11],
                reg[i + 12], reg[i + 13], reg[i + 14], reg[i + 15]
            );
        }
    }
    return KS_OK;
}

static uint32_t command_i2c_write_single( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t writeData )
{
    // >> i2c -w 0x0c 0x00 32
    kSerial_TwiWriteReg(slaveAddress, deviceRegister, writeData);
    printf("  %02X\n", writeData);
    return KS_OK;
}

// static uint32_t command_i2c_write_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t *writeData )
// {
// #if 1
//     printf("  i2c multiple write not ready\n");
//     return KS_ERROR;
// #else
//     // multiple write
//     uint8_t slaveAddress = strtoul(argv[2], NULL, 0);
//     uint8_t deviceRegister = strtoul(argv[3], NULL, 0);
//     uint8_t writeLength = argc - 4;
//     uint8_t lens;
//     for (uint32_t i = 0; i < writeLength; i++)
//     {
//         uart2twibuff[i] = strtoul(argv[i + 4], NULL, 0);
//     }
//     // lens = i2c_write(&s, slaveAddress, deviceRegister, uart2twibuff, writeLength);
//     if (lens < 2)
//     {
//         printf("  i2c multiple write error\n");
//     }
//     else
//     {
//         printf("  %d\n", writeLength);
//     }
// #endif
// }

static uint32_t command_i2c_read_single( uint32_t slaveAddress, uint32_t deviceRegister )
{
    uint8_t res;
    uint32_t status;

    // single read
    status = kSerial_TwiReadReg(slaveAddress, deviceRegister, &res);
    if (status != KS_OK)
    {
        printf("  i2c single read error\n");
        return KS_ERROR;
    }
    else
    {
        printf("  %02X\n", res);
    }

    return KS_OK;
}

static uint32_t command_i2c_read_multiple( uint32_t slaveAddress, uint32_t deviceRegister, uint32_t readLength )
{
    uint8_t res[256];
    uint32_t status;

    status = kSerial_TwiReadRegs(slaveAddress, deviceRegister, res, readLength);
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
            printf(" %02X", res[i]);
        }
        printf("\n");
    }
    // printf("address = %02X, register = %02X\n", slaveAddress, deviceRegister);

    return KS_OK;
}

/*************************************** END OF FILE ****************************************/
