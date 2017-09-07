#include <Servo.h>

Servo serv1;
Servo serv2;

int pos = 0;
void setup() {
  serv1.attach(10);
  serv2.attach(11);

  int count = 0;
  int i = 0;
  for(i=0;i<10;i+=1){
    for(pos = 90; pos <= 180; pos += 1){
      //if(pos==180) count = 1;
      if(count==1) break;
      serv1.write(pos);
      serv2.write(pos);
      delay(15);
    }
  }
  serv1.detach();

}

void loop() {
  // put your main code here, to run repeatedly:

}
