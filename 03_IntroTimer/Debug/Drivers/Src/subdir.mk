################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/exti_driver_hal.c \
../Drivers/Src/gpio_driver_hal.c \
../Drivers/Src/timer_driver_hal.c 

OBJS += \
./Drivers/Src/exti_driver_hal.o \
./Drivers/Src/gpio_driver_hal.o \
./Drivers/Src/timer_driver_hal.o 

C_DEPS += \
./Drivers/Src/exti_driver_hal.d \
./Drivers/Src/gpio_driver_hal.d \
./Drivers/Src/timer_driver_hal.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o Drivers/Src/%.su: ../Drivers/Src/%.c Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I../Inc -I"/home/edison/STM32CubeIDE/CMSIS-repo/STM32Cube_FW_F4_V1.27.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/edison/STM32CubeIDE/CMSIS-repo/STM32Cube_FW_F4_V1.27.0/Drivers/CMSIS/Include" -I/home/edison/STM32CubeIDE/03_IntroTimer/Drivers/Inc -I/home/edison/STM32CubeIDE/03_IntroTimer/Drivers/Src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Src

clean-Drivers-2f-Src:
	-$(RM) ./Drivers/Src/exti_driver_hal.d ./Drivers/Src/exti_driver_hal.o ./Drivers/Src/exti_driver_hal.su ./Drivers/Src/gpio_driver_hal.d ./Drivers/Src/gpio_driver_hal.o ./Drivers/Src/gpio_driver_hal.su ./Drivers/Src/timer_driver_hal.d ./Drivers/Src/timer_driver_hal.o ./Drivers/Src/timer_driver_hal.su

.PHONY: clean-Drivers-2f-Src

