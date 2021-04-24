
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include "MY_FLASH.h"

#define TRUE_COMPLETE 	1
#define FALSE_COMPLETE 	0
#define LENGTH 10
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
char* startString = "START\r\n";
uint32_t len_of_data = 0, count = 0, preCount = 0;
uint8_t data_buffer[LENGTH], flash_buffer[LENGTH];	
uint8_t recv_byte = 0;	//temp byte received by UART
uint8_t rx_complete = FALSE_COMPLETE, exti_complete = FALSE_COMPLETE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart2.Instance)
	{
		if(recv_byte == '@') //byte key == @
		{
			rx_complete = TRUE_COMPLETE;
//			data_buffer[count++] = '@';	//comment = not save byte key
			HAL_UART_Transmit(huart, data_buffer, count, HAL_MAX_DELAY);
		} 
		else data_buffer[count++] = recv_byte;
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_Delay(100); //debounce
	if(GPIO_Pin == GPIO_PIN_0)
	{
		exti_complete = TRUE_COMPLETE;
	}
}
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t u8Var = 0x02, u8ReadVar = 0;
	uint8_t u8Arr[3] = {0x01, 0x02, 0x03}, u8ArrRead[3]={0};
	uint16_t u16Var = 0x3456, u16ReadVar = 0;
	uint32_t u32Var = 0x31, u32ReadVar = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
	
  /* USER CODE BEGIN 2 */
				/*Start*/
	len_of_data = strlen(startString);
	HAL_UART_Transmit(&huart2,(uint8_t*) startString, len_of_data, HAL_MAX_DELAY);
	//SELECT sector to work
	MY_FLASH_SetSectorAddrs(1, SECTORADD_1);

		//TEST FLASH 
//	MY_FLASH_WriteN(0x00000002, u8Arr, 3, DATA_TYPE_8);
//	MY_FLASH_WriteN(0x00000002, &u8Var, 1, DATA_TYPE_8);
//	MY_FLASH_WriteN(0x00000004, &u16Var, 1, DATA_TYPE_16);
//	MY_FLASH_WriteN(0x00000004, &u32Var, 1, DATA_TYPE_32);
//	HAL_Delay(500);
//	MY_FLASH_ReadN(0x02, u8ArrRead, 3, DATA_TYPE_8);
//	MY_FLASH_ReadN(0x00000002, &u8ReadVar, 1, DATA_TYPE_8);
//	MY_FLASH_ReadN(0x00000004, &u16ReadVar, 1, DATA_TYPE_16);
//	MY_FLASH_ReadN(0x00000004, &u32ReadVar, 1, DATA_TYPE_32);
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
				//continous receive data if not RX COMPLETE
			if(rx_complete != TRUE_COMPLETE) HAL_UART_Receive_IT(&huart2, &recv_byte, 1);
				//if RX complete	
			if(rx_complete == TRUE_COMPLETE) 
			{
				MY_FLASH_WriteN(0x00000002, data_buffer, count, DATA_TYPE_8);//save data to Flash
				preCount = count;	//save pre number of
				rx_complete = FALSE_COMPLETE;
				count = 0;
			}
			//if press btn
			if(exti_complete == TRUE_COMPLETE)
			{
				MY_FLASH_ReadN(0x00000002, flash_buffer, preCount, DATA_TYPE_8);
//				HAL_Delay(200);
				HAL_UART_Transmit(&huart2, flash_buffer, preCount, HAL_MAX_DELAY);
				exti_complete = FALSE_COMPLETE;
			}
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
