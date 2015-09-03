
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

void loop() {
  const unsigned long cycle_micros = 20000;
  static boolean f = true;
  run_const_speed(f ? winding_quarters_forward : winding_quarters_reverse,
                  cycle_micros,
                  2000);
  f = !f;
  delay(2000);
}
