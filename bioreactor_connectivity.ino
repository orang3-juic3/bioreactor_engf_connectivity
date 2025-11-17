/*
 * Bioreactor Connectivity
 * Arduino sketch for bioreactor monitoring and control
 * 
 * UART Communication Protocol:
 * - Uses Serial1 for Arduino-to-Arduino communication
 * - Message format: <START>DATA<END>
 * - START marker: '<'
 * - END marker: '>'
 * - Maximum message length: 64 bytes
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
  Serial1.begin(UART_BAUD_RATE, SERIAL_8N1, 16 , 17);
  
  Serial.println("Bioreactor UART Communication Initialized");
  Serial.println("Waiting for data...");
}

void loop() {
  // Receive data from another Arduino
  receiveData();
  
  // Process received data
  if (newData) {
    processReceivedData();
    newData = false;
  }
  
  // Example: Send data periodically (every 5 seconds)
  static unsigned long lastSendTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= 5000) {
    sendData("HEARTBEAT");
    lastSendTime = currentTime;
  }
}

/*
 * Receive data from UART with start/end markers
 * Data format: <message>
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
        receivedChars[index] = '\0'; // Terminate string
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
  
  // Debug output
  Serial.print("Sent: ");
  Serial.println(message);
}

/*
 * Process received data
 * Override this function to implement custom message handling
 */
void processReceivedData() {
  Serial.print("Received: ");
  Serial.println(receivedChars);
  
  // Add custom message processing here
  // Example: Parse commands, sensor data, etc.
  
  // Echo back received data (for testing)
  String response = "ACK:";
  response += receivedChars;
  sendData(response.c_str());
}
