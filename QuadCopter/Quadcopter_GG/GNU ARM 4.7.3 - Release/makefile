################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/efm32gg/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Quadcopter_GG.axf

# Tool invocations
Quadcopter_GG.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb  --specs=nano.specs -Xlinker --gc-sections -Xlinker -Map="Quadcopter_GG.map" -T "Quadcopter_GG.ld" -o Quadcopter_GG.axf "./src/I2C.o" "./src/MPU6050.o" "./src/PID.o" "./src/UART.o" "./src/XBEE.o" "./src/main.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_lcd.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./Drivers/segmentlcd.o" "./Drivers/vddcheck.o" "./CMSIS/efm32gg/startup_gcc_efm32gg.o" "./CMSIS/efm32gg/system_efm32gg.o" "./BSP/bsp_bcc.o" "./BSP/bsp_stk.o" "./BSP/bsp_stk_leds.o" "./BSP/bsp_trace.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: Quadcopter_GG.hex'
	arm-none-eabi-objcopy -O ihex "Quadcopter_GG.axf" "Quadcopter_GG.hex"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Quadcopter_GG.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Quadcopter_GG.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
