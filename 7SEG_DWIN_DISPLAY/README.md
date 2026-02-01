# 7SEG_DWIN_DISPLAY

## Description
This project interfaces an STM32 microcontroller with a DWIN display to control and monitor 7-segment UI elements. It handles the serial communication protocols required to update values on the DWIN screen.

## Hardware
- **Microcontroller**: STM32F401RE
- **Display**: DWIN Intelligent UART Display + TM1637 7-Segment Display
- **Interface**: UART (for DWIN) and GPIO (for TM1637)

## Pin Configuration
| Component | STM32 Pin | Description |
| :--- | :--- | :--- |
| **TM1637 CLK** | **PA8** | Clock Pin for 7-Segment Driver |
| **TM1637 DIO** | **PC9** | Data Pin for 7-Segment Driver |
| **DWIN TX/RX** | **USART1** | Connected to DWIN Display (PA9/PA10 typically) |
| **Debug UART** | **USART2** | Serial Monitor Output |

## Connection Logic
Typically involves connecting:
- STM32 TX -> DWIN RX
- STM32 RX -> DWIN TX
- GND -> GND
- VCC -> VCC (Ensure correct voltage, often 5V or 12V for display)

## Setup & Usage
1. Open the project in STM32CubeIDE or Keil.
2. Ensure the correct UART peripheral is configured in the `.ioc` file.
3. Flash the code to the STM32.
4. Power up the DWIN display with the appropriate GUI project files loaded.
