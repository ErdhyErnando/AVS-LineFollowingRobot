#define ENCRA 32
#define ENCRB 34
#define ENCLA 33
#define ENCLB 35
#define MLIN1 17
#define MLIN2 18
#define MRIN1 19
#define MRIN2 23

int counter1;
int counter2;
int pwmR = 150;
int pwmL = 150;
double prevTime;
double currentTime;
int dt;
int prevDt;
int dTeta1;
int dTeta2;
int prevCounter1;
int prevCounter2;
float rate1;
float rate2;

void IRAM_ATTR isrRA() {
  if (digitalRead(ENCRB))
    counter1++;
  else
    counter1--;
}

void IRAM_ATTR isrRB() {
  if (digitalRead(ENCRA))
    counter1--;
  else
    counter1++;
}

void IRAM_ATTR isrLA() {
  if (digitalRead(ENCLB))
    counter2--;
  else
    counter2++;
}

void IRAM_ATTR isrLB() {
  if (digitalRead(ENCLA))
    counter2++;
  else
    counter2--;
}

void setup() {
  // Set up interrupts for wheel encoders
  attachInterrupt(ENCRA, isrRA, FALLING);
  attachInterrupt(ENCRB, isrRB, FALLING);
  attachInterrupt(ENCLA, isrLA, FALLING);
  attachInterrupt(ENCLB, isrLB, FALLING);
  pinMode(ENCRA, INPUT);
  pinMode(ENCLA, INPUT);
  pinMode(ENCRB, INPUT);
  pinMode(ENCLB, INPUT);

  // Set up motor control pins
  ledcAttachPin(MLIN2, 0);
  ledcAttachPin(MRIN1, 1);
  ledcSetup(0, 50, 8);
  ledcSetup(1, 50, 8);
  pinMode(MLIN1, OUTPUT);
  pinMode(MRIN2, OUTPUT);

  Serial.begin(115200);  // Initialize Serial communication
}

void loop() {
  // Main code that runs repeatedly
  digitalWrite(MLIN2, LOW);
  digitalWrite(MRIN2, LOW);
  ledcWrite(1, pwmR);
  ledcWrite(0, pwmL);

  currentTime = millis();
  dt = (int)currentTime - prevTime;

  if (dt == 0) {
    dt = prevDt;
  }
  prevDt = dt;

  dTeta1 = counter1 - prevCounter1;
  prevCounter1 = counter1;
  rate1 = (float)dTeta1 / dt;

  dTeta2 = counter2 - prevCounter2;
  prevCounter2 = counter2;
  rate2 = (float)dTeta2 / dt;

  prevTime = currentTime;

  // Print values to Serial monitor
  Serial.print("rate1 = ");
  Serial.println(rate1);  // Degree per second for motor R
  Serial.print("rate2 = ");
  Serial.println(rate2);  // Degree per second for motor L
  Serial.print("dTeta1 = ");
  Serial.println(dTeta1); // Degree change for R wheel (current - previous)
  Serial.print("dTeta2 = ");
  Serial.println(dTeta2); // Degree change for L wheel (current - previous)
  Serial.print("Counter1 = ");
  Serial.println(counter1);  // Total degrees for R wheel
  Serial.print("Counter2 = ");
  Serial.println(counter2);  // Total degrees for L wheel
  Serial.println("----------------------------");

  delay(200);
}
