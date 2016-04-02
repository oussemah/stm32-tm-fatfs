#define TARGET Name
TARGET=test_app

#define TOOLCHAIN
CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

STLINK=/opt/STM/stlink
DEBUGGER=arm-none-eabi-gdb

LIB_PATH   ?=/usr/lib/gcc/arm-none-eabi/4.9.3

HAL_PATH   ?=/usr/local/workspace/stm32/fw_repo/STM32Cube_FW_F4_V1.11.0/Drivers/STM32F4xx_HAL_Driver
CMSIS_PATH ?=/usr/local/workspace/stm32/fw_repo/STM32Cube_FW_F4_V1.11.0/Drivers/CMSIS

TM_PATH    ?=/usr/local/workspace/stm32/fw_repo/TM

#define INCLUDES
BASE_INCLUDES= -I$(LIB_PATH)/include-fixed -L$(LIB_PATH)/thumb,-lc

HAL_INCLUDES= -I$(HAL_PATH)/Inc -I$(CMSIS_PATH)/Include -I$(CMSIS_PATH)/Device/ST/STM32F4xx/Include

CUSTOM_INCLUDES= -I$(TM_PATH) -I$(TM_PATH)/fatfs -I$(TM_PATH)/fatfs/drivers

#define CFLAGS
DBG = -Os -fdata-sections -ffunction-sections -Wl,--gc-sections
ifneq (,$(DEBUG))
DBG = -g
endif

CFLAGS = $(DBG) -Wall -TSTM32F407VGTx_FLASH.ld -DUSE_STD_PERIPH_DRIVER
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=soft
CFLAGS += -IInc $(CUSTOM_INCLUDES) $(HAL_INCLUDES) $(BASE_INCLUDES)

CFLAGS += -DSTM32F4_DISCOVERY -DSTM32F407xx -DUSE_HAL_DRIVER -DSTM32F4xx -DHSE_VALUE=8000000

#define SRC FILES
SRCS  = startup_stm32f407xx.s
SRCS += Src/system_stm32f4xx.c
SRCS += Src/stm32fxxx_it.c
SRCS += Src/stm32f4xx_newlib_stubs.c

SRCS += $(HAL_PATH)/Src/stm32f4xx_hal.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_gpio.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_rcc.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_uart.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_cortex.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_dma.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_hal_sd.c
SRCS += $(HAL_PATH)/Src/stm32f4xx_ll_sdmmc.c

SRCS += $(TM_PATH)/tm_stm32_rcc.c
SRCS += $(TM_PATH)/tm_stm32_gpio.c
SRCS += $(TM_PATH)/tm_stm32_delay.c
SRCS += $(TM_PATH)/tm_stm32_disco.c
SRCS += $(TM_PATH)/tm_stm32_buffer.c
SRCS += $(TM_PATH)/tm_stm32_usart.c
SRCS += $(TM_PATH)/tm_stm32_fatfs.c

SRCS += $(TM_PATH)/fatfs/ff.c
SRCS += $(TM_PATH)/fatfs/diskio.c
SRCS += $(TM_PATH)/fatfs/drivers/fatfs_sd_sdio.c
SRCS += $(TM_PATH)/fatfs/option/syscall.c
SRCS += $(TM_PATH)/fatfs/option/ccsbcs.c

SRCS += Src/main.c

#define build targets
$(TARGET).elf:
	$(CC) $(CFLAGS) $(SRCS) -o $@
	@$(OBJCOPY) -O ihex $(TARGET).elf $(TARGET).hex
	@$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

all: $(TARGET).elf
	@echo -e "\033[0;93mBuild Successful\033[0;0m"

clean:
	rm -rf *.o *.elf *.hex *.bin 2>/dev/null

burn:$(TARGET).bin
	$(STLINK)/st-flash write $(TARGET).bin 0x8000000

debug:$(TARGET).bin
	$(DEBUGGER) $(TARGET).elf -ex "target remote :4242" -ex "b main" -ex "continue"
