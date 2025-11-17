void setup() {
  Serial.begin(115200);
  // rxPin = 17 (A3), txPin = 16 (A2)
  Serial1.begin(115200, SERIAL_8N1, 17, 16);
  delay(100);
  Serial.println("Started (Serial1 RX=17, TX=16)");
}

unsigned long last = 0;
void loop() {
  if (millis() - last >= 1000) {
    last = millis();
    Serial1.println("heartbeat");
    Serial.println("Sent: heartbeat");
  }
  while (Serial1.available()) {
    Serial.write(Serial1.read()); // show incoming data on USB monitor
  }
}