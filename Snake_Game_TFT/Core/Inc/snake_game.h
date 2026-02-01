#ifndef __SNAKE_GAME_H
#define __SNAKE_GAME_H

#include "main.h"
#include "st7735.h"
#include <stdbool.h>
#include <stdlib.h>


// Game Settings
#define GRID_SIZE 8
#define GRID_WIDTH (ST7735_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (ST7735_HEIGHT / GRID_SIZE)
#define MAX_SNAKE_LENGTH 100

// Colors
#define COLOR_BG ST7735_BLACK
#define COLOR_SNAKE ST7735_GREEN
#define COLOR_APPLE ST7735_RED

// Direction Enums
typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } Direction;

// Snake Structure
typedef struct {
  int8_t x;
  int8_t y;
} Point;

typedef struct {
  Point body[MAX_SNAKE_LENGTH];
  uint8_t length;
  Direction dir;
} Snake;

// Function Prototypes
void Snake_Game_Start(void);

#endif // __SNAKE_GAME_H
