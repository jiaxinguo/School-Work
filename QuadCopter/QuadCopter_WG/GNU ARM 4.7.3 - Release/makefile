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
-include CMSIS/efm32wg/subdir.mk
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
all: QuadCopter_WG.axf

# Tool invocations
QuadCopter_WG.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb  --specs=nano.specs -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Xlinker --gc-sections -Xlinker -Map="QuadCopter_WG.map" -T "QuadCopter_WG.ld" -o QuadCopter_WG.axf "./src/I2C.o" "./src/MPU6050.o" "./src/PID.o" "./src/UART.o" "./src/XBEE.o" "./src/main.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_lcd.o" "./emlib/em_system.o" "./emlib/em_timer.o" "./emlib/em_usart.o" "./Drivers/segmentlcd.o" "./Drivers/vddcheck.o" "./CMSIS/efm32wg/startup_gcc_efm32wg.o" "./CMSIS/efm32wg/system_efm32wg.o" "./BSP/bsp_bcc.o" "./BSP/bsp_stk.o" "./BSP/bsp_stk_leds.o" "./BSP/bsp_trace.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: QuadCopter_WG.hex'
	arm-none-eabi-objcopy -O ihex "QuadCopter_WG.axf" "QuadCopter_WG.hex"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "QuadCopter_WG.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) QuadCopter_WG.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
