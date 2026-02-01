# STM32 7-Segment Display & DWIN Control

This project controls a 4-digit 7-segment display based on values received from a DWIN DGUS display via UART.

## Connections

### STM32 <-> TM1637 7-Segment
- **CLK**: PA8
- **DIO**: PC9
- **VCC**: 3.3V or 5V
- **GND**: GND

### STM32 <-> DWIN Display
- **STM32 TX (PA9)** -> DWIN RX
- **STM32 RX (PA10)** -> DWIN TX
- **GND** -> GND

### Serial Monitor (Debug)
- **PA2 (TX)** / **PA3 (RX)** -> ST-LINK USB (Serial Monitor)
- Baud Rate: 115200

## Hardware Setup
1. Flash the STM32 code from the `7SEG` folder.
2. Copy the `DWIN_SET` folder from `ui_disp2` to the DWIN SD card.
3. Insert SD card into DWIN display and power on (blue screen update).
4. Remove SD card and cycle power.
5. Connect wires as above.
6. Slider on DWIN display should update the 7-segment display (Value 1-10).

## Code Logic
- `main.c` listens for DWIN UART packets (`5A A5 ...`).
- Parses 0x82 (Write Variable) or 0x83 (Return Key) commands.
- Updates TM1637 display.
