
const unsigned int solenoid = 11;
const unsigned int motors_enable = 7;
const unsigned int motor_a_dir = 5;
const unsigned int motor_a_stp = 6;
const unsigned int motor_b_dir = 3;
const unsigned int motor_b_stp = 4;

void setPwmFrequency(int pin, int divisor);

void setup() {
  Serial.begin (9600);
  
  pinMode(solenoid, OUTPUT);
  setPwmFrequency(solenoid, 1);
  pinMode(motors_enable, OUTPUT);
  pinMode(motors_enable, OUTPUT);
  pinMode(motor_a_dir, OUTPUT);
  pinMode(motor_a_stp, OUTPUT);
  pinMode(motor_b_dir, OUTPUT);
  pinMode(motor_b_stp, OUTPUT);  

  // Default values
  digitalWrite(motor_a_dir, LOW);
  digitalWrite(motor_a_stp, LOW);
  digitalWrite(motor_b_dir, LOW);
  digitalWrite(motor_b_stp, LOW);

  
  // Turn off solenoid
  digitalWrite(solenoid, LOW);
  // Enable motor controllers
  digitalWrite(motors_enable, LOW);
}

void loop() {
  
  static boolean s_dir = false;
  
  s_dir = !s_dir;
  
  digitalWrite(motor_a_dir, s_dir ? HIGH : LOW);
  digitalWrite(motor_b_dir, s_dir ? HIGH : LOW);
  /*
  for(int i=0;i<40;++i) {
    digitalWrite(motor_a_stp, HIGH);
    delayMicroseconds(2);
    digitalWrite(motor_a_stp, LOW);
    delay(2);
  }
  
  
  for(int i=0;i<40;++i) {
    digitalWrite(motor_b_stp, HIGH);
    delayMicroseconds(2);
    digitalWrite(motor_b_stp, LOW);
    delay(2);
  }
  */

  for(int i=0;i<5;++i) {
//    analogWrite(solenoid, 200);
    digitalWrite(solenoid, HIGH);
    delay(50);
    analogWrite(solenoid, 32);
    delay(200);
    digitalWrite(solenoid, LOW);
    delay(300);
  }
 
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
