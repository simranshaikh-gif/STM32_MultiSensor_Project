# Relay Control

## Description
This project demonstrates how to control a relay module using the STM32. Relays allow the low-voltage microcontroller to switch high-voltage or high-current loads (like lights, motors, or fans).

## Hardware
- **Microcontroller**: STM32 Series
- **Component**: 5V/12V Relay Module
- **Circuit**: Usually requires a driver circuit (transistor/optocoupler) if not using a pre-made module.

## Pin Configuration
| Component | STM32 Pin | Logic |
| :--- | :--- | :--- |
| **RELAY 1** | **PD2** | Active LOW |
| **RELAY 2** | **PB12** | Active LOW |

## Setup & Usage
1. Connect the Relay Signal pin to the designated STM32 GPIO.
2. Provide VCC and GND to the relay module.
3. Run the code to toggle the relay state.
