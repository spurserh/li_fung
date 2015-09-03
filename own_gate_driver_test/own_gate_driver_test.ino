
const unsigned int high_gate_high = 11;
const unsigned int high_gate_low = 10;
const unsigned int low_gate = 12;

void setup() {
  // initialize digital pin 13 as an output.
  pinMode(high_gate_high, OUTPUT);
  pinMode(high_gate_low, OUTPUT);
  pinMode(low_gate, OUTPUT);
  
  digitalWrite(high_gate_high, LOW);
  digitalWrite(high_gate_low, LOW);
  digitalWrite(low_gate, HIGH);
}

void loop() {
  // High side switch needs to be turned on to get the process moving
  digitalWrite(high_gate_high, HIGH);
  digitalWrite(low_gate, LOW);
  delayMicroseconds(50);
  
  delay(10);

  digitalWrite(high_gate_high, LOW);
  delayMicroseconds(50);
  digitalWrite(low_gate, HIGH);
  delayMicroseconds(50);
  
  for(unsigned long ms=micros();(micros()-ms) < 10000;) {
      digitalWrite(high_gate_low, LOW);
      delayMicroseconds(75);
      digitalWrite(high_gate_high, HIGH);
      delayMicroseconds(50);
      
      digitalWrite(high_gate_high, LOW);
      delayMicroseconds(75);
      digitalWrite(high_gate_low, HIGH);
      delayMicroseconds(50);
  }
  
  //delay(10);
  
}
