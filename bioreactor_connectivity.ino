#include <WiFi.h>
#include <PicoMQTT.h>
#include <ArduinoJson.h>
#include <cred.h>

// WiFi credentials
const char* ssid = SSID;
const char* password = PASS;

// MQTT Broker settings
const char* mqtt_broker = "broker.hivemq.com";  // Change to your MQTT broker
const int mqtt_port = 1883;
const char* mqtt_topic = "orang3-juic3nano-esp32/telemetry/data";   // Topic to publish to

// Create MQTT client
PicoMQTT::Client mqtt(mqtt_broker, mqtt_port);

// Buffer for serial data
String serialBuffer = "";

void setup() {
    // Initialize Serial (USB) for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting Arduino Nano ESP32 Serial to MQTT");
    
  // rxPin = 17 (A3), txPin = 16 (A2)
    Serial1.begin(115200, SERIAL_8N1, 19, 20);
    delay(100);
    Serial.println("Started (Serial1 RX=19, TX=20)");
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Set MQTT client ID (optional)
    mqtt.client_id = "nano-esp32-serial";
    
    // Connect to MQTT broker
    Serial. print("Connecting to MQTT broker.. .");
    mqtt.begin();
    Serial.println("MQTT client started");
}

void loop() {
    // Keep MQTT connection alive
    mqtt.loop();
    
    // Check if data is available on Serial1
    while (Serial1.available() > 0) {
        char inChar = Serial1.read();
        
        // Check for newline character (end of string)
        if (inChar == '\n' || inChar == '\r') {
            if (serialBuffer.length() > 0) {
                // Publish the received string to MQTT
                Serial.print("Publishing to MQTT: ");
                Serial.println(serialBuffer);
                
                mqtt.publish(mqtt_topic, serialBuffer);
                
                // Clear the buffer for the next string
                serialBuffer = "";
            }
        } else {
            // Add character to buffer
            serialBuffer += inChar;
        }
    }
    
    // Small delay to prevent overwhelming the system
    delay(10);
}