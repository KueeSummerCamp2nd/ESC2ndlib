#include "ESC2nd.h"

Motor::Motor(char direction)
{ 
  if (direction == 'r'){
      this->motorP1 = 4;
      this->motorP2 = 7;
      this->PWM_motP = 6;
      pinMode(4,OUTPUT);
      pinMode(7,OUTPUT);
  }
  else if(direction == 'l'){
      this->motorP1 = 3;
      this->motorP2 = 2;
      this->PWM_motP = 5;
      pinMode(3,OUTPUT);
      pinMode(2,OUTPUT);
  }
}

//モーター駆動
void Motor::drive(int pwm){
  int pwm_out;
  if(pwm > PWM_MAX){
      pwm_out=PWM_MAX;
    }
  else if(pwm < -PWM_MAX){
      pwm_out=-PWM_MAX;
  }
  else{
    pwm_out=pwm;
  }
  if(pwm==0){
    digitalWrite(this->motorP1,HIGH);
    digitalWrite(this->motorP2,HIGH);
    analogWrite(this->PWM_motP,pwm_out);    
  }else if(pwm>0){
    digitalWrite(this->motorP1,HIGH);
    digitalWrite(this->motorP2,LOW);
    analogWrite(this->PWM_motP,pwm_out);
  }
  else{
    digitalWrite(this->motorP1,LOW);
    digitalWrite(this->motorP2,HIGH);
    analogWrite(this->PWM_motP,-pwm_out);
  }
}
Sensor::Sensor(byte deviceadr, const byte (&ch_order_)[8]){
  this->deviceadr = deviceadr;

  for(byte i = 0; i < 8; i++){
    this->ch_order[i] = ch_order_[i];
  }

  // this->spi_setting
  pinMode(deviceadr, OUTPUT);
  digitalWrite(deviceadr, HIGH);
  SPI.begin();//SPI開始
}

Sensor::Sensor(byte deviceadr){
  this->deviceadr = deviceadr;
  
  const byte *ch_order_;
  switch (deviceadr)
  {
  case DEVICE_ADDR1:
    ch_order_ = default_ch_order_addr1;
    break;
  case DEVICE_ADDR2:
    ch_order_ = default_ch_order_addr2;
    break;
  default:
    ch_order_ = default_ch_order;
    break;
  }

  for(byte i = 0; i < 8; i++){
    this->ch_order[i] = ch_order_[i];
  }

  // this->spi_setting
  pinMode(deviceadr, OUTPUT);
  digitalWrite(deviceadr, HIGH);
  SPI.begin();//SPI開始
}

void Sensor::read(uint16_t value[8]) {   //AD変換 各センサー読み取り
  
  // 取得した値を格納するバッファ
  byte adc_buff[2];

  // CHの配列
  // byte ADC_CHS[8] = {ADC_CH0, ADC_CH3, ADC_CH2, ADC_CH1, ADC_CH6, ADC_CH5, ADC_CH4, ADC_CH7};
  
  for(int i=0;i<8;i++){
    value[i] = readSPI(ch_order[i]);
  }
}

//AD変換用 I2Cへの読み込み
void Sensor::readI2c(byte register_addr, int num, byte buffer[]) {
  Wire.beginTransmission(this->deviceadr); 
  Wire.write(register_addr);           
  Wire.endTransmission();         

  Wire.beginTransmission(this->deviceadr); 
  Wire.requestFrom((int)this->deviceadr, num);

  int i = 0;
  while(Wire.available())        
  { 
    buffer[i] = Wire.read();   
    i++;
  }
  Wire.endTransmission();         
}


// void Sensor::writeSPI(byte register_addr, byte value);
int16_t Sensor::readSPI(byte ch) {
  if((ch >=8) || (ch < 0)){
    return -1;
  }

  pinMode(deviceadr, OUTPUT);
  digitalWrite(deviceadr, HIGH);
  byte ch_h = (ch & 0b100) >> 2;
  byte ch_l = (ch & 0b011);
  SPI.beginTransaction(spi_setting);
    digitalWrite(deviceadr, LOW);
    byte v1 = SPI.transfer((0b11 << 1) | ch_h);
    byte v2 = SPI.transfer((ch_l) << 6);
    byte v3 = SPI.transfer(0);
    digitalWrite(deviceadr, HIGH);
  SPI.beginTransaction(spi_setting);
  uint16_t value = ((v2 &0b00001111) << 8) | (v3& 0xff);
  return value;
}
