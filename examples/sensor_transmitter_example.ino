/*
 * Example: Temperature Sensor Data Transmission
 * 
 * This example demonstrates how to use the UART communication
 * to send simulated temperature sensor data between two Arduinos.
 * 
 * Upload this to the SENDER Arduino
 */

// UART Configuration
#define UART_BAUD_RATE 9600
#define MAX_MESSAGE_LENGTH 64
#define START_MARKER '<'
#define END_MARKER '>'

// Buffer for incoming UART data
char receivedChars[MAX_MESSAGE_LENGTH];
boolean newData = false;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize UART communication with another Arduino
  Serial1.begin(UART_BAUD_RATE);
  
  Serial.println("Temperature Sensor Transmitter Initialized");
}

void loop() {
  // Simulate temperature reading
  float temperature = 20.0 + random(0, 100) / 10.0; // Random temp between 20-30°C
  float humidity = 40.0 + random(0, 200) / 10.0;    // Random humidity between 40-60%
  
  // Format sensor data message
  char sensorData[MAX_MESSAGE_LENGTH];
  snprintf(sensorData, MAX_MESSAGE_LENGTH, "TEMP:%.1f,HUM:%.1f", temperature, humidity);
  
  // Send sensor data
  sendData(sensorData);
  
  // Check for acknowledgments
  receiveData();
  if (newData) {
    processReceivedData();
    newData = false;
  }
  
  // Send data every 2 seconds
  delay(2000);
}

/*
 * Receive data from UART with start/end markers
 */
void receiveData() {
  static boolean receiveInProgress = false;
  static byte index = 0;
  char receivedChar;
  
  while (Serial1.available() > 0 && newData == false) {
    receivedChar = Serial1.read();
    
    if (receiveInProgress == true) {
      if (receivedChar != END_MARKER) {
        receivedChars[index] = receivedChar;
        index++;
        if (index >= MAX_MESSAGE_LENGTH) {
          index = MAX_MESSAGE_LENGTH - 1;
        }
      }
      else {
        receivedChars[index] = '\0';
        receiveInProgress = false;
        index = 0;
        newData = true;
      }
    }
    else if (receivedChar == START_MARKER) {
      receiveInProgress = true;
    }
  }
}

/*
 * Send data via UART with start/end markers
 */
void sendData(const char* message) {
  Serial1.print(START_MARKER);
  Serial1.print(message);
  Serial1.print(END_MARKER);
  
  Serial.print("Sent: ");
  Serial.println(message);
}

/*
 * Process received acknowledgments
 */
void processReceivedData() {
  Serial.print("Received ACK: ");
  Serial.println(receivedChars);
}
