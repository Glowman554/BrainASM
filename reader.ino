void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  for (int i = 0; i < 8; i++) {
    pinMode(i + 2, INPUT);
  }
  pinMode(10, INPUT);

  Serial.println("resetting...");

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (!digitalRead(10))
    ;
  uint8_t in = 0;
  for (int i = 0; i < 8; i++) {
    in |= digitalRead(i + 2) << (i);
  }
  Serial.print((char) in);
  while (digitalRead(10))
    ;
}
