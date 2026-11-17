/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, OP_RST2_Pin|FOC1_GPIO1_Pin|DSW3_Pin|LCD_CS_Pin
                          |LCD_WR_RS_Pin|LCD_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, OP_CS2_Pin|FOC1_GPIO2_Pin|FOC2_STATUS_Pin|FOC1_STATUS_Pin
                          |QTR_LEDON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, FOC1_CS2_Pin|FOC1_CS1_Pin|IMU_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, FOC3_STATUS_Pin|OP_CS1_Pin|QSPI_NCS_Pin|FOC4_STATUS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, OP_RST1_Pin|FOC2_CS1_Pin|FOC2_GPIO1_Pin|FOC2_CS2_Pin
                          |FOC2_GPIO2_Pin|NRF_CS1_Pin|NRF_CE1_Pin|NRF_CE2_Pin
                          |NRF_CSN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : OP_RST2_Pin FOC1_GPIO1_Pin DSW3_Pin LCD_CS_Pin
                           LCD_WR_RS_Pin LCD_RESET_Pin */
  GPIO_InitStruct.Pin = OP_RST2_Pin|FOC1_GPIO1_Pin|DSW3_Pin|LCD_CS_Pin
                          |LCD_WR_RS_Pin|LCD_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : DSW4_Pin DSW1_Pin DSW2_Pin */
  GPIO_InitStruct.Pin = DSW4_Pin|DSW1_Pin|DSW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OP_CS2_Pin FOC1_GPIO2_Pin FOC2_STATUS_Pin FOC1_STATUS_Pin
                           QTR_LEDON_Pin */
  GPIO_InitStruct.Pin = OP_CS2_Pin|FOC1_GPIO2_Pin|FOC2_STATUS_Pin|FOC1_STATUS_Pin
                          |QTR_LEDON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : FOC1_CS2_Pin FOC1_CS1_Pin IMU_CS_Pin */
  GPIO_InitStruct.Pin = FOC1_CS2_Pin|FOC1_CS1_Pin|IMU_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : OP_MOT2_Pin */
  GPIO_InitStruct.Pin = OP_MOT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OP_MOT2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BALL_IR_EXTI_Pin INA_AL_EXTI_Pin IMU_EXTI1_Pin IMU_EXTI2_Pin */
  GPIO_InitStruct.Pin = BALL_IR_EXTI_Pin|INA_AL_EXTI_Pin|IMU_EXTI1_Pin|IMU_EXTI2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : FOC3_STATUS_Pin OP_CS1_Pin QSPI_NCS_Pin FOC4_STATUS_Pin */
  GPIO_InitStruct.Pin = FOC3_STATUS_Pin|OP_CS1_Pin|QSPI_NCS_Pin|FOC4_STATUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : OP_RST1_Pin FOC2_CS1_Pin FOC2_GPIO1_Pin FOC2_CS2_Pin
                           FOC2_GPIO2_Pin NRF_CS1_Pin NRF_CE1_Pin NRF_CE2_Pin
                           NRF_CSN2_Pin */
  GPIO_InitStruct.Pin = OP_RST1_Pin|FOC2_CS1_Pin|FOC2_GPIO1_Pin|FOC2_CS2_Pin
                          |FOC2_GPIO2_Pin|NRF_CS1_Pin|NRF_CE1_Pin|NRF_CE2_Pin
                          |NRF_CSN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : OP_MOT1_Pin PD3 PD4 PD6 */
  GPIO_InitStruct.Pin = OP_MOT1_Pin|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : B2_Pin B3_Pin */
  GPIO_InitStruct.Pin = B2_Pin|B3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
