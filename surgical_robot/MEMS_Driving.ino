// Parameters
const int drive_distance = 100;   // cm
const int motor_power = 100;      // 0-255
const int motor_offset = 5;       // Diff. when driving straight
const int wheel_d = 64;           // Wheel diameter (mm)
const float wheel_c = PI * wheel_d; // Wheel circumference (mm)
const int counts_per_rev = 384;   // (4 pairs N-S) * (48:1 gearbox) * (2 falling/rising edges) = 384

// Pins
const int d1_enc_l_pin = 19;          // Motor A
const int d1_enc_r_pin = 21;          // Motor B
const int d1_pwma_pin = 10;
const int d1_ain1_pin = 22;
const int d1_ain2_pin = 23;
const int d1_pwmb_pin = 11;
const int d1_bin1_pin = 24;
const int d1_bin2_pin = 25;
const int d1_stby_pin = 8;

const int d2_enc_l_pin = 3;          // Motor C
const int d2_enc_r_pin = 54;          // Motor D
const int d2_pwma_pin = 12;
const int d2_ain1_pin = 30;
const int d2_ain2_pin = 31;
const int d2_pwmb_pin = 13;
const int d2_bin1_pin = 50;
const int d2_bin2_pin = 51;
const int d2_stby_pin = 9;

// Globals
volatile unsigned long d1_enc_l = 0;
volatile unsigned long d1_enc_r = 0;
volatile unsigned long d2_enc_l = 0;
volatile unsigned long d2_enc_r = 0;

void setup() {

  // Debug
  Serial.begin(9600);

  // Set up pins
  pinMode(d1_enc_l_pin, INPUT_PULLUP);
  pinMode(d1_enc_r_pin, INPUT_PULLUP);
  pinMode(d1_pwma_pin, OUTPUT);
  pinMode(d1_ain1_pin, OUTPUT);
  pinMode(d1_ain2_pin, OUTPUT);
  pinMode(d1_pwmb_pin, OUTPUT);
  pinMode(d1_bin1_pin, OUTPUT);
  pinMode(d1_bin2_pin, OUTPUT);
  pinMode(d1_stby_pin, OUTPUT);

  pinMode(d2_enc_l_pin, INPUT_PULLUP);
  pinMode(d2_enc_r_pin, INPUT_PULLUP);
  pinMode(d2_pwma_pin, OUTPUT);
  pinMode(d2_ain1_pin, OUTPUT);
  pinMode(d2_ain2_pin, OUTPUT);
  pinMode(d2_pwmb_pin, OUTPUT);
  pinMode(d2_bin1_pin, OUTPUT);
  pinMode(d2_bin2_pin, OUTPUT);
  pinMode(d2_stby_pin, OUTPUT);

  // Set up interrupts
  attachInterrupt(digitalPinToInterrupt(d1_enc_l_pin), countLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(d1_enc_r_pin), countRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(d2_enc_l_pin), countLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(d2_enc_r_pin), countRight, CHANGE);

  // Drive straight
  delay(1000);
  enableMotors(true);
  driveStraight(drive_distance, motor_power);
}

void loop() {
  // Do nothing
}

void driveStraight(float dist, int power) {

  unsigned long d1_num_ticks_l;
  unsigned long d1_num_ticks_r;
  unsigned long d2_num_ticks_l;
  unsigned long d2_num_ticks_r;

  // Set initial motor power
  int power_l = motor_power;
  int power_r = motor_power;

  // Used to determine which way to turn to adjust
  unsigned long d1_diff_l;
  unsigned long d1_diff_r;
  unsigned long d2_diff_l;
  unsigned long d2_diff_r;

  // Reset encoder counts
  d1_enc_l = 0;
  d1_enc_r = 0;
  d2_enc_l = 0;
  d2_enc_r = 0;
 
  // Remember previous encoder counts
  unsigned long d1_enc_l_prev = d1_enc_l;
  unsigned long d1_enc_r_prev = d1_enc_r;
  unsigned long d2_enc_l_prev = d2_enc_l;
  unsigned long d2_enc_r_prev = d2_enc_r;

  // Calculate target number of ticks
  float num_rev = (dist * 10) / wheel_c;  // Convert to mm
  unsigned long target_count = num_rev * counts_per_rev;
 
  // Debug
  Serial.print("Driving for ");
  Serial.print(dist);
  Serial.print(" cm (");
  Serial.print(target_count);
  Serial.print(" ticks) at ");
  Serial.print(power);
  Serial.println(" motor power");

  // Drive until one of the encoders reaches desired count
  while ( (d1_enc_l < target_count) && (d1_enc_r < target_count)&& (d2_enc_l < target_count) ) {

    // Sample number of encoder ticks
    d1_num_ticks_l = d1_enc_l;
    d1_num_ticks_r = d1_enc_r;
    d2_num_ticks_l = d2_enc_l;
    d2_num_ticks_r = d2_enc_r;

    // Print out current number of ticks


    // Drive
    drive(power_l, power_r);

    // Number of ticks counted since last time
    d1_diff_l = d1_num_ticks_l - d1_enc_l_prev;
    d1_diff_r = d1_num_ticks_r - d1_enc_r_prev;
    d2_diff_l = d2_num_ticks_l - d2_enc_l_prev;
    d2_diff_r = d2_num_ticks_r - d2_enc_r_prev;

    // Store current tick counter for next time
    d1_enc_l_prev = d1_num_ticks_l;
    d1_enc_r_prev = d1_num_ticks_r;
    d2_enc_l_prev = d2_num_ticks_l;
    d2_enc_r_prev = d2_num_ticks_r;

    // If left is faster, slow it down and speed up right
    if ( (d1_diff_l && d1_diff_r) < d2_diff_r ) {
      power_l -= motor_offset;
      power_r += motor_offset;
    }

    // If right is faster, slow it down and speed up left
    if ( (d1_diff_l && d1_diff_r) < d2_diff_l ) {
      power_l += motor_offset;
      power_r -= motor_offset;
    }

    // Brief pause to let motors respond
    delay(20);
  }

  // Brake
  brake();
}

void enableMotors(boolean en) {
  if ( en ) {
    digitalWrite(d1_stby_pin, HIGH);
    digitalWrite(d2_stby_pin, HIGH);
  } else {
    digitalWrite(d1_stby_pin, LOW);
    digitalWrite(d2_stby_pin, LOW);
  }
}

void drive(int power_a, int power_b) {

  // Constrain power to between -255 and 255
  power_a = constrain(power_a, -255, 255);
  power_b = constrain(power_b, -255, 255);

  // Left motor direction
  if ( power_a < 0 ) {
    digitalWrite(d1_ain1_pin, LOW);
    digitalWrite(d1_ain2_pin, HIGH);
  } else {
    digitalWrite(d1_ain1_pin, HIGH);
    digitalWrite(d1_ain2_pin, LOW);
  }

  // Right motor direction
  if ( power_b < 0 ) {
    digitalWrite(d1_bin1_pin, LOW);
    digitalWrite(d1_bin2_pin, HIGH);
  } else {
    digitalWrite(d1_bin1_pin, HIGH);
    digitalWrite(d1_bin2_pin, LOW);
  }

  // Set speed
  analogWrite(d1_pwma_pin, abs(power_a));
  analogWrite(d1_pwmb_pin, abs(power_b));
}

void brake() {
  digitalWrite(d1_ain1_pin, LOW);
  digitalWrite(d1_ain2_pin, LOW);
  digitalWrite(d1_bin1_pin, LOW);
  digitalWrite(d1_bin2_pin, LOW);
  analogWrite(d1_pwma_pin, 0);
  analogWrite(d1_pwmb_pin, 0);
  digitalWrite(d2_bin1_pin, LOW);
  digitalWrite(d2_bin2_pin, LOW);
  analogWrite(d2_pwma_pin, 0);
}

void countLeft() {
  d1_enc_l++;
}

void countRight() {
  d1_enc_r++;
}
