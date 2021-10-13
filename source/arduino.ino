#include <LiquidCrystal.h>
#define GOTOWORK 7
#define OFFWORK 6
#define ENROLLMENT 5
#define REMOVE 4

LiquidCrystal lcd{13, 12, 11, 10, 9, 8};

void setup() {
  lcd.begin(16, 2);
  //Serial.begin(9600);
  pinMode(GOTOWORK, INPUT);
  pinMode(OFFWORK, INPUT);
  pinMode(ENROLLMENT, INPUT);
  pinMode(REMOVE, INPUT);
}

void loop() {
  //int value = digitalRead(GOTOWORK);
  
  //int value = digitalRead(GOTOWORK);
  //Serial.println(digitalRead(GOTOWORK));
  
  if (digitalRead(GOTOWORK) == LOW) {
    printGoToWork();
  }
  else if (digitalRead(OFFWORK) == LOW) {
    printOffWork();
  }
  else if (digitalRead(ENROLLMENT) == LOW) {
    printEnrollment();
  }
  else if (digitalRead(REMOVE) == LOW) {
    printRemove();
  }
  
}

void printGoToWork() {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("go to work");
}

void printOffWork() {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("off work");
}

void printEnrollment() {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("enrollment");
}

void printRemove() {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("remove");
}
