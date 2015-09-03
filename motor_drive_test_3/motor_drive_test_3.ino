

const int winding_quarters[2][4] = {
  {1,1,0,0},
  {0,1,1,0},
};

// 8 -> 10
// 7 -> 11
const unsigned int high_side[2][2] = {{6,10}, {11,9}};
const unsigned int low_side[2][2] = {{2,3}, {4,5}};

const unsigned int voltage_in = A3;
const unsigned int current_low_freq = A2;
const unsigned int current_high_freq = A1;

const unsigned long fix_micros() {
//  return micros() / 10;
  return micros();
}

#define micros do_not_use

void setPwmFrequency(int pin, int divisor);

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
      digitalWrite(high_side[i][j], HIGH);
      pinMode(low_side[i][j], OUTPUT);
      digitalWrite(low_side[i][j], HIGH);
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


void pulseAllPins(const boolean high_side_on[2][2], const boolean low_side_on[2][2], unsigned long ms) {
  const unsigned long cycle_time_micros = 100;
  unsigned long m_start = fix_micros();
  do{
      for(int i=0;i<2;++i) {
        for(int j=0;j<2;++j) {
          digitalWrite(high_side[i][j], high_side_on[i][j] ? HIGH : LOW);
          digitalWrite(low_side[i][j], low_side_on[i][j] ? LOW : HIGH);
        }
      }
     delayMicroseconds(cycle_time_micros);
    
    for(int i=0;i<2;++i) {
      for(int j=0;j<2;++j) {
        // Turn off the low side driver first
        if(!high_side_on[i][j]) {
          digitalWrite(low_side[i][j] , HIGH);
        }
        digitalWrite(high_side[i][j], high_side_on[i][j] ? LOW : HIGH);
      }
    }
  }while((fix_micros() - m_start) < ms);
}


#if 0
void loop() {
  // TODO: Dead time!
  const unsigned long mss = fix_micros();
  while(true) {
    
    const boolean on[2][2] = {
      true, true,
      true, true,
    };
    const boolean off[2][2] = {
      false, false,
      false, false,
    };
    pulseAllPins(on, on, 10000);
    pulseAllPins(off, off, 10000);
  }
}
#else

void loop() {
  const unsigned long dead_time_micros = 1;
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

//  for(unsigned long start_millis = millis();(millis() - start_millis) < 200;) {
  for(;;) {
    unsigned long quarter_cycle_micros = cycle_micros / 4;
    
    for(int quarter=0;quarter<4;++quarter) {
      {
        boolean high_side_on[2][2] = {
          false, false,
          false, false,
        };
        boolean low_side_on[2][2] = {
          false, false,
          false, false,
        };
        pulseAllPins(high_side_on, low_side_on, dead_time_micros);
      }
      
      {
        boolean high_side_on[2][2] = {
          false, false,
          false, false,
        };
        boolean low_side_on[2][2] = {
          false, false,
          false, false,
        };
              
        for(int winding=0;winding<2;++winding) {
          if(winding_quarters[winding][quarter]) {
            low_side_on[winding][1] = true;
            high_side_on[winding][0] = true;
          } else {
            low_side_on[winding][0] = true;
            high_side_on[winding][1] = true;
          }
        }
        pulseAllPins(high_side_on, low_side_on, quarter_cycle_micros - dead_time_micros);
      }
//      delayMicroseconds(quarter_cycle_micros - dead_time_micros);
    }
  }
}
#endif

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
