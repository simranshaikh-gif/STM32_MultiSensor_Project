# STM32 MultiSensor Project

Welcome to the **STM32 MultiSensor Project**. This repository contains a collection of STM32-based projects focusing on interfacing with various sensors, displays, and communication modules.

## Project Structure

This directory is organized into several sub-projects, each dedicated to a specific component or functionality.

| Module Name | Description |
| :--- | :--- |
| **[7SEG_DWIN_DISPLAY](./7SEG_DWIN_DISPLAY)** | Interfacing STM32 with a DWIN display to control/monitor 7-segment UI elements. |
| **[Buzzer](./Buzzer)** | Basic control code for driving a buzzer (active/passive) for audio feedback. |
| **[LED_DISPLAY](./LED_DISPLAY)** | Logic for controlling LED displays (matrix or 7-segment) directly or via drivers. |
| **[Led_check](./Led_check)** | Simple firmware to verify LED functionality and GPIO configurations. |
| **[Push_buttons](./Push_buttons)** | Implementation of button input reading, debouncing, and interrupt handling. |
| **[RELAY](./RELAY)** | Control logic for relays to switch high-voltage or high-current loads. |
| **[RGB](./RGB)** | Drivers for controlling RGB LEDs, including color mixing and PWM usage. |
| **[Rotation_Sensor](./Rotation_Sensor)** | Code for reading inputs from rotary encoders or similar rotation sensors. |
| **[STM_TO_ESP_DASHBOARD](./STM_TO_ESP_DASHBOARD)** | UART communication bridge between STM32 and ESP32 for web-based dashboards. |
| **[Snake_Game_TFT](./Snake_Game_TFT)** | A complete implementation of the Snake game on a TFT display. |
| **[TOUCHSENSOR](./TOUCHSENSOR)** | Drivers and logic for handling capacitive or resistive touch sensors. |
| **[Temperature_sensor](./Temperature_sensor)** | Interfacing with temperature sensors (e.g., DHT11, LM35) to read and process data. |

## Getting Started

Each folder contains its own source code and project files (typically for STM32CubeIDE or Keil). Please refer to the specific subdirectory for detailed implementation details.

## Hardware Requirements

- **Microcontroller**: STM32 Series (Specific board depends on the sub-project, commonly Nucleo or Discovery boards).
- **Peripherals**: As listed in the module names (Sensors, Displays, Relays, etc.).
