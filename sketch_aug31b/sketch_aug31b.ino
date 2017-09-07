int motorPin = 10;
void setup() {
  pinMode(motorPin,OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(motorPin,HIGH);
  delay(500);
  digitalWrite(motorPin,LOW);
  delay(500);
}
