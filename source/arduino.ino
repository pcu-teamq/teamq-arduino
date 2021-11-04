/***
# Copyright (C) TeamQ <http://www.teamq.notion.kro.kr>
# @auther TeamQ (hyeokju1313@gmail.com)
# @file source/arduino.ino
# @brief TeamQ Arduino H/W code
# @version 1.0.0
# GitHub <https://github.com/pcu-teamq>
***/

// include Header File
#include <virtuabotixRTC.h>
#include <Wire.h>
#include <LiquidCrystal.h>

// Button name define
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

// RTC Module & LCD Display Digital port
virtuabotixRTC myRTC(6, 7, 8);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int lcd_key = 0;
int adc_key_in = 0;

// Button global setting
// 각 버튼들은 회로도 상 저항 값으로 구별
int read_LCD_buttons()
{
    adc_key_in = analogRead(0);
    if (adc_key_in < 50)
        return btnRIGHT;
    if (adc_key_in < 250)
        return btnUP;
    if (adc_key_in < 450)
        return btnDOWN;
    if (adc_key_in < 650)
        return btnLEFT;
    if (adc_key_in < 850)
        return btnSELECT;
    return btnNONE;
}

// setup
void setup()
{
    lcd.begin(16, 2); // LCD spec 16x2 Display define
    myRTC.setDS1302Time(00, 20, 42, 1, 13, 10, 2021); // RTC default Time setting
}

// loop
void loop()
{
    myRTC.updateTime(); // setDS1302Time() -> updateTime()
    lcd.setCursor(1, 0); // RTC Display view setting
    
    // RTC week define
    switch (myRTC.dayofweek)
    {
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
    
    // Day, Week, Time print LCD
    lcd.print(myRTC.month);
    lcd.print("/");
    lcd.print(myRTC.dayofmonth);
    lcd.print(" ");
    lcd.print(myRTC.hours);
    lcd.print(":");
    lcd.print(myRTC.minutes);

    lcd.setCursor(0, 1); // Button Display view setting
    lcd_key = read_LCD_buttons(); // Button read signal

    // Button key define
    switch(lcd_key)
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
