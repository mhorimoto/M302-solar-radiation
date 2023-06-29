///////////////////////////////////////////////////////////////////
// M302-solar-radiation
//  MIT License
//  Copyright (c) 2023 Masafumi Horimoto
//  Release on 
//  
///////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h> // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include "LiquidCrystal_I2C.h"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
void get_mcusr(void)     \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
void get_mcusr(void) {
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}

#define  UECS_PORT  16520
#define  pUECSID      0
#define  pMACADDR     6
#define  pINAIRTEMP  16
#define  pINAIRHUMID 33
#define  pINILLUMI   51
#define  pCND        0x43
#define  delayMillis 5000UL // 5sec
#define  SXD0        A0
#define  SXD1        A1
#define  SXREADY     A2

const char VERSION[16] PROGMEM = "M302 Solar Radi ";

char uecsid[6], uecstext[180],strIP[16],linebuf[80];
byte lineptr = 0;
int  sx8725c   = 0x48;  // Default SX8725C I2C Address
unsigned long cndVal;   // CCM cnd Value
char api[] = "api.smart-agri.jp";

/////////////////////////////////////
// Hardware Define
/////////////////////////////////////

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
char lcdtext[6][17];

byte macaddr[6];
IPAddress localIP,broadcastIP,subnetmaskIP,remoteIP;
EthernetUDP Udp16520; //,Udp16529;
EthernetClient EthClient; // GIS Client

volatile int period1sec = 0;
volatile int period10sec = 0;
volatile int period60sec = 0;

void setup(void) {
  int i;
  const char *ids PROGMEM = "%s:%02X%02X%02X%02X%02X%02X";
  extern void lcdout(int,int,int);

  cndVal = 0L;    // Reset cnd value
  lcd.init();
  lcd.backlight();
  configure_wdt();
  pinMode(SXD0,OUTPUT);
  pinMode(SXD1,OUTPUT);
  digitalWrite(SXD0,LOW);
  digitalWrite(SXD1,LOW);
  pinMode(,INPUT_PULLUP);
  EEPROM.get(pUECSID,uecsid);
  EEPROM.get(pMACADDR,macaddr);
  for(i=0;i<16;i++) {
    lcdtext[0][i] = pgm_read_byte(&(VERSION[i]));
  }
  lcdtext[0][i] = 0;
  sprintf(lcdtext[1],ids,"ID",
          uecsid[0],uecsid[1],uecsid[2],uecsid[3],uecsid[4],uecsid[5]);
  lcdout(0,1,1);
  Serial.begin(115200);
  Serial.println(lcdtext[0]);
  delay(500);
  Ethernet.init(10);
  if (Ethernet.begin(macaddr)==0) {
    sprintf(lcdtext[1],"NFL");
  } else {
    localIP = Ethernet.localIP();
    subnetmaskIP = Ethernet.subnetMask();
    for(i=0;i<4;i++) {
      broadcastIP[i] = ~subnetmaskIP[i]|localIP[i];
    }
    sprintf(lcdtext[2],ids,"HW",
            macaddr[0],macaddr[1],macaddr[2],macaddr[3],macaddr[4],macaddr[5]);
    sprintf(strIP,"%d.%d.%d.%d",localIP[0],localIP[1],localIP[2],localIP[3]);
    sprintf(lcdtext[3],"%s",strIP);
    lcdout(2,3,1);
    Udp16520.begin(16520);
  }
  wdt_reset();
  cndVal |= 0x00000001;  // Setup completed
  delay(1000);
  //
  // Setup Timer1 Interrupt
  //
  TCCR1A  = 0;
  TCCR1B  = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);  //CTCmode //prescaler to 1024
  OCR1A   = 15625-1;
  TIMSK1 |= (1 << OCIE1A);
}

/////////////////////////////////
// Reset Function goto Address 0
/////////////////////////////////
void(*resetFunc)(void) = 0;

extern void lcdout(int,int,int);
extern int setParam(char *);
extern void dumpLowCore(void);
  
#define CCMFMT "<?xml version=\"1.0\"?><UECS ver=\"1.00-E10\"><DATA type=\"%s.mIC\" room=\"%d\" region=\"%d\" order=\"%d\" priority=\"%d\">%s</DATA><IP>%s</IP></UECS>";
//char *ids = "%s:%02X%02X%02X%02X%02X%02X";

int dk=0;
void UserEvery10Seconds(void) {
  extern void lcdout(int,int,int);
  char *xmlDT PROGMEM = CCMFMT;
  char name[10],dname[11],val[6];
  int ia,cdsv;
  wdt_reset();
  if (sht31addr>0) {
    //      ther = sht31.readTemperature();
    ia = pINAIRTEMP;
    getSHTdata(&val[0],pINAIRTEMP,0);  // 整数型にしない
    //     Serial.println(val);
    sprintf(linebuf,"T=%sC",val);
    uecsSendData(pINAIRTEMP,xmlDT,val,10);
    getSHTdata(&val[0],pINAIRHUMID,0);  // 整数型
    uecsSendData(pINAIRHUMID,xmlDT,val,10);
    sprintf(lcdtext[4],"%s H=%s%%",linebuf,val);
  } else {
    strcpy(lcdtext[4],("NO SHT SENSOR"));
  }
  if (analogRead(CDS0SW)<100) {
    cdsv = 1023 - analogRead(CDS0);
    sprintf(&val[0],"%d",cdsv);
    sprintf(lcdtext[5],"ILL=%d",cdsv);
    uecsSendData(pINILLUMI,xmlDT,val,10);
  }
  Serial.print("@");
}

void lcd_display_loop(void) {
  dk++;
  Serial.print("=");
  Serial.print(dk);
  Serial.print("=");
  switch(dk) {
  case 3:
    if (analogRead(CDS0SW)>=100) {
      lcdout(3,4,1);
      dk = -1;
    }
    break;
  case 4:
    if (analogRead(CDS0SW)<100) {
      lcdout(4,5,1);
    }
    break;
  case 5:
    dk = 0;
    lcdout(0,4,1);
    break;
  default:
    lcdout(dk,4,1);
  }
}


/////////////////////////////////
void loop() {
  static int k=0;
  int i,ia,ta,tb,cdsv;
  byte room,region,priority,interval;
  int  order;
  int  inchar ;
  float ther,humi;
  char name[10],dname[11],val[6];

  //  extern void lcdout(int,int,int);
  //  extern int setParam(char *);
  //  extern void dumpLowCore(void);

  char *xmlDT PROGMEM = CCMFMT;
  const char *ids PROGMEM = "%s:%02X%02X%02X%02X%02X%02X";
  
   wdt_reset();
   // 10 sec interval
   if (period10sec==1) {
     UserEvery10Seconds();
     lcd_display_loop();
     period10sec=0;
   }
   // 1 min interval
   if (period60sec==1) {
     period60sec = 0;
     wdt_reset();
     if (sht31addr>0) {
       getSHTdata(&val[0],pINAIRTEMP,1); // 小数点下1桁の整数型
       ia = gisSendData(pINAIRTEMP,16,val);
       switch(ia) {
       case 0:
         cndVal &= 0xffcffeff; // 
         break;
       case 1:
         cndVal |= 0x00100100; // Not connect to Server
         break;
       case 2:
         cndVal |= 0x00200100; // Server response timeout
         break;
       }
       wdt_reset();
       getSHTdata(&val[0],pINAIRHUMID,0); // 整数型
       ia = gisSendData(pINAIRHUMID,17,val);
       switch(ia) {
       case 0:
         cndVal &= 0xffcffdff; // 
         break;
       case 1:
         cndVal |= 0x00100200; // Not connect to Server
         break;
       case 2:
         cndVal |= 0x00200200; // Server response timeout
         break;
       }
       cndVal &= 0xffffff0f;  // Reset maintain code
       ia = Ethernet.maintain();
       if (ia!=0) {
         cndVal |= ((ia << 4) & 0xf0);
       }
     }
     if (analogRead(CDS0SW)<100) {
       cdsv = 1023 - analogRead(CDS0);
       sprintf(&val[0],"%d",cdsv);
       ia = gisSendData(pINILLUMI,261,val);
       wdt_reset();
     }
   }
   // 1 sec interval
   //if (period1sec==1) {
     // period1sec = 0;
     // ia = pCND;
     // sprintf(val,"%u",cndVal);
     // uecsSendData(ia,xmlDT,val);
     // cndVal &= 0xfffffffe;            // Clear setup completed flag
   //}
   wdt_reset();
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


void configure_wdt(void) {
  cli();                           // disable interrupts for changing the registers
  MCUSR = 0;                       // reset status register flags
                                   // Put timer in interrupt-only mode:
  WDTCSR |= 0b00011000;            // Set WDCE (5th from left) and WDE (4th from left) to enter config mode,
                                   // using bitwise OR assignment (leaves other bits unchanged).
  WDTCSR =  0b00001000 | 0b100001; // clr WDIE: interrupt enabled
                                   // set WDE: reset disabled
                                   // and set delay interval (right side of bar) to 8 seconds
  sei();                           // re-enable interrupts
                                   // reminder of the definitions for the time before firing
                                   // delay interval patterns:
                                   //  16 ms:     0b000000
                                   //  500 ms:    0b000101
                                   //  1 second:  0b000110
                                   //  2 seconds: 0b000111
                                   //  4 seconds: 0b100000
                                   //  8 seconds: 0b100001
}

void uecsSendData(int a,char *xmlDT,char *val,int z) {
  byte room,region,priority,interval;
  int  order,i;
  char name[10],dname[11]; // ,val[6];
  EEPROM.get(a+0x01,room);
  EEPROM.get(a+0x02,region);
  EEPROM.get(a+0x03,order);
  EEPROM.get(a+0x05,priority);
  EEPROM.get(a+0x06,interval);
  EEPROM.get(a+0x07,name);
  for(i=0;i<10;i++) {
    dname[i] = name[i];
    if (name[i]==NULL) break;
  }
  dname[i] = NULL;
  sprintf(uecstext,xmlDT,dname,room,region,order,priority+z,val,strIP);
  Udp16520.beginPacket(broadcastIP,16520);
  Udp16520.write(uecstext);
  Udp16520.endPacket();
}

byte gisSendData(int a,int sk,char *val) {
  int connectLoop = 0;
  int inChar;
  char headline[20];
  const char *gisDT PROGMEM = "M=%02X%02X.%02X%02X.%02X%02X&I=%d&P=%d&V=%s&RM=%d&RE=%d&OD=%d&PR=%d&TP=%s.mIC";
  byte room,region,priority,interval;
  int  order,i;
  char name[10],dname[11];
  return 0;
  lcd.setCursor(15,1);
  lcd.print("W");
  EEPROM.get(a+0x01,room);
  EEPROM.get(a+0x02,region);
  EEPROM.get(a+0x03,order);
  EEPROM.get(a+0x05,priority);
  EEPROM.get(a+0x06,interval);
  EEPROM.get(a+0x07,name);
  for(i=0;i<10;i++) {
    dname[i] = name[i];
    if (name[i]==NULL) break;
  }
  dname[i] = NULL;
  
  wdt_reset();
  sprintf(linebuf,gisDT,macaddr[0],macaddr[1],macaddr[2],macaddr[3],macaddr[4],macaddr[5],
          order,sk,val,room,region,order,priority,dname);
  EthClient.stop();
  if (EthClient.connect(api,80)) {
    EthClient.println(F("POST /farmem/rx/rxdata.php HTTP/1.1"));
    EthClient.println(F("Host: api.smart-agri.jp"));
    EthClient.println(F("Connection: close\r\nContent-Type: application/x-www-form-urlencoded"));
    sprintf(headline,"Content-Length: %u\r\n",strlen(linebuf));
    EthClient.println(headline);
    EthClient.print(linebuf);
  } else {
    Serial.println(F("SISE"));
    lcd.setCursor(15,1);
    lcd.print("E");
    return 1;
  }
  while(EthClient.connected()) {
    wdt_reset();
    while(EthClient.available()) {
      inChar = EthClient.read();
      connectLoop = 0;
    }
    delay(1);
    connectLoop++;
    wdt_reset();
    if(connectLoop > delayMillis) {
      lcd.setCursor(15,1);
      lcd.print("T");
      EthClient.stop();
      return 2;
    }
  }
  EthClient.stop();
  lcd.setCursor(15,1);
  lcd.print(" ");
  return 0;
}

void getSHTdata(char *v,int a,int f) {
  float fv;
  int ta,tb;
  switch(a) {
  case pINAIRTEMP:
    fv = sht31.readTemperature();
    if (f==0) {
      ta = (int)fv;
      tb = round((double)((fv-ta)*10));
      if (tb>=10.0) {
        ta++;
        tb-=10;
      }
      sprintf(v,"%d.%01d",ta,tb);
    } else {
      ta = round(fv*pow(10.0,f)) ; // f=2ならば、100倍する
      sprintf(v,"%d",ta);
    }
    break;
  case pINAIRHUMID:
    fv = sht31.readHumidity();
    sprintf(v,"%d",(int)fv);
    break;
  }
}

void UserEverySecond(void) {
  static byte a=0 ;
  char val[6];
  int ia;
  char *xmlDT PROGMEM = CCMFMT;
  if (a>9) {
    a=0;
  }
  Serial.print(a);
  a++;
  period1sec = 0;
  ia = pCND;
  sprintf(val,"%u",cndVal);
  uecsSendData(ia,xmlDT,val,0);
  cndVal &= 0xfffffffe;            // Clear setup completed flag
}

void UserEveryMinute(void) {
  static byte a=0 ;
  char *xmlDT PROGMEM = CCMFMT;
  if (a>9) {
    a=0;
  }
  Serial.print("*");
  Serial.println(a);
  a++;
}

