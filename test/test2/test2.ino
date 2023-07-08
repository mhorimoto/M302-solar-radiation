#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h> // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define UECS_PORT     16520
#define sx8725_addr   0x48  //  SX8725C typical I2C Address
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

const char VERSION[16] PROGMEM = "M302 Solar V0.02";
char uecsid[6], uecstext[180],strIP[16],linebuf[80];

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
char lcdtext[6][17];
uint8_t  regs[14];
int      outval;
bool      ready,busy;
byte macaddr[6];
IPAddress localIP,broadcastIP,subnetmaskIP,remoteIP;
EthernetUDP Udp16520; //,Udp16529;
volatile int period1sec = 0;
volatile int period10sec = 0;
volatile int period60sec = 0;


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
  
  // EEPROM.get(pUECSID,uecsid);
  //EEPROM.get(pMACADDR,macaddr);
  for(i=0;i<16;i++) {
    lcdtext[0][i] = pgm_read_byte(&(VERSION[i]));
  }
  lcdtext[0][i] = 0;

  Wire.begin();
  Serial.begin(115200);
  for (i=0;i<14;i++) {
    regs[i]=0xaa;
  }
  sx8725_read_allreg();

  sx8725_setReg(RegACCfg0 , 0b00101000);
  sx8725_setReg(RegACCfg1 , 0b11111101); // PGA enable
  //sx8725_setReg(RegACCfg1 , 0b11110001); // PGA stopped
  sx8725_setReg(RegACCfg2 , 0b00110000); // PGA2 = 10times
  sx8725_setReg(RegACCfg3 , 0b01100000); // PGA3 = 8times
  sx8725_setReg(RegACCfg5 , 0b00100100); // AC2 Vinp, AC3
  sx8725_read_allreg();
  ready = false;
  busy = false;
  Ethernet.init(10);
  //  if (Ethernet.begin(macaddr)==0) {
  //    sprintf(lcdtext[1],"NFL");
  //  } else {
  //    localIP = Ethernet.localIP();
  //    subnetmaskIP = Ethernet.subnetMask();
  //    for(i=0;i<4;i++) {
  //      broadcastIP[i] = ~subnetmaskIP[i]|localIP[i];
  //    }
  //    sprintf(lcdtext[2],ids,"HW",
  //            macaddr[0],macaddr[1],macaddr[2],macaddr[3],macaddr[4],macaddr[5]);
  //    sprintf(strIP,"%d.%d.%d.%d",localIP[0],localIP[1],localIP[2],localIP[3]);
  //    sprintf(lcdtext[3],"%s",strIP);
  //    lcdout(2,3,1);
  //    Udp16520.begin(16520);
  //  }

  TCCR1A  = 0;
  TCCR1B  = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);  //CTCmode //prescaler to 1024
  OCR1A   = 15625-1;
  TIMSK1 |= (1 << OCIE1A);
}


void loop(void) {
  int r ;
  extern int period1sec;
  if (period1sec==1) {
    UES();
    period1sec = 0;
  }
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

void UES(void) {
  static bool aaa;
  if (aaa) {
    lcd.setCursor(10,1);
    aaa=false;
    lcd.print("X");
  } else {
    lcd.setCursor(10,1);
    aaa=true;
    lcd.print("O");
  }
}

void UserEverySecond(void) {
}

void UserEveryMinute(void) {
}

ISR(TIMER1_COMPA_vect) {
  static byte cnt10,cnt60;
  cnt10++;
  cnt60++;
  period1sec = 1;
  UserEverySecond();
  if (cnt10 >= 10) {
    cnt10 = 0;
    period10sec = 1;
  }
  if (cnt60 >= 60) {
    cnt60 = 0;
    period60sec = 1;
    UserEveryMinute();
  }
}

void lcdout(int l1,int l2,int cl) {
  if (cl!=0) {
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print(lcdtext[l1]);
  lcd.setCursor(0,1);
  lcd.print(lcdtext[l2]);
}

