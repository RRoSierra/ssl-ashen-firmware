/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OP_RST2_Pin GPIO_PIN_2
#define OP_RST2_GPIO_Port GPIOE
#define FOC1_GPIO1_Pin GPIO_PIN_3
#define FOC1_GPIO1_GPIO_Port GPIOE
#define DSW4_Pin GPIO_PIN_4
#define DSW4_GPIO_Port GPIOE
#define DSW1_Pin GPIO_PIN_5
#define DSW1_GPIO_Port GPIOE
#define DSW2_Pin GPIO_PIN_6
#define DSW2_GPIO_Port GPIOE
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define QTR_1_Pin GPIO_PIN_0
#define QTR_1_GPIO_Port GPIOC
#define QTR_2_Pin GPIO_PIN_1
#define QTR_2_GPIO_Port GPIOC
#define QTR3_Pin GPIO_PIN_0
#define QTR3_GPIO_Port GPIOA
#define QTR4_Pin GPIO_PIN_1
#define QTR4_GPIO_Port GPIOA
#define QTR5_Pin GPIO_PIN_2
#define QTR5_GPIO_Port GPIOA
#define QTR6_Pin GPIO_PIN_3
#define QTR6_GPIO_Port GPIOA
#define OP_CS2_Pin GPIO_PIN_4
#define OP_CS2_GPIO_Port GPIOA
#define FOC1_CS2_Pin GPIO_PIN_4
#define FOC1_CS2_GPIO_Port GPIOC
#define OP_MOT2_Pin GPIO_PIN_5
#define OP_MOT2_GPIO_Port GPIOC
#define BALL_IR_EXTI_Pin GPIO_PIN_0
#define BALL_IR_EXTI_GPIO_Port GPIOB
#define FOC3_STATUS_Pin GPIO_PIN_1
#define FOC3_STATUS_GPIO_Port GPIOB
#define INA_AL_EXTI_Pin GPIO_PIN_2
#define INA_AL_EXTI_GPIO_Port GPIOB
#define DSW3_Pin GPIO_PIN_9
#define DSW3_GPIO_Port GPIOE
#define LCD_LED_Pin GPIO_PIN_10
#define LCD_LED_GPIO_Port GPIOE
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOE
#define LCD_SCK_Pin GPIO_PIN_12
#define LCD_SCK_GPIO_Port GPIOE
#define LCD_WR_RS_Pin GPIO_PIN_13
#define LCD_WR_RS_GPIO_Port GPIOE
#define LCD_MOSI_Pin GPIO_PIN_14
#define LCD_MOSI_GPIO_Port GPIOE
#define LCD_RESET_Pin GPIO_PIN_15
#define LCD_RESET_GPIO_Port GPIOE
#define OP_CS1_Pin GPIO_PIN_12
#define OP_CS1_GPIO_Port GPIOB
#define OP_RST1_Pin GPIO_PIN_8
#define OP_RST1_GPIO_Port GPIOD
#define OP_MOT1_Pin GPIO_PIN_9
#define OP_MOT1_GPIO_Port GPIOD
#define FOC2_CS1_Pin GPIO_PIN_10
#define FOC2_CS1_GPIO_Port GPIOD
#define FOC2_GPIO1_Pin GPIO_PIN_11
#define FOC2_GPIO1_GPIO_Port GPIOD
#define FOC2_CS2_Pin GPIO_PIN_14
#define FOC2_CS2_GPIO_Port GPIOD
#define FOC2_GPIO2_Pin GPIO_PIN_15
#define FOC2_GPIO2_GPIO_Port GPIOD
#define FOC1_CS1_Pin GPIO_PIN_6
#define FOC1_CS1_GPIO_Port GPIOC
#define IMU_CS_Pin GPIO_PIN_7
#define IMU_CS_GPIO_Port GPIOC
#define FOC1_GPIO2_Pin GPIO_PIN_8
#define FOC1_GPIO2_GPIO_Port GPIOA
#define FOC2_STATUS_Pin GPIO_PIN_9
#define FOC2_STATUS_GPIO_Port GPIOA
#define FOC1_STATUS_Pin GPIO_PIN_10
#define FOC1_STATUS_GPIO_Port GPIOA
#define B2_Pin GPIO_PIN_11
#define B2_GPIO_Port GPIOA
#define B3_Pin GPIO_PIN_12
#define B3_GPIO_Port GPIOA
#define QTR_LEDON_Pin GPIO_PIN_15
#define QTR_LEDON_GPIO_Port GPIOA
#define NRF_CS1_Pin GPIO_PIN_0
#define NRF_CS1_GPIO_Port GPIOD
#define NRF_CE1_Pin GPIO_PIN_1
#define NRF_CE1_GPIO_Port GPIOD
#define NRF_CE2_Pin GPIO_PIN_5
#define NRF_CE2_GPIO_Port GPIOD
#define NRF_CSN2_Pin GPIO_PIN_7
#define NRF_CSN2_GPIO_Port GPIOD
#define QSPI_NCS_Pin GPIO_PIN_6
#define QSPI_NCS_GPIO_Port GPIOB
#define IMU_EXTI1_Pin GPIO_PIN_7
#define IMU_EXTI1_GPIO_Port GPIOB
#define IMU_EXTI2_Pin GPIO_PIN_8
#define IMU_EXTI2_GPIO_Port GPIOB
#define FOC4_STATUS_Pin GPIO_PIN_9
#define FOC4_STATUS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
