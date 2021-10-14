#include <virtuabotixRTC.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

virtuabotixRTC myRTC(6, 7, 8);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int lcd_key     = 0;
int adc_key_in  = 0;

int read_LCD_buttons()
{
 adc_key_in = analogRead(0)
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 250)  return btnUP;
 if (adc_key_in < 450)  return btnDOWN;
 if (adc_key_in < 650)  return btnLEFT;
 if (adc_key_in < 850)  return btnSELECT;
 return btnNONE;
}

void setup()  {
  lcd.begin(16, 2);
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

  lcd.setCursor(0,1);
 lcd_key = read_LCD_buttons();

 switch (lcd_key)
 {
   case btnRIGHT:
     {
     lcd.print(" Remove     ");
     break;
     }
   case btnLEFT:
     {
     lcd.print(" Enrollment");
     break;
     }
   case btnUP:
     {
     lcd.print(" Go to work");
     break;
     }
   case btnDOWN:
     {
     lcd.print(" Off work  ");
     break;
     }
 }
}
