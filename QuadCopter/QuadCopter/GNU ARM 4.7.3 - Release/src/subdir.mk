################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/I2C.c \
../src/MPU6050.c \
../src/UART.c \
../src/XBEE.c \
../src/main.c 

OBJS += \
./src/I2C.o \
./src/MPU6050.o \
./src/UART.o \
./src/XBEE.o \
./src/main.o 

C_DEPS += \
./src/I2C.d \
./src/MPU6050.d \
./src/UART.d \
./src/XBEE.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/I2C.o: ../src/I2C.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DDEBUG_EFM=1' '-DEFM32ZG222F32=1' '-DNDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/textdisplay" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/EnergyMicro/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -O3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/I2C.d" -MT"src/I2C.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MPU6050.o: ../src/MPU6050.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DDEBUG_EFM=1' '-DEFM32ZG222F32=1' '-DNDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/textdisplay" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/EnergyMicro/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -O3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/MPU6050.d" -MT"src/MPU6050.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/UART.o: ../src/UART.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DDEBUG_EFM=1' '-DEFM32ZG222F32=1' '-DNDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/textdisplay" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/EnergyMicro/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -O3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/UART.d" -MT"src/UART.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/XBEE.o: ../src/XBEE.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DDEBUG_EFM=1' '-DEFM32ZG222F32=1' '-DNDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/textdisplay" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/EnergyMicro/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -O3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/XBEE.d" -MT"src/XBEE.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/main.o: ../src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb '-DDEBUG_EFM=1' '-DEFM32ZG222F32=1' '-DNDEBUG=1' -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/examples/textdisplay" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/CMSIS/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/emlib/inc" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/bsp" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/common/drivers" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/Device/EnergyMicro/EFM32ZG/Include" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/glib" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/reptile/glib/dmd/ssd2119" -I"C:\SiliconLabs\SimplicityStudio\v2\developer\sdks\efm32\v2/kits/EFM32ZG_STK3200/config" -O3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"src/main.d" -MT"src/main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


