#include <WiFi.h>
#include <PicoMQTT.h>
#include <ArduinoJson.h>
#include <cred.h>

// Make a cred.h file defining the SSID and PASS string macros locally and add to .gitignore
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
    mqtt.publish(mqtt_topic, "{\"window\":{\"start\":1762794759,\"end\":1762794760,\"seconds\":1,\"samples\":11},\"temperature_C\":{\"mean\":30.001115453992462,\"min\":29.946763221075685,\"max\":30.084778361129167},\"pH\":{\"mean\":5.105667377472655,\"min\":5.009077017190612,\"max\":5.18615934437436},\"rpm\":{\"mean\":1000.9577900173852,\"min\":973.4490901597198,\"max\":1020.7836436420446},\"actuators_avg\":{\"heater_pwm\":0.4657603368098872,\"motor_pwm\":0.8494784722549338,\"acid_pwm\":0.004858224462864328,\"base_pwm\":0.0},\"dosing_l\":{\"acid\":6.426360615537362e-05,\"base\":5.513404796702242e-06},\"heater_energy_Wh\":0.018600174251273166,\"photoevents\":35,\"setpoints\":{\"temperature_C\":30.0,\"pH\":5.0,\"rpm\":1000.0},\"faults\":{\"last_active\":[],\"counts\":{}}}", 1);

    Serial.println("Test message sent!");
    
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
    delay(1000);
}