/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Debug.h"
#include "string.h"
#include "get_speed.h"
#include "pid.h"
#include "motor.h"
#include "stdio.h"
#include "jy61p.h"
#include "math.h"
#include "oled.h"
#include "font.h"
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
float base_speed = 0.0f; //基础速度
uint8_t RxData;//陀螺仪数据变量
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//浮点数转字符串
char* fts(float num) {
  static char buffer[50];
  sprintf(buffer, "%.2f", num); // buffer = "123"
  return buffer;
}
//陀螺仪数据获取
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart2) {
    // 直接处理接收到的字节
    jy61p_ReceiveData(RxData);
    // 重新启用单字节接收
    HAL_UART_Receive_DMA(huart, &RxData, 1);
  }
}
//定时pid
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim9) {
    //视觉信号
    char data[]="1\n";
    HAL_UART_Transmit_DMA(&huart1,data, strlen(data));
    //速度获取
    if (stop_flag != 1){
      //角度环
      angle_ring.actual = Yaw;
      PID_update(&angle_ring);
      if (fabsf(angle_ring.target - angle_ring.actual) < 3) angle_ring.out = 0;
      speed_ring_l.target =base_speed - angle_ring.out;
      speed_ring_r.target =base_speed + angle_ring.out;
      //速度环
      speed_ring_l.actual = (float) get_speed(&htim4);;
      speed_ring_r.actual = (float) get_speed(&htim5);
      PID_update(&speed_ring_l);
      PID_update(&speed_ring_r);
      //vofa控制
      UART_Printf(&huart4, "%.2f,%.2f\r\n", speed_ring_l.target, speed_ring_l.actual);
      //OLED显示区域
      OLED_NewFrame();
      OLED_PrintASCIIString(0,0,fts(Yaw),&afont16x8,OLED_COLOR_NORMAL);
      OLED_PrintASCIIString(0,18,fts(speed_ring_l.kp),&afont16x8,OLED_COLOR_NORMAL);
      OLED_PrintASCIIString(0,36,fts(speed_ring_r.ki),&afont16x8,OLED_COLOR_NORMAL);
      OLED_ShowFrame();
      //控制电机
      Set_Pwml((int) speed_ring_l.out);
      Set_Pwmr((int) speed_ring_r.out);
    }
    else {
      Motor_stop();
    }
  }
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
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM9_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //变量定义
  //陀螺仪初始化
  HAL_UART_Receive_DMA(&huart2, &RxData, 1);
  //定时器初始化
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1); //开启编码器模式
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  //开启pwm
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  HAL_TIM_Base_Start_IT(&htim9);//开启定时
  //vofa接收初始化
  HAL_UARTEx_ReceiveToIdle_DMA(&huart4, sofa_data, sizeof(sofa_data));
  //视觉接收初始化
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, camera_data, sizeof(camera_data));
  //OLED初始化
  OLED_Init();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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
