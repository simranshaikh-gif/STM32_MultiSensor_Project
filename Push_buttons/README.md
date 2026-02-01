# Push Buttons

## Description
This module checks the state of push buttons connected to the GPIO pins of the STM32. It typically includes logic for:
- Reading digital inputs.
- Software debouncing to prevent false triggers.
- External Interrupt (EXTI) configuration for event-driven response.

## Hardware
- **Microcontroller**: STM32 Series
- **Component**: Tacitile Push Buttons (Active High or Active Low configuration)

## Pin Configuration
| Signal | STM32 Pin | Description |
| :--- | :--- | :--- |
| **UP** | **PC6** | Active HIGH (Pulled Down) |
| **DOWN** | **PC5** | Active HIGH (Pulled Down) |
| **LEFT** | **PB8** | Active HIGH (Pulled Down) |
| **RIGHT** | **PB9** | Active HIGH (Pulled Down) |
| **Output** | **PA4** | Feedback Signal |

## Setup & Usage
1. Configure the GPIO pins as Input (Pull-up or Pull-down depending on circuit).
2. If using intercepts, enable the NVIC settings for the corresponding line.
3. Check `main.c` or interrupt service routines (ISR) for button logic.
