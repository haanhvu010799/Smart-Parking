#include <ESP8266WiFi.h>
#include <Servo.h>


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
int  hh, mm, ss;
int pos;
int pos1;

String h, m, EntryTimeSlot1, ExitTimeSlot1, EntryTimeSlot2, ExitTimeSlot2, EntryTimeSlot3, ExitTimeSlot3;
boolean entrysensor, exitsensor, s1, s2, s3;

boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;


void setup() {
  delay(1000);
  Serial.begin (115200);

  servoout.attach(D4);      // servo pin to D6
  servoin.attach(D5);       // servo pin to D5
  pinMode(carExited, INPUT);    // ir as input
  pinMode(carEnter, INPUT);     // ir as input
  pinMode(slot1, INPUT);
  pinMode(slot2, INPUT);
  pinMode(slot3, INPUT);
  
}

void loop() {


  entrysensor = digitalRead(carEnter);
  exitsensor = digitalRead(carExited);
  s1 = digitalRead(slot1);
  s2 = digitalRead(slot2);
  s3 = digitalRead(slot3);

  if (entrysensor == 0) {                     // if high then count and send data
    count =  count + 1;                          //increment count
    if (count > 2) {
      Serial.println("Bai xe da full");
      count=3;
    }
    Serial.print("So xe trong bai: ");
    Serial.print(count);
    Serial.print("\n");
    if (count > 0 && count < 3)
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
  if (s3 == 1 && s3_occupied == false) {
    Serial.println("Occupied3 ");
    s3_occupied = true;
  }
  if (s3 == 0 && s3_occupied == true) {
    Serial.println("Available3 ");
    s3_occupied = false;
  }


}
