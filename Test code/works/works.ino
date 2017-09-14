/*  
 *   Code to operate the MMP-5 with a Sabertooth Motor Controller set to R/C mode
 *   The Sabertooth expects to receive a "center" or "stop" position first (as if the R/C 
 *   joystick was in the neutral position.
 *   Then it expect to receive the end limits.
 *   If this code is not executed when the controller is powered up, the MMP-5 may move
 *   eratically or not at all
 *   COL Lisa A. Shay
 */

#include <Servo.h> 
 
Servo myservo;
Servo serv2;
 
int i = 0;      // loop counter
const int dur = 2;  // counter duration 2 is the minimun setting to 
                    // produce reliable results.  Increase to 25 if 
                    // the motor controller behaves eratically.

const int pingPin = 7;
 
void setup() 
{ 
  myservo.attach(10);   
  serv2.attach(11);

  Serial.begin(9600);
  
  for (i = 0; i < dur; i++ )
 { 
  myservo.writeMicroseconds(1500);    // motor stop 
  delay(15); 
  }

  for (i = 0; i< dur; i++)
  {
     myservo.writeMicroseconds(1000);
     serv2.writeMicroseconds(1000); // full speed in one direction
     delay(15);  
  } 
  for (i = 0; i< dur; i++)
  {
    myservo.writeMicroseconds(2000);
    serv2.writeMicroseconds(2000); // full speed in the other direction
    delay(15);
  }  
    for (i = 0; i < dur; i++ )
 { 
  myservo.writeMicroseconds(1500);
  serv2.writeMicroseconds(1500);// motor stop 
  delay(15); 
  }

  delay(1000);
//// test that code did what it was supposed to do (omit or replace with your algorithm)
//  for (i = 0; i< dur*55; i++)
//  {
//     myservo.writeMicroseconds(1250);
//     serv2.writeMicroseconds(1250);// half speed in one direction
//     delay(15);  
//  }
//    //for (i = 0; i< dur*100; i++)
//  //{
//    // myservo.writeMicroseconds(1750);
//     //serv2.writeMicroseconds(1750);// half speed in one direction
//     //delay(15);  
//  //}
//   myservo.writeMicroseconds(1500);
//   serv2.writeMicroseconds(1500);// motor stop 

  
} 

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}
 
void loop() 
{ 
//get distance
  long duration, inches;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin,LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin,LOW);

  pinMode(pingPin,INPUT);
  duration = pulseIn(pingPin, HIGH);

  inches = microsecondsToInches(duration);
  Serial.print(inches);
  Serial.println();

  if(inches > 12){
    for (i = 0; i< dur*5; i++){
     myservo.writeMicroseconds(1250);
     serv2.writeMicroseconds(1250);// half speed in one direction
     delay(15);  
    }
  }
  else {
    //turn right
    for (i = 0; i < dur*20; i ++){
      myservo.writeMicroseconds(1250);
      serv2.writeMicroseconds(1750);
      delay(15);
    }
  }
  myservo.writeMicroseconds(1500);
  serv2.writeMicroseconds(1500);// motor stop 
} 

