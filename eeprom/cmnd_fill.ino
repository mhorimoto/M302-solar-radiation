#include <EEPROM.h>

void cmnd_fill(String p,int at) {
  unsigned int saddr,eaddr,addr,ofs1,ofs2,ofs3;
  String q;
  byte d,v,buf[10];
  ofs1 = p.indexOf(" ");
  ofs2 = p.indexOf(" ",ofs1+1);
  ofs3 = p.indexOf(" ",ofs2+1);
  if (ofs1>0) {  // 始点アドレス引数がある
    q = p.substring(ofs1,ofs2);
    q.toCharArray(&buf[0],7);
    saddr = (unsigned int)strtol(buf,NULL,16);
    if (ofs2>0) { // 終点アドレス引数がある
      q = p.substring(ofs2,ofs3);
      q.toCharArray(&buf[0],7);
      eaddr = (unsigned int)strtol(buf,NULL,16);
      if (at==-1) { // EEPROM with CPU
        if (eaddr >= EEPROM.length()) {
          sprintf(lbf,"EEPROM Address %04XH is Overflow.\n",eaddr);
          Serial.print(lbf);
          return;
        }
      }
      if (ofs3>0) { // fillするデータがある
	q = p.substring(ofs3);
	q.toCharArray(&buf[0],4);
	d = (unsigned int)strtol(buf,NULL,16);
	sprintf(lbf,"[%04XH,%04XH]<=%02XH",saddr,eaddr,d);
	Serial.println(lbf);
	for(addr=saddr;addr<=eaddr;addr++) {
          if (at==-1) {
            EEPROM.update(addr,d);
          }
	}
	Serial.println(F("DONE"));
      } else {
	Serial.println(F("NO WRITE,NO FILL DATA"));
      }
    } else {
      Serial.println(F("ERROR NO END ADDRESS"));
    }
  } else {
    Serial.println(F("ERROR NO START ADDR"));
  }
}
