# Rotation Sensor

## Description
This project interfaces with a rotation sensor, such as a rotary encoder or a potentiometer, to detect angular position or direction of rotation.

## Hardware
- **Microcontroller**: STM32 Series
- **Component**: Rotary Encoder (KY-040 etc.) or Analog Potentiometer

## Pin Configuration
| Signal | Pin | Description |
| :--- | :--- | :--- |
| **Analog In** | **PA0** | ADC Channel 0 (Potentiometer) |
| **Debug RX** | **PA3** | UART2 RX |
| **Debug TX** | **PA2** | UART2 TX |

## Logic
- **Encoder**: Reads Quadrature signals (A and B) to determine direction and step count.
- **Potentiometer**: Uses ADC (Analog-to-Digital Converter) to read voltage varying with rotation.

## Setup & Usage
1. Connect the sensor outputs to the STM32 pins.
2. If using an encoder, check for Timer Encoder Mode configuration.
3. Monitor variables in debug mode to see rotation values change.
