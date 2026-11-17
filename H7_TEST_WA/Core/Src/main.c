/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "image_data.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* ---- ST7735 160x80 Landscape Driver ---- */

#define LCD_WIDTH  160
#define LCD_HEIGHT 80

#define ST7735_X_OFFSET 0
#define ST7735_Y_OFFSET 24

/* Control pin macros */
#define LCD_CS_LOW()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port,   LCD_CS_Pin,   GPIO_PIN_RESET)
#define LCD_CS_HIGH()  HAL_GPIO_WritePin(LCD_CS_GPIO_Port,   LCD_CS_Pin,   GPIO_PIN_SET)
#define LCD_RS_LOW()   HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port, LCD_WR_RS_Pin, GPIO_PIN_RESET)
#define LCD_RS_HIGH()  HAL_GPIO_WritePin(LCD_WR_RS_GPIO_Port, LCD_WR_RS_Pin, GPIO_PIN_SET)

static void LCD_LED_ON(void)
{
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}

/* ST7735 commands */
#define ST7735_SWRESET  0x01
#define ST7735_SLPOUT   0x11
#define ST7735_NORON    0x13
#define ST7735_INVON    0x21
#define ST7735_DISPON   0x29
#define ST7735_CASET    0x2A
#define ST7735_RASET    0x2B
#define ST7735_RAMWR    0x2C
#define ST7735_MADCTL   0x36
#define ST7735_COLMOD   0x3A
#define ST7735_FRMCTR1  0xB1
#define ST7735_INVCTR   0xB4
#define ST7735_PWCTR1   0xC0
#define ST7735_PWCTR2   0xC1
#define ST7735_PWCTR3   0xC2
#define ST7735_VMCTR1   0xC5
#define ST7735_GMCTRP1  0xE0
#define ST7735_GMCTRN1  0xE1

extern SPI_HandleTypeDef hspi4;
extern TIM_HandleTypeDef htim1;

static void ST7735_WriteCmd(uint8_t cmd)
{
  LCD_RS_LOW();
  LCD_CS_LOW();
  HAL_SPI_Transmit(&hspi4, &cmd, 1, HAL_MAX_DELAY);
  LCD_CS_HIGH();
}

static void ST7735_WriteData8(uint8_t data)
{
  LCD_RS_HIGH();
  LCD_CS_LOW();
  HAL_SPI_Transmit(&hspi4, &data, 1, HAL_MAX_DELAY);
  LCD_CS_HIGH();
}

static void ST7735_WriteData16(uint16_t data)
{
  uint8_t buf[2] = { data >> 8, data & 0xFF };
  LCD_RS_HIGH();
  LCD_CS_LOW();
  HAL_SPI_Transmit(&hspi4, buf, 2, HAL_MAX_DELAY);
  LCD_CS_HIGH();
}

static void ST7735_WriteDataBulk(const uint16_t *data, uint32_t len)
{
  /* Convert RGB565 big-endian for ST7735 and send via SPI */
  LCD_RS_HIGH();
  LCD_CS_LOW();
  uint8_t buf[2];
  for (uint32_t i = 0; i < len; i++) {
    buf[0] = data[i] >> 8;
    buf[1] = data[i] & 0xFF;
    HAL_SPI_Transmit(&hspi4, buf, 2, HAL_MAX_DELAY);
  }
  LCD_CS_HIGH();
}

static void ST7735_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  ST7735_WriteCmd(ST7735_CASET);
  ST7735_WriteData16(x0 + ST7735_X_OFFSET);
  ST7735_WriteData16(x1 + ST7735_X_OFFSET);

  ST7735_WriteCmd(ST7735_RASET);
  ST7735_WriteData16(y0 + ST7735_Y_OFFSET);
  ST7735_WriteData16(y1 + ST7735_Y_OFFSET);

  ST7735_WriteCmd(ST7735_RAMWR);
}

static void ST7735_Init(void)
{
  /* Backlight ON */
  LCD_LED_ON();
  HAL_Delay(50);

  /* Software Reset — send twice per reference driver */
  ST7735_WriteCmd(ST7735_SWRESET);
  HAL_Delay(120);
  ST7735_WriteCmd(ST7735_SWRESET);
  HAL_Delay(120);

  /* Sleep Out */
  ST7735_WriteCmd(ST7735_SLPOUT);
  HAL_Delay(10);

  /* Frame rate control - normal mode: fosc/(1x2+40) * (LINE+2C+2D) */
  ST7735_WriteCmd(ST7735_FRMCTR1);
  ST7735_WriteData8(0x01); ST7735_WriteData8(0x2C); ST7735_WriteData8(0x2D);

  /* Frame rate control - idle mode */
  ST7735_WriteCmd(0xB2); /* FRMCTR2 */
  ST7735_WriteData8(0x01); ST7735_WriteData8(0x2C); ST7735_WriteData8(0x2D);

  /* Frame rate control - partial mode */
  ST7735_WriteCmd(0xB3); /* FRMCTR3 */
  ST7735_WriteData8(0x01); ST7735_WriteData8(0x2C); ST7735_WriteData8(0x2D);
  ST7735_WriteData8(0x01); ST7735_WriteData8(0x2C); ST7735_WriteData8(0x2D);

  /* Display inversion control — no inversion (BOE panel) */
  ST7735_WriteCmd(ST7735_INVCTR);
  ST7735_WriteData8(0x07);

  /* Power control 1: -4.6V, AUTO mode */
  ST7735_WriteCmd(ST7735_PWCTR1);
  ST7735_WriteData8(0xA2); ST7735_WriteData8(0x02); ST7735_WriteData8(0x84);

  /* Power control 2: VGH25=2.4C VGSEL=-10 VGH=3*AVDD */
  ST7735_WriteCmd(ST7735_PWCTR2);
  ST7735_WriteData8(0xC5);

  /* Power control 3: Opamp current small, Boost frequency */
  ST7735_WriteCmd(ST7735_PWCTR3);
  ST7735_WriteData8(0x0A); ST7735_WriteData8(0x00);

  /* Power control 4: BCLK/2, Opamp current small & Medium low */
  ST7735_WriteCmd(0xC3); /* PWCTR4 */
  ST7735_WriteData8(0x8A); ST7735_WriteData8(0x2A);

  /* Power control 5 */
  ST7735_WriteCmd(0xC4); /* PWCTR5 */
  ST7735_WriteData8(0x8A); ST7735_WriteData8(0xEE);

  /* VCOM control 1 */
  ST7735_WriteCmd(ST7735_VMCTR1);
  ST7735_WriteData8(0x0E);

  /* Display inversion OFF (BOE panel — use INVON for HannStar panel) */
  ST7735_WriteCmd(0x21);

  /* Color mode: 16-bit RGB565 */
  ST7735_WriteCmd(ST7735_COLMOD);
  ST7735_WriteData8(0x05);

  /* Positive gamma correction */
  ST7735_WriteCmd(ST7735_GMCTRP1);
  ST7735_WriteData8(0x02); ST7735_WriteData8(0x1C); ST7735_WriteData8(0x07);
  ST7735_WriteData8(0x12); ST7735_WriteData8(0x37); ST7735_WriteData8(0x32);
  ST7735_WriteData8(0x29); ST7735_WriteData8(0x2D); ST7735_WriteData8(0x29);
  ST7735_WriteData8(0x25); ST7735_WriteData8(0x2B); ST7735_WriteData8(0x39);
  ST7735_WriteData8(0x00); ST7735_WriteData8(0x01); ST7735_WriteData8(0x03);
  ST7735_WriteData8(0x10);

  /* Negative gamma correction */
  ST7735_WriteCmd(ST7735_GMCTRN1);
  ST7735_WriteData8(0x03); ST7735_WriteData8(0x1D); ST7735_WriteData8(0x07);
  ST7735_WriteData8(0x06); ST7735_WriteData8(0x2E); ST7735_WriteData8(0x2C);
  ST7735_WriteData8(0x29); ST7735_WriteData8(0x2D); ST7735_WriteData8(0x2E);
  ST7735_WriteData8(0x2E); ST7735_WriteData8(0x37); ST7735_WriteData8(0x3F);
  ST7735_WriteData8(0x00); ST7735_WriteData8(0x00); ST7735_WriteData8(0x02);
  ST7735_WriteData8(0x10);

  /* Normal display mode ON */
  ST7735_WriteCmd(ST7735_NORON);
  HAL_Delay(10);

  ST7735_WriteCmd(ST7735_MADCTL);
  ST7735_WriteData8(0xA8);

  /* Display ON */
  ST7735_WriteCmd(ST7735_DISPON);
  HAL_Delay(100);
}

static void ST7735_FillScreen(uint16_t color)
{
  ST7735_SetWindow(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
  LCD_RS_HIGH();
  LCD_CS_LOW();
  uint8_t buf[2] = { color >> 8, color & 0xFF };
  for (uint32_t i = 0; i < (uint32_t)LCD_WIDTH * LCD_HEIGHT; i++) {
    HAL_SPI_Transmit(&hspi4, buf, 2, HAL_MAX_DELAY);
  }
  LCD_CS_HIGH();
}

static void ST7735_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  ST7735_SetWindow(x, y, x + w - 1, y + h - 1);
  LCD_RS_HIGH();
  LCD_CS_LOW();
  uint8_t buf[2] = { color >> 8, color & 0xFF };
  for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
    HAL_SPI_Transmit(&hspi4, buf, 2, HAL_MAX_DELAY);
  }
  LCD_CS_HIGH();
}

static void ST7735_DisplayImage(const uint16_t *image, uint16_t w, uint16_t h)
{
  ST7735_SetWindow(0, 0, w - 1, h - 1);
  ST7735_WriteDataBulk(image, (uint32_t)w * h);
}

static void ST7735_ColorTest(void)
{
  ST7735_FillRect(0, 0, LCD_WIDTH/2, LCD_HEIGHT/2, 0xF800);
  ST7735_FillRect(LCD_WIDTH/2, 0, LCD_WIDTH/2, LCD_HEIGHT/2, 0x07E0);
  ST7735_FillRect(0, LCD_HEIGHT/2, LCD_WIDTH/2, LCD_HEIGHT/2, 0x001F);
  ST7735_FillRect(LCD_WIDTH/2, LCD_HEIGHT/2, LCD_WIDTH/2, LCD_HEIGHT/2, 0xFFE0);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();
  CPU_CACHE_Enable();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI4_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();
  ST7735_DisplayImage(logo_sysmic_160x80, LCD_WIDTH, LCD_HEIGHT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void CPU_CACHE_Enable(void)
{
  SCB_EnableICache();
  SCB_EnableDCache();
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
