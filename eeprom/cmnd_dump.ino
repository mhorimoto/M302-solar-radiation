#include <EEPROM.h>

void cmnd_dump(String p, int at) {
  int startptr,addh,addl,ofs;
  String q;
  unsigned char romd[16],buf[4];
  ofs = p.indexOf(" ");
  if (ofs>0) { // 引数がある
    q = p.substring(ofs);
    q.toCharArray(&buf[0],4);
    startptr = (int)strtol(buf,NULL,16);
    startptr *= 0x100;
  } else {
    startptr = 0;
  }
  if (at<0) {
    Serial.print(F("Arduino EEPROM"));
  }
  Serial.println(" EEPROM DUMP");
  Serial.println(F(" ADDR | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF"));
  Serial.println(F("------+-------------------------------------------------+----------------"));
  for(addh=startptr;addh<(startptr+0x100);addh+=0x10) {
    for(addl=0;addl<0x10;addl++) {
      if (at<0) {
	romd[addl]=EEPROM.read(addh+addl);
      }
    }
    sprintf(lbf," %04X | %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X |%1c%1c%1c%1c%1c%1c%1c%1c%1c%1c%1c%1c%1c%1c%1c%1c",
	    addh,romd[0],romd[1],romd[2],romd[3],romd[4],romd[5],romd[6],romd[7],
	    romd[8],romd[9],romd[10],romd[11],romd[12],romd[13],romd[14],romd[15],
	    bytemap(romd[0]),bytemap(romd[1]),bytemap(romd[2]),bytemap(romd[3]),
	    bytemap(romd[4]),bytemap(romd[5]),bytemap(romd[6]),bytemap(romd[7]),
	    bytemap(romd[8]),bytemap(romd[9]),bytemap(romd[10]),bytemap(romd[11]),
	    bytemap(romd[12]),bytemap(romd[13]),bytemap(romd[14]),bytemap(romd[15]));
    Serial.println(lbf);
  }
}
