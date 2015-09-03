
const int motor_brake_p = 13;
const int brake_winding_engage = 3;
const int enable_p = 2;
const int step_p = 4;

void enable_stepper_ctl(boolean enabled) {
  digitalWrite(enable_p, enabled ? HIGH : LOW);
  digitalWrite(brake_winding_engage, enabled ? HIGH : LOW);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Rebooted");
  
  pinMode(motor_brake_p, OUTPUT);
  pinMode(step_p, OUTPUT);
  digitalWrite(step_p, LOW);
  pinMode(enable_p, OUTPUT);
  pinMode(brake_winding_engage, OUTPUT);

  digitalWrite(motor_brake_p, LOW);
  enable_stepper_ctl(false);
}

void unbrake_motor() {
  enable_stepper_ctl(false);  // Safety first
  digitalWrite(brake_winding_engage, LOW);
  delayMicroseconds(500);
  digitalWrite(motor_brake_p, HIGH);
  //delay(30);
  delay(200);
  digitalWrite(motor_brake_p, LOW);
  delayMicroseconds(100);
}

void loop() {
  
  /*
  const unsigned long micros_per_step = (625*32) / 4; // 32 steps per cycle, 50 Hz (why 4?)
  enable_stepper_ctl(true);
  for(int i=0;i<(250*32);++i) {
    digitalWrite(step_p, HIGH);
    delayMicroseconds(2);
    digitalWrite(step_p, LOW);
    delayMicroseconds(micros_per_step - 10);
  }
  enable_stepper_ctl(false);
  delay(1000);
  */
  
  unbrake_motor();
  
  enable_stepper_ctl(true);
  const unsigned long micros_per_cycle = 20000;
  const unsigned long half_cycle_micros = micros_per_cycle / 2;
  const unsigned long start_micros = micros();
  while((micros() - start_micros) < 2000000) {
    digitalWrite(step_p, HIGH);
    delayMicroseconds(half_cycle_micros);
    digitalWrite(step_p, LOW);
    delayMicroseconds(half_cycle_micros);
  }
  enable_stepper_ctl(false);
  
  delay(2000);
}

/*
  const unsigned long micros_per_cycle = 20000;
  const unsigned long winding_offset_micros = micros_per_cycle / 4;  // 90 degree phase shift
  const unsigned long half_cycle_micros = micros_per_cycle / 2;
  const unsigned long start_micros = micros();
  
  boolean last_high_side_drive[2] = {false, false};
  while((micros() - start_micros) < 2000000) {
    const unsigned first_winding_micros = ((micros() + 0) - start_micros) % micros_per_cycle;
    const unsigned second_winding_micros = ((micros() + winding_offset_micros) - start_micros) % micros_per_cycle;
    const boolean high_side_drive[2] = {first_winding_micros < half_cycle_micros,
                                         second_winding_micros < half_cycle_micros};
    for(int i=0;i<2;++i) {
      // Dead time
      if(high_side_drive[i] != last_high_side_drive[i]) {
        digitalWrite(driver_pins[i][0], LOW);
        digitalWrite(driver_pins[i][1], LOW);
        delayMicroseconds(2);
        last_high_side_drive[i] = high_side_drive[i];
      }
      digitalWrite(driver_pins[i][0], high_side_drive[i] ? HIGH : LOW);
      digitalWrite(driver_pins[i][1], high_side_drive[i] ? LOW : HIGH);
    }
  }
*/
