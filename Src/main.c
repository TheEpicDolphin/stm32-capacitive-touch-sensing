/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
/* These values can be modified, board shield dependency */
//#define TSCx_MIN_THRESHOLD              1000
//#define TSCx_LOW_MAXTHRESHOLD           2845
//#define TSCx_MEDIUM_MAXTHRESHOLD        2640
//#define TSCx_HIGH_MAXTHRESHOLD          2500

#define TSCx_TS1_MINTHRESHOLD			30
#define TSCx_TS1_MAXTHRESHOLD			100
#define TSCx_TS2_MINTHRESHOLD			30
#define TSCx_TS2_MAXTHRESHOLD			100

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TSC_HandleTypeDef htsc;

/* USER CODE BEGIN PV */
/* Array used to store the acquisition value */
//volatile uint32_t uhTSCAcquisitionValue;
volatile uint32_t uhTSCAcquisitionValue[2];
uint8_t IdxBank = 0;
TSC_IOConfigTypeDef IoConfig;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TSC_Init(void);
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
  MX_TSC_Init();
  /* USER CODE BEGIN 2 */

  /*##-2- Configure the touch-sensing IOs ####################################*/
  IoConfig.ChannelIOs  = TSC_GROUP5_IO1; /* Start with the first channel */
  IoConfig.SamplingIOs = TSC_GROUP5_IO3;
  IoConfig.ShieldIOs = 0;

  if (HAL_TSC_IOConfig(&htsc, &IoConfig) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-1- Discharge the touch-sensing IOs ####################################*/
  HAL_TSC_IODischarge(&htsc, ENABLE);
  HAL_Delay(1); /* 1 ms is more than enough to discharge all capacitors */

  /*##-2- Start the acquisition process ######################################*/
  if (HAL_TSC_Start_IT(&htsc) != HAL_OK)
  {
    /* Acquisition Error */
    Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_SuspendTick();
	  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	  HAL_ResumeTick();
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief TSC Initialization Function
  * @param None
  * @retval None
  */
static void MX_TSC_Init(void)
{

  /* USER CODE BEGIN TSC_Init 0 */

  /* USER CODE END TSC_Init 0 */

  /* USER CODE BEGIN TSC_Init 1 */

  /* USER CODE END TSC_Init 1 */
  /** Configure the TSC peripheral 
  */
  htsc.Instance = TSC;
  htsc.Init.CTPulseHighLength = TSC_CTPH_2CYCLES;
  htsc.Init.CTPulseLowLength = TSC_CTPL_2CYCLES;
  htsc.Init.SpreadSpectrum = DISABLE;
  htsc.Init.SpreadSpectrumDeviation = 1;
  htsc.Init.SpreadSpectrumPrescaler = TSC_SS_PRESC_DIV1;
  htsc.Init.PulseGeneratorPrescaler = TSC_PG_PRESC_DIV4;
  htsc.Init.MaxCountValue = TSC_MCV_8191;
  htsc.Init.IODefaultMode = TSC_IODEF_OUT_PP_LOW;
  htsc.Init.SynchroPinPolarity = TSC_SYNC_POLARITY_FALLING;
  htsc.Init.AcquisitionMode = TSC_ACQ_MODE_NORMAL;
  htsc.Init.MaxCountInterrupt = DISABLE;
  htsc.Init.ChannelIOs = TSC_GROUP5_IO1|TSC_GROUP5_IO2;
  htsc.Init.ShieldIOs = 0;
  htsc.Init.SamplingIOs = TSC_GROUP5_IO3;
  if (HAL_TSC_Init(&htsc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TSC_Init 2 */

  /* USER CODE END TSC_Init 2 */

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
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_TSC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_TSC_IRQn);

}

/* USER CODE BEGIN 4 */

/**
  * @brief  Acquisition completed callback in non blocking mode
  * @param  htsc: pointer to a TSC_HandleTypeDef structure that contains
  *         the configuration information for the specified TSC.
  * @retval None
  */

void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc){
	  /*##-5- Discharge the touch-sensing IOs ####################################*/
	  HAL_TSC_IODischarge(htsc, ENABLE);
	  /* Note: a delay can be added here */

	  /*##-6- Check if the acquisition is correct (no max count) #################*/
	  if (HAL_TSC_GroupGetStatus(htsc, TSC_GROUP5_IDX) == TSC_GROUP_COMPLETED)
	  {
	    /*##-7- Read the acquisition value #######################################*/
	    uhTSCAcquisitionValue[IdxBank] = HAL_TSC_GroupGetValue(htsc, TSC_GROUP5_IDX);
	    /* Note: Show the touch activity on LEDs.
	       The threshold values are indicative and may need to be adjusted */
	    if(IdxBank == 0){
	    	if ((uhTSCAcquisitionValue[0] > TSCx_TS1_MINTHRESHOLD) && (uhTSCAcquisitionValue[0] < TSCx_TS1_MAXTHRESHOLD)) // Channel 1
	    	{
	    		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	    	}
	    	else
	    	{
	    		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	    	}
	    }
	    else{
	    	if ((uhTSCAcquisitionValue[1] > TSCx_TS2_MINTHRESHOLD) && (uhTSCAcquisitionValue[1] < TSCx_TS2_MAXTHRESHOLD)) // Channel 2
	    	{
	    		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	    	}
	    	else
	    	{
	    		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	    	}
	    }

	  }


	  /*##-8- Configure the next channels to be acquired #########################*/

	  if (IdxBank == 0)
	  {
	    IoConfig.ChannelIOs = TSC_GROUP5_IO2; /* TS2 touchkey */
	    IdxBank = 1;
	  }
	  else
	  {
	    IoConfig.ChannelIOs = TSC_GROUP5_IO1; /* TS1 touchkey */
	    IdxBank = 0;
	  }


	  if (HAL_TSC_IOConfig(htsc, &IoConfig) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-9- Re-start the acquisition process ###################################*/
	  if (HAL_TSC_Start_IT(htsc) != HAL_OK)
	  {
	    /* Acquisition Error */
	    Error_Handler();
	  }
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
