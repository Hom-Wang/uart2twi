# uart2twi
uart2twi 是一個實現於 mcu 上的 uart 轉 i2c 專案，mcu 為被動裝置，需要透過 kSerial 協定接收指令與發出，詳細封包格式可以參考 [這裡](https://github.com/KitSprout/kSerial)，目前 MCU 專案在 mcu_kserial 目錄下，實現於 NRF52832 上，並透過 Keil 編譯與燒錄。  

控制端目前實現兩種方  
1. MATLAB (直接加載使用)  
2. Windows cmd (透過 gcc 編譯 makefile，並呼叫 [.\out\i2c.exe](https://github.com/Hom-Wang/uart2twi/tree/master/windows/out/) 運行，已實現指令如下)   
- i2c -HELP                            # 指令說明  
- i2c -SCAN                            # 掃描 I2C 裝置  
- i2c -REG [ADDRESS]                   # 掃描 I2C ADDRESS 下的全部暫存器數值  
- i2c -UART [PORT] [BAUDRATE]           # 設定 UART COM PORT 和 BAUD RATE  
- i2c -R [ADDRESS] [REGISTER]          # 讀取 I2C ADDRESS 下的 REGISTER 地址的暫存器數數值  
- i2c -R [ADDRESS] [REGISTER] [LENS]   # 讀取 I2C ADDRESS 下的 REGISTER 到 REGISTER + LENS 的暫存器數數值  
- i2c -W [ADDRESS] [REGISTER] [DATA]   # 對 I2C ADDRESS 下的 REGISTER 地址寫入 DATA 數值  
