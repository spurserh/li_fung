
const unsigned int high_gate_pullup[2][2] = {
  {12,2}, {6,9},
};

const unsigned int high_gate_pulldown[2][2] = {
  {11,3}, {5,8},
};

const unsigned int low_gate[2][2] = {
  {13,10}, {4,7},
};


void setup() {
  
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      pinMode(high_gate_pullup[i][j], OUTPUT);
      pinMode(high_gate_pulldown[i][j], OUTPUT);
      pinMode(low_gate[i][j], OUTPUT);
      digitalWrite(high_gate_pullup[i][j], LOW);
      digitalWrite(high_gate_pulldown[i][j], LOW);
      digitalWrite(low_gate[i][j], HIGH);
    }
  }
}

void switch_gates(int which_low) {
  for(int i=0;i<2;++i) {
    if(i==which_low) {
      digitalWrite(high_gate_pullup[0][i], LOW);
      digitalWrite(high_gate_pulldown[0][i], HIGH);
    } else {
      digitalWrite(low_gate[0][i], HIGH);   // OFF
    }
  }
  delayMicroseconds(250);

  for(int i=0;i<2;++i) {
    if(i==which_low) {
      digitalWrite(low_gate[0][i], LOW);    // ON
    } else {
      digitalWrite(high_gate_pulldown[0][i], LOW);
      digitalWrite(high_gate_pullup[0][i], HIGH);
    }
  }
  delayMicroseconds(250);
}

void switch_off() {
  for(int i=0;i<2;++i) {
    digitalWrite(high_gate_pullup[0][i], LOW);
    digitalWrite(high_gate_pulldown[0][i], HIGH);
    digitalWrite(low_gate[0][i], LOW);   // ON
  }
  delayMicroseconds(250);
}

#if 1
void loop() {
  for(unsigned long start_ml = millis();(millis()-start_ml) < 2000;) { 
    switch_gates(0);
    delay(10);
    switch_gates(1);
    delay(10);
  }
  //switch_off();
  //delay(1000);
}
#else
void loop()
{
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      // High side switch needs to be turned on to get the process moving
      digitalWrite(high_gate_pullup[i][j], LOW);
      digitalWrite(high_gate_pulldown[i][j], HIGH);
    }
  }
  delayMicroseconds(200);
  
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      digitalWrite(low_gate[i][j], LOW);
    }
  }
  delay(10);
  
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      digitalWrite(low_gate[i][j], HIGH);
    }
  }
  delayMicroseconds(50);
  
  for(int i=0;i<2;++i) {
    for(int j=0;j<2;++j) {
      digitalWrite(high_gate_pullup[i][j], HIGH);
      digitalWrite(high_gate_pulldown[i][j], LOW);
    }
  }
  delay(10);
}
#endif

