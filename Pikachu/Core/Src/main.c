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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
#define INACTIVITY_TIMEOUT 300000
static uint32_t inactivity_timer = 0;

bool isSecondlayer = false;

#define ROW_COUNT    2
#define COL_COUNT    5
#define MCU1_ADDRESS  0x01
#define MCU2_ADDRESS  0x02
#define MCU3_ADDRESS  0x03

uint16_t rowPins[ROW_COUNT] = {R1_Pin, R2_Pin};
uint16_t colPins[COL_COUNT] = {C1_Pin, C2_Pin, C3_Pin, C4_Pin, C5_Pin};

uint8_t layer0[ROW_COUNT][COL_COUNT] = {
    {0x14, 0x1A, 0x08, 0x15, 0x17},  
    {0x04, 0x16, 0x07, 0x09, 0x0A}  
};

uint8_t layer1[ROW_COUNT][COL_COUNT] = {
    {0x04, 0x16, 0x19, 0x1E, 0x1F},  
    {0x20, 0x21, 0x22, 0x23, 0x24}
};

uint8_t layer2[ROW_COUNT][COL_COUNT] = {
    {0x04, 0x16, 0x19, 0x1E, 0x1F},  
    {0x20, 0x21, 0x22, 0x23, 0x24}
};
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void UART_Receive_Data(void);
static void Send_HID_Key(uint8_t key);
static void scan_keypad(void);

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
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      inactivity_timer++;
	  scan_keypad();
	  UART_Receive_Data();

	  if (inactivity_timer >= INACTIVITY_TIMEOUT)
	          {
	              Enter_Sleep_Mode();
	          }
	  inactivity_timer = 0;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : R1_Pin R2_Pin C1_Pin C2_Pin
                           C3_Pin C4_Pin C5_Pin */
  GPIO_InitStruct.Pin = R1_Pin|R2_Pin|C1_Pin|C2_Pin
                          |C3_Pin|C4_Pin|C5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

void scan_keypad(void) {
    for (int row = 0; row < ROW_COUNT; row++) {
        HAL_GPIO_WritePin(GPIOA, rowPins[row], GPIO_PIN_SET);  // Set row to high

        for (int col = 0; col < COL_COUNT; col++) {
            if (HAL_GPIO_ReadPin(GPIOA, colPins[col]) == GPIO_PIN_RESET) {  // Check if key is pressed
            	
            	if(isSecondlayer){
            		key_code = layer1[row][col];
            	}
            	else{
            		key_code = layer0[row][col];
            	}
            	
            	inactivity_timer = 0;
            }
        }

        HAL_GPIO_WritePin(GPIOA, rowPins[row], GPIO_PIN_RESET);  // Reset row to low
        HAL_Delay(10);  // Debouncing delay
    }
    HAL_Delay(100);  // Additional debounce or delay between scans
}

void Send_HID_Key(uint8_t key) {
    uint8_t HID_report[8] = {0};  // HID report with a size of 8 bytes

    // Set the key in the HID report (the key code for 'W', 'A', 'S', 'D')
    HID_report[2] = key;  // Using byte 2 to represent the key pressed

    // Send the HID report (press key)
    if (hpcd_USB_FS.State == HAL_PCD_STATE_READY) {
        USBD_HID_SendReport(&hUsbDeviceFS, HID_report, sizeof(HID_report));
    }

    // Delay to simulate key press duration
    HAL_Delay(100);

    // Release the key (send 0)
    HID_report[2] = 0;
    if (hpcd_USB_FS.State == HAL_PCD_STATE_READY) {
        USBD_HID_SendReport(&hUsbDeviceFS, HID_report, sizeof(HID_report));
    }
}

void UART_Receive_Data(void) {
	uint8_t received_data[2];

	if (HAL_UART_Receive(&huart1, (uint8_t *)received_data, 2, 100) == HAL_OK) {
		uint8_t received_address = received_data[0];
        uint8_t data = received_data[1];

        inactivity_timer = 0;

        if (received_address == MCU1_ADDRESS) {
        	switch (data)
        	{
        	case 'Z':
        		Send_HID_Key(0x1D);
        		break;

        	case 'X':
        		Send_HID_Key(0x1B);
        	    break;

        	case 'C':
        	    Send_HID_Key(0x06);
        	    break;

        	case 'V':
        	    Send_HID_Key(0x19);
        	    break;

        	case 'M':
        	    Send_HID_Key(0x2C);
        	    break;

        	case 'M':
        		Send_HID_Key(0x2C);
        	    break;

        	case 'N':
        		isSecondlayer = !isSecondlayer;
        	    break;
    }
}
        if(received_address == MCU2_ADDRESS){
        	switch(data){

        	case 'Y':
        		Send_HID_Key(0x1C);
        	    break;

        	case 'U':
        		Send_HID_Key(0x18);
        		break;

        	case 'I':
        		Send_HID_Key(0x0C);
        		break;

        	case 'O':
        		Send_HID_Key(0x12);
        		break;

        	case 'P':
        		Send_HID_Key(0x13);
        		break;

        	case 'H':
        		Send_HID_Key(0x0B);
        		break;

        	case 'J':
        		Send_HID_Key(0x0D);
        		break;

        	case 'K':
        		Send_HID_Key(0x0E);
        		break;

        	case 'L':
        		Send_HID_Key(0x0F);
        		break;
        	}
}
        if(received_data == MCU3_ADDRESS){
        	switch(data){

        	case 'B':
        		Send_HID_Key(0x05);
        		break;

        	case 'N':
        		Send_HID_Key(0x11);
        		break;

        	case 'M':
        		Send_HID_Key(0x10);
        		break;

        	case 'A':
        		Send_HID_Key(0x2C);
        		break;

        	case 'A':
        		Send_HID_Key(0x2C);
        		break;

        	case 'V':
        		Send_HID_Key(0x2C);
        		break;
        	}
        }
}

	void Enter_Sleep_Mode(void)
	{
	    // Optional: Disable peripherals if needed before entering sleep mode

	    // Enter Sleep mode (low power mode)
	    HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	}

	// Need to add turn off and that shld sent through uart to other 3 mcus to turnf of too


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
