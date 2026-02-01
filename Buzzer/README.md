# Buzzer Control

## Description
This module contains code for controlling a buzzer to provide audio feedback. It supports simple on/off control and potentially PWM for tone generation, depending on implementation.

## Hardware
- **Microcontroller**: STM32 Series
- **Component**: Active or Passive Buzzer

## Logic
- **Active Buzzer**: Simple GPIO High/Low toggle.
- **Passive Buzzer**: Requires a PWM signal to generate sound at specific frequencies.

## Pin Configuration
| Component | STM32 Pin | Description |
| :--- | :--- | :--- |
| **Buzzer** | **PA4** | Active High Control |

## Setup & Usage
1. Open the project.
2. Check `main.c` or relevant driver files to see which GPIO pin is used for the buzzer.
3. Build and upload to test audio output.
