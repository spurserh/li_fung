
const unsigned int pullups_positive[2] = {8, 7};
const unsigned int pulldowns_positive[2] = {9, 6};
const unsigned int pullups_negative[2] = {5, 3};
const unsigned int pulldowns_negative[2] = {4, 2};

void setup() {
  // initialize digital pin 13 as an output.
  for(int i=2;i<=9;++i) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void go_dead() {
  for(int i=0;i<2;++i) {
    digitalWrite(pullups_positive[i], LOW);
    digitalWrite(pulldowns_positive[i], HIGH);
    digitalWrite(pullups_negative[i], LOW);
    digitalWrite(pulldowns_negative[i], HIGH);
  }
}

void loop() { 
  go_dead();
  delayMicroseconds(50);
  for(int i=0;i<2;++i) {
    digitalWrite(pullups_positive[i], HIGH);
    digitalWrite(pulldowns_positive[i], LOW);
    digitalWrite(pullups_negative[i], LOW);
    digitalWrite(pulldowns_negative[i], HIGH);
  }
  delay(10);
  go_dead();
  delayMicroseconds(50);
  for(int i=0;i<2;++i) {
    digitalWrite(pullups_positive[i], LOW);
    digitalWrite(pulldowns_positive[i], HIGH);
    digitalWrite(pullups_negative[i], HIGH);
    digitalWrite(pulldowns_negative[i], LOW);
  }
  delay(10);
}
