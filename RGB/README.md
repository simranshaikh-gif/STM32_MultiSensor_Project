# RGB LED Control

## Description
This module is dedicated to controlling RGB LEDs. It likely uses Pulse Width Modulation (PWM) to adjust the intensity of the Red, Green, and Blue channels independently, allowing for color mixing.

## Hardware
- **Microcontroller**: STM32 Series
- **Component**: Common Anode or Common Cathode RGB LED

## Pin Configuration
| Channel | Pin | Description |
| :--- | :--- | :--- |
| **Red** | **PC0** | Control Pin |
| **Green** | **PC2** | Control Pin |
| **Blue** | **PC3** | Control Pin |

## Setup & Usage
1. Verify if the LED is Common Anode (active low) or Common Cathode (active high).
2. Ensure timer channels are configured for PWM generation.
3. Adjust duty cycles in the code to change colors.
