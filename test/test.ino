#include "ThingsBoard.h"
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

#define WIFI_AP             "HaAnh"
#define WIFI_PASSWORD       "khongbiet"

#define TOKEN               "PcdXMN3Jb33JAE0C5UZv"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;
Servo servoin;                          //servo as gate
Servo servoout;                               //servo as gate

int carEnter = D0;                      // entry sensor
int carExited = D7;                   //exi sensor
int slot2 = D6;
int slot1  = D3;
int count = 0, sumin = 0, sumout = 0;
int CLOSE_ANGLE = 90;  // The closing angle of the servo motor arm
int OPEN_ANGLE = 0;  // The opening angle of the servo motor arm

int entrysensor, exitsensor, s1, s2, s3;
String trangthai = "", trangthai1 = "", trangthai2 = "", trangthai3 = "";
boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;
void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
  servoout.attach(D4);      // servo pin to D6
  servoin.attach(D5);
  pinMode(carExited, INPUT);    // ir as input
  pinMode(carEnter, INPUT);     // ir as input
  pinMode(slot1, INPUT);
  pinMode(slot2, INPUT);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
  delay(1500);
  Serial.println("Sending data...");

  entrysensor = digitalRead(carEnter);
  exitsensor = digitalRead(carExited);
  s1 = digitalRead(slot1);
  s2 = digitalRead(slot2);
  lcd.setCursor(0, 0);
  lcd.print("Cho trong  ");
  lcd.setCursor(11, 0);
  int trong = 2 - count;
  lcd.print(trong);
  if (entrysensor == 0) {                     // if high then count and send data
    //increment count
    if (count >= 0 && count < 2 )
    {
      count =  count + 1;
      sumin = sumin + 1;
      servoin.write(180);
      lcd.setCursor(0, 1);
      lcd.print("Kinh chao quy khanh");
      delay(2000);
      servoin.write(90);
      lcd.clear();
      if (count > 2) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bai xe khong con cho trong");
        Serial.println("Bai xe da full");
        count = 2;
        delay(1500);
        lcd.clear();
      }

    }
    Serial.print("So xe trong bai: ");
    Serial.print(count);
    Serial.print("\n");


  }

  if (exitsensor == 0) {                            //if high then count and send
    if (count > 0 && count < 3 ) {
      count = count - 1;

      if (count < 0) {
        count = 0;
        Serial.print("So xe trong bai: ");
        Serial.print(count);
      }//decrement count
      servoout.write(180);
      sumout = sumout + 1;
      lcd.setCursor(0, 1);
      lcd.print("Tam biet quy khach");
      delay(2000);
      servoout.write(90);
      Serial.println("So xe trong bai: ");
      Serial.print(count);
      lcd.clear();
    }
  }

  if (s1 == 1 && s1_occupied == false) {
    Serial.println("Occupied1 ");
    s1_occupied = true;


  }
  if (s1 == 0 && s1_occupied == true) {
    Serial.println("Available1 ");

    s1_occupied = false;
  }
  if (s2 == 1 && s2_occupied == false) {
    Serial.println("Occupied2 ");

    s2_occupied = true;
  }
  if (s2 == 0 && s2_occupied == true) {
    Serial.println("Available2 ");

    s2_occupied = false;
  }
  tb.sendTelemetryInt("Xe trong bai", count);
  tb.sendTelemetryInt("Tong luong xe vao", sumin);
  tb.sendTelemetryInt("Tong luong xe ra", sumout);
  tb.sendTelemetryBool("Slot 1 con trong", s1_occupied);
  tb.sendTelemetryBool("Slot 2 con trong", s2_occupied);
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
