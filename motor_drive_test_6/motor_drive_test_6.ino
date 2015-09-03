
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

void setup() {
  for(int i=2;i<=9;++i) {
    pinMode(i, OUTPUT);
  }
  
  // All switches off, so that the outputs go to neutral
  for(int i=0;i<2;++i) {
    go_dead(i);
  }
}

const int winding_quarters[2][4] = {
  {1,1,0,0},
  {0,1,1,0},
};

int quarter_before(int q) {
  if(q == 0)
    return 3;
  return q-1;
}

void loop() {
  const unsigned long cycle_micros = 20000;
  const unsigned long quarter_micros = cycle_micros / 4;
  
  static int quarter = 0;

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
