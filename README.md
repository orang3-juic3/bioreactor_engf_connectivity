# Bioreactor ENGF Connectivity

Arduino-based communication system for bioreactor monitoring and control.

## Features

- **UART Communication**: Arduino-to-Arduino communication via hardware UART
- **Message Protocol**: Robust framing with start/end markers
- **Non-blocking**: Efficient state machine-based reception
- **Debug Support**: Serial monitor output for troubleshooting

## Quick Start

1. Upload `bioreactor_connectivity.ino` to your Arduino boards
2. Wire the boards together (see documentation)
3. Open Serial Monitor at 9600 baud to view communication

## Documentation

For detailed information about UART communication, wiring, and usage examples, see:
- [UART Communication Guide](UART_COMMUNICATION.md)

## Hardware Requirements

- 2 Arduino boards with hardware UART support (e.g., Mega, Due, Leonardo)
- 3 jumper wires
- USB cables for programming and debugging

## License

This project is part of the ENGF bioreactor connectivity initiative.
