
#include "Vec2f.h"
#include "Path.h"

const unsigned int solenoid = 11;
const unsigned int motors_enable = 7;
const unsigned int motor_left_dir = 5;
const unsigned int motor_left_stp = 6;
const unsigned int motor_right_dir = 3;
const unsigned int motor_right_stp = 4;

// All units are meters
// Horizontal axis is x, increasing to the right
// Vertical axis is y, increasing downward

// TODO: Make these settable
const Vec2f right_motor_offset(1, 0);
const float step_length = 3.0f / 1000.0f;
const float steps_per_second = 50.0f;
const Extrema2i step_bounds(Vec2i(-500, -500), Vec2i(500, 500));

Vec2i current_position_steps(0,0);

Vec2f StepsToMeters(Vec2i const&steps) {
  return Vec2f(steps.x, steps.y) * step_length;
}

Vec2i MetersToSteps(Vec2f const&steps) {
  return Vec2i(steps.x / step_length, steps.y / step_length);
}

// TODO: Actually do this
Vec2f CartesianToMotor(Vec2f const&pos) {
  return pos;
}

// TODO: Actually do this
Vec2f MotorToCartesian(Vec2f const&pos) {
  return pos;
}

void Reset() {
  current_position_steps = Vec2i(0,0);
  // Turn off solenoid
  digitalWrite(solenoid, LOW);
  // Enable motor controllers
  digitalWrite(motors_enable, LOW);
}

void setPwmFrequency(int pin, int divisor);

void setup() {
  Serial.begin (9600);
  
  pinMode(solenoid, OUTPUT);
  setPwmFrequency(solenoid, 1);
  pinMode(motors_enable, OUTPUT);
  pinMode(motors_enable, OUTPUT);
  pinMode(motor_left_dir, OUTPUT);
  pinMode(motor_left_stp, OUTPUT);
  pinMode(motor_right_dir, OUTPUT);
  pinMode(motor_right_stp, OUTPUT);  

  // Default values
  digitalWrite(motor_left_dir, LOW);
  digitalWrite(motor_left_stp, LOW);
  digitalWrite(motor_right_dir, LOW);
  digitalWrite(motor_right_stp, LOW);
  
  Reset();
}

char readByteBlocking()
{
  while(!Serial.available())
    delay(1);
  char b = Serial.read();
  return b;
}

// TODO: Improve this to find the LAST four in a row!
void seekDelimiterBlocking()
{
  for(int n_found=0;n_found < 4;) {
    unsigned char bb = (unsigned char)readByteBlocking();
    if(bb == 0xFF)
      ++n_found;
    else
      n_found = 0;
  }
}

short readShortBlocking()
{
  const short bytes[2] = {
    (short)readByteBlocking(),
    (short)readByteBlocking(),
  };
  return bytes[0] | (bytes[1] << 8);
}

long readLongBlocking()
{
  const long bytes[4] = {
    (long)readByteBlocking(),
    (long)readByteBlocking(),
    (long)readByteBlocking(),
    (long)readByteBlocking(),
  };
  return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
}

Vec2f readPositionBlocking() {
  // Units are 1/2^16 of a meter
  const long x = readLongBlocking();
  const long y = readLongBlocking();
  return Vec2f(float(x) / 65536.0f, float(y) / 65536.0f);
}

// Should only be used for small amounts of steps (ie 0 or 1 in each axis)
void DoSteps(Vec2i const&offset)
{  
  if(abs(offset.x) > 1 || abs(offset.y) > 1) {
    Serial.println("Warning: DoSteps() being used for too many steps");
  }
  if(abs(offset.x) == 0 && abs(offset.y) == 0)
    return;
  const Vec2i target = current_position_steps + offset;
  if(!step_bounds.DoesEnclose(target)) {
    Serial.println("Warning: Ignoring command to step out of bounds");
    return;
  }
  /*
  Serial.print("DoSteps: ");
  Serial.print(offset.x);
  Serial.print(", ");
  Serial.println(offset.y);
  */

  // TODO: Directions
  digitalWrite(motor_left_dir, (offset.x < 0) ? HIGH : LOW);
  digitalWrite(motor_right_dir, (offset.y < 0) ? HIGH : LOW);

  if(abs(offset.x) > 0) {
  Serial.println("Step left");
    digitalWrite(motor_left_stp, HIGH);
    delayMicroseconds(2);
    digitalWrite(motor_left_stp, LOW);
  }

  if(abs(offset.y) > 0) {
  Serial.println("Step right");
    digitalWrite(motor_right_stp, HIGH);
    delayMicroseconds(2);
    digitalWrite(motor_right_stp, LOW);
  }

  delayMicroseconds((unsigned long)(1000000.0f / steps_per_second));
  current_position_steps = target;
}

// TODO: Dotted lines?
void MoveAlong(PathI const*path) {
  digitalWrite(motors_enable, LOW);
  // TODO: More intelligent way of breaking down the path
  const float sEpsilon = 0.0001f;
  for(float t=0;t<=1.0f;t=min(1.0f, t+sEpsilon)) {
    const Vec2f target_meters = CartesianToMotor(path->target_position(t));
    const Vec2i target_steps = MetersToSteps(target_meters);
    const Vec2i offset_steps = target_steps - current_position_steps;
    DoSteps(offset_steps);
  }
  digitalWrite(motors_enable, HIGH);
}

const char sSeekCode = 0x00;

void SeekCommand() {
  const Vec2f to_pos = readPositionBlocking();
  LinearPath path(to_pos - MotorToCartesian(StepsToMeters(current_position_steps)));
  MoveAlong(&path);
}
/*
void loop() {
  static int next_seqno_expected = 0;
  
  // Read a command
  seekDelimiterBlocking();
  const unsigned char sequence_no = (unsigned char)readByteBlocking();
  const unsigned char opcode = (unsigned char)readByteBlocking();
  
  if(sequence_no == 0) {
    Reset();
    next_seqno_expected = 0;
  } else if(sequence_no != next_seqno_expected) {
    Serial.print("Aborting unexpected sequence number:");
    Serial.print(sequence_no);
    Serial.print(", expected: ");
    Serial.println(next_seqno_expected);
    return;
  }
  ++next_seqno_expected;
  
  if(opcode == sSeekCode) {
    SeekCommand();
  }
  else {
    Serial.print("Aborting unknown opcode: ");
    Serial.println(opcode);
    return;
  }
}
*/
void loop() {
  static boolean s_dir = false;
  
  s_dir = !s_dir;
  
  digitalWrite(motors_enable, HIGH);
  
  digitalWrite(motor_left_dir, s_dir ? HIGH : LOW);
  digitalWrite(motor_right_dir, s_dir ? HIGH : LOW);
  
  /*
  for(int i=0;i<200;++i) {
    digitalWrite(motor_left_stp, HIGH);
    delayMicroseconds(2);
    digitalWrite(motor_left_stp, LOW);
    delay(4);
  }
  
  for(int i=0;i<200;++i) {
    digitalWrite(motor_right_stp, HIGH);
    delayMicroseconds(2);
    digitalWrite(motor_right_stp, LOW);
    delay(4);
  }
  */

  for(int i=0;i<2;++i) {
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
