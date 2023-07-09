void lcdout(int l1,int l2,int cl) {
  if (cl!=0) {
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print(lcdtext[l1]);
  lcd.setCursor(0,1);
  lcd.print(lcdtext[l2]);
}

