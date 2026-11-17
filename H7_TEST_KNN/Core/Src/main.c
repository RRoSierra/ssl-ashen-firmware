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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
// Buffer DMA de 6 posiciones (16-bit)
ALIGN_32BYTES (uint16_t adc_buffer[6]);

// Buffer para la firma de luz limpia (Activo - Ambiente)
int16_t qtr_diff[6];

volatile uint8_t qtr_state = 0; // 0: Midiendo ambiente, 1: Midiendo activo
volatile uint8_t knn_ready = 0; // Bandera para ejecutar el cálculo
int16_t qtr_result[6];
/* USER CODE BEGIN PV */
ALIGN_32BYTES (uint16_t adc_buffer[6]);
int16_t qtr_diff[6];
int16_t qtr_result[6];

#define PUNTOS 27
const float LUT[PUNTOS][8] = {
    {0.0, 50.0, 20, 43, 38, 46, 29, 27},
    {0.0, 50.0, 24, 46, 37, 50, 35, 27},
    {0.0, 50.0, 27, 45, 33, 46, 32, 24},
    {-15.0, 50.0, 21, 44, 35, 51, 30, 26},
    {-15.0, 50.0, 17, 44, 37, 46, 31, 27},
    {-30.0, 50.0, 31, 44, 35, 43, 27, 32},
    {-30.0, 50.0, 28, 41, 27, 39, 29, 26},
    {15.0, 50.0, 39, 48, 37, 44, 38, 32},
    {15.0, 50.0, 35, 50, 41, 51, 36, 34},
    {15.0, 50.0, 24, 50, 41, 53, 40, 38},
    {30.0, 50.0, 25, 51, 45, 52, 34, 33},
    {30.0, 50.0, 24, 48, 38, 51, 34, 35},
    {30.0, 50.0, 31, 44, 37, 49, 39, 37},
    {-30.0, 30.0, 36, 47, 37, 56, 34, 34},
    {-30.0, 30.0, 43, 68, 52, 56, 32, 33},
    {-15.0, 30.0, 47, 62, 49, 54, 37, 35},
    {-15.0, 30.0, 42, 71, 71, 70, 51, 32},
    {-15.0, 30.0, 52, 76, 62, 68, 45, 42},
    {0.0, 30.0, 41, 72, 59, 68, 47, 38},
    {0.0, 30.0, 38, 77, 73, 82, 60, 35},
    {0.0, 30.0, 39, 79, 74, 84, 60, 36},
    {15.0, 30.0, 38, 78, 76, 83, 59, 44},
    {15.0, 30.0, 27, 56, 55, 78, 55, 50},
    {15.0, 30.0, 34, 56, 61, 77, 66, 42},
    {30.0, 30.0, 27, 54, 54, 77, 63, 40},
    {30.0, 30.0, 25, 50, 43, 59, 55, 47},
    {30.0, 30.0, 21, 47, 44, 61, 50, 49},
};

float ball_x = 0.0f;
float ball_y = 0.0f;
/* USER CODE END PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_SPI4_Init();
  MX_TIM1_Init();
  MX_I2C2_Init();
//  MX_SDMMC1_SD_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_UART7_Init();
  MX_I2C4_Init();
  MX_ADC1_Init();
  MX_UART8_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  // Iniciar el Timer 6 que disparará al ADC cada 1 ms
  HAL_TIM_Base_Start(&htim6);

  // Armar la primera lectura del DMA en estado ambiente (LEDs apagados)
  HAL_GPIO_WritePin(QTR_LEDON_GPIO_Port, QTR_LEDON_Pin, GPIO_PIN_RESET);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 6);
  uint32_t last_uart_time = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (knn_ready) {
	          knn_ready = 0;

	          // Inicializar distancias mínimas al máximo valor posible (32 bits)
	          uint32_t dist_min[3] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	          uint8_t idx_min[3] = {0, 0, 0};

	          // 1. Calcular distancia euclidiana cuadrada contra todos los puntos
	          for (int i = 0; i < PUNTOS; i++) {
	              uint32_t dist_sq = 0;

	              for (int j = 0; j < 6; j++) {
	                  // LUT[i][j+2] porque las columnas 0 y 1 son X e Y
	                  int32_t error = qtr_result[j] - (int16_t)LUT[i][j+2];
	                  dist_sq += (error * error);
	              }

	              // 2. Mantener solo los 3 vecinos más cercanos
	              if (dist_sq < dist_min[0]) {
	                  dist_min[2] = dist_min[1]; idx_min[2] = idx_min[1];
	                  dist_min[1] = dist_min[0]; idx_min[1] = idx_min[0];
	                  dist_min[0] = dist_sq;     idx_min[0] = i;
	              }
	              else if (dist_sq < dist_min[1]) {
	                  dist_min[2] = dist_min[1]; idx_min[2] = idx_min[1];
	                  dist_min[1] = dist_sq;     idx_min[1] = i;
	              }
	              else if (dist_sq < dist_min[2]) {
	                  dist_min[2] = dist_sq;     idx_min[2] = i;
	              }
	          }

	          // 3. Estimar la posición (X, Y) promediando las coordenadas de los 3 vecinos
	          ball_x = (LUT[idx_min[0]][0] + LUT[idx_min[1]][0] + LUT[idx_min[2]][0]) / 3.0f;
	          ball_y = (LUT[idx_min[0]][1] + LUT[idx_min[1]][1] + LUT[idx_min[2]][1]) / 3.0f;

	          // 4. Transmitir por UART SOLO cada 50 milisegundos (20 FPS)
			  if (HAL_GetTick() - last_uart_time >= 50) {
				  last_uart_time = HAL_GetTick(); // Actualizar el cronómetro

				  char uart_buf[64];
				  int len = snprintf(uart_buf, sizeof(uart_buf), "%.1f,%.1f,%d,%d,%d,%d,%d,%d\n",
									 ball_x, ball_y,
									 qtr_result[0], qtr_result[1], qtr_result[2],
									 qtr_result[3], qtr_result[4], qtr_result[5]);

				  // Ahora la función tiene tiempo de sobra para transmitir el mensaje completo
				  HAL_UART_Transmit(&huart8, (uint8_t*)uart_buf, len, 10);
	      }
	  }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 32;
  RCC_OscInitStruct.PLL.PLLN = 129;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
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
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if(hadc->Instance == ADC1) {
        // MUY IMPORTANTE en el H7: Invalidar la caché de datos para leer la memoria real que escribió el DMA
        SCB_InvalidateDCache_by_Addr((uint32_t *)adc_buffer, 32);

        if (qtr_state == 0) {
                    // Fase 1: Guardar el valor ambiente positivo (Típicamente un valor ALTO de voltaje, ej. 3900)
                    for(int i = 0; i < 6; i++) {
                        qtr_diff[i] = adc_buffer[i];
                    }

                    HAL_GPIO_WritePin(QTR_LEDON_GPIO_Port, QTR_LEDON_Pin, GPIO_PIN_SET);
                    qtr_state = 1;
                    HAL_ADC_Start_DMA(hadc, (uint32_t*)adc_buffer, 6);
                }
                else if (qtr_state == 1) {
                    // Fase 2: LEDs encendidos
                    HAL_GPIO_WritePin(QTR_LEDON_GPIO_Port, QTR_LEDON_Pin, GPIO_PIN_RESET);

                    for(int i = 0; i < 6; i++) {
                        // Señal Neta = Lectura Oscura (Voltaje Alto) - Lectura Brillante (Voltaje Bajo)
                        qtr_diff[i] -= adc_buffer[i];

                        // Si el resultado es negativo por ruido aleatorio, forzar a 0
                        if(qtr_diff[i] < 0) qtr_diff[i] = 0;

                        qtr_result[i] = qtr_diff[i]; // Copiar al buffer del depurador
                    }

                    qtr_state = 0;
                    knn_ready = 1;
                    HAL_ADC_Start_DMA(hadc, (uint32_t*)adc_buffer, 6);
                }
    }
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
