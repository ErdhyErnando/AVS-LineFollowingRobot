#define MLIN1 17
#define MLIN2 18
#define MRIN1 19
#define MRIN2 23

int s0 = 15;
int s1 = 4;
int s2 = 16;
int s3 = 5;
int SIG_pin = 39;
int multiplier1 = -8;
int multiplier2 = 1;

int isZero;
int resultantPrev;
int j = 0;

float I;
float D;

int resultant;

float errorPrev;

float pwmRbase = 60;  // Right Motor Power Speed
float pwmLbase = 60;  // Left Motor Power Speed

float speedL = 0;
float speedR = 0;

float addSpeed;

float Kp = 4.0;  // Proportional Gain
float Ki = 0.7;  // Integral Gain   
float Kd = 0.9;  // Derivative Gain   

void setup() {
  // Set up the pin modes and initial states
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(SIG_pin, INPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  ledcAttachPin(MLIN2, 0);  // Attach MLIN2 pin to LEDC channel 0
  ledcAttachPin(MRIN1, 1);  // Attach MRIN1 pin to LEDC channel 1
  ledcSetup(0, 50, 8);      // Set up LEDC channel 0 with a frequency of 50Hz and 8-bit resolution
  ledcSetup(1, 50, 8);      // Set up LEDC channel 1 with a frequency of 50Hz and 8-bit resolution
  pinMode(MLIN1, OUTPUT);   // Set MLIN1 pin as OUTPUT
  pinMode(MRIN2, OUTPUT);   // Set MRIN2 pin as OUTPUT

  Serial.begin(115200);  // Initialize Serial communication
}

void loop() {
  // Main code that runs repeatedly
  for (int i = 0; i < 8; i++) {
    Serial.print("Value at channel ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.println(readMux(i));  // Read and print the value at the current channel
    if (readMux(i) == 0) {
      isZero = isZero + 1;
    }
    resultant = resultant + multiplier1 * readMux(i);  // Calculate the resultant
    multiplier1++;
    delay(10);
  }

  for (int i = 8; i < 16; i++) {
    Serial.print("Value at channel ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.println(readMux(i));  // Read and print the value at the current channel
    if (readMux(i) == 0) {
      isZero = isZero + 1;
    }
    resultant = resultant + multiplier2 * readMux(i);  // Calculate the resultant
    multiplier2++;
    delay(10);
  }
  
  if (isZero == 16) {
    resultant = resultantPrev;
  }
  
  Serial.print("Resultant = ");
  Serial.println(resultant);  // Print the resultant value
  
  multiplier1 = -8;
  multiplier2 = 1;
  
  Serial.println("-----------------------------------------------");

  float error = (float)0 - resultant;
  I = I + error;
  D = error - errorPrev;

  if (I > 100 || I < -100) {
    I = 0;
  }

  addSpeed = (float)Kp * resultant + Kd * D; // Ki * I; (remove comment to add Integral Gain)

  errorPrev = error;

  if (addSpeed > 190) {
    addSpeed = 190;
  }
  speedL = (float)pwmLbase - addSpeed;
  speedR = (float)pwmLbase + addSpeed;

  if (speedL < 40) {
    speedL = 40;
  }
  if (speedR < 40) {
    speedR = 40;
  }

  Serial.println(speedL);
  Serial.println(speedR);

  // Control the motors
  digitalWrite(MLIN1, LOW);    // Set MLIN1 pin LOW (one direction)
  digitalWrite(MRIN2, LOW);    // Set MRIN2 pin LOW (opposite direction)
  ledcWrite(1, speedL);        // Set the PWM value for the right motor
  ledcWrite(0, speedR);        // Set the PWM value for the left motor

  delay(10);
  resultantPrev = resultant;
  resultant = 0;
  isZero = 0;
}

int readMux(int channel) {
  int controlPin[] = {s0, s1, s2, s3};
  int muxChannel[16][4] = {
    {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0},
    {0, 0, 1, 0}, {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0},
    {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1},
    {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}
  };

  // Loop through the 4 control pins
  for (int i = 0; i < 4; i++) {
    digitalWrite(controlPin[i], muxChannel[channel][i]);  // Set the control pins
  }

  int val = digitalRead(SIG_pin);  // Read the value at the SIG pin
  return val;  // Return the value
}

