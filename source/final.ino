#include <virtuabotixRTC.h> // RTC 모듈을 사용하기 위한 헤더파일 include
#include <Wire.h>   // I2C통신을 사용하기 위한 헤더파일 include
#include <LiquidCrystal.h>  // LCD를 사용하기 위한 헤더파일 include
#include <Adafruit_Fingerprint.h> // 지문인식 모듈을 사용하기 위한 헤더파일 include

#define btnRIGHT  0   // lcd shield의 btnRIGHT 버튼 define
#define btnUP     1   // lcd shield의 btnUP 버튼 define
#define btnDOWN   2   // lcd shield의 btnDOWN 버튼 define
#define btnLEFT   3   // lcd shield의 btnLEFT 버튼 define
#define btnSELECT 4   // lcd shield의 btnSELECT 버튼 define
#define btnNONE   5   // lcd shield의 btnNONE 버튼 define


// 아두이노의 종류를 판단하기 위한 if 문
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

virtuabotixRTC myRTC(6, 7, 8);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int lcd_key     = 0;  // lcd shield 버튼을 사용하기 위한 변수
int adc_key_in  = 0;  // lcd shield 아날로그 신호를 읽기 위한 변수
int peopleId    = 1;  // 지문인식 모듈의 아이디 변수

String nameArr[3] = {"KIM", "LEE", "CHO"};  // 등록된 사용자의 이름을 저장하는 배열
int logArr[3] = {0, 0, 0};  // 사용자 번호 -> default는 0으로 저장

int read_LCD_buttons()  // lcd shield의 버튼을 구별하기 위한 if문
{
  adc_key_in = analogRead(0);
  if (adc_key_in < 50)   return btnRIGHT;   // btnRIGHT 버튼
  if (adc_key_in < 250)  return btnUP;      // btnUP 버튼
  if (adc_key_in < 450)  return btnDOWN;    // btnDOWN 버튼
  if (adc_key_in < 650)  return btnLEFT;    // btnLEFT 버튼
  if (adc_key_in < 850)  return btnSELECT;  // btnSELECT 버튼
  return btnNONE;
}

void setup()  {
  lcd.begin(16, 2);   // lcd의 크기 설정
  myRTC.setDS1302Time(00, 11, 16, 5, 21, 10, 2021); // RTC 모듈의 초기 날짜와 시간의 값

  //시리얼 통신 시작
  Serial.begin(9600);
  //lcd 통신 시작
  lcd.begin(16, 2);

  //엑셀에 출력될 라벨(타이틀) 출력
  Serial.println("CLEARDATA");
  Serial.println("LABEL, TIME, ID, NAME, LOG");
  
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);

  //지문인식 센서 통신 시작
  finger.begin(57600);
  delay(5);
  //지문인식 센서 디바이스 연결 확인
  if (finger.verifyPassword()) {
  } else {
    while (1) { delay(1); }
  }

  //지문인식 센서 정보 확인
  finger.getParameters();

  //등록된 지문 유무 판단
  finger.getTemplateCount();

  if (finger.templateCount == 0) {
  }
  else {
  }
}

void loop()  {
  myRTC.updateTime();   // 시간 업데이트
  lcd.setCursor(0, 0);
  switch (myRTC.dayofweek) {  // 요일을 출력하기 위한 swich 문
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

  lcd.print(myRTC.month);   // 월 출력
  lcd.print("/");
  lcd.print(myRTC.dayofmonth);  // 일 출력
  lcd.print(" ");

  lcd.print(myRTC.hours);   // 시간 출력
  lcd.print(":");
  lcd.print(myRTC.minutes);   // 분 출력

  lcd.setCursor(0, 1);
  lcd.print("click the button");  //click the button 출력

  lcd.setCursor(0, 1);
  lcd_key = read_LCD_buttons();

  getFingerprintID();
  delay(50);

  switch (lcd_key)  // lcd shield 버튼 swich문
  {
    case btnLEFT:   // case btnLEFT 선택
      {
        lcd.print("Enrollment      ");
        Serial.begin(9600);
        while (!Serial);  // For Yun/Leo/Micro/Zero/...
        delay(1000);
        Serial.println("\n\nAdafruit Fingerprint sensor enrollment");


        // set the data rate for the sensor serial port
        finger.begin(57600);

        if (finger.verifyPassword()) {  // 지문인식 모듈 유무 확인 if 문
          Serial.println("Found fingerprint sensor!");
        } else {
          Serial.println("Did not find fingerprint sensor :(");
          while (1) {
            delay(1);
          }
        }

        Serial.println(F("Reading sensor parameters"));
        finger.getParameters();
        Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
        Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
        Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
        Serial.print(F("Security level: ")); Serial.println(finger.security_level);
        Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
        Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
        Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

        Serial.println("Ready to enroll a fingerprint!");
        Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Put your finger ");
        lcd.setCursor(0, 1);
        lcd.print("on");
        id = peopleId++;  // 사용자 번호 자동 증가
        if (id == 0) {// ID #0 not allowed, try again!
          return;
        }
        Serial.print("Enrolling ID #");
        Serial.println(id);

        while (!  getFingerprintEnroll() );

        break;
      }
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

uint8_t getFingerprintEnroll() {    // 지문을 등록하기 위한 함수

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {     // 인식된 지문 판단을 위한 swich문
      case FINGERPRINT_OK:    // 정상적인 지문 인식
        Serial.println("Image taken");  
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Image taken");
        break;
      case FINGERPRINT_NOFINGER:  // 지문이 인식되지 않음
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:  // 모듈 분리 및 하드웨어 에러
        Serial.println("Communication error");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        break;
      case FINGERPRINT_IMAGEFAIL:   // 정상적이지 않은 지문
        Serial.println("Imaging error");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        break;
      default:
        Serial.println("Unknown error");  // default
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {    // 첫번째 지문과 두번째 지문을 동시에 판단하는 swich 문
    case FINGERPRINT_OK:    // 정상적인 지문 인식
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:   // 정상적이지 않은 지문
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:  // 모듈 분리 및 하드웨어 에러
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:   // 지문인식 기능 실패
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:    // 지문인식 기능 무효화
      Serial.println("Could not find fingerprint features");
      return p;
    default:    // default
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place same fing");
  lcd.setCursor(0, 1);
  lcd.print("er again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        break;
      default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}

uint8_t getFingerprintID() {    // 지문의 일치 여부를 판단하기 위한 함수
  uint8_t p = finger.getImage();
  /*lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Put your finger");
  lcd.setCursor(0, 1);
  lcd.print("on the sensor!");*/
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_IMAGEFAIL:
      return p;
    default:
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    return p;
  } else {
    return p;
  }

  // found a match!
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("#");
  lcd.print(finger.fingerID);
  lcd.print(" ");
  lcd.print(nameArr[finger.fingerID - 1]);
  lcd.print(" ");

  Serial.print("DATA, TIME, ");
  Serial.print(finger.fingerID);
  Serial.print(", ");
  Serial.print(nameArr[finger.fingerID - 1]);
  Serial.print(", ");

  if(logArr[finger.fingerID - 1] == 0){
    logArr[finger.fingerID - 1] = 1;
    Serial.println("Log In");
    lcd.print("Log In");
    lcd.setCursor(0, 1);
    lcd.print("Have a nice day!");
  }
  else{
    logArr[finger.fingerID - 1] = 0;
    Serial.println("Log Out");
    lcd.print("Log Out");
    lcd.setCursor(0, 1);
    lcd.print("See you again!");
  }
  delay(2000);

  return finger.fingerID;
}
