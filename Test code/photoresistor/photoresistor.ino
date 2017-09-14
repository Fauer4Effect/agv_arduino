const int leftPhoto = A1;
const int rightPhoto = A0;

int leftBase, rightBase;

int readLight(int sensorPin){
  return analogRead(sensorPin);
}

void setup(){
  leftBase = readLight(leftPhoto);
  rightBase = readLight(rightPhoto);
  Serial.begin(9600);
  Serial.print("Left base:  ");
  Serial.print(leftBase);
  Serial.print("     Right base:  ");
  Serial.print(rightBase);
  Serial.println();
}

void loop(){
  Serial.begin(9600);
  Serial.print("Left:  ");
  Serial.print(readLight(leftPhoto));
  Serial.print("     Right:  ");
  Serial.print(readLight(rightPhoto));
  Serial.println();
  delay(500);
}

