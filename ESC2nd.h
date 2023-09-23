#ifndef ESC2nd_h
#define ESC2nd_h
#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>

/**************************AD変換**********************************/
// #define DEVICE_ADDR1 (0x48) // AC1(ADS7828)のスレーブデバイスのアドレス
// #define DEVICE_ADDR2 (0x4B) // AC2(ADS7828)のスレーブデバイスのアドレス
#define DEVICE_ADDR1 (9) // AC1(ADS7828)のスレーブデバイスのアドレス
#define DEVICE_ADDR2 (10) // AC2(ADS7828)のスレーブデバイスのアドレス

#define ADC_CH0 (0x8C) // CH1
#define ADC_CH1 (0xCC) // CH2
#define ADC_CH2 (0x9C) // CH3
#define ADC_CH3 (0xDC) // CH4
#define ADC_CH4 (0xAC) // CH5
#define ADC_CH5 (0xEC) // CH6
#define ADC_CH6 (0xBC) // CH7
#define ADC_CH7 (0xFC) // CH8

const byte default_ch_order_addr1[8] = {0,3,2,1,6,5,4,7};
const byte default_ch_order_addr2[8] = {0,1,2,3,4,5,6,7};
const byte default_ch_order[8] = {0,1,2,3,4,5,6,7};



#define PWM_MAX 255 //デューティの最大値

class Motor
{
  public:
    Motor(char direction);
    void drive(int pwm);
    
  private:
    int motorP1;
    int motorP2;
    int PWM_motP;
};

class Sensor
{
  public:
    Sensor(byte deviceadr);
    Sensor(byte deviceadr, const byte (&ch_order_)[8]);
    void read(uint16_t value[8]);
    
  private:
    byte ch_order[8];
    byte deviceadr;
    SPISettings spi_setting = SPISettings(1000000,MSBFIRST,SPI_MODE0);
    void writeI2c(byte register_addr, byte value);
    void readI2c(byte register_addr, int num, byte buffer[]);
    // void writeSPI(byte register_addr, byte value);
    int16_t readSPI(byte ch);
};

#endif

  
