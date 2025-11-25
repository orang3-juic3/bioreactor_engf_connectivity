#include <PicoMQTT.h>
#include <ArduinoJson.h>
PicoMQTT::Client mqtt("broker.hivemq.com");

void setup() {
  Serial.begin(115200);
  // rxPin = 17 (A3), txPin = 16 (A2)
  Serial1.begin(115200, SERIAL_8N1, 19, 20);
  delay(100);
  Serial.println("Started (Serial1 RX=19, TX=20)");
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