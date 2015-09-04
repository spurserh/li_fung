
const unsigned int pullups_positive[2] = {8, 7};
const unsigned int pulldowns_positive[2] = {9, 6};
const unsigned int pullups_negative[2] = {5, 3};
const unsigned int pulldowns_negative[2] = {4, 2};

static char sine_values[256];

void go_dead(int winding) {
  digitalWrite(pullups_positive[winding], LOW);
  digitalWrite(pulldowns_positive[winding], HIGH);
  digitalWrite(pullups_negative[winding], LOW);
  digitalWrite(pulldowns_negative[winding], HIGH);
}

void go_dead_all() {
  for(int i=0;i<2;++i)
    go_dead(i);
}

char sawtooth(unsigned char t) {
  if(t <= 63)
    return t * 2;
  else if(t <= 191)
    return 127 - (t - 64) * 2;
  return -127 + (t - 192) * 2;
}

void setup() {

  Serial.begin(9600);
  Serial.println("------- Rebooted -----");
  
  for(unsigned int i=0;i<256;++i) {
    sine_values[i] = char(127.0f * sin((float(i) / 255.0f) * 2.0f * 3.141592f));
  }
  
  /*
  for(unsigned int i=0;i<256;++i) {
    Serial.print("[");
    Serial.print(i);
    Serial.print("] sine ");
    Serial.print((int)sine_values[i]);
    Serial.print("\tsaw ");
    Serial.println((int)sawtooth(i));
  }
  while(true);
  */
  
  for(int i=2;i<=9;++i) {
    pinMode(i, OUTPUT);
  }
  
  // All switches off, so that the outputs go to neutral
  for(int i=0;i<2;++i) {
    go_dead(i);
  }
}

const int winding_quarters_forward[2][4] = {
  {1,1,0,0},
  {0,1,1,0},
};

const int winding_quarters_reverse[2][4] = {
  {0,1,1,0},
  {1,1,0,0},
};

int quarter_before(int q) {
  if(q == 0)
    return 3;
  return q-1;
}

void run_const_speed(const int winding_quarters[2][4],
                     const unsigned long cycle_micros,
                     const unsigned long for_millis)
{
  const unsigned long quarter_micros = cycle_micros / 4;
  // Need to reset each time to ensure forward/reverse
  int quarter = 0;
  for(unsigned long ml_s = millis();(millis() - ml_s) < for_millis;ml_s) {
    for(int winding=0;winding<2;++winding) {
      if(winding_quarters[winding][quarter] !=
         winding_quarters[winding][quarter_before(quarter)]) {
        go_dead(winding);
        delayMicroseconds(50);
        if(winding_quarters[winding][quarter]) {
          digitalWrite(pullups_negative[winding], LOW);
          digitalWrite(pulldowns_negative[winding], HIGH);
          
          digitalWrite(pullups_positive[winding], HIGH);
          digitalWrite(pulldowns_positive[winding], LOW);
        } else {
          digitalWrite(pullups_positive[winding], LOW);
          digitalWrite(pulldowns_positive[winding], HIGH);
          
          digitalWrite(pullups_negative[winding], HIGH);
          digitalWrite(pulldowns_negative[winding], LOW);
        }
      }
    }
    delayMicroseconds(quarter_micros);
    quarter = (quarter+1) % 4;
  }
  go_dead_all();
}

char output_state(char output_val, char sawtooth_val) {
  if(output_val >= 0)
    return (output_val > sawtooth_val) ? 1 : 0;
  else
    return (output_val < sawtooth_val) ? -1 : 0;
}  

#if 1
void loop() {
//  const unsigned long cycle_micros_min = 12500;
  const unsigned long cycle_micros_min = 10000;
  const unsigned long cycle_micros_max = 22000;
//  const unsigned long cycle_micros_max = 25000;
  
  static unsigned long cycle_micros = 0;
  static int winding_quarters[2][4];
  
  if (Serial.available() > 0) {
    unsigned char incomingByteU = Serial.read();
    char incomingByte = *reinterpret_cast<char*>(&incomingByteU);
    if(incomingByte == -128) {
      cycle_micros = 0;
    } else {
      cycle_micros = (unsigned long)(cycle_micros_min + 
        (1.0f - abs(incomingByte) / 127.0f) * float(cycle_micros_max - cycle_micros_min));
      memcpy(&winding_quarters[0][0], 
             (incomingByte >= 0) ? &winding_quarters_forward[0][0] : &winding_quarters_reverse[0][0],
             sizeof(winding_quarters));
    }
  }
  
  if(cycle_micros != 0) {
    const unsigned long quarter_micros = cycle_micros / 4;
    for(int quarter = 0;quarter<4;++quarter) {
      for(int winding=0;winding<2;++winding) {
        
/*        Serial.print("quarter ");
        Serial.print(quarter);
        Serial.print(" winding ");
        Serial.print(winding);
        Serial.print(": ");
        Serial.println(winding_quarters[winding][quarter]);*/
        
        if(winding_quarters[winding][quarter] !=
           winding_quarters[winding][quarter_before(quarter)]) {
          go_dead(winding);
          delayMicroseconds(50);
          if(winding_quarters[winding][quarter]) {
            digitalWrite(pullups_negative[winding], LOW);
            digitalWrite(pulldowns_negative[winding], HIGH);
            
            digitalWrite(pullups_positive[winding], HIGH);
            digitalWrite(pulldowns_positive[winding], LOW);
          } else {
            digitalWrite(pullups_positive[winding], LOW);
            digitalWrite(pulldowns_positive[winding], HIGH);
            
            digitalWrite(pullups_negative[winding], HIGH);
            digitalWrite(pulldowns_negative[winding], LOW);
          }
        }
      }
      delayMicroseconds(quarter_micros);
    }
  } else {
    go_dead_all();
  }

#if 0
  if (Serial.available() > 0) {
    
    unsigned char incomingByteU = Serial.read();
    char incomingByte = *reinterpret_cast<char*>(&incomingByteU);
    const unsigned long cycle_micros = (unsigned long)(cycle_micros_min + 
      (1.0f - abs(incomingByte) / 127.0f) * float(cycle_micros_max - cycle_micros_min));
      
    run_const_speed((incomingByte >= 0) ? winding_quarters_forward : winding_quarters_reverse,
                    cycle_micros,
                    300);
  }
#endif
}
#endif

#if 0
void loop() {
  const unsigned long st_m = micros();
  
  const unsigned long pwm_cycle_micros = 125;
  const unsigned long output_cycle_micros = 20000;
//  const char sawtooth_val = sawtooth((micros()%pwm_cycle_micros)*2);
  const char sawtooth_val = sawtooth((micros() & 128L) << 1L);
  
//  const char output_val = sine_values[((micros()%output_cycle_micros)*780)/100];
  const char output_val = sine_values[(micros() & 16384L) >> 6L];
  const int this_output_state = output_state(output_val, sawtooth_val);
  static int last_output_state = 0;
  static char last_output_val = 0.0f;
  
  const unsigned long end_m = micros();
  
  Serial.print("calc ");
  Serial.println(end_m - st_m);
  Serial.print("sawtooth ");
  Serial.println((int)sawtooth_val);
  Serial.print("output ");
  Serial.println((int)output_val);
  
  const int winding = 0;
  
  if((last_output_val < 0.0f && output_val >= 0.0f) ||
     (last_output_val >= 0.0f && output_val < 0.0f)) {
    go_dead(winding);
    delayMicroseconds(25);
  }
  
  if(this_output_state != last_output_state) {
    digitalWrite(pullups_positive[winding], LOW);
    digitalWrite(pulldowns_positive[winding], LOW);
    digitalWrite(pullups_negative[winding], LOW);
    digitalWrite(pulldowns_negative[winding], LOW);
    delayMicroseconds(5);
    
    if(this_output_state > 0) {
      digitalWrite(pullups_positive[winding], HIGH);
      digitalWrite(pulldowns_positive[winding], LOW);
    } else if(this_output_state < 0) {
      digitalWrite(pullups_negative[winding], HIGH);
      digitalWrite(pulldowns_negative[winding], LOW);
    } else if(last_output_state > 0) {
      digitalWrite(pullups_positive[winding], LOW);
      digitalWrite(pulldowns_positive[winding], HIGH);
    } else if(last_output_state < 0) {
      digitalWrite(pullups_negative[winding], LOW);
      digitalWrite(pulldowns_negative[winding], HIGH);
    }
  }
  last_output_state = this_output_state;
  last_output_val = output_val;
  
}
#endif

#if 0
// Continuous variable speed test
void loop() {
  const unsigned long cycle_micros_min = 12500;
  const unsigned long cycle_micros_max = 22000;

  const unsigned long test_period_millis = 5000;
  const unsigned long cycle_micros = cycle_micros_min + 
      int((cycle_micros_max - cycle_micros_min) * (0.5f + 0.5f * sin((millis()%test_period_millis) * (6.28f/test_period_millis))));
  const unsigned long quarter_micros = cycle_micros / 4;
  // Need to reset each time to ensure forward/reverse
  static int quarter = 0;
  for(int winding=0;winding<2;++winding) {
    if(winding_quarters_forward[winding][quarter] !=
       winding_quarters_forward[winding][quarter_before(quarter)]) {
      go_dead(winding);
      delayMicroseconds(50);
      if(winding_quarters_forward[winding][quarter]) {
        digitalWrite(pullups_negative[winding], LOW);
        digitalWrite(pulldowns_negative[winding], HIGH);
        
        digitalWrite(pullups_positive[winding], HIGH);
        digitalWrite(pulldowns_positive[winding], LOW);
      } else {
        digitalWrite(pullups_positive[winding], LOW);
        digitalWrite(pulldowns_positive[winding], HIGH);
        
        digitalWrite(pullups_negative[winding], HIGH);
        digitalWrite(pulldowns_negative[winding], LOW);
      }
    }
  }
  delayMicroseconds(quarter_micros);
  quarter = (quarter+1) % 4;
}
#endif

#if 0
// Basic test
void loop() {
  static int test_idx = 0;
  boolean f = (test_idx % 2) == 0;
  const unsigned long cycle_micros = 1000000L / (50 + 10 * ((test_idx)/2));

  Serial.print("Beginning test: ");
  if(f) {
    Serial.print("forward ");
  } else {
    Serial.print("reverse ");
  }
  Serial.print("cycle micros (");
  Serial.print(cycle_micros);
  Serial.println(")");

  run_const_speed(f ? winding_quarters_forward : winding_quarters_reverse,
                  cycle_micros,
                  1500);
  test_idx = (test_idx+1) % 8;
  delay(1000);
}
#endif

#if 0
void loop() {
  run_const_speed(winding_quarters_forward,
                  20000,
                  100000);
}
#endif






