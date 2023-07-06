#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define sx8725_addr      0x48  //  SX8725C typical I2C Address
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

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
char lcdtext[6][17];
uint8_t  regs[14];
int      outval;
bool      ready,busy;

void setup(void) {
  uint8_t data;
  int i;
  lcd.init();
  lcd.backlight();
  pinMode(A0,OUTPUT);
  digitalWrite(A0,LOW);
  pinMode(A1,OUTPUT);
  digitalWrite(A1,LOW);
  pinMode(A2,INPUT_PULLUP);
  
  Wire.begin();
  Serial.begin(115200);
  for (i=0;i<14;i++) {
    regs[i]=0xaa;
  }
  sx8725_read_allreg();

  sx8725_setReg(RegACCfg0 , 0b00101000);
  sx8725_setReg(RegACCfg1 , 0b11111101);
  sx8725_setReg(RegACCfg2 , 0b00110000); // PGA2 = 10times
  sx8725_setReg(RegACCfg3 , 0b00110000); // PGA3 = 4times
  sx8725_setReg(RegACCfg5 , 0b00100100); // AC2 Vinp, AC3 
  sx8725_read_allreg();
  ready = false;
  busy = false;
}


void loop(void) {
  int r ;
  if (busy==false) {
    sx8725_setReg(RegACCfg0 , 0b10101000); // Start
    busy = true;
    ready= false;
  }
  while(true) {
    if (digitalRead(A2)) {
      ready = true;
    } else {
      ready = false;
    }
    if (ready) {
      r = sx8725_read_ACOut();
      Serial.print("  R=");
      Serial.println(r);
      ready = false;
      busy = false;
      return;
    }
  }
}

/* RegACOutLsb   0x50  //  LSB of ADC result, Page 44
   RegACOutMsb   0x51  //  MSB of ADC result, Page 44
*/

int sx8725_read_ACOut(void) {
  uint8_t msb,lsb;
  int     rv;
  Wire.beginTransmission(sx8725_addr);
  Wire.write(RegACOutLsb);
  Wire.endTransmission();
  Wire.requestFrom(sx8725_addr, 2);
  while(Wire.available()){
    lsb = Wire.read();
    msb = Wire.read();
  }
  rv = (msb<<8)|lsb;
  return(rv);
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
  Wire.beginTransmission(sx8725_addr);
  Wire.write(RegRCen);
  Wire.endTransmission();
  Wire.requestFrom(sx8725_addr, 1);
  while(Wire.available()){
    regs[i] = Wire.read();
    i++;
    if (i>13) return;
  }
  Wire.beginTransmission(sx8725_addr);
  Wire.write(RegOut);
  Wire.endTransmission();
  Wire.requestFrom(sx8725_addr, 4);
  while(Wire.available()){
    regs[i] = Wire.read();
    i++;
    if (i>13) return;
  }
  Wire.beginTransmission(sx8725_addr);
  Wire.write(RegACOutLsb);
  Wire.endTransmission();
  Wire.requestFrom(sx8725_addr, 8);
  while(Wire.available()){
    regs[i] = Wire.read();
    i++;
    if (i>13) return;
  }
  Wire.beginTransmission(sx8725_addr);
  Wire.write(RegMode);
  Wire.endTransmission();
  Wire.requestFrom(sx8725_addr, 1);
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
  Wire.beginTransmission(sx8725_addr);
  Wire.write(reg);
  Wire.write(va);
  Wire.endTransmission();
}
