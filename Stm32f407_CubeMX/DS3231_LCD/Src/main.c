#include "main.h"
#include "i2c-lcd.h"
#include "stdio.h"
#include "fonts.h"
#include "ssd1306.h"
#include "horse_anim.h"
/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;

#define address_DS3231 0x68		//address I2C DS3231


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_DMA_Init(void);

			/*My function */
void Get_Time(void);
void force_temp_conv (void);
float Get_Temp(void);
void Horse_Running(void);
uint8_t BCDtoDEC(uint8_t data);
uint8_t DECtoBCD(uint8_t data);
	
			/*My variables*/
uint8_t get_time[7], send_time[7];
uint8_t second = 0, minute = 0, hour = 0, day = 0, date = 0, month = 0, year = 0 ;
				//giay      phut         gio       thu      ngay     thang       nam
uint8_t temp[2];\
float temp_value = 0;
char buffer[20];
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_DMA_Init();
	//lcd_init();
	SSD1306_Init();
	
	SSD1306_Fill(0); //fill black screen
	SSD1306_UpdateScreen();
		
	Horse_Running();
	Horse_Running();
	Horse_Running();
	
  while (1)
  {
		Get_Time();
		
		/*display  hour:month:year
		sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);
		lcd_put_cur(0,0);
		lcd_send_string(buffer);
		  //display  date:month:year 
		sprintf(buffer, "%02d-%02d-%02d", date, month, year);
		lcd_put_cur(1, 0);
		lcd_send_string(buffer);

		force_temp_conv();		//convert temperature to digital 	
		temp_value = Get_Temp();
	
		//display value of temperature
		sprintf(buffer, "%f", temp_value);		
		lcd_put_cur(0, 10);
		lcd_send_string(buffer);
		
		HAL_Delay(1000); */
		
			//display time
		SSD1306_GotoXY(1, 1);	
		sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);
		SSD1306_Puts(buffer, &Font_7x10, 1);
		SSD1306_UpdateScreen(); 
		
		SSD1306_GotoXY(56, 1);
		SSD1306_Puts("||", &Font_7x10, 1);
		SSD1306_UpdateScreen(); 
		
			//display date
		SSD1306_GotoXY(70, 1);
		sprintf(buffer, "%02d-%02d-%02d", date, month, year);
		SSD1306_Puts(buffer, &Font_7x10, 1);
		SSD1306_UpdateScreen(); 	
			
			//convert temperature to digital 
		force_temp_conv();			
		temp_value = Get_Temp();
		
		//display temperature
		SSD1306_GotoXY(5, 40);
		SSD1306_Puts("The temperature :", &Font_7x10, 1);
		SSD1306_UpdateScreen(); 
		
		SSD1306_GotoXY(35, 50 );
		sprintf(buffer, "%0.2f", temp_value);
		SSD1306_Puts(buffer, &Font_7x10, 1);		
		SSD1306_UpdateScreen(); 
		
		SSD1306_GotoXY(70, 50);
		SSD1306_Puts("*C", &Font_7x10, 1);	
		SSD1306_UpdateScreen(); 
		
		if(day == 1)
		{		
			SSD1306_GotoXY(35, 20);
			SSD1306_Puts("SUNDAY", &Font_7x10, 1);
			SSD1306_UpdateScreen();
		}
		else if(day == 2)
		{
			SSD1306_GotoXY(35, 20);
			SSD1306_Puts("MONDAY", &Font_7x10, 1);
			SSD1306_UpdateScreen(); 
		}
			else if(day == 3)
		{
			SSD1306_GotoXY(35, 20);
			SSD1306_Puts("TUESDAY", &Font_7x10, 1);
			SSD1306_UpdateScreen(); 
		}
			else if(day == 4)
		{
			SSD1306_GotoXY(35, 20);
			SSD1306_Puts("WEDNESDAY", &Font_7x10, 1);
			SSD1306_UpdateScreen(); 
		}
			else if(day == 5)
		{
			SSD1306_GotoXY(35, 20);
			SSD1306_Puts("THURSDAY", &Font_7x10, 1);
			SSD1306_UpdateScreen(); 
		}
			else if(day == 6)
		{
			SSD1306_GotoXY(35, 20);
			SSD1306_Puts("FRIDAY", &Font_7x10, 1);
			SSD1306_UpdateScreen(); 
		}
			else if(day == 7)
		{
			SSD1306_GotoXY(35, 20);
			SSD1306_Puts("SATURDAY", &Font_7x10, 1);
			SSD1306_UpdateScreen(); 
		}
		
  }

}

void Get_Time(void)
{
			//read data from DS3231 to Stm32
		HAL_I2C_Mem_Read(&hi2c1, address_DS3231<<1, 0, I2C_MEMADD_SIZE_8BIT, get_time, 7, 1000);
	
		second = BCDtoDEC(get_time[0]);
		minute = BCDtoDEC(get_time[1]);
		hour   = BCDtoDEC(get_time[2]);
		
		day    = BCDtoDEC(get_time[3]);
		date   = BCDtoDEC(get_time[4]);
		month  = BCDtoDEC(get_time[5]);
		year   = BCDtoDEC(get_time[6]);
		
}
void force_temp_conv (void) // forces temperature sensor to convert the temperature into digital 
{
	uint8_t status= 0;
	uint8_t control= 0;
	HAL_I2C_Mem_Read(&hi2c1, address_DS3231<<1, 0x0F, 1, &status, 1, 100);  // read status register
	if (!(status&0x04))  //if BSY is not set
	{
		HAL_I2C_Mem_Read(&hi2c1, address_DS3231<<1, 0x0E, 1, &control, 1, 100);  // read control register
		HAL_I2C_Mem_Write(&hi2c1, address_DS3231<<1, 0x0E, 1, (uint8_t *)(control|(0x20)), 1, 100);
	}
}
float Get_Temp(void)
{
	HAL_I2C_Mem_Read(&hi2c1, address_DS3231<<1, 0x11, I2C_MEMADD_SIZE_8BIT, temp, 2, 1000);
	return ( (temp[0]) +(temp[1]>>6)/4.0);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)		//Set Time
{
 if(GPIO_Pin == GPIO_PIN_0)
 {
	 send_time[0] = DECtoBCD(0);		//second
	 send_time[1] = DECtoBCD(39);		//minute
	 send_time[2] = DECtoBCD(21);		//hour 
	 
	 send_time[3] = DECtoBCD(3);		//day
	 send_time[4] = DECtoBCD(18);		//date
	 send_time[5] = DECtoBCD(2);		//month
	 send_time[6] = DECtoBCD(20);		//year
	 
	 HAL_I2C_Mem_Write_IT(&hi2c1, address_DS3231<<1, 0, I2C_MEMADD_SIZE_8BIT, send_time, 7);
 }
}

uint8_t BCDtoDEC(uint8_t data)
{
	return (int)( (data/16*10) + (data%16) );
}
uint8_t DECtoBCD(uint8_t data)
{
	return (uint8_t)( (data/10*16) + (data%10) );
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE3 PE4 PE5 
                           PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

void Horse_Running(void)
{
	/*Horse Running */
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse1, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse2, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse3, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse4, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse5, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse6, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse7, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse8, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse9, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, horse10, 128, 64, 1);
	SSD1306_UpdateScreen();
	
	SSD1306_Clear();
	SSD1306_UpdateScreen();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
