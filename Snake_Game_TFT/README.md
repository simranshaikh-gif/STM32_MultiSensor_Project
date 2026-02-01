# Snake Game (TFT)

## Description
A classic Snake game implementation running on the STM32, rendered on a TFT LCD screen.

## Hardware
- **Microcontroller**: STM32 Series
- **Display**: TFT LCD (e.g., ST7735, ILI9341)
- **Controls**: Push Buttons (Up, Down, Left, Right)

## Features
- Real-time graphics rendering.
- Collision detection (walls and self).
- Score tracking.

## Pin Configuration
| Component | STM32 Pin | Description |
| :--- | :--- | :--- |
| **TFT CS** | **PB6** | Chip Select |
| **TFT RES** | **PC7** | Reset |
| **TFT DC** | **PA9** | Data/Command |
| **TFT SPI** | **SPI1** | CLK/MOSI (Standard PA5/PA7) |
| **Btn UP** | **PC6** | Active High |
| **Btn DOWN** | **PC5** | Active High |
| **Btn LEFT** | **PB8** | Active High |
| **Btn RIGHT** | **PB9** | Active High |
| **Buzzer** | **PA4** | Sound Output |
| **Potentiometer**| **PA0** | Analog Input (Speed/Seed?) |

## Setup & Usage
1. Connect the TFT display via SPI.
2. Connect navigation buttons to the configured GPIOs.
3. Run the project to play.
