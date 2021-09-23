#ifndef ESC2nd_h
#define ESC2nd_h
#include <Wire.h>
#include <Arduino.h>

/**************************AD変換**********************************/
#define DEVICE_ADDR1 (0x48) // AC1(ADS7828)のスレーブデバイスのアドレス
#define DEVICE_ADDR2 (0x4B) // AC2(ADS7828)のスレーブデバイスのアドレス

#define ADC_CH0 (0x8C) // CH1
#define ADC_CH1 (0xCC) // CH2
#define ADC_CH2 (0x9C) // CH3
#define ADC_CH3 (0xDC) // CH4
#define ADC_CH4 (0xAC) // CH5
#define ADC_CH5 (0xEC) // CH6
#define ADC_CH6 (0xBC) // CH7
#define ADC_CH7 (0xFC) // CH8





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
    void read(int value[8]);
    
  private:
    byte deviceadr;
    void writeI2c(byte register_addr, byte value);
    void readI2c(byte register_addr, int num, byte buffer[]);
};

#endif

  
