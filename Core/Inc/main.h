/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
  extern float base_speed;
  extern int state;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
  char* fts(float num);
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Right_A_Pin GPIO_PIN_0
#define Right_A_GPIO_Port GPIOA
#define Right_B_Pin GPIO_PIN_1
#define Right_B_GPIO_Port GPIOA
#define jy61p_TX_Pin GPIO_PIN_2
#define jy61p_TX_GPIO_Port GPIOA
#define jy61p_RX_Pin GPIO_PIN_3
#define jy61p_RX_GPIO_Port GPIOA
#define Left_PWMA_Pin GPIO_PIN_6
#define Left_PWMA_GPIO_Port GPIOA
#define Left_PWMB_Pin GPIO_PIN_7
#define Left_PWMB_GPIO_Port GPIOA
#define Right_PWMA_Pin GPIO_PIN_0
#define Right_PWMA_GPIO_Port GPIOB
#define Right_PWMb_Pin GPIO_PIN_1
#define Right_PWMb_GPIO_Port GPIOB
#define Screen_TX_Pin GPIO_PIN_10
#define Screen_TX_GPIO_Port GPIOB
#define Screen_RX_Pin GPIO_PIN_11
#define Screen_RX_GPIO_Port GPIOB
#define Left_A_Pin GPIO_PIN_12
#define Left_A_GPIO_Port GPIOD
#define Left_B_Pin GPIO_PIN_13
#define Left_B_GPIO_Port GPIOD
#define L_IN1_Pin GPIO_PIN_2
#define L_IN1_GPIO_Port GPIOG
#define L_IN2_Pin GPIO_PIN_3
#define L_IN2_GPIO_Port GPIOG
#define R_IN2_Pin GPIO_PIN_4
#define R_IN2_GPIO_Port GPIOG
#define R_IN1_Pin GPIO_PIN_5
#define R_IN1_GPIO_Port GPIOG
#define Camera_TX_Pin GPIO_PIN_9
#define Camera_TX_GPIO_Port GPIOA
#define Camera_RX_Pin GPIO_PIN_10
#define Camera_RX_GPIO_Port GPIOA
#define Bluetooth_TX_Pin GPIO_PIN_10
#define Bluetooth_TX_GPIO_Port GPIOC
#define Bluetooth_RX_Pin GPIO_PIN_11
#define Bluetooth_RX_GPIO_Port GPIOC
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
