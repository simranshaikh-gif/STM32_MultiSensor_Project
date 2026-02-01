# Temperature Sensor

## Description
This project interfaces the STM32 with a temperature sensor to monitor environmental conditions.

## Hardware
- **Microcontroller**: STM32 Series
- **Sensor**: Common options include DHT11, DHT22, LM35, or DS18B20.

## Protocol
- **MLX90614**: I2C Protocol (SMBus compatible).

## Pin Configuration
| Signal | Pin | Description |
| :--- | :--- | :--- |
| **SCL** | **I2C3 (PA8)**| Clock Line |
| **SDA** | **I2C3 (PC9)**| Data Line |
| **UART TX** | **PA2** | Serial Output |
| **UART RX** | **PA3** | Serial Input |

## Setup & Usage
1. Check the code to identify the specific sensor driver used.
2. Connect the sensor ensuring correct VCC (3.3V vs 5V) and pull-up resistors if needed.
3. View the temperature readings via Serial Monitor or a connected display.
