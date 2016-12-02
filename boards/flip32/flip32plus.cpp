#include "flip32plus.h"

void start_wall_clock(void)
{
    RCC_ClkInitTypeDef clkinitstruct = {0};
    RCC_OscInitTypeDef oscinitstruct = {0};

    // Use Internal Oscillator
    oscinitstruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
    oscinitstruct.HSEPredivValue        = RCC_HSE_PREDIV_DIV1;
    oscinitstruct.HSICalibrationValue   = RCC_HSICALIBRATION_DEFAULT;
    oscinitstruct.HSIState              = RCC_HSI_OFF;
    oscinitstruct.HSEState              = RCC_HSE_ON;
    oscinitstruct.PLL.PLLSource         = RCC_PLLSOURCE_HSE;
    oscinitstruct.PLL.PLLMUL            = RCC_PLL_MUL16;
    oscinitstruct.PLL.PLLState          = RCC_PLL_ON;
    if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }

    // Set Interal Oscillator as clock source
    clkinitstruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
    clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
    {
        /* Initialization Error */
        while(1);
    }
}

void hardware_init()
{
    HAL_Init();

    // Turn on the AFIO Clock
    __HAL_RCC_AFIO_CLK_ENABLE();

    // Free up LED pins (used by the JTAG unless they are turned off)
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;

    // GPIOB for LEDs
    __HAL_RCC_GPIOB_CLK_ENABLE();

}

uint32_t millis()
{
    uint32_t out = HAL_GetTick();
    return out;
}

uint64_t micros()
{
    volatile uint64_t ticks = millis();
    volatile uint64_t clock = HAL_RCC_GetHCLKFreq();
    volatile uint64_t val = SysTick->VAL;
    volatile uint64_t us_from_ms = ticks*1000;
    volatile uint64_t val_backwards =  ((uint64_t)SysTick->VAL * 1000000) / HAL_RCC_GetHCLKFreq();
    volatile uint64_t us_from_val = 1000 - ((uint64_t)SysTick->LOAD - SysTick->VAL) / HAL_RCC_GetHCLKFreq();
    volatile uint64_t out = (uint64_t)(millis()) * 1000 + us_from_val;

    static uint64_t last = out;

    if(out < last)
        volatile int err = 0;
    if(out - last > 15)
        volatile int err = 1;
    last = out;
    return out;
}

void delay_ms(uint32_t ms)
{
    uint32_t start_time = millis();
    while(millis() - start_time < ms)
    {
        ;
    }
}

void delay_us(uint64_t us)
{
    uint64_t start_time = micros();
    uint64_t now = micros();
    while(now - start_time < us)
    {
        now = micros();
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
