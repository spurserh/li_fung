
const int voltage_in = 10;
/*
const int high_side_pullups[2] = {
  2,4,6,8,
};

const int high_side_pulldowns[2] = {
  1,3,5,7,
};
*/

void setup() {
  pinMode(voltage_in, INPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
  
  Serial.begin(9600);
 // while(!Serial);
  
  
}

void loop() {
  /*
  Serial.print("Voltage ");
  Serial.println(digitalRead(voltage_in));
  */
  
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(10);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(10);
}
