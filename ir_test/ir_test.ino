const int irLedLeft = 9;
const int irDetLeft = 10;
const int irLedRight = 4;
const int irDetRight = 5;

const int freq = 38000;

int irRead(int irLed, int irDet){
  tone(irLed,freq,8);
  delay(1);
  int ir = digitalRead(irDet);
  delay(250);
  return ir;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(irLedLeft, OUTPUT);
  pinMode(irDetLeft, INPUT);
  pinMode(irLedRight, OUTPUT);
  pinMode(irDetRight, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int irLeft;
  irLeft = irRead(irLedLeft, irDetLeft);
  int irRight;
  irRight = irRead(irLedRight, irDetRight);

  Serial.print("LEFT: ");
  Serial.print(irLeft);
  Serial.print("    RIGHT: ");
  Serial.print(irRight);
  Serial.println();
}
