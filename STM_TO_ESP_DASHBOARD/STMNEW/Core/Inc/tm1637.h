/*
 * tm1637.h
 *
 *  Created on: Jan 29, 2026
 *      Author: Antigravity
 */

#ifndef INC_TM1637_H_
#define INC_TM1637_H_

#include "stm32f4xx_hal.h"

// Define Pins
// CLK -> PA8
// DIO -> PC9
#define TM1637_CLK_PORT GPIOA
#define TM1637_CLK_PIN GPIO_PIN_8
#define TM1637_DIO_PORT GPIOC
#define TM1637_DIO_PIN GPIO_PIN_9

void TM1637_Init(void);
void TM1637_DisplayDecimal(int v, int displaySeparator);
void TM1637_SetBrightness(uint8_t brightness);

#endif /* INC_TM1637_H_ */
