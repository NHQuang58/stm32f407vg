#include "stm32f4xx_hal.h"
#include "fonts.h"
#include "ssd1306.h"
#include "bitmap.h"
#include "horse_anim.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();

	SSD1306_Init();
	
	SSD1306_Fill(1);		//fill color 1 is blue
	SSD1306_UpdateScreen();
	
	SSD1306_GotoXY(10, 20);
	SSD1306_Puts("HELLO", &Font_7x10, 0);	
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts("WORLD", &Font_7x10, 0);
	SSD1306_UpdateScreen();
	
	HAL_Delay(2000);
	
	SSD1306_Fill(0);		//fill color 0 is black
	SSD1306_UpdateScreen();
	
	
	
  while (1)
  {
	/*SSD1306_Clear();
	SSD1306_DrawBitmap(0, 0, logo, 128, 64, 1);
	SSD1306_UpdateScreen();*/
	
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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);

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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
