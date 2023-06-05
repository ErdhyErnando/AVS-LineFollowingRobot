int s0 = 15;
int s1 = 4;
int s2 = 16;
int s3 = 5;
int SIG_pin = 39;
int multiplier1 = -8;
int multiplier2 = 1;
int resultant;

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

  Serial.begin(115200);  // Initialize Serial communication
}

void loop() {
  // Main code that runs repeatedly
  for (int i = 0; i < 8; i++) {
    Serial.print("Value at channel ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.println(readMux(i));  // Read and print the value at the current channel
    resultant = resultant + multiplier1 * readMux(i);  // Calculate the resultant
    multiplier1++;
    delay(10);
  }

  for (int i = 8; i < 16; i++) {
    Serial.print("Value at channel ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.println(readMux(i));  // Read and print the value at the current channel
    resultant = resultant + multiplier2 * readMux(i);  // Calculate the resultant
    multiplier2++;
    delay(10);
  }

  Serial.print("Resultant = ");
  Serial.println(resultant);  // Print the resultant value
  multiplier1 = -8;
  multiplier2 = 1;
  Serial.println("-----------------------------------------------");
  delay(1500);
  resultant = 0;
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
