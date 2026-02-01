# Touch Sensor Control

## Description
This module handles input from touch sensors. This could refer to simple capacitive touch buttons (like TTP223) or resistive/capacitive touch screens overlaying a display.

## Hardware
- **Microcontroller**: STM32 Series
- **Sensor**: Capacitive Touch Sensor or Touch Panel

## Logic
- **Digital Mode**: Detects touch as a binary High/Low signal.
- **Code Logic**: Reads **PC8**. LOW = Touched, HIGH = Released.

## Pin Configuration
| Signal | Pin | Description |
| :--- | :--- | :--- |
| **Sensor Input** | **PC8** | Digital Input (Active Low) |
| **Status LED** | **PA5** | Indicates Touch State |

## Setup & Usage
1. Verify the sensor type and connection (GPIO vs I2C/SPI).
2. Configure the threshold sensitivity if applicable.
3. Use the output to trigger events (like turning on an LED).
