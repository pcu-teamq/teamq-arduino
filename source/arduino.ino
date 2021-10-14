#include <virtuabotixRTC.h>
#include <Wire.h>
#include <LiquidCrystal.h>

virtuabotixRTC myRTC(6, 7, 8);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int lcd_key     = 0;
int adc_key_in  = 0;

void setup()  {
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  myRTC.setDS1302Time(00, 20, 42, 1, 13, 10, 2021);
}

void loop()  {
  myRTC.updateTime();
  lcd.setCursor(1,0);
  switch (myRTC.dayofweek) {
    case 1: 
      lcd.print("SUN ");
      break;
    case 2: 
      lcd.print("MON ");
      break;
    case 3: 
      lcd.print("TUE ");
      break;
    case 4:   
      lcd.print("WED ");
      break;
    case 5:  
      lcd.print("THU ");
      break;
    case 6:   
      lcd.print("FRI ");
      break;
    case 7:    
      lcd.print("SAT ");
      break;
  } 
  
  lcd.print(myRTC.month);
  lcd.print("/");
  lcd.print(myRTC.dayofmonth); 
  lcd.print(" ");
      
  lcd.print(myRTC.hours);
  lcd.print(":");
  lcd.print(myRTC.minutes);

  delay(1000);
}
