#define ENCRA 32
#define ENCRB 34
#define ENCLA 33
#define ENCLB 35

int counter1;  // Counter for right wheel degrees
int counter2;  // Counter for left wheel degrees

// Interrupt service routine for right wheel A
void IRAM_ATTR isrRA() {
  if (digitalRead(ENCRB))
    counter1++;  // Increment counter if ENCRB is HIGH
  else
    counter1--;  // Decrement counter if ENCRB is LOW
}

// Interrupt service routine for right wheel B
void IRAM_ATTR isrRB() {
  if (digitalRead(ENCRA))
    counter1--;  // Decrement counter if ENCRA is HIGH
  else
    counter1++;  // Increment counter if ENCRA is LOW
}

// Interrupt service routine for left wheel A
void IRAM_ATTR isrLA() {
  if (digitalRead(ENCLB))
    counter2--;  // Decrement counter if ENCLB is HIGH
  else
    counter2++;  // Increment counter if ENCLB is LOW
}

// Interrupt service routine for left wheel B
void IRAM_ATTR isrLB() {
  if (digitalRead(ENCLA))
    counter2++;  // Increment counter if ENCLA is HIGH
  else
    counter2--;  // Decrement counter if ENCLA is LOW
}

void setup() {
  // Set up the interrupts and pin modes
  attachInterrupt(ENCRA, isrRA, FALLING);  // Attach isrRA function to ENCRA pin interrupt (falling edge)
  attachInterrupt(ENCRB, isrRB, FALLING);  // Attach isrRB function to ENCRB pin interrupt (falling edge)
  attachInterrupt(ENCLA, isrLA, FALLING);  // Attach isrLA function to ENCLA pin interrupt (falling edge)
  attachInterrupt(ENCLB, isrLB, FALLING);  // Attach isrLB function to ENCLB pin interrupt (falling edge)
  pinMode(ENCRA, INPUT);                   // Set ENCRA pin as INPUT
  pinMode(ENCLA, INPUT);                   // Set ENCLA pin as INPUT
  pinMode(ENCRB, INPUT);                   // Set ENCRB pin as INPUT
  pinMode(ENCLB, INPUT);                   // Set ENCLB pin as INPUT
  Serial.begin(115200);                    // Initialize Serial communication
}

void loop() {
  // Print the counter values for both wheels (degrees) and delay
  Serial.println(counter1);
  Serial.println(counter2);
  delay(500);
}
