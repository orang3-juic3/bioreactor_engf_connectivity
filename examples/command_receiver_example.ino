/*
 * Example: Command and Control Receiver
 * 
 * This example demonstrates how to receive and process commands
 * via UART communication from another Arduino.
 * 
 * Upload this to the RECEIVER Arduino
 */

// UART Configuration
#define UART_BAUD_RATE 9600
#define MAX_MESSAGE_LENGTH 64
#define START_MARKER '<'
#define END_MARKER '>'

// Buffer for incoming UART data
char receivedChars[MAX_MESSAGE_LENGTH];
boolean newData = false;

// System state
boolean systemRunning = false;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Initialize UART communication with another Arduino
  Serial1.begin(UART_BAUD_RATE);
  
  Serial.println("Command Receiver Initialized");
  Serial.println("Waiting for commands...");
}

void loop() {
  // Receive commands from another Arduino
  receiveData();
  
  // Process received commands
  if (newData) {
    processReceivedData();
    newData = false;
  }
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
 * Process received commands
 */
void processReceivedData() {
  Serial.print("Received: ");
  Serial.println(receivedChars);
  
  // Parse and execute commands
  if (strcmp(receivedChars, "CMD:START") == 0) {
    systemRunning = true;
    Serial.println("System STARTED");
    sendData("ACK:STARTED");
  }
  else if (strcmp(receivedChars, "CMD:STOP") == 0) {
    systemRunning = false;
    Serial.println("System STOPPED");
    sendData("ACK:STOPPED");
  }
  else if (strcmp(receivedChars, "CMD:STATUS") == 0) {
    if (systemRunning) {
      sendData("STATUS:RUNNING");
    } else {
      sendData("STATUS:STOPPED");
    }
  }
  else if (strncmp(receivedChars, "TEMP:", 5) == 0) {
    // Received sensor data
    Serial.print("Sensor data received: ");
    Serial.println(receivedChars);
    sendData("ACK:DATA_RECEIVED");
  }
  else {
    // Unknown command
    sendData("ERROR:UNKNOWN_CMD");
  }
}
