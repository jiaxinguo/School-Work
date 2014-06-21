################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/segmentlcd.c \
../Drivers/vddcheck.c 

OBJS += \
./Drivers/segmentlcd.o \
./Drivers/vddcheck.o 

C_DEPS += \
./Drivers/segmentlcd.d \
./Drivers/vddcheck.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/segmentlcd.o: ../Drivers/segmentlcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb '-DDEBUG_EFM=1' '-DEFM32WG990F256=1' '-DNDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32WG_STK3800/examples/blink" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/SiliconLabs/EFM32WG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32WG_STK3800/config" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -O3 -Wall -c -fmessage-length=0 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"Drivers/segmentlcd.d" -MT"Drivers/segmentlcd.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/vddcheck.o: ../Drivers/vddcheck.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb '-DDEBUG_EFM=1' '-DEFM32WG990F256=1' '-DNDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32WG_STK3800/examples/blink" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/SiliconLabs/EFM32WG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32WG_STK3800/config" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -O3 -Wall -c -fmessage-length=0 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"Drivers/vddcheck.d" -MT"Drivers/vddcheck.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


