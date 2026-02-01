/*
 * tm1637.c
 *
 *  Created on: Jan 29, 2026
 *      Author: Antigravity
 */

#include "tm1637.h"

#define CLK_LOW()                                                              \
  HAL_GPIO_WritePin(TM1637_CLK_PORT, TM1637_CLK_PIN, GPIO_PIN_RESET)
#define CLK_HIGH()                                                             \
  HAL_GPIO_WritePin(TM1637_CLK_PORT, TM1637_CLK_PIN, GPIO_PIN_SET)
#define DIO_LOW()                                                              \
  HAL_GPIO_WritePin(TM1637_DIO_PORT, TM1637_DIO_PIN, GPIO_PIN_RESET)
#define DIO_HIGH()                                                             \
  HAL_GPIO_WritePin(TM1637_DIO_PORT, TM1637_DIO_PIN, GPIO_PIN_SET)

const uint8_t segmentMap[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71  // 8-9, A-F
};

static void TM1637_DelayMicroseconds(uint32_t us) {
  // Simple delay loop
  // Approx for 84MHz (STM32F401)
  // 84 cycles per us roughly
  volatile uint32_t count = us * 20;
  while (count--)
    ;
}

static void TM1637_Start(void) {
  CLK_HIGH();
  DIO_HIGH();
  TM1637_DelayMicroseconds(2);
  DIO_LOW();
  TM1637_DelayMicroseconds(2);
  CLK_LOW();
  TM1637_DelayMicroseconds(2);
}

static void TM1637_Stop(void) {
  CLK_LOW();
  TM1637_DelayMicroseconds(2);
  DIO_LOW();
  TM1637_DelayMicroseconds(2);
  CLK_HIGH();
  TM1637_DelayMicroseconds(2);
  DIO_HIGH();
  TM1637_DelayMicroseconds(2);
}

static uint8_t TM1637_WriteByte(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    CLK_LOW();
    if (data & 0x01)
      DIO_HIGH();
    else
      DIO_LOW();
    TM1637_DelayMicroseconds(3);
    data >>= 1;
    CLK_HIGH();
    TM1637_DelayMicroseconds(3);
  }

  // Ack
  CLK_LOW();
  DIO_HIGH(); // Float DIO to input?
  // We configured DIO as Output PP. To read ACK we technically should switch to
  // Input. However, TM1637 pulls low for ACK. For simplicity with Output PP, we
  // can just set it High (Output) and if TM1637 pulls it low, it might conflict
  // but usually works for simple Write-only. Or we implement direction
  // switching. Let's implement direction switching for correctness.

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = TM1637_DIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TM1637_DIO_PORT, &GPIO_InitStruct);

  TM1637_DelayMicroseconds(2);
  CLK_HIGH();
  TM1637_DelayMicroseconds(2);
  uint8_t ack =
      HAL_GPIO_ReadPin(TM1637_DIO_PORT, TM1637_DIO_PIN) == GPIO_PIN_RESET;
  CLK_LOW();

  // Switch back to Output
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(TM1637_DIO_PORT, &GPIO_InitStruct);

  return ack;
}

void TM1637_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable Clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  // Configure CLK
  GPIO_InitStruct.Pin = TM1637_CLK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TM1637_CLK_PORT, &GPIO_InitStruct);

  // Configure DIO
  GPIO_InitStruct.Pin = TM1637_DIO_PIN;
  HAL_GPIO_Init(TM1637_DIO_PORT, &GPIO_InitStruct);

  // Set Default Low (Idle High handled by Start/Stop logic)
  CLK_HIGH();
  DIO_HIGH();
}

void TM1637_DisplayDecimal(int v, int displaySeparator) {
  uint8_t digits[4];
  if (v > 9999)
    v = 9999;
  digits[3] = v % 10;
  digits[2] = (v / 10) % 10;
  digits[1] = (v / 100) % 10;
  digits[0] = (v / 1000) % 10;

  // Remove leading zeros
  if (v < 1000)
    digits[0] = 0x7f; // Blank? No, 0x00 is segment map. 0x7F is 8 with dot?
  // Special blank handling needed or just 0?
  // Let's just maps 0-9. 127 is usually BLANK in libraries? No.
  // 0x00 is nothing.
  // Let's stick to showing all zeros for now or basic suppression.
  // User wants 1-10. So mostly 1 or 2 digits.

  // Simple implementation:
  // Only last 2 digits matters for 1-10.

  TM1637_Start();
  TM1637_WriteByte(0x40); // Command: Automatic address increment
  TM1637_Stop();

  TM1637_Start();
  TM1637_WriteByte(0xC0); // Command: Set address to 00H

  for (int i = 0; i < 4; i++) {
    uint8_t seg = 0;
    if (i < 2 && v < 100)
      seg = 0; // Blank leading
    else
      seg = segmentMap[digits[i]];

    // Blanking logic refined:
    if (v < 10 && i < 3)
      seg = 0; // Blank first 3
    else if (v < 100 && i < 2)
      seg = 0;
    else if (v < 1000 && i < 1)
      seg = 0;

    // Map digits correctly. 1-10.
    // If v=10, digits=[0,0,1,0]. Blank 0,1. Show 1,0.

    TM1637_WriteByte(seg);
  }
  TM1637_Stop();

  TM1637_Start();
  TM1637_WriteByte(0x88 | 0x07); // Display ON, Brightness Max
  TM1637_Stop();
}

void TM1637_SetBrightness(uint8_t brightness) {
  // 0-7
  TM1637_Start();
  TM1637_WriteByte(0x88 | (brightness & 0x07));
  TM1637_Stop();
}
