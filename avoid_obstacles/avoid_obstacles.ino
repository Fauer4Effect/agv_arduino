/*  
  Code to operate the MMP-5 with a Sabertooth Motor Controller set to R/C mode
  The Sabertooth expects to receive a "center" or "stop" position first (as if the R/C 
  joystick was in the neutral position.
  Then it expect to receive the end limits.
  If this code is not executed when the controller is powered up, the MMP-5 may move
  eratically or not at all
  COL Lisa A. Shay

----------------------------------------------------------------------------------------------

  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return. The length of the
  returning pulse is proportional to the distance of the object from the sensor.

  The circuit:
    - +V connection of the PING))) attached to +5V
    - GND connection of the PING))) attached to ground
    - SIG connection of the PING))) attached to digital pin 7

  created 3 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Ping

----------------------------------------------------------------------------------------------

 http://learn.parallax.com/tutorials/robot/shield-bot/robotics-board-education-shield-arduino/chapter-7-navigating-infrared-9
 Robotics with the BOE Shield - TestLeftIR
 Display 1 if the left IR detector does not detect an object,
 or 0 if it does.
 
----------------------------------------------------------------------------------------------

  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInput

*/

#include <Servo.h> 
#include <QueueList.h>
 
Servo servo1;   //right side
Servo servo2;   //left side
 
int i = 0;      
const int dur = 2;
//WTF only works on pin 7?  
const int pingPin = 7;
const int irLedLeft = 9;
const int irDetLeft = 10;
const int irLedRight = 4;
const int irDetRight = 5;

const int freq = 38000;
int next;

const int lightPinRight = A0;
const int lightPinLeft = A1;

int last, two_back;
int leftBaseline, rightBaseline;
int dark;

/*0 -> forwards
1 -> right_90
2 -> left_90
3 -> turn_180
4 -> stop*/


void servoCenter() {
  servo1.attach(12);   
  servo2.attach(13);

  for (i = 0; i < dur; i++ ) { 
    servo1.writeMicroseconds(1500);    // motor stop 
    //servo2.writeMicroseconds(1500);   //WTF this breaks if included
    delay(15); 
  }
  for (i = 0; i< dur; i++){
    servo1.writeMicroseconds(1000);
    servo2.writeMicroseconds(1000); // full speed in one direction
    delay(15);  
  } 
  for (i = 0; i< dur; i++) {
    servo1.writeMicroseconds(2000);
    servo2.writeMicroseconds(2000); // full speed in the other direction
    delay(15);
  }  
  for (i = 0; i < dur; i++ ) { 
    servo1.writeMicroseconds(1500);
    servo2.writeMicroseconds(1500);// motor stop 
    delay(15); 
  }
  delay(1000);
}

//get distance to obstacle from ping sensor
long getDistance(){
  long duration;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin,LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin,HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin,LOW);

  pinMode(pingPin,INPUT);
  duration = pulseIn(pingPin, HIGH);

  return duration / 74 / 2;
}

int readLightSensor(int sensorPin){
  return analogRead(sensorPin);
}

//turn 90 deg right
void right_90() {
  Serial.println("RIGHT");
  for (i = 0; i < dur*18; i ++){
    servo1.writeMicroseconds(1250);
    servo2.writeMicroseconds(1750);
    delay(15);
  }  
}

//turn 90 deg left
void left_90(){
  Serial.println("LEFT");
  for (i=0;i<dur*18;i++){
    servo1.writeMicroseconds(1750);
    servo2.writeMicroseconds(1250);
    delay(15);
  }
}

//go forwards
void forwards() {
  Serial.println("FORWARDS");
  for (i = 0; i< dur*15; i++){
    servo1.writeMicroseconds(1250);
    servo2.writeMicroseconds(1250);
    delay(10);   //TODO figure out if this is what causes the problems.
  }
}

void stop() {
  servo1.writeMicroseconds(1500);
  servo2.writeMicroseconds(1500); 
}

void turn_180() {
  Serial.println("180");
  right_90();
  right_90();
}

int irRead(int irLed, int irDet) {
  tone(irLed,freq,8);
  delay(1);
  int ir = digitalRead(irDet);
  delay(100);
  return ir;
}
 
void setup() { 
  Serial.begin(9600);
  pinMode(irLedLeft, OUTPUT);
  pinMode(irDetLeft, INPUT);
  pinMode(irLedRight, OUTPUT);
  pinMode(irDetRight, INPUT);
  servoCenter(); 
  last = 0;
  leftBaseline = readLightSensor(lightPinLeft);
  rightBaseline = readLightSensor(lightPinRight);
  dark = 0;
} 
 
void loop() { 
  //get distance to obstacle
  long inches;
  inches = getDistance();

  int irLeft;
  irLeft = irRead(irLedLeft, irDetLeft);
  int irRight;
  irRight = irRead(irLedRight, irDetRight);

  int lightLeft;
  lightLeft = readLightSensor(lightPinLeft);
  int lightRight;
  lightRight = readLightSensor(lightPinRight);

  int leftPoll, rightPoll;
  leftPoll = 0;
  rightPoll = 0;
  leftPoll += irRead(irLedLeft, irDetLeft);
  leftPoll += irRead(irLedLeft, irDetLeft);
  rightPoll += irRead(irLedRight, irDetRight);
  rightPoll += irRead(irLedRight, irDetRight);

  if((lightLeft < 200) && (lightRight < 200)){
    dark = 1;
  }

  //TODO if one side sees tape turn other way
  //if both see tape reverse
  if((inches < 12) || ((irLeft == 0) && (irRight == 0))) {
    stop();
    if(last == 1) {
      Serial.println("CORNERED"); 
      turn_180();
      last = 3;
    }  
    else{
      Serial.println("WALL");
      right_90();
      last = 1;
    }
  }

  else if(dark == 1){
    if((lightLeft + 200) >= lightRight){
      left_90();
    }
    else if((lightRight + 200) >= lightLeft) {
      right_90();
    }
    else {
      forwards();
    }
  }

  else if(dark == 0){
    if((lightLeft + 200) <= lightRight) {
      left_90();
    }
    else if((lightRight + 200) <= lightLeft) {
      right_90();
    }
    
    else if(leftPoll == 0){
      right_90();
      last = 1;
      two_back = 0;
    }
    else if(rightPoll == 0){
      left_90();
      last = 2;
      two_back = 0;
    }
  
    else {
      //get on course again after obstacle
      if(last == 3){
        forwards();
        last = 0;
        two_back = 3;
      }
      else if(two_back==3){
        right_90();
        last = 0;
        two_back = 0;
      }
      else if(last == 1){
        forwards();
        last = 0;
        two_back = 1;
      }
      else if(two_back == 1){
        left_90();
        two_back = last;
        last = 2;
        
      }
      //no obstacle go forwards
      else {
        forwards();
        last = 0;
      }
    }
  }

  //stop the motor everytime
  stop();
  //delay(500);
} 

