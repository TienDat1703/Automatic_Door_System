#include "main.h"

// --- CẤU HÌNH (Dễ dàng chỉnh sửa nếu phần cứng thay đổi) ---
#define DOOR_DELAY_MS        3000            // Thời gian chờ đóng cửa (3 giây)

uint32_t last_motion_time = 0; // Lưu thời điểm cuối cùng phát hiện chuyển động

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

// --- CÁC HÀM ĐIỀU KHIỂN ĐỘNG CƠ ---
void Stop_Motor() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

void Open_Door() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);   // Chiều Mở
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

void Close_Door() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);   // Chiều Đóng
}

// Biến debug (để xem trong Live Expression nếu cần)
uint8_t status_closed_ls1, status_opened_ls2, status_pir;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  Stop_Motor();

  // Khởi tạo thời gian ban đầu để tránh lỗi logic khi vừa bật nguồn
  last_motion_time = HAL_GetTick();

  while (1)
  {
      // 1. ĐỌC CẢM BIẾN
      uint8_t pir1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);
      uint8_t pir2 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4);

      // LS1: Chạm = Cửa đã Đóng hoàn toàn
      uint8_t limit_LS1 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
      // LS2: Chạm = Cửa đã Mở hoàn toàn
      uint8_t limit_LS2 = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);

      // Cập nhật biến debug
      status_closed_ls1 = limit_LS1;
      status_opened_ls2 = limit_LS2;
      status_pir = (pir1 == GPIO_PIN_SET || pir2 == GPIO_PIN_SET) ? 1 : 0;
      // Debug LED (nếu cần)
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, status_pir ? GPIO_PIN_SET : GPIO_PIN_RESET);

      // 2. XỬ LÝ LOGIC

      // KIỂM TRA CÓ NGƯỜI
      if (status_pir) {
          // Reset bộ đếm thời gian ngay lập tức
          last_motion_time = HAL_GetTick();

          // AN TOÀN: Bất kể đang làm gì, nếu có người -> Mở cửa ngay
          if (limit_LS2 != GPIO_PIN_RESET) {
              Open_Door();
          } else {
              Stop_Motor(); // Đã mở hết cỡ
          }
      }
      // KHÔNG CÓ NGƯỜI
      else {
          // Kiểm tra xem còn trong thời gian chờ (3s) hay không
          if (HAL_GetTick() - last_motion_time < DOOR_DELAY_MS) {
              // Vẫn còn thời gian chờ:
              // Logic tối ưu: Nếu cửa chưa mở hết, hãy mở cho hết (để tránh cửa lơ lửng)
              // Sau đó giữ trạng thái mở chờ hết 3s.
              if (limit_LS2 != GPIO_PIN_RESET) {
                  Open_Door();
              } else {
                  Stop_Motor();
              }
          }
          else {
              // ĐÃ HẾT THỜI GIAN CHỜ -> ĐÓNG CỬA
              if (limit_LS1 != GPIO_PIN_RESET) {
                  Close_Door();
              } else {
                  Stop_Motor(); // Đã đóng kín
              }
          }
      }

      HAL_Delay(10); // Giảm tải CPU và khử nhiễu nhẹ
  }
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
