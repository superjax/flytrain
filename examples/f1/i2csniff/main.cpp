#define USE_FULL_ASSERT

#include "stm32f1xx_hal.h"
#include "flip32plus.h"

void SystemClock_Config(void);

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    __HAL_RCC_AFIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();
    LED2_GPIO_CLK_ENABLE();

    // Free up LED pins (used by the JTAG unless they are turned off)
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED2_PIN;
    HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);

    while (1)
    {
      HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED1_PIN);
      /* Insert delay 100 ms */
      HAL_Delay(100);
      HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
      /* Insert delay 100 ms */
      HAL_Delay(100);
    }
}


void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef clkinitstruct = {0};
    RCC_OscInitTypeDef oscinitstruct = {0};

    /* Configure PLLs ------------------------------------------------------*/
    /* PLL2 configuration: PLL2CLK = (HSE / HSEPrediv2Value) * PLL2MUL = (25 / 5) * 8 = 40 MHz */
    /* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8 MHz */
    /* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    oscinitstruct.OscillatorType        = RCC_OSCILLATORTYPE_HSI;
//    oscinitstruct.HSEState              = RCC_HSE_OFF;
    oscinitstruct.HSICalibrationValue   = RCC_HSICALIBRATION_DEFAULT;
    oscinitstruct.HSIState              = RCC_HSI_ON;
//    oscinitstruct.Prediv1Source         = RCC_PREDIV1_SOURCE_PLL2;
//    oscinitstruct.PLL.PLLState          = RCC_PLL_ON;
//    oscinitstruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
//    oscinitstruct.PLL.PLLMUL            = RCC_PLL_MUL9;
//    oscinitstruct.PLL2.PLL2State        = RCC_PLL2_ON;
//    oscinitstruct.PLL2.PLL2MUL          = RCC_PLL2_MUL8;
//    oscinitstruct.PLL2.HSEPrediv2Value  = RCC_HSE_PREDIV2_DIV5;
    if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
    clkinitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
    clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
extern "C" void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
