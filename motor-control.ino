#define MLIN1 17
#define MLIN2 18
#define MRIN1 19
#define MRIN2 23

int pwmR = 255;  // PWM value for the right motor
int pwmL = 255;  // PWM value for the left motor

void setup() {
  // Set up the motor control pins and PWM settings
  ledcAttachPin(MLIN2, 0);  // Attach MLIN2 pin to LEDC channel 0
  ledcAttachPin(MRIN1, 1);  // Attach MRIN1 pin to LEDC channel 1
  ledcSetup(0, 50, 8);      // Set up LEDC channel 0 with a frequency of 50Hz and 8-bit resolution
  ledcSetup(1, 50, 8);      // Set up LEDC channel 1 with a frequency of 50Hz and 8-bit resolution
  pinMode(MLIN1, OUTPUT);   // Set MLIN1 pin as OUTPUT
  pinMode(MRIN2, OUTPUT);   // Set MRIN2 pin as OUTPUT
}

void loop() {
  // Control the motors
  digitalWrite(MLIN1, LOW);    // Set MLIN1 pin LOW (one direction)
  digitalWrite(MRIN2, LOW);    // Set MRIN2 pin LOW (opposite direction)
  ledcWrite(1, pwmR);          // Set the PWM value for the right motor
  ledcWrite(0, pwmL);          // Set the PWM value for the left motor
}
