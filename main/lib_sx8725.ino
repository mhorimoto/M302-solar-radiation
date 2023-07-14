
void setup_sx8725(void) {
  extern bool      ready,busy;
  Wire.begin();
  sx8725_setReg(RegACCfg0 , 0b00101000);
  sx8725_setReg(RegACCfg1 , 0b11111101); // PGA enable
  //sx8725_setReg(RegACCfg1 , 0b11110001); // PGA stopped
  sx8725_setReg(RegACCfg2 , 0b00110000); // PGA2 = 10times
  sx8725_setReg(RegACCfg3 , 0b01100000); // PGA3 = 8times
  sx8725_setReg(RegACCfg5 , 0b00100100); // AC2 Vinp, AC3
  sx8725_read_allreg();
  ready = false;
  busy = false;
}


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
  //  for (i=0;i<14;i++) {
  //    Serial.print(regname[i]);
  //    Serial.print("=");
  //    Serial.println(regs[i],BIN);
  //  }
}

void sx8725_setReg(uint8_t reg,uint8_t va) {
  Wire.beginTransmission(sx8725_addr);
  Wire.write(reg);
  Wire.write(va);
  Wire.endTransmission();
}

