# STM32 to ESP32 Dashboard

## Description
This project enables communication between an STM32 microcontroller and an ESP32 Wi-Fi module. The goal is to send data (like sensor readings) from the STM32 to the ESP32, which then hosts a web dashboard to visualize this data.

## Hardware
- **STM32 Board**: Collects data from sensors.
- **ESP32 Board**: Acts as a Wi-Fi gateway/Web Server.

## Communication
- **Protocol**: UART (Serial) @ 115200 baud
- **Wiring**:
  - STM32 TX (USART1) -> ESP32 RX
  - STM32 RX (USART1) -> ESP32 TX
  - GND -> GND

## Pin Configuration
| Signal | STM32 Pin | Description |
| :--- | :--- | :--- |
| **Touch Input** | **PC8** | Active Low (Simulated or Real Sensor) |
| **Status LED** | **PA5** | On-board LED (LD2) |
| **ESP32 UART** | **USART1** | Communication with WiFi Module |
| **Debug UART** | **USART2** | PC Serial Monitor |

## Setup & Usage
1. Flash the STM32 code to send formatted data strings over UART.
2. Flash the ESP32 code (often Arduino/IDF based) to parse UART data and serve the webpage.
3. Access the dashboard via the ESP32's IP address.
