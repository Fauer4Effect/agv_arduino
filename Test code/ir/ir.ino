/*
 * http://learn.parallax.com/tutorials/robot/shield-bot/robotics-board-education-shield-arduino/chapter-7-navigating-infrared-9
 * Robotics with the BOE Shield - TestLeftIR
 * Display 1 if the left IR detector does not detect an object,
 * or 0 if it does.
 */

void setup()                                 // Built-in initialization block
{
Serial.begin(9600);
}  
 
void loop()                                  // Main loop auto-repeats
{
  tone(9, 38000, 8);                // IRLED 38 kHz for at least 1 ms
  delay(1);                         // Wait 1 ms
  int ir = digitalRead(10);         // IR receiver -> ir variable
  Serial.println(ir);
  delay(1000);                      //wait 1 sec between readings
}

