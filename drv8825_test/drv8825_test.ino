
#include <util/delay_basic.h>

const unsigned int dir = 11;
const unsigned int stp = 10;
const unsigned int flt = 9;
const unsigned int micro = 4;

void setup() {
  Serial.begin (9600);
  pinMode(dir, OUTPUT);
  digitalWrite(dir, LOW);
  pinMode(stp, OUTPUT);
  digitalWrite(stp, LOW);
  pinMode(flt, INPUT);
  pinMode(micro, OUTPUT);
  digitalWrite(micro, LOW);
}

void loop() {
  digitalWrite(stp, HIGH);
  delayMicroseconds(2);
  
//  delay(10);
  digitalWrite(stp, LOW);
  delayMicroseconds(2);

  if(digitalRead(flt) == LOW)
    Serial.println("fault!");
  // TODO
//  delay(10);

  static unsigned long mm = millis();

  const boolean is_micro = ((millis() - mm)%5000) < 2500;
  
  digitalWrite(dir, ((millis() - mm)%2000) < 500 ? HIGH : LOW);
  digitalWrite(micro, is_micro ? HIGH : LOW);
  delayMicroseconds(is_micro ? 100 : 3200);
}

