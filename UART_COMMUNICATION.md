# UART Communication Guide

## Overview

This Arduino sketch implements UART communication between two Arduino boards for bioreactor connectivity. The implementation uses Serial1 (hardware UART) for inter-Arduino communication while keeping Serial (USB) available for debugging.

## Hardware Requirements

- 2 Arduino boards with hardware UART support (e.g., Arduino Mega, Arduino Due, Arduino Leonardo, etc.)
- 3 jumper wires for connections

## Wiring Diagram

Connect the two Arduino boards as follows:

```
Arduino 1          Arduino 2
---------          ---------
TX1 (Pin 18) ----> RX1 (Pin 19)
RX1 (Pin 19) <---- TX1 (Pin 18)
GND -------------- GND
```

**Important Notes:**
- Pin numbers may vary depending on your Arduino board model
- For Arduino Mega: TX1 is Pin 18, RX1 is Pin 19
- For Arduino Leonardo: TX1 is Pin 1, RX1 is Pin 0
- Always connect GND between the boards
- DO NOT cross 5V connections (can damage boards)

## Communication Protocol

### Message Format
Messages are framed with start and end markers:
```
<MESSAGE_CONTENT>
```
- Start Marker: `<`
- End Marker: `>`
- Maximum Message Length: 64 bytes

### Example Messages
```
<HEARTBEAT>
<SENSOR:TEMP:25.5>
<CMD:START>
<ACK:HEARTBEAT>
```

## How It Works

### Initialization
```cpp
void setup() {
  Serial.begin(9600);    // Debug console (USB)
  Serial1.begin(9600);   // UART communication
}
```

### Sending Data
Use the `sendData()` function:
```cpp
sendData("HELLO");
sendData("SENSOR:TEMP:25.5");
```

### Receiving Data
The `receiveData()` function is called automatically in the loop. When a complete message is received, `processReceivedData()` is called.

### Processing Received Data
Modify the `processReceivedData()` function to implement your custom message handling:
```cpp
void processReceivedData() {
  Serial.print("Received: ");
  Serial.println(receivedChars);
  
  // Add your custom logic here
  if (strcmp(receivedChars, "START") == 0) {
    // Handle START command
  }
  else if (strncmp(receivedChars, "SENSOR:", 7) == 0) {
    // Parse and handle sensor data
  }
}
```

## Features

1. **Non-blocking Reception**: Uses a state machine to receive data without blocking other operations
2. **Message Framing**: Ensures complete messages are received
3. **Buffer Overflow Protection**: Prevents memory corruption from oversized messages
4. **Debug Output**: All sent/received messages are logged to Serial monitor
5. **Automatic ACK**: Received messages are automatically acknowledged
6. **Heartbeat**: Periodic heartbeat messages every 5 seconds

## Testing the Implementation

### Step 1: Upload to Both Arduinos
1. Upload the sketch to the first Arduino
2. Upload the sketch to the second Arduino

### Step 2: Wire the Boards
Connect the boards according to the wiring diagram above

### Step 3: Monitor Serial Output
1. Open Serial Monitor for one of the Arduinos (9600 baud)
2. You should see:
   - "Bioreactor UART Communication Initialized"
   - "Waiting for data..."
   - "Sent: HEARTBEAT" (every 5 seconds)
   - "Received: HEARTBEAT" (from the other Arduino)
   - "Sent: ACK:HEARTBEAT" (automatic acknowledgment)

## Customization

### Change Baud Rate
```cpp
#define UART_BAUD_RATE 115200  // Change from 9600 to 115200
```

### Change Message Markers
```cpp
#define START_MARKER '{'
#define END_MARKER '}'
```

### Change Maximum Message Length
```cpp
#define MAX_MESSAGE_LENGTH 128  // Increase buffer size
```

### Disable Auto-ACK
Comment out or remove the acknowledgment code in `processReceivedData()`:
```cpp
// String response = "ACK:";
// response += receivedChars;
// sendData(response.c_str());
```

### Disable Heartbeat
Comment out the heartbeat code in `loop()`:
```cpp
// static unsigned long lastSendTime = 0;
// unsigned long currentTime = millis();
// if (currentTime - lastSendTime >= 5000) {
//   sendData("HEARTBEAT");
//   lastSendTime = currentTime;
// }
```

## Troubleshooting

### No Communication
1. Check wiring (TX to RX, RX to TX)
2. Verify GND is connected
3. Ensure both Arduinos use the same baud rate
4. Check that your Arduino board has Serial1 support

### Garbled Messages
1. Verify baud rate matches on both sides
2. Check for loose connections
3. Ensure proper grounding

### Messages Not Complete
1. Check MAX_MESSAGE_LENGTH is sufficient
2. Verify start/end markers match on both sides
3. Look for buffer overflow protection triggering

## Example Use Cases

### Sensor Data Transmission
```cpp
void loop() {
  float temperature = readTemperature();
  String data = "TEMP:" + String(temperature);
  sendData(data.c_str());
  delay(1000);
}
```

### Command and Control
```cpp
void processReceivedData() {
  if (strcmp(receivedChars, "CMD:START") == 0) {
    startBioreactor();
    sendData("ACK:STARTED");
  }
  else if (strcmp(receivedChars, "CMD:STOP") == 0) {
    stopBioreactor();
    sendData("ACK:STOPPED");
  }
}
```

### Bidirectional Data Exchange
```cpp
void loop() {
  // Receive and process data
  receiveData();
  if (newData) {
    processReceivedData();
    newData = false;
  }
  
  // Send periodic sensor readings
  if (shouldSendData()) {
    String sensorData = collectSensorData();
    sendData(sensorData.c_str());
  }
}
```

## Additional Resources

- [Arduino Serial Reference](https://www.arduino.cc/reference/en/language/functions/communication/serial/)
- [Arduino Hardware Serial](https://www.arduino.cc/en/Reference/Serial)
- UART Tutorial: Understanding asynchronous serial communication

## License

This code is part of the bioreactor_engf_connectivity project.
