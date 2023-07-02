// TEST1

#include <Wire.h>

#define i2c_addr      0x48  //  SX8725C typical I2C Address
#define Vref          1.22  //  SX8725C typical reference voltage
#define RegRCen       0x30  //  RC oscillator control, Page 43
#define RegOut        0x40  //  D0,D1 pads data output and direction control, Page 43
#define RegIn         0x41  //  D0,D1 pads input data, Page 43
#define RegTimeout    0x42  //  Enable/Disable I2C timeout, Page 43
#define RegExtAdd     0x43  //  Set address by external pin
#define RegACOutLsb   0x50  //  LSB of ADC result, Page 44
#define RegACOutMsb   0x51  //  MSB of ADC result, Page 44
#define RegACCfg0     0x52  //  ADC conversion control, Page 44
#define RegACCfg1     0x53  //  ADC conversion control, Page 44
#define RegACCfg2     0x54  //  ADC conversion control, Page 45
#define RegACCfg3     0x55  //  ADC conversion control, Page 45
#define RegACCfg4     0x56  //  ADC conversion control, Page 45
#define RegACCfg5     0x57  //  ADC conversion control, Page 45
#define RegMode       0x70  //  Chip operating mode register, Page 46

//  Vout will be 14mV at 2000W/m^2
//  Input voltage limit is 610mV
//  610/14=43.5 =~ 40
//  PGA2 = 11(10times)
//  PGA3 = 101000(48/12=4times)
//  PGA2*PGA3 = 40times

//init

//              Default   RegACCfg0 , 0b00101000
//                     not continuous conversion and 11bits
/* i2c.write_byte_data(addr, RegACCfg0 , 0b00101000) */

//              Default   RegACCfg1 , 0b11110000
//                     PGA3,2,1 are enable and ADC enable
/* i2c.write_byte_data(addr, RegACCfg1 , 0b11111101) */

//              Default   RegACCfg2 , 0b00000000
//                 Over-Sampling Frequency(FS),Page 29
//                     SetFs 0b00:  62.5kHz
//                     PGA2Gain 0b11: 10, Page 23
//                     PGA2Offset 0b0000: 0
/* i2c.write_byte_data(addr, RegACCfg2 , 0b00110000) */

//              Default   RegACCfg3 , 0b00001100
//                 PGA1Gain 0b0: 1, Page 22
//                 PGA3Gain 0b0001100: 12/12=1, Page 23
/* i2c.write_byte_data(addr, RegACCfg3 , 0b00101000) */

//              Default   RegACCfg5 , 0b00000000
// AC2(-) and AC3(+) differential inputs,
//  using internal reference voltage source (1.22V);
//  Measurement voltage range ±0.61V
/* i2c.write_byte_data(addr, RegACCfg5 , 0b00000011) */

//              Default   RegMode , 0b10000100
/* i2c.write_byte_data(addr, RegMode , 0b10000100)  */

uint8_t  regs[14];

void setup(void) {
  uint8_t data;
  int i;
  Wire.begin();
  Serial.begin(115200);
  for (i=0;i<14;i++) {
    regs[i]=0xaa;
  }
  sx8725_read_allreg();
  //  sx8725_setReg(RegACCfg5 , 0b00000011);
  //  sx8725_read_allreg();
}

void loop(void) {
}


/* RegACOutLsb   0x50  //  LSB of ADC result, Page 44
   RegACOutMsb   0x51  //  MSB of ADC result, Page 44
*/

int sx8725_read_ACOut(void) {
}

/*
  Read all registers 14 regs into uint8_t
*/
void sx8725_read_allreg(void) {
  int i;
  char *regname[14];
  regname[0] = "RegRCen";
  regname[1] = "RegOut";
  regname[2] = "RegIn";
  regname[3] = "RegTimeout";
  regname[4] = "RegExtAdd";
  regname[5] = "RegACOutLsb";
  regname[6] = "RegACOutMsb";
  regname[7] = "RegACCfg0";
  regname[8] = "RegACCfg1";
  regname[9] = "RegACCfg2";
  regname[10] = "RegACCfg3";
  regname[11] = "RegACCfg4";
  regname[12] = "RegACCfg5";
  regname[13] = "RegMode";

  i=0;
  Wire.beginTransmission(i2c_addr);
  Wire.write(RegRCen);
  Wire.endTransmission();
  Wire.requestFrom(i2c_addr, 1);
  while(Wire.available()){
    regs[i] = Wire.read();
    i++;
    if (i>13) return;
  }
  Wire.beginTransmission(i2c_addr);
  Wire.write(RegOut);
  Wire.endTransmission();
  Wire.requestFrom(i2c_addr, 4);
  while(Wire.available()){
    regs[i] = Wire.read();
    i++;
    if (i>13) return;
  }
  Wire.beginTransmission(i2c_addr);
  Wire.write(RegACOutLsb);
  Wire.endTransmission();
  Wire.requestFrom(i2c_addr, 8);
  while(Wire.available()){
    regs[i] = Wire.read();
    i++;
    if (i>13) return;
  }
  Wire.beginTransmission(i2c_addr);
  Wire.write(RegMode);
  Wire.endTransmission();
  Wire.requestFrom(i2c_addr, 1);
  while(Wire.available()){
    regs[i] = Wire.read();
  }
  for (i=0;i<14;i++) {
    Serial.print(regname[i]);
    Serial.print("=");
    Serial.println(regs[i],BIN);
  }
}

void sx8725_setReg(uint8_t reg,uint8_t va) {
  Wire.beginTransmission(i2c_addr);
  Wire.write(reg);
  Wire.write(va);
  Wire.endTransmission();
}
