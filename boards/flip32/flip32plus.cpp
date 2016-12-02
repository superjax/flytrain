#include "flip32plus.h"

static uint32_t num_ticks;

void start_wall_clock(void)
{
    RCC_ClkInitTypeDef clkinitstruct = {0};
    RCC_OscInitTypeDef oscinitstruct = {0};

    // Use Internal Oscillator
    oscinitstruct.OscillatorType        = RCC_OSCILLATORTYPE_HSE;
    oscinitstruct.HSEPredivValue        = RCC_HSE_PREDIV_DIV1;
    oscinitstruct.HSICalibrationValue   = RCC_HSICALIBRATION_DEFAULT;
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

    start_wall_clock();

    LED1.init(LED1_GPIO_PORT, LED1_PIN);
    LED2.init(LED2_GPIO_PORT, LED2_PIN);



   // USART1 Initialization
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
}

uint32_t millis()
{
    uint32_t out = num_ticks;
    return out;
}

uint64_t micros()
{
    static uint32_t clock_MHz = HAL_RCC_GetHCLKFreq()/1000000;
    uint32_t ticks, cycles;
    do {
        ticks = num_ticks;
        cycles = SysTick->LOAD - SysTick->VAL;
    } while(ticks != num_ticks); // We might have had a SysTick interrupt while we were loading the tick registers.  If so, do it again

    return (uint64_t)ticks * 1000 + cycles/clock_MHz;
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
    while(micros() - start_time < us)
    {
        ;
    }
}

extern "C" void SysTick_Handler(void)
{
  num_ticks++;
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
