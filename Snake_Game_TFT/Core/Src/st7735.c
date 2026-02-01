#include "st7735.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;

#define ST7735_SPI_PORT hspi1

#define ST7735_DEL_TIME 10

#define ST7735_CMD_SWRESET 0x01
#define ST7735_CMD_SLPIN 0x10
#define ST7735_CMD_SLPOUT 0x11
#define ST7735_CMD_PTLON 0x12
#define ST7735_CMD_NORON 0x13
#define ST7735_CMD_INVOFF 0x20
#define ST7735_CMD_INVON 0x21
#define ST7735_CMD_GAMSET 0x26
#define ST7735_CMD_DISPOFF 0x28
#define ST7735_CMD_DISPON 0x29
#define ST7735_CMD_CASET 0x2A
#define ST7735_CMD_RASET 0x2B
#define ST7735_CMD_RAMWR 0x2C
#define ST7735_CMD_RAMRD 0x2E
#define ST7735_CMD_PTLAR 0x30
#define ST7735_CMD_COLMOD 0x3A
#define ST7735_CMD_MADCTL 0x36
#define ST7735_CMD_FRMCTR1 0xB1
#define ST7735_CMD_FRMCTR2 0xB2
#define ST7735_CMD_FRMCTR3 0xB3
#define ST7735_CMD_INVCTR 0xB4
#define ST7735_CMD_PWCTR1 0xC0
#define ST7735_CMD_PWCTR2 0xC1
#define ST7735_CMD_PWCTR3 0xC2
#define ST7735_CMD_PWCTR4 0xC3
#define ST7735_CMD_PWCTR5 0xC4
#define ST7735_CMD_VMCTR1 0xC5
#define ST7735_CMD_RDID1 0xDA
#define ST7735_CMD_RDID2 0xDB
#define ST7735_CMD_RDID3 0xDC
#define ST7735_CMD_GMCTRP1 0xE0
#define ST7735_CMD_GMCTRN1 0xE1

static void ST7735_WriteCommand(uint8_t cmd) {
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void ST7735_WriteData(uint8_t *buff, size_t buff_size) {
  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);
  HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size, HAL_MAX_DELAY);
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1,
                                    uint8_t y1) {
  // column address set
  ST7735_WriteCommand(ST7735_CMD_CASET);
  uint8_t data[] = {0x00, x0, 0x00, x1};
  ST7735_WriteData(data, sizeof(data));

  // row address set
  ST7735_WriteCommand(ST7735_CMD_RASET);
  data[1] = y0;
  data[3] = y1;
  ST7735_WriteData(data, sizeof(data));

  // write to RAM
  ST7735_WriteCommand(ST7735_CMD_RAMWR);
}

void ST7735_Init(void) {
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_RESET);
  HAL_Delay(ST7735_DEL_TIME);
  HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_SET);
  HAL_Delay(ST7735_DEL_TIME);

  ST7735_WriteCommand(ST7735_CMD_SWRESET);
  HAL_Delay(150);
  ST7735_WriteCommand(ST7735_CMD_SLPOUT);
  HAL_Delay(500);

  // FRMCTR1
  ST7735_WriteCommand(ST7735_CMD_FRMCTR1);
  uint8_t data1[] = {0x01, 0x2C, 0x2D};
  ST7735_WriteData(data1, sizeof(data1));

  // FRMCTR2
  ST7735_WriteCommand(ST7735_CMD_FRMCTR2);
  ST7735_WriteData(data1, sizeof(data1));

  // FRMCTR3
  ST7735_WriteCommand(ST7735_CMD_FRMCTR3);
  uint8_t data2[] = {0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D};
  ST7735_WriteData(data2, sizeof(data2));

  // INVCTR
  ST7735_WriteCommand(ST7735_CMD_INVCTR);
  uint8_t data3[] = {0x07};
  ST7735_WriteData(data3, sizeof(data3));

  // PWCTR1
  ST7735_WriteCommand(ST7735_CMD_PWCTR1);
  uint8_t data4[] = {0xA2, 0x02, 0x84};
  ST7735_WriteData(data4, sizeof(data4));

  // PWCTR2
  ST7735_WriteCommand(ST7735_CMD_PWCTR2);
  uint8_t data5[] = {0xC5};
  ST7735_WriteData(data5, sizeof(data5));

  // PWCTR3
  ST7735_WriteCommand(ST7735_CMD_PWCTR3);
  uint8_t data6[] = {0x0A, 0x00};
  ST7735_WriteData(data6, sizeof(data6));

  // PWCTR4
  ST7735_WriteCommand(ST7735_CMD_PWCTR4);
  uint8_t data7[] = {0x8A, 0x2A};
  ST7735_WriteData(data7, sizeof(data7));

  // PWCTR5
  ST7735_WriteCommand(ST7735_CMD_PWCTR5);
  uint8_t data8[] = {0x8A, 0xEE};
  ST7735_WriteData(data8, sizeof(data8));

  // VMCTR1
  ST7735_WriteCommand(ST7735_CMD_VMCTR1);
  uint8_t data9[] = {0x0E};
  ST7735_WriteData(data9, sizeof(data9));

  // INVOFF
  ST7735_WriteCommand(ST7735_CMD_INVOFF);

  // MADCTL
  ST7735_WriteCommand(ST7735_CMD_MADCTL);
  uint8_t data10[] = {0xA8}; // Landscape (MY=1, MV=1, BGR=1)
  ST7735_WriteData(data10, sizeof(data10));

  // COLMOD
  ST7735_WriteCommand(ST7735_CMD_COLMOD);
  uint8_t data11[] = {0x05};
  ST7735_WriteData(data11, sizeof(data11));

  // CASET
  ST7735_WriteCommand(ST7735_CMD_CASET);
  uint8_t data12[] = {0x00, 0x00, 0x00, 0x7F};
  ST7735_WriteData(data12, sizeof(data12));

  // RASET
  ST7735_WriteCommand(ST7735_CMD_RASET);
  uint8_t data13[] = {0x00, 0x00, 0x00, 0x9F};
  ST7735_WriteData(data13, sizeof(data13));

  // GMCTRP1
  ST7735_WriteCommand(ST7735_CMD_GMCTRP1);
  uint8_t data14[] = {0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d,
                      0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10};
  ST7735_WriteData(data14, sizeof(data14));

  // GMCTRN1
  ST7735_WriteCommand(ST7735_CMD_GMCTRN1);
  uint8_t data15[] = {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
                      0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10};
  ST7735_WriteData(data15, sizeof(data15));

  // NORON
  ST7735_WriteCommand(ST7735_CMD_NORON);
  HAL_Delay(10);

  // DISPON
  ST7735_WriteCommand(ST7735_CMD_DISPON);
  HAL_Delay(100);

  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
  if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
    return;

  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

  ST7735_SetAddressWindow(x, y, x + 1, y + 1);
  uint8_t data[] = {color >> 8, color & 0xFF};
  ST7735_WriteData(data, sizeof(data));

  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font,
                        uint16_t color, uint16_t bgcolor) {
  uint16_t i, j;
  while (*str) {
    if (x + font.width >= ST7735_WIDTH) {
      x = 0;
      y += font.height;
      if (y + font.height >= ST7735_HEIGHT) {
        break;
      }

      if (*str == ' ') {
        // skip spaces in the beginning of the new line
        str++;
        continue;
      }
    }

    for (i = 0; i < font.height; i++) {
      uint32_t b = font.data[(str[0] - 32) * font.height + i];
      for (j = 0; j < font.width; j++) {
        if ((b << j) & 0x8000) {
          ST7735_DrawPixel(x + j, y + i, color);
        } else {
          ST7735_DrawPixel(x + j, y + i, bgcolor);
        }
      }
    }
    x += font.width;
    str++;
  }
}

void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint16_t color) {
  if ((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
    return;
  if ((x + w) > ST7735_WIDTH)
    w = ST7735_WIDTH - x;
  if ((y + h) > ST7735_HEIGHT)
    h = ST7735_HEIGHT - y;

  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
  ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);

  // Optimize: Use a buffer for the line data to reduce SPI overhead
  // Max width is 128, so 256 bytes max.
  static uint8_t line_buffer[256];
  for (int i = 0; i < w; i++) {
    line_buffer[2 * i] = color >> 8;
    line_buffer[2 * i + 1] = color & 0xFF;
  }

  for (int j = 0; j < h; j++) {
    HAL_SPI_Transmit(&ST7735_SPI_PORT, line_buffer, w * 2, HAL_MAX_DELAY);
  }

  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void ST7735_FillScreen(uint16_t color) {
  ST7735_FillRectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}
