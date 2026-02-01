# Led_check

## Description
A simple diagnostic project designed to verify that LEDs and GPIO configurations are working correctly on the STM32 board. Useful for "Hello World" style testing and hardware verification.

## Hardware
- **Microcontroller**: STM32 Series
- **Component**: On-board User LED or External LEDs

## Pin Configuration
| LED | Pin |
| :--- | :--- |
| **LED 1** | **PA1** |
| **LED 2** | **PB0** |
| **LED 3** | **PB10** |
| **LED 4** | **PC0** |
| **LED 5** | **PC1** |
| **LED 6** | **PC2** |
| **LED 7** | **PC3** |

## Functionality
- Toggles an LED at a set interval (Blinky).
- Specific patterns to test multiple GPIOs.

## Setup & Usage
1. Open the project.
2. Identify the port and pin defined for the LED.
3. Run the code to confirm the LED blinks as expected.
