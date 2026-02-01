/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  * STM32F401RE + TCS3200 Color Sensor
  * Pins:
  *   S0 = PC14
  *   S1 = PC15
  *   S2 = PA11
  *   S3 = PA12
  *   OE = PB7 (Active LOW)
  *   OUT = PB4 (EXTI4 Interrupt)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint32_t pulseCount = 0;
char msg[100];
/* USER CODE END PV */

/* Function prototypes -------------------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
void TCS3200_SetColor(uint8_t s2, uint8_t s3);
uint32_t TCS3200_ReadFreq(void);
void Convert_To_RGB(uint32_t Rf, uint32_t Gf, uint32_t Bf,
                    uint8_t *R8, uint8_t *G8, uint8_t *B8);
char* DetectColor(uint8_t R, uint8_t G, uint8_t B);
void UART_Print(char *text);
/* USER CODE END PFP */

/* Application entry point */
int main(void)
{
  uint32_t Rf, Gf, Bf;
  uint8_t R8, G8, B8;

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  UART_Print("TCS3200 Initialized\r\n");

  // Enable sensor output (OE LOW)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  // Frequency scaling S0=1, S1=0 (20%)
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);

  while (1)
  {
      // --- READ RED ---
	  TCS3200_SetColor(0,0);
	     HAL_Delay(100);
	     Rf = TCS3200_ReadFreq();

	     // --- READ GREEN ---
	     TCS3200_SetColor(1,1);
	     HAL_Delay(100);
	     Gf = TCS3200_ReadFreq();

	     // --- READ BLUE ---
	     TCS3200_SetColor(0,1);
	     HAL_Delay(100);
	     Bf = TCS3200_ReadFreq();

	     sprintf(msg,
	         "FREQ => R=%lu  G=%lu  B=%lu\r\n",
	         Rf, Gf, Bf);

	     UART_Print(msg);

	     HAL_Delay(500);
  }
}

/* ------------------------ SYSTEM CLOCK ------------------------ */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { Error_Handler(); }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!=HAL_OK)
        Error_Handler();
}

/* ------------------------ GPIO ------------------------ */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* S0 PC14, S1 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* S2 PA11, S3 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* OE PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* OUT PB4 (EXTI4 Rising edge) */
  GPIO_InitStruct.Pin  = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;   // IMPORTANT
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
  /* Enable EXTI4 interrupt */


/* ------------------------ UART2 ------------------------ */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
    Error_Handler();
}

/* ------------------------ USER FUNCTIONS ------------------------ */

uint8_t mapColor(uint32_t v, uint32_t min, uint32_t max)
{
    if (v < min) v = min;
    if (v > max) v = max;
    return (uint8_t)((v - min) * 255 / (max - min));
}
void Convert_To_RGB(uint32_t Rf, uint32_t Gf, uint32_t Bf,
                    uint8_t *R8, uint8_t *G8, uint8_t *B8)
{
    *R8 = mapColor(Rf, 300, 6500);
    *G8 = mapColor(Gf, 300, 6500);
    *B8 = mapColor(Bf, 300, 6500);
}
char* DetectColor(uint8_t R, uint8_t G, uint8_t B)
{
    if (R > G && R > B) return "RED";
    if (G > R && G > B) return "GREEN";
    if (B > R && B > G) return "BLUE";
    if (R > 200 && G > 200 && B > 200) return "WHITE";
    if (R < 30 && G < 30 && B < 30) return "BLACK";
    return "UNKNOWN";
}

/* Count pulses coming from TCS3200 OUT pin */
uint32_t TCS3200_ReadFreq(void)
{
    uint32_t count = 0;
    uint32_t start = HAL_GetTick();
    GPIO_PinState last = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);

    while ((HAL_GetTick() - start) < 100)   // 100 ms window
    {
        GPIO_PinState now = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);

        // detect falling edge
        if (last == GPIO_PIN_SET && now == GPIO_PIN_RESET)
        {
            count++;
        }

        last = now;
    }

    return count * 10;   // convert to Hz
}
void TCS3200_SetColor(uint8_t s2, uint8_t s3)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, s2 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, s3 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void UART_Print(char *text)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)text, strlen(text), HAL_MAX_DELAY);
}
/* Error Handler */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
