* This is a template makefile project that uses latest STM32F4 Hal libraries and can be compiled on any linux OS

* Prerequisites:
  This project supposes you have on your PC :
  > make and arm-none-eabi- toolchain  (Can be easily updated in Makefile for custom versions)
  > STM32Cube_FW_F4_V1.11.0 Hal package from st.com (can be easily updated in Makefile for custom versions)
  > Linux version of stlink package/driver (Can be updated in Makefile for custom versions)

* Features:
  This project provides the possibility to add source files (from HAL, or custom source files like libraries/user code)
  and the possibility to build and burn the code directly to a STM32F4-Discovery board connected via USB to your PC
  The project also provides the possibility to debug the app code step by step using debug makefilet target

* How to use:
  - BUILD - using make clean all
  - BURN  - using make burn
  - DEBUG - using make debug

* Usage Terms/License :
  I here by grant all users of this project code to use it without limits, including copy, update, distribute and sell
  without any needed code or mentions to add, except for the original files coming from STMicroelectronics HAL package
  which should be used as described in the original License of the HAL package.

* Contact:
  For any updates or remarks feel free to reach me at oussema.elharbi@gmail.com
