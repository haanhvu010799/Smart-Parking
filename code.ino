#include <ESP8266WiFi.h>
#include <Servo.h>
Servo servo1;                          //servo cổng vào
Servo servo2;                               //servo cổng ra

int carEnter = 33;                      // entry sensor 
int carExited = 32;                   //exi sensor
int slot3 = 35;                  
int slot2 = 34;
int slot1  = 25;
int count =0; 
int CLOSE_ANGLE = 90;  // The closing angle of the servo motor arm
int OPEN_ANGLE = 0;  // The opening angle of the servo motor arm              
int  hh, mm, ss;
int pos;
int pos1;

String h, m,EntryTimeSlot1,ExitTimeSlot1, EntryTimeSlot2,ExitTimeSlot2, EntryTimeSlot3,ExitTimeSlot3;
boolean entrysensor, exitsensor,s1,s2,s3;

boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;


void setup() {
  delay(1000);
  Serial.begin (9600); 
  timeClient.begin(); 
  servo1.attach(2);      // servo pin to D6
  servo2.attach(4);       // servo pin to D5
  pinMode(carExited, INPUT);    // ir as input
  pinMode(carEnter, INPUT);     // ir as input
  pinMode(slot1, INPUT);
  pinMode(slot2, INPUT);
  pinMode(slot3, INPUT);

}

void loop() {

 timeClient.update();
 hh = timeClient.getHours();
 mm = timeClient.getMinutes();
 ss = timeClient.getSeconds();
 h= String(hh); 
 m= String(mm);
 h +" :" + m;
 
 entrysensor= !digitalRead(carEnter);
 exitsensor = !digitalRead(carExited);
 s1 = digitalRead(slot1);
 s2 = digitalRead(slot2);
 s3 = digitalRead(slot3);
 
   if (entrysensor == 1) {                     // if high then count and send data
   count=  count+1;                             //increment count
   myservos.write(OPEN_ANGLE);
   delay(3000);
   myservos.write(CLOSE_ANGLE);
   }
   
   if (exitsensor == 1) {                            //if high then count and send
   count= count-1;                                    //decrement count
   myservo.write(OPEN_ANGLE);
   delay(3000);
   myservo.write(CLOSE_ANGLE);
  }

  if (s1 == 1 && s1_occupied == false) {                     
        Serial.println("Occupied1 ");
        EntryTimeSlot1 =  h +" :" + m;
        Serial.print("EntryTimeSlot1: ");
        Serial.print(EntryTimeSlot1);
    }
  if(s1 == 0 && s1_occupied == true) {
       Serial.println("Available1 ");
       ExitTimeSlot1 =  h +" :" + m;
       Serial.print("ExitTimeSlot1");
       Serial.print(ExitTimeSlot1);
     
       
}
  if (s2 == 1&& s2_occupied == false) {                     
      Serial.println("Occupied2 ");
      EntryTimeSlot2 =  h +" :" + m;
      Serial.print("EntryTimeSlot2");
      Serial.print(EntryTimeSlot2);
     
      s2_occupied = true;
      if (! EntrySlot2.publish((char*) EntryTimeSlot2.c_str())){}
    }
  if(s2 == 0 && s2_occupied == true) {
       Serial.println("Available2 ");
       ExitTimeSlot2 =  h +" :" + m;
       Serial.print("ExitTimeSlot2");
       Serial.print(ExitTimeSlot2);
       
       s2_occupied = false;
        if (! ExitSlot2.publish((char*) ExitTimeSlot2.c_str())){}
  }
  if (s3 == 1&& s3_occupied == false) {                     
      Serial.println("Occupied3 ");
      EntryTimeSlot3 =  h +" :" + m;
      Serial.print("EntryTimeSlot3: ");
      Serial.print(EntryTimeSlot3);
    }
  if(s3 == 0 && s3_occupied == true) {
       Serial.println("Available3 ");
       ExitTimeSlot3 =  h +" :" + m;
       Serial.print("ExitTimeSlot3: ");
       Serial.print(ExitTimeSlot3);

  } 

  
}