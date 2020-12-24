#include <Servo.h>
Servo servo;


int val;
float temp;

void setup() {
  // Bluetooth setup
  Serial.begin(9600);
  
  // For rele
  pinMode(4, OUTPUT);
  // For servo
//  servo.attach(9);
//  servo.write(0);
}

void loop() {
  
  if (Serial.available()) {
    val = Serial.read();
    if (val == 1) {
      digitalWrite(4, LOW);
    } else if (val == 0) {
      digitalWrite(4, HIGH);
    } else if (val > 1 && val < 180) {
//      servo.write(val);
    }
  }
}
