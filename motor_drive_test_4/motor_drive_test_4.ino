

const int winding_quarters[2][4] = {
  {1,1,0,0},
  {0,1,1,0},
//  {0,0,0,0},
};

const unsigned int high_side[2][2] = {{9,7}, {5,3}};
const unsigned int low_side[2][2] = {{8,6}, {4,2}};

const unsigned int voltage_in = A3;
const unsigned int current_low_freq = A2;
const unsigned int current_high_freq = A1;

const unsigned long fix_micros() {
//  return micros() / 10;
  return micros();
}

#define micros do_not_use

void setup() {
  Serial.begin(9600);                                                                                    
  Serial.println("---------> Rebooted");
  
  pinMode(voltage_in, INPUT);
  pinMode(current_low_freq, INPUT);
  pinMode(current_high_freq, INPUT);
  
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
//      setPwmFrequency(high_side[i][j], 8);
      pinMode(high_side[i][j], OUTPUT);
      digitalWrite(high_side[i][j], LOW);
      pinMode(low_side[i][j], OUTPUT);
      digitalWrite(low_side[i][j], LOW);
    }
  }
}

int quarter_before(int q) {
  if(q == 0)
    return 3;
  return q-1;
}

const float voltage_read() {
  const int voltage_raw = analogRead(voltage_in);
  static const float fudge = 34.0f/6.35;
  // Account for voltage divider
  return float(voltage_raw) * fudge * (1.0f / 10.24f);    // 100 / 1024 (divider / 10 bits)
}

const float current_read(boolean high_freq) {
  const int shunt_voltage_raw = analogRead(high_freq ? current_high_freq : current_low_freq);
  // inst amp gain, divider, 10 bits
  // (1/1000) * 100 * (1/1024)
  const float shunt_voltage = float(shunt_voltage_raw) * (1.0f/10240.0f);
  static const float shunt_resistance = 0.1f;
  static const float fudge = 1.0f;
  // Account for voltage divider
  return fudge * (shunt_voltage / shunt_resistance);
}



#if 0
void loop() {
  // TODO: Dead time!
  const unsigned long mss = fix_micros();
  while(true) {
    for(int i=0;i<2;++i) {
      for(int j=0;j<2;++j) {
        digitalWrite(high_side[i][j], HIGH);
        digitalWrite(low_side[i][j], LOW);
      }
    }
    delay(10);
    for(int i=0;i<2;++i) {
      for(int j=0;j<2;++j) {
        digitalWrite(high_side[i][j], LOW);
        digitalWrite(low_side[i][j], LOW);
      }
    }
    for(int i=0;i<2;++i) {
      for(int j=0;j<2;++j) {
        digitalWrite(high_side[i][j], LOW);
        digitalWrite(low_side[i][j], HIGH);
      }
    }
   delay(10);
   for(int i=0;i<2;++i) {
      for(int j=0;j<2;++j) {
        digitalWrite(high_side[i][j], LOW);
        digitalWrite(low_side[i][j], LOW);
      }
    }
  }
}
#else

void loop() {
  //const unsigned long dead_time_micros = 1;
/*
  static boolean next_speed = false;
  unsigned long cycle_micros = (next_speed = !next_speed) ? 20000 : 15000;
  */
  /*
  // TODO: Print voltages
  // TODO: Take out these statements
  Serial.println("---- Instrument readings");
  Serial.print("Voltage: ");
  Serial.println(voltage_read());
  Serial.print("Current (high freq): ");
  Serial.println(current_read(true));
  Serial.print("Current (low freq): ");
  Serial.println(current_read(false));
  Serial.println();
  */
  
  unsigned long cycle_micros = 20000;
  /*
  while(true) {
      digitalWrite(high_side[0][0], LOW);
      digitalWrite(high_side[0][1], LOW);
      digitalWrite(high_side[1][0], LOW);
      digitalWrite(high_side[1][1], LOW);
      delay(30);
      digitalWrite(high_side[0][0], HIGH);
      digitalWrite(high_side[0][1], HIGH);
      digitalWrite(high_side[1][0], HIGH);
      digitalWrite(high_side[1][1], HIGH);
      delay(30);
    
  }
*/
//  for(unsigned long start_millis = millis();(millis() - start_millis) < 200;) {
  for(;;) {
    unsigned long quarter_cycle_micros = cycle_micros / 4;

    for(int quarter=0;quarter<4;++quarter) {

      
      for(int winding=0;winding<2;++winding) {
        if(winding_quarters[winding][quarter] !=
           winding_quarters[winding][quarter_before(quarter)]) {
          if(winding_quarters[winding][quarter]) {
            digitalWrite(high_side[winding][1], LOW);
            delayMicroseconds(20);
            digitalWrite(low_side[winding][0], LOW);
            delayMicroseconds(20);
            
            digitalWrite(low_side[winding][1], HIGH);
            digitalWrite(high_side[winding][0], HIGH);
          } else {
            digitalWrite(high_side[winding][0], LOW);
            delayMicroseconds(20);
            digitalWrite(low_side[winding][1], LOW);
            delayMicroseconds(20);
            
            digitalWrite(low_side[winding][0], HIGH);
            digitalWrite(high_side[winding][1], HIGH);
          }
        }
      }
      delayMicroseconds(quarter_cycle_micros/* - dead_time_micros*/);
    }
  }
}
#endif
