################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lib_src/Time/subdir.mk
-include lib_src/SoftwareSerial/subdir.mk
-include lib_src/RCSwitch/subdir.mk
-include lib_src/HomeAutoLight/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
FLASH_IMAGE += \
Aduino_Vehicle_Project.hex \


# All Target
all: Aduino_Vehicle_Project.elf secondary-outputs

# Tool invocations
Aduino_Vehicle_Project.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc -s -Os -o"Aduino_Vehicle_Project.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,Aduino_Vehicle_Project.map,--cref -Wl,--gc-sections -L"C:\Users\USER\Documents\Workspace\ArduinoCore\328P_16MHz" -mmcu=atmega328p
	@echo 'Finished building target: $@'
	@echo ' '

Aduino_Vehicle_Project.hex: Aduino_Vehicle_Project.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Aduino_Vehicle_Project.elf  "Aduino_Vehicle_Project.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(CC_DEPS)$(ELFS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) Aduino_Vehicle_Project.elf
	-@echo ' '

secondary-outputs: $(FLASH_IMAGE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
