# LED Display Control

## Description
**Note:** While the folder is named `LED_DISPLAY`, the source code implements a **TCS3200 Color Sensor** interface. It reads RGB frequency values and outputs them via UART.

## Hardware
- **Microcontroller**: STM32F401RE
- **Sensor**: TCS3200 Color Sensor

## Pin Configuration
| Signal | Pin | Description |
| :--- | :--- | :--- |
| **S0** | **PC14** | Frequency Scaling Select 0 |
| **S1** | **PC15** | Frequency Scaling Select 1 |
| **S2** | **PA11** | Photodiode Type Select 2 |
| **S3** | **PA12** | Photodiode Type Select 3 |
| **OE** | **PB7** | Output Enable (Active Low) |
| **OUT** | **PB4** | Frequency Output (Read by Timer/Interrupt) |

## Setup & Usage
1. Inspect the code to understand the display multiplexing or scanning method used.
2. Verify pin connections against the code definitions.
3. Compile and Run.
