################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main_timer_pwm.c 

OBJS += \
./src/main_timer_pwm.o 

C_DEPS += \
./src/main_timer_pwm.d 


# Each subdirectory must supply rules for building sources it contributes
src/main_timer_pwm.o: ../src/main_timer_pwm.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DSTKG8XX=1' '-DEFM32G890F128=1' '-DDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/an/an0014_efm32_timer" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/EnergyMicro/EFM32G/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/main_timer_pwm.d" -MT"src/main_timer_pwm.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


