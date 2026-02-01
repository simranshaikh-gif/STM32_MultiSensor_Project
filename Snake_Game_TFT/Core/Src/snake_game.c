#include "../Inc/snake_game.h"
#include "main.h"
#include <stdbool.h>
#include <stdlib.h>

// Button GPIO Definitions (Mapping based on user request)
// UP: PC6
// DOWN: PC5
// LEFT: PB8
// RIGHT: PB9

// Note: Ensure these ports/pins match the CubeMX configuration we will add to
// main.c
// UP = PC6
#define BTN_UP_PORT GPIOC
#define BTN_UP_PIN GPIO_PIN_6

// DOWN = PC5
#define BTN_DOWN_PORT GPIOC
#define BTN_DOWN_PIN GPIO_PIN_5

// LEFT = PB8
#define BTN_LEFT_PORT GPIOB
#define BTN_LEFT_PIN GPIO_PIN_8

// RIGHT = PB9
#define BTN_RIGHT_PORT GPIOB
#define BTN_RIGHT_PIN GPIO_PIN_9

// Centering Offsets
// 160 % 10 = 0 -> Offset 0
// 128 % 10 = 8 -> Offset 4
#define OFFSET_X ((ST7735_WIDTH % GRID_SIZE) / 2)
#define OFFSET_Y ((ST7735_HEIGHT % GRID_SIZE) / 2)

static Snake snake;
static Point apple;
static bool game_over;

// Helper to spawn apple in a valid location
static void SpawnApple() {
  bool valid;
  do {
    valid = true;
    // Simple random generation
    apple.x = rand() % GRID_WIDTH;
    apple.y = rand() % GRID_HEIGHT;

    // Check if apple spawned on snake
    for (int i = 0; i < snake.length; i++) {
      if (snake.body[i].x == apple.x && snake.body[i].y == apple.y) {
        valid = false;
        break;
      }
    }
  } while (!valid);
}

// Initialize Game State
static void InitGame() {
  snake.length = 3;
  snake.dir = DIR_RIGHT;
  snake.body[0].x = GRID_WIDTH / 2;
  snake.body[0].y = GRID_HEIGHT / 2;
  snake.body[1].x = snake.body[0].x - 1;
  snake.body[1].y = snake.body[0].y;
  snake.body[2].x = snake.body[0].x - 2;
  snake.body[2].y = snake.body[0].y;

  SpawnApple();
  game_over = false;

  ST7735_FillScreen(COLOR_BG);
}

// Read Inputs
static void ProcessInput() {
  // Check buttons (assuming Active HIGH / Pull-Down)
  // You might need debounce, but for now strict polling
  if (HAL_GPIO_ReadPin(BTN_UP_PORT, BTN_UP_PIN) == GPIO_PIN_SET &&
      snake.dir != DIR_DOWN) {
    snake.dir = DIR_UP;
  } else if (HAL_GPIO_ReadPin(BTN_DOWN_PORT, BTN_DOWN_PIN) == GPIO_PIN_SET &&
             snake.dir != DIR_UP) {
    snake.dir = DIR_DOWN;
  } else if (HAL_GPIO_ReadPin(BTN_LEFT_PORT, BTN_LEFT_PIN) == GPIO_PIN_SET &&
             snake.dir != DIR_RIGHT) {
    snake.dir = DIR_LEFT;
  } else if (HAL_GPIO_ReadPin(BTN_RIGHT_PORT, BTN_RIGHT_PIN) == GPIO_PIN_SET &&
             snake.dir != DIR_LEFT) {
    snake.dir = DIR_RIGHT;
  }
}

static void UpdateAndDraw() {
  Point new_head = snake.body[0];

  switch (snake.dir) {
  case DIR_UP:
    new_head.y--;
    break;
  case DIR_DOWN:
    new_head.y++;
    break;
  case DIR_LEFT:
    new_head.x--;
    break;
  case DIR_RIGHT:
    new_head.x++;
    break;
  }

  // Check Wall Collision
  if (new_head.x < 0 || new_head.x >= GRID_WIDTH || new_head.y < 0 ||
      new_head.y >= GRID_HEIGHT) {
    game_over = true;
    return;
  }

  // Check Self Collision
  for (int i = 0; i < snake.length; i++) {
    if (new_head.x == snake.body[i].x && new_head.y == snake.body[i].y) {
      game_over = true;
      return;
    }
  }

  // Render Apple
  ST7735_FillRectangle(OFFSET_X + apple.x * GRID_SIZE,
                       OFFSET_Y + apple.y * GRID_SIZE, GRID_SIZE, GRID_SIZE,
                       COLOR_APPLE);

  // Erase Tail if not eating
  bool ate = (new_head.x == apple.x && new_head.y == apple.y);
  if (!ate) {
    ST7735_FillRectangle(OFFSET_X + snake.body[snake.length - 1].x * GRID_SIZE,
                         OFFSET_Y + snake.body[snake.length - 1].y * GRID_SIZE,
                         GRID_SIZE, GRID_SIZE, COLOR_BG);
  }

  // Move Body
  for (int i = snake.length; i > 0; i--) {
    snake.body[i] = snake.body[i - 1];
  }
  snake.body[0] = new_head;

  // Draw New Head
  ST7735_FillRectangle(OFFSET_X + snake.body[0].x * GRID_SIZE,
                       OFFSET_Y + snake.body[0].y * GRID_SIZE, GRID_SIZE,
                       GRID_SIZE, COLOR_SNAKE);

  if (ate) {
    if (snake.length < MAX_SNAKE_LENGTH) {
      snake.length++;
    }
    SpawnApple();
  }
}

void Snake_Game_Start(void) {
  // Seed random (better ways exist, but HAL_GetTick is basically 0 at start
  // unless we wait)
  srand(HAL_GetTick());

  InitGame();

  while (1) {
    ProcessInput();

    if (!game_over) {
      UpdateAndDraw();
    } else {
      // Game Over: Buzzer beep for 2 seconds
      // Turn Buzzer ON (PA4 High)
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

      // Screen Flash Red
      ST7735_FillScreen(ST7735_RED);

      // Draw Game Over Text
      ST7735_WriteString(15, 70, "GAME OVER", Font_11x18, ST7735_WHITE,
                         ST7735_RED);

      // Wait 2 seconds
      HAL_Delay(2000);

      // Turn Buzzer OFF (PA4 Low)
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

      InitGame();
    }

    // Dynamic Speed Control via Rotation Sensor (ADC1 Channel 0 - Registers)
    // 1. Start Conversion via SWSTART
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // 2. Wait for EOC (End of Conversion)
    // Simple timeout loop
    uint32_t timeout = 1000;
    while (!(ADC1->SR & ADC_SR_EOC) && timeout > 0) {
      timeout--;
    }

    if (timeout > 0) {
      uint32_t adc_val = ADC1->DR;
      // Map 0-4095 to Delay 200ms-50ms
      uint32_t delay = 200 - (adc_val * 150 / 4096);
      HAL_Delay(delay);
    } else {
      HAL_Delay(100);
    }
  }
}
