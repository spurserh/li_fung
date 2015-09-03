

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(30);              // wait for a second
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(13, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(5000);              // wait for a second
}
