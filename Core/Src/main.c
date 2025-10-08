/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
#define num_of_timers 4
const int MAX_LED = 4;
const int MAX_LED_MATRIX = 8;
int count = 100;
int index_led_matrix = 0;
int hour = 15, minute = 8, second = 50;
int index_led = 0;
int led_buffer[4] = {1,5,0,8};
int timer_flag[num_of_timers];
int timer_counter[num_of_timers];
int TIMER_CYCLE = 10;
uint8_t matrix_buffer[8] = {0b11100111, 0b11011011, 0b10111101, 0b10111101, 0b10000001, 0b10111101, 0b10111101, 0b10111101};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void setTimer(int idx, int duration);
void timer_run();
void clearAllLeds();
void updateClockBuffer();
void update7SEG(int index);
void display7SEG(int num);
void updateLEDMatrix(int index);
void LEDMatrix_run();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void setTimer(int idx, int duration){
	if(idx < 0 || idx >= num_of_timers){
		return;
	}
	timer_counter[idx] = duration / TIMER_CYCLE;
	timer_flag[idx] = 0;
}
void timer_run(){
	for(int i = 0;i < num_of_timers; i++){
		int running = (timer_counter[i] > 0);
		timer_counter[i] -= running;
		timer_flag[i] |= (timer_counter[i] == 0 && running);
	}
}
void updateClockBuffer(){
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;
}
void clearAllLeds(){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
}
void update7SEG(int index){
	clearAllLeds();
	switch(index){
	case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
		display7SEG(led_buffer[0]);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
		display7SEG(led_buffer[1]);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET);
		display7SEG(led_buffer[2]);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
		display7SEG(led_buffer[3]);
		break;
	}
}
void display7SEG(int num){
	switch(num){
		case 0:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SET);
	  		  break;
	  	case 1:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SET);
	  		  break;
	  	case 2:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, SET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
	  		  break;
	  	case 3:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
	  		  break;
	  	case 4:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, SET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
	  		  break;
	  	case 5:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
	  		  break;
	  	case 6:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, SET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
	  		  break;
	  	case 7:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, SET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, SET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, SET);
	  		  break;
	  	case 8:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, RESET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
	  		  break;
	  	case 9:
	  		  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, RESET);
	  		  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, RESET);
	  		  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, RESET);
	  		  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, RESET);
	  		  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, SET);
	  		  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, RESET);
	  		  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, RESET);
	  		  break;
	  	}
}
void updateLEDMatrix(int index){
	//tat tat ca led matrix
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, SET);

	//Bien dung de bieu dien trang thai sang cua tung hang
	uint16_t row_pattern = 0;
	for (int row = 0; row < 8; row++) {
	    if (matrix_buffer[row] & (1 << index)) {
	        row_pattern |= (1 << row);
	    }
	}

	//gan dung yeu cau
	GPIOB->ODR = (GPIOB->ODR & ~0xFF00) | (row_pattern << 8);

	//bat tung cot, voi delay nhanh thi` gan nhu la` chu A
	switch(index){
	case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, RESET);
		break;
	}
}
void LEDMatrix_run(){
	uint8_t temp[8];
	for(int i = 0; i < 8; i++){
		temp[i] = (matrix_buffer[i] & 0x80) >> 7;
		matrix_buffer[i] <<= 1;
	}
	for(int i = 0; i < 8; i++){
		matrix_buffer[i] |= temp[i];
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  clearAllLeds();
  //updateClockBuffer();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  setTimer(0, 1000);
  setTimer(1, 1000);
  setTimer(2, 250);
  setTimer(3, 10);
  while (1)
  {
	  //Nhap nhay den de kiem tra he thong
	  if(timer_flag[0]){
		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		  setTimer(0, 1000);
	  }

	  //Dong ho so
	  if(timer_flag[1]){
		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
		  second++;
		  if(second >= 60){
			  second = 0;
			  minute++;
		  }
		  if(minute >= 60){
			  minute = 0;
			  hour++;
		  }
		  if(hour >= 24){
			  hour =  0;
		  }
		  updateClockBuffer();
		  setTimer(1, 1000);
	  }

	  //Cap nhat den
	  if(timer_flag[2]){
	      update7SEG(index_led);
	      index_led++;
	      if(index_led >= MAX_LED){
	    	  index_led = 0;
	      }
	      setTimer(2, 250);
	  }

	  //Led_matrix
	  /*if(timer_flag[3]){
		  updateLEDMatrix(index_led_matrix);
		  index_led_matrix++;
		  if(index_led_matrix >= MAX_LED_MATRIX){
			  index_led_matrix = 0;
			  //Khi hoan thanh chu A, thi` cho chu A dich sang phai
			  LEDMatrix_run();
		  }
		  setTimer(3, 10);
	  }*/
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3|DOT_Pin|LED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|d_Pin|e_Pin|f_Pin
                          |g_Pin|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA2 PA3 DOT_Pin LED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           PA10 PA11 PA12 PA13
                           PA14 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|DOT_Pin|LED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : a_Pin b_Pin c_Pin PB10
                           PB11 PB12 PB13 PB14
                           PB15 d_Pin e_Pin f_Pin
                           g_Pin PB8 PB9 */
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|d_Pin|e_Pin|f_Pin
                          |g_Pin|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
int cforEx1 = 100;
int cforLedEx1 = 50;
int cforLedEx2 = 25;
int cforLedEx4 = 100;
int seg_index = 0;
int indexforEx3 = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	timer_run();

	//EX1
	/*cforEx1--;
	if(cforEx1 <= 0) {
		cforEx1 = 100;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}*/
	/*
	cforLedEx1--;
	if(cforLedEx1 <= 0){
		cforLedEx1 = 50;
		clearAllLeds();
		if(seg_index == 0){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
			display7SEG(1);
			seg_index = 1;
		}
		else if(seg_index == 1){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
			display7SEG(2);
			seg_index = 0;
		}
	}*/

	//EX2
	/*cforLedEx2--;
	if(cforLedEx2 <= 0){
		cforLedEx2 = 25;
		clearAllLeds();
		if(seg_index == 0){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
			display7SEG(1);
			seg_index = 1;
		}
		else if(seg_index == 1){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
			display7SEG(2);
			seg_index = 2;
		}
		else if(seg_index == 2){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET);
			display7SEG(3);
			seg_index = 3;
		}
		else if(seg_index == 3){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
			display7SEG(0);
			seg_index = 0;
		}
	}*/
	//EX3
	/*cforLedEx2--;
	if(cforLedEx2 <= 0){
		cforLedEx2 = 100;
		update7SEG(indexforEx3);
		indexforEx3++;
		if(indexforEx3 >= 4){
			indexforEx3 = 0;
		}
	}*/

	//EX4
	/*cforLedEx4--;
	if(cforLedEx4 <= 0){
		cforLedEx4 = 100;
		update7SEG(indexforEx3);
		indexforEx3++;
		if(indexforEx3 >= 4){
			indexforEx3 = 0;
		}
	}*/
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
