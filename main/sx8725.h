#define UECS_PORT     16520
#define pUECSID       0
#define pMACADDR      6

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

