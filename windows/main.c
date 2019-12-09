
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

#include "serial.h"
#include "kSerial.h"

#define FILENAME "setting.txt"
#define KSERIAL_RW_BUFFER_SIZE  4096
uint8_t i2cbuff[2][KSERIAL_RW_BUFFER_SIZE] = {0};

void i2c_delay( serial_t *serial, uint32_t delay )
{
    serial->delay(delay);
}

uint32_t i2c_write( serial_t *serial, const uint8_t address, const uint8_t reg, uint8_t wdata )
{
    uint8_t param[2] = {address << 1, reg};
    uint32_t type = KS_R1;
    uint32_t lens;

    lens = kSerial_Pack(i2cbuff[0], param, type, 1, &wdata);
    serial->send(&serial->cfg, i2cbuff[0], lens);
#if 0
    printf("[W] param = %02X, %02X, type = %d, bytes = %d, data = %02X\n", param[0], param[1], type, lens, wdata);
#endif

    return lens;
}

#define READ_WAIT_TIME 100
uint32_t i2c_read( serial_t *serial, const uint8_t address, const uint8_t reg, uint8_t *rdata, uint8_t lenght, const uint32_t timeout )
{
    uint8_t param[2] = {(address << 1) + 1, reg};
    uint32_t type = KS_R1;
    uint32_t lens;
    uint32_t status;
    uint32_t timecount = 0;

    Serial_flush(&serial->cfg);

    lens = kSerial_Pack(i2cbuff[0], param, type, 1, &lenght);
    serial->send(&serial->cfg, i2cbuff[0], lens);

    lens = 0;
    while (lens == 0)
    {
        i2c_delay(serial, READ_WAIT_TIME);
        lens = serial->recv(&serial->cfg, i2cbuff[0], KSERIAL_RW_BUFFER_SIZE);
        if (timecount > timeout)
        {
            return KS_TIMEOUT;
        }
        timecount += READ_WAIT_TIME;
    }

    status = kSerial_Unpack(i2cbuff[0], param, &type, &lens, i2cbuff[1]);
    if (status == KS_OK)
    {
        for (uint32_t i = 0; i < lens; i++)
        {
            rdata[i] = i2cbuff[1][i];
        }
#if 0
        printf("[R] param = %02X, %02X, type = %d, bytes = %d, data =", param[0], param[1], type, lens + 8);
        for (uint32_t i = 0; i < lens; i++)
        {
            printf(" %02X", i2cbuff[1][i]);
        }
        printf("\n");
#endif
    }

    return status;
}

uint32_t i2c_scandevice( serial_t *serial, uint8_t *address, uint32_t delay )
{
    uint8_t param[2] = {0xAB, 0};
    uint32_t type = KS_R2;
    uint32_t lens;
    uint32_t status;

    Serial_flush(&serial->cfg);

    lens = kSerial_Pack(i2cbuff[0], param, type, 0, NULL);
    serial->send(&serial->cfg, i2cbuff[0], lens);
    i2c_delay(serial, delay);
    lens = serial->recv(&serial->cfg, i2cbuff[0], KSERIAL_RW_BUFFER_SIZE);

    status = kSerial_Unpack(i2cbuff[0], param, &type, &lens, i2cbuff[1]);
    if (status == KS_OK)
    {
        for (uint32_t i = 0; i < lens; i++)
        {
            address[i] = i2cbuff[1][i];
        }
#if 0
        printf("\n");
        printf(" >> i2c device list (found %d device)\n\n", lens);
        printf("    ");
        for (uint32_t i = 0; i < lens; i++)
        {
            printf(" %02X", i2cbuff[1][i]);
        }
        printf("\n\n");
#endif
    }
    else
    {
        lens = 255;
    }

    return lens;
}

uint32_t i2c_scanregister( serial_t *serial, uint8_t address, uint8_t reg[256], uint32_t delay )
{
    uint8_t param[2] = {0xCB, address << 1};
    uint32_t type = KS_R2;
    uint32_t lens;
    uint32_t status;

    Serial_flush(&serial->cfg);

    lens = kSerial_Pack(i2cbuff[0], param, type, 0, NULL);
    serial->send(&serial->cfg, i2cbuff[0], lens);
    i2c_delay(serial, delay);
    lens = serial->recv(&serial->cfg, i2cbuff[0], KSERIAL_RW_BUFFER_SIZE);

    status = kSerial_Unpack(i2cbuff[0], param, &type, &lens, i2cbuff[1]);
    if (status == KS_OK)
    {
        for (uint32_t i = 0; i < 256; i++)
        {
            reg[i] = i2cbuff[1][i];
        }
#if 0
        printf("\n");
        printf(" >> i2c device register (address 0x%02X)\n\n", address);
        printf("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
        for (uint32_t i = 0; i < 256; i += 16)
        {
            printf(" %02X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                i,
                i2cbuff[1][i +  0], i2cbuff[1][i +  1], i2cbuff[1][i +  2], i2cbuff[1][i +  3],
                i2cbuff[1][i +  4], i2cbuff[1][i +  5], i2cbuff[1][i +  6], i2cbuff[1][i +  7],
                i2cbuff[1][i +  8], i2cbuff[1][i +  9], i2cbuff[1][i + 10], i2cbuff[1][i + 11],
                i2cbuff[1][i + 12], i2cbuff[1][i + 13], i2cbuff[1][i + 14], i2cbuff[1][i + 15]
            );
        }
        printf("\n\n");
#endif
    }

    return status;
}

uint32_t readSetting( char *filename, serial_config_t *cfg )
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Open raw data file error!!!\n");
        fp = fopen(filename, "ab");
        fprintf(fp, "2,115200,8,N,1,0");    /// default setting
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

uint32_t writeSetting( char *filename, uint32_t port, uint32_t baudrate )
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

/*
uart2twi            -> scan device address
uart2twi [address]  -> read all register data
*/
uint8_t uart2twibuff[4096] = {0};

int main( int argc, char **argv )
{
    uint32_t status;

#if 0
    printf("argc = %d\n", argc);
    for (uint32_t i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s, %d\n", i, argv[i], strtoul(argv[i], NULL, 0));
    }
#endif

    serial_t s = {.cfg = {2, 115200, {'8', 'N', '1', 0}}};
    serial_config_t loadcfg;

    // serial port setting
    if (argc == 4)
    {
        if ((strcmp("-uart", argv[1]) == 0) || (strcmp("-UART", argv[1]) == 0))
        {
            uint32_t port = strtoul(argv[2], NULL, 0);
            uint32_t baudrate = strtoul(argv[3], NULL, 0);
            printf("COM%d, baudrate %d\n", port, baudrate);
            s.cfg.port = port - 1;
            s.cfg.baudrate = baudrate;
            status = writeSetting(FILENAME, s.cfg.port, s.cfg.baudrate);
            if (status != KS_OK)
            {
                printf("  write setting file error\n");
                return KS_ERROR;
            }
            return KS_OK;
        }
    }

    status = readSetting(FILENAME, &loadcfg);
    if (status == KS_OK)
    {
        // printf("COM%d, baudrate = %d, mode = %c,%c,%c,%d\n",
        //     loadcfg.port + 1, loadcfg.baudrate,
        //     loadcfg.mode[0], loadcfg.mode[1], loadcfg.mode[2], loadcfg.mode[3]);
        memcpy(&s.cfg, &loadcfg, sizeof(serial_config_t));
    }
    else
    {
        return KS_ERROR;
    }

    Serial_Config(&s);

    // printf("COM%d, baudrate = %d\n", s.cfg.port + 1, s.cfg.baudrate);
    if (s.open(&s.cfg))
    {
        // printf(">> open com port ... ok\n\n");
    }
    else
    {
        // printf(">> open com port ... error\n");
        return KS_ERROR;
    }

    uint8_t command = 'N';

    switch (argc)
    {
        case 1:
        {
            // scan device address
            uint32_t devicenum;
            devicenum = i2c_scandevice(&s, uart2twibuff, 100);
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
            break;
        }
        default:
        {
            command = *((uint8_t *)argv[1]);
            break;
        }
    }
    // printf("command = %c\n", command);

    switch (command)
    {
        case 'R':
        case 'r':
        {
            switch (argc)
            {
                case 4:
                {
                    // single read
                    uint8_t slaveAddress = strtoul(argv[2], NULL, 0);
                    uint8_t deviceRegister = strtoul(argv[3], NULL, 0);
                    status = i2c_read(&s, slaveAddress, deviceRegister, uart2twibuff, 1, 50);
                    if (status != KS_OK)
                    {
                        printf("  i2c single read error\n");
                        return KS_ERROR;
                    }
                    else
                    {
                        printf("  %02X\n", uart2twibuff[0]);
                    }
                    break;
                }
                case 5:
                {
                    // multiple read
                    uint8_t slaveAddress = strtoul(argv[2], NULL, 0);
                    uint8_t deviceRegister = strtoul(argv[3], NULL, 0);
                    uint8_t readLength = strtoul(argv[4], NULL, 0);
                    status = i2c_read(&s, slaveAddress, deviceRegister, uart2twibuff, readLength, 50);
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
                    break;
                }
                default:
                {
                    printf("  i2c read input error\n");
                    return KS_ERROR;
                }
            }
            break;
        }
        case 'W':
        case 'w':
        {
            switch (argc)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                {
                    printf("  i2c write input error\n");
                    return KS_ERROR;
                }
                case 5:
                {
                    // single write
                    uint8_t slaveAddress = strtoul(argv[2], NULL, 0);
                    uint8_t deviceRegister = strtoul(argv[3], NULL, 0);
                    uint8_t writeData = strtoul(argv[4], NULL, 0);
                    uint8_t lens;
                    lens = i2c_write(&s, slaveAddress, deviceRegister, writeData) - 8;
                    printf("  %d\n", lens);
                    break;
                }
                default:
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
                    // printf("address = %02X, register = %02X\n", slaveAddress, deviceRegister);
                    break;
                }
            }
            break;
        }
        case 'D':
        case 'd':
        {
            if (argc > 3)
            {
                printf("  i2c delay input error\n");
                return KS_ERROR;
            }
            uint32_t delaytimes = strtoul(argv[2], NULL, 0);
            printf("delay %d ms\n", delaytimes);
            i2c_delay(&s, delaytimes);
            break;
        }
        case '-':
        {
            switch (argc)
            {
                case 2:
                {
                    if ((strcmp("-scan", argv[1]) == 0) || (strcmp("-SCAN", argv[1]) == 0))
                    {
                        // scan device address
                        uint32_t devicenum;
                        devicenum = i2c_scandevice(&s, uart2twibuff, 100);
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
                    }
                    else
                    {
                        printf("  i2c command input error\n");
                        return KS_ERROR;
                    }
                    break;
                }
                case 3:
                {
                    if ((strcmp("-reg", argv[1]) == 0) || (strcmp("-REG", argv[1]) == 0))
                    {
                        uint8_t slaveAddress = strtoul(argv[2], NULL, 0);
                        // scan device register
                        status = i2c_scanregister(&s, slaveAddress, uart2twibuff, 100);
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
                            // printf("\n");
                        }
                    }
                    else
                    {
                        printf("  i2c command input error\n");
                        return KS_ERROR;
                    }
                    break;
                }
                default:
                {
                    printf("  i2c command input error\n");
                    return KS_ERROR;
                }
            }
            break;
        }
        case 'H':
        case 'h':
        {
            printf("uart2twi help\n");
            break;
        }
    }

    // Close COM Port
    s.close(&s.cfg);
    // printf("\n\n>> close com port\n");

    // system("PAUSE");
    return KS_OK;
}
