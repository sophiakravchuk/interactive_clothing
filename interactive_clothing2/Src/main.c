/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SystemCoreClockInMHz (SystemCoreClock/1000000);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay_L(void) {
    int useconds = 10;
    asm volatile("   mov r0, %[useconds]    \n"
                 "1: subs r0, #1            \n"
                 "   bhi 1b                 \n": :[useconds] "r" (useconds): "r0");
}
void delay_H(void) {
    int useconds = 20;
    asm volatile("   mov r0, %[useconds]    \n"
                 "1: subs r0, #1            \n"
                 "   bhi 1b                 \n":: [useconds] "r" (useconds): "r0");
}

inline void send_byte(int a) {
    for (int i = 7; i >= 0; i--) {
        int x = (a >> i) & 1;
        if (x == 1) {
        	GPIOB->BSRR = GPIO_PIN_8;
			delay_H();
			GPIOB->BSRR = (uint32_t) GPIO_PIN_8 << 16U;
			delay_L();
        } else {
			GPIOB->BSRR = GPIO_PIN_8;
			delay_L();
			GPIOB->BSRR = (uint32_t) GPIO_PIN_8 << 16U;
			delay_H();
        }
    }
}

void light(float distance) {
    distance = (int) distance;
    for (int i = 0; i < distance; i++) {
        int num = i % 7;
        if (num == 0) { //red
            send_byte(0x00);
            send_byte(0xFF);
            send_byte(0x00);
        } else if (num == 1) { //orange
             send_byte(0x45);
             send_byte(0xFF);
             send_byte(0x00);
        } else if (num == 2) { //yellow
            send_byte(0xFF);
            send_byte(0xFF);
            send_byte(0x00);
        } else if (num == 3) { //green
            send_byte(0xFF);
            send_byte(0x00);
            send_byte(0x00);
        } else if (num == 4) { //lightblue
            send_byte(0xBF);
            send_byte(0x00);
            send_byte(0xFF);
        } else if (num == 5) { //blue
            send_byte(0x00);
            send_byte(0x00);
            send_byte(0xFF);
        } else { //purple
            send_byte(0x2B);
            send_byte(0x8A);
            send_byte(0xE2);
        }
    }
}

int DWT_Init(void) {
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    DWT->CYCCNT = 0;
    uint32_t before = DWT->CYCCNT;
    __NOP();
    __NOP();
    return (DWT->CYCCNT - before);
}

inline void udelay_DWT (uint32_t useconds) {
    useconds *= SystemCoreClockInMHz;
    while( DWT->CYCCNT < useconds){}
}

inline uint32_t get_DWT_cycles()
{
    return DWT->CYCCNT;
}

inline uint32_t get_DWT_us()
{
    return get_DWT_cycles()/SystemCoreClockInMHz;
}

inline void reset_DWT_cycles()
{
    DWT->CYCCNT = 0;
}

inline uint32_t get_us(){
    return get_DWT_us();
}

inline void udelay (uint32_t useconds){
    udelay_DWT(useconds);
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
  /* USER CODE BEGIN 2 */
  DWT_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
  int distance_cm;
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
        while (1)
        {
            while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8) == GPIO_PIN_SET )
            {
                HAL_Delay(300);
            }
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
            if ( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8) != GPIO_PIN_SET )
            {
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_Delay(300);
                continue;
            }
            udelay(16);
            HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
            if ( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8) != GPIO_PIN_RESET )
            {
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_Delay(300);
                continue;
            }
            if ( HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_SET )
            {
                HAL_Delay(300);
                continue;
            }

            uint32_t watchdog_begin = get_us();
            int didnt_had_1_at_echo = 0;
            while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_RESET )
            {
                if( get_us() - watchdog_begin > 500000 )
                {
                    didnt_had_1_at_echo = 1;
                    break;
                }
            }
            if(didnt_had_1_at_echo)
            {
                HAL_Delay(300);
                continue;
            }

            uint32_t before = get_us();
            int didnt_had_0_at_echo = 0;
            while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_SET )
            {
                if( get_us() - watchdog_begin > 500000 )
                {
                    didnt_had_0_at_echo = 1;
                    break;
                }
            }
            if(didnt_had_0_at_echo)
            {
                HAL_Delay(300);
                continue;
            }

            uint32_t pulse_time = get_us()-before;
            uint32_t distance_mm = (int)((float)pulse_time/5.8);
            if(distance_mm > 30 && distance_mm < 5000) {
                if (distance_cm != distance_mm) {

                    light((float)distance_mm / 10.0);
                    distance_cm = distance_mm;
                }
                udelay(10000);
            }
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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
