#include <Servo.h> 
 
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

//turn 90 deg right
void right_90() {
  for (i = 0; i < dur*20; i ++){
    servo1.writeMicroseconds(1250);
    servo2.writeMicroseconds(1750);
    delay(15);
  }  
}

//turn 90 deg left
void left_90(){
  for (i=0;i>dur*20;i++){
    servo1.writeMicroseconds(1750);
    servo2.writeMicroseconds(1250);
    delay(15);
  }
}

//go forwards
void forwards() {
  for (i = 0; i< dur*5; i++){
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
} 
 
void loop() { 
  //get distance to obstacle
  long inches;
  inches = getDistance();

  int irLeft;
  irLeft = irRead(irLedLeft, irDetLeft);
  int irRight;
  irRight = irRead(irLedRight, irDetRight);

  //TODO if one side sees tape turn other way
  //if both see tape reverse

  Serial.print(inches);
  Serial.println();
  Serial.print("LEFT: ");
  Serial.print(irLeft);
  Serial.print("    RIGHT: ");
  Serial.print(irRight);
  Serial.println();

  //if distance > 12 drive forwards
  //FIXME figure out how to stop farther from obstacle
  /*if(inches > 12) {
    forwards();
  }
  //else turn
  else {
    right_90();
  }
  */

  if(inches < 12) {
    right_90();
  }
  else if(irLeft == 0 && irRight == 0) {
    turn_180();
  }
  else if(irLeft == 0) {
    right_90();
  }
  else if(irRight == 0) {
    left_90();
  }
  else {
    forwards();
  }

  //stop the motor everytime
  stop();
} 
