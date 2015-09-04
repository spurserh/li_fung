
const unsigned int pullups_positive[2] = {8, 7};
const unsigned int pulldowns_positive[2] = {9, 6};
const unsigned int pullups_negative[2] = {5, 3};
const unsigned int pulldowns_negative[2] = {4, 2};

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

void setup() {
  for(int i=2;i<=9;++i) {
    pinMode(i, OUTPUT);
  }
  
  // All switches off, so that the outputs go to neutral
  for(int i=0;i<2;++i) {
    go_dead(i);
  }

  Serial.begin(9600);
  Serial.println("------- Rebooted -----");
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

#if 1
void loop() {
  const int winding = 0;  
  const unsigned long output_cycle_micros = 20000;
  const unsigned long pwm_cycle_micros = 250;
  
  static int last_state = 0;
  int this_state = 0;
  
  const float t = float(

  for(unsigned long ml_s = millis();(millis() - ml_s) < 10;ml_s) {
    /*
    digitalWrite(pullups_positive[winding], HIGH);
    digitalWrite(pulldowns_positive[winding], LOW);
    delayMicroseconds(half_pulse_micros);
    digitalWrite(pullups_positive[winding], LOW);
    digitalWrite(pulldowns_positive[winding], LOW);
    delayMicroseconds(5);
    digitalWrite(pullups_positive[winding], LOW);
    digitalWrite(pulldowns_positive[winding], HIGH);
    delayMicroseconds(half_pulse_micros);
    */
  }
  go_dead(winding);
  delayMicroseconds(50);
  /*
  for(unsigned long ml_s = millis();(millis() - ml_s) < 10;ml_s) {
    digitalWrite(pullups_negative[winding], HIGH);
    digitalWrite(pulldowns_negative[winding], LOW);
    delayMicroseconds(half_pulse_micros);
    digitalWrite(pullups_negative[winding], LOW);
    digitalWrite(pulldowns_negative[winding], LOW);
    delayMicroseconds(5);
    digitalWrite(pullups_negative[winding], LOW);
    digitalWrite(pulldowns_negative[winding], HIGH);
    delayMicroseconds(half_pulse_micros);
  }
  */
  
  go_dead(winding);
  delayMicroseconds(50);
  delay(10);
}
#endif

#if 0
// Continuous variable speed test
void loop() {
  const unsigned long cycle_micros_min = 12500;
  const unsigned long cycle_micros_max = 22000;
  const unsigned long cycle_micros = cycle_micros_min + 
      int((cycle_micros_max - cycle_micros_min) * (0.5f + 0.5f * sin((millis()%10000) * (6.28f/10000.0f))));
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

