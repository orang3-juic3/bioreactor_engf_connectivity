# UART Communication Examples

This directory contains example Arduino sketches demonstrating different use cases for the UART communication system.

## Examples

### 1. sensor_transmitter_example.ino
**Purpose**: Demonstrates sending sensor data from one Arduino to another.

**Features**:
- Simulates temperature and humidity sensor readings
- Formats data as structured messages
- Sends data every 2 seconds
- Receives and displays acknowledgments

**Hardware Setup**:
- Upload to the "sender" Arduino
- Connect to another Arduino running `bioreactor_connectivity.ino` or `command_receiver_example.ino`

**Expected Output** (Serial Monitor):
```
Temperature Sensor Transmitter Initialized
Sent: TEMP:25.3,HUM:52.1
Received ACK: ACK:TEMP:25.3,HUM:52.1
Sent: TEMP:23.7,HUM:48.5
...
```

### 2. command_receiver_example.ino
**Purpose**: Demonstrates receiving and processing commands via UART.

**Features**:
- Receives commands (START, STOP, STATUS)
- Maintains system state
- Sends appropriate acknowledgments
- Processes sensor data messages

**Hardware Setup**:
- Upload to the "receiver" Arduino
- Connect to another Arduino that sends commands

**Supported Commands**:
- `CMD:START` - Start the system
- `CMD:STOP` - Stop the system
- `CMD:STATUS` - Query system status
- `TEMP:xx.x,HUM:xx.x` - Receive sensor data

**Expected Output** (Serial Monitor):
```
Command Receiver Initialized
Waiting for commands...
Received: CMD:START
System STARTED
Sent: ACK:STARTED
Received: TEMP:25.3,HUM:52.1
Sensor data received: TEMP:25.3,HUM:52.1
Sent: ACK:DATA_RECEIVED
```

## Testing the Examples

### Test Setup 1: Sensor Data Transmission
1. Upload `sensor_transmitter_example.ino` to Arduino #1
2. Upload `command_receiver_example.ino` to Arduino #2
3. Wire the Arduinos (TX1 to RX1, RX1 to TX1, GND to GND)
4. Open Serial Monitor for both Arduinos
5. Observe sensor data being transmitted and acknowledged

### Test Setup 2: Bidirectional Communication
1. Modify `sensor_transmitter_example.ino` to also receive commands
2. Upload to Arduino #1
3. Upload `command_receiver_example.ino` to Arduino #2
4. Wire the Arduinos
5. Send commands from Arduino #1 to control Arduino #2

## Wiring Diagram

```
Arduino #1              Arduino #2
----------              ----------
TX1 (Pin 18) ---------> RX1 (Pin 19)
RX1 (Pin 19) <--------- TX1 (Pin 18)
GND --------------------- GND
```

**Note**: Pin numbers vary by board model. Check your Arduino's documentation.

## Customization Tips

1. **Change Data Format**: Modify the `snprintf()` format string in `sensor_transmitter_example.ino`
2. **Add Commands**: Extend the `if-else` chain in `command_receiver_example.ino`
3. **Adjust Timing**: Change the `delay()` value to control transmission rate
4. **Add Error Handling**: Implement timeouts and retransmission logic

## Common Issues

**Problem**: "newData" is always false
- **Solution**: Check wiring, especially TX-to-RX connections

**Problem**: Garbled messages
- **Solution**: Ensure both sketches use the same `UART_BAUD_RATE`

**Problem**: Compilation error "Serial1 not defined"
- **Solution**: Your Arduino board doesn't have hardware UART. Use SoftwareSerial instead or choose a different board (Mega, Due, Leonardo)

## Next Steps

- Combine examples to create bidirectional command and sensor data exchange
- Add real sensor libraries (DHT22, DS18B20, etc.)
- Implement CRC or checksum for data integrity
- Add message queuing for multiple simultaneous messages
- Implement error recovery and retransmission

## See Also

- Main documentation: [UART_COMMUNICATION.md](../UART_COMMUNICATION.md)
- Main sketch: [bioreactor_connectivity.ino](../bioreactor_connectivity.ino)
