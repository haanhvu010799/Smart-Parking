#include <ESP8266WiFi.h>
#include <Servo.h>
#include "ThingsBoard.h"

#define WIFI_AP             "HaAnh"
#define WIFI_PASSWORD       "khongbiet"
#define TOKEN               "OUwr8wyb9MWJmoG4ReLG"
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
int carExited = D2;                   //exi sensor
int slot3 = D7;
int slot2 = D6;
int slot1  = D3;
int count = 0;
int CLOSE_ANGLE = 80;  // The closing angle of the servo motor arm
int OPEN_ANGLE = 0;  // The opening angle of the servo motor arm
int pos;
int pos1;

boolean entrysensor, exitsensor, s1, s2, s3;
String trangthai = "", trangthai1 = "", trangthai2 = "", trangthai3 = "";
boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;
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

void setup() {

  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
  servoout.attach(D4);      // servo pin to D6
  servoin.attach(D5);       // servo pin to D5
  pinMode(carExited, INPUT);    // ir as input
  pinMode(carEnter, INPUT);     // ir as input
  pinMode(slot1, INPUT);
  pinMode(slot2, INPUT);
  pinMode(slot3, INPUT);

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
  Serial.println("Sending data...");
  entrysensor = digitalRead(carEnter);
  exitsensor = digitalRead(carExited);
  s1 = digitalRead(slot1);
  s2 = digitalRead(slot2);
  s3 = digitalRead(slot3);

  if (entrysensor == 0) {                     // if high then count and send data
    count =  count + 1;                          //increment count
    if (count >= 3) {
      Serial.println("Bai xe da full");
      trangthai = "Bai xe da day";
      count = 3;
    }
    Serial.print("So xe trong bai: ");
    Serial.print(count);
    Serial.print("\n");
    if (count > 0 && count <= 3)
    {
      servoin.write(OPEN_ANGLE);
      delay(1000);
      servoin.write(CLOSE_ANGLE);
    }

  }

  if (exitsensor == 0) {                            //if high then count and send
    count = count - 1;
    if (count < 0) {
      count = 0;
    }//decrement count
    Serial.print("So xe trong bai: ");
    Serial.print(count);

    Serial.println(count);
    if (count > 0 ) {
      servoout.write(OPEN_ANGLE);
      delay(1000);
      servoout.write(CLOSE_ANGLE);
    }
  }

  if (s1 == 1 && s1_occupied == false) {
    Serial.println("Occupied1 ");
    s1_occupied = true;
    trangthai1 = "Da co xe dau";

  }
  if (s1 == 0 && s1_occupied == true) {
    Serial.println("Available1 ");
    trangthai1 = "Con trong";
    s1_occupied = false;
  }
  if (s2 == 1 && s2_occupied == false) {
    Serial.println("Occupied2 ");
    trangthai2 = "Da co xe dau";
    s2_occupied = true;
  }
  if (s2 == 0 && s2_occupied == true) {
    Serial.println("Available2 ");
    trangthai2 = "Con trong";
    s2_occupied = false;
  }
  if (s3 == 1 && s3_occupied == false) {
    Serial.println("Occupied3 ");
    trangthai3 = "Da co xe dau";
    s3_occupied = true;
  }
  if (s3 == 0 && s3_occupied == true) {
    Serial.println("Available3 ");
    trangthai3 = "Con trong";
    s3_occupied = false;
  }
//  tb.sendTelemtry("Tinh trang bai xe", tinhtrang);
//  tb.sendTelemtry("Tinh trang cho dau xe 1", tinhtrang1);
//  tb.sendTelemtry("Tinh trang cho dau xe 2", tinhtrang2);
//  tb.sendTelemtry("Tinh trang cho dau xe 3", tinhtrang3);
//  tb.sendTelemtryInt("Tong so xe trong bai", count);
//  tb.loop();
}
