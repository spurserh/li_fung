
const int pins[3] = {3,4,5};

void setup() {
  for(int i=0;i<3;++i) {
    pinMode(pins[i], OUTPUT);
  }
}

#if 1
void loop() {
  // 50Hz
  const unsigned long cycle_micros = 20000;
  
  for(int i=0;i<2;++i) {
    const unsigned long m = ((cycle_micros/4) * i + micros()) % cycle_micros;
    digitalWrite(pins[i], (m < cycle_micros/2) ? HIGH : LOW);
  }
}
#else
// the loop function runs over and over again forever
void loop() {
  // 50Hz
  const unsigned long half_cycle_micros = 10000;
  // 2Khz
//  const unsigned long half_cycle_micros = 250;

  // 10Hz - 100Hz
 /* static unsigned long s_millis = millis();
  const unsigned long half_cycle_micros = 5000 + 2*((millis() - s_millis) % (9500*5));
*/

  const unsigned long dead_time_micros = 10;
  
  digitalWrite(pins[0], LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(pins[1], HIGH);   // turn the LED on (HIGH is the voltage level)
  delayMicroseconds(half_cycle_micros - dead_time_micros);
  digitalWrite(pins[0], LOW);
  digitalWrite(pins[1], LOW);
  delayMicroseconds(dead_time_micros);
  digitalWrite(pins[0], HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(pins[1], LOW);    // turn the LED off by making the voltage LOW
  delayMicroseconds(half_cycle_micros - dead_time_micros);
  digitalWrite(pins[0], LOW);
  digitalWrite(pins[1], LOW);
  delayMicroseconds(dead_time_micros);
}
#endif
