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
-include CMSIS/efm32zg/subdir.mk
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
all: EFM32ZG_spaceinvaders.axf

# Tool invocations
EFM32ZG_spaceinvaders.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb  --specs=nano.specs -Xlinker --gc-sections -Xlinker -Map="EFM32ZG_spaceinvaders.map" -T "EFM32ZG_spaceinvaders.ld" -o EFM32ZG_spaceinvaders.axf "./src/capsense_spaceinvaders.o" "./src/game.o" "./src/main.o" "./src/render.o" "./src/spaceinvaders.o" "./emlib/em_acmp.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_int.o" "./emlib/em_rtc.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./Drivers/display.o" "./Drivers/displayls013b7dh03.o" "./Drivers/displaypalemlib.o" "./Drivers/udelay.o" "./CMSIS/efm32zg/startup_gcc_efm32zg.o" "./CMSIS/efm32zg/system_efm32zg.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: EFM32ZG_spaceinvaders.hex'
	arm-none-eabi-objcopy -O ihex "EFM32ZG_spaceinvaders.axf" "EFM32ZG_spaceinvaders.hex"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "EFM32ZG_spaceinvaders.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) EFM32ZG_spaceinvaders.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
