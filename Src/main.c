/**
 * Keil project for SDCARD reader using SDIO/SDMMC example
 *
 * Before you start, select your target, on the right of the "Load" button
 *
 * @author    Tilen Majerle
 * @email     tilen@majerle.eu
 * @website   http://stm32f4-discovery.com
 * @ide       Keil uVision 5
 * @conf      PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 * @packs     STM32F4xx/STM32F7xx Keil packs are requred with HAL driver support
 * @stdperiph STM32F4xx/STM32F7xx HAL drivers required
 */

#include "defines.h"
#include "stdio.h"

/* Include core modules */
#include "stm32fxxx_hal.h"

/* Include my libraries here */
#include "tm_stm32_disco.h"
#include "tm_stm32_delay.h"
#include "tm_stm32_fatfs.h"
#include "tm_stm32_usart.h"

static void SystemClock_Config(void);
static void Error_Handler(void);

/* Fatfs structure */
FATFS FS;
FIL fil;
FRESULT fres;

/* Size structure for FATFS */
TM_FATFS_Size_t CardSize;

/* Buffer variable */
char buffer[100];

int main(void) {
    uint8_t rtext[100];
    unsigned int bytesread = 0;
  
	/* Init HAL layer */
	HAL_Init();
	
	/* Init system clock for maximum system speed */
	SystemClock_Config(); //TM_RCC_InitSystem();

	/* Init leds */
	TM_DISCO_LedInit();
    TM_DISCO_LedOn(LED_ORANGE);
    
    /* Init UART comm */
    /* Init USART, TX: PD5, RX: PD6, 115200 bauds */
    TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
    TM_USART_Puts(USART1, (char*) MSG);
	TM_DISCO_LedOn(LED_GREEN);
	/* Try to mount card */


	if (f_mount(&FS, "SD:", 1) == FR_OK) {
#if 0		/* Try to open file */
		if ((fres = f_open(&fil, "SD:first_file.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE)) == FR_OK) {
			/* Read SDCARD size */
			TM_FATFS_GetDriveSize("SD:", &CardSize);
			
			/* Format string */
			sprintf(buffer, "Total card size: %u kBytes\n", (unsigned int)CardSize.Total+10);
			
			/* Write total card size to file */
			f_puts(buffer, &fil);
			
			/* Format string for free card size */
			sprintf(buffer, "Free card size:  %u kBytes\n", (unsigned int)CardSize.Free);
			
			/* Write free card size to file */
			f_puts(buffer, &fil);
			
			/* Close file */
			f_close(&fil);
			
            /* read file contents */
            if ((fres = f_open(&fil, "SD:first_file.txt", FA_READ)) == FR_OK) {
                /* Read data from the text file */
                fres = f_read(&fil, rtext, sizeof(rtext), (void *)&bytesread);
          
                if((bytesread == 0) || (fres != FR_OK))
                {
                    /* file Read or EOF Error */
                    TM_DISCO_LedOn(LED_RED);
                } else {
                    rtext[bytesread]='\0';
                    TM_USART_Puts(USART2, (char*) rtext);
                }
                f_close(&fil);
            }
			/* Turn led ON */
			TM_DISCO_LedOn(LED_GREEN);
		} else {
            TM_DISCO_LedOn(LED_RED);
        }
		TM_DISCO_LedOff(LED_ORANGE);
#endif
		/* Unmount SDCARD */
		f_mount(NULL, "SD:", 1);
	} else {
        TM_DISCO_LedOff(LED_GREEN);
        TM_DISCO_LedOn(LED_RED);
    }

	/* Do nothing */
	while (1) {
		
	}
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED5 on */
  TM_DISCO_LedOff(LED_GREEN);
  TM_DISCO_LedOn(LED_RED);
  while(1)
  {
  }
}
