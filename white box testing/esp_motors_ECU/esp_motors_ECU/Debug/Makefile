################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../dio_api.c \
../dio_mcu_config.c \
../main.c \
../mcp2515.c \
../millis_api.c \
../millis_mcu_config.c \
../pwm1.c \
../pwm_api.c \
../Queue.c \
../spi_mcu_config.c \
../spi_api.c \
../uart_api.c \
../usart_at_handler.c \
../uart_mcu_config.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
dio_api.o \
dio_mcu_config.o \
main.o \
mcp2515.o \
millis_api.o \
millis_mcu_config.o \
pwm1.o \
pwm_api.o \
Queue.o \
spi_mcu_config.o \
spi_api.o \
uart_api.o \
usart_at_handler.o \
uart_mcu_config.o

OBJS_AS_ARGS +=  \
dio_api.o \
dio_mcu_config.o \
main.o \
mcp2515.o \
millis_api.o \
millis_mcu_config.o \
pwm1.o \
pwm_api.o \
Queue.o \
spi_mcu_config.o \
spi_api.o \
uart_api.o \
usart_at_handler.o \
uart_mcu_config.o

C_DEPS +=  \
dio_api.d \
dio_mcu_config.d \
main.d \
mcp2515.d \
millis_api.d \
millis_mcu_config.d \
pwm1.d \
pwm_api.d \
Queue.d \
spi_mcu_config.d \
spi_api.d \
uart_api.d \
usart_at_handler.d \
uart_mcu_config.d

C_DEPS_AS_ARGS +=  \
dio_api.d \
dio_mcu_config.d \
main.d \
mcp2515.d \
millis_api.d \
millis_mcu_config.d \
pwm1.d \
pwm_api.d \
Queue.d \
spi_mcu_config.d \
spi_api.d \
uart_api.d \
usart_at_handler.d \
uart_mcu_config.d

OUTPUT_FILE_PATH +=esp_motors_ECU.elf

OUTPUT_FILE_PATH_AS_ARGS +=esp_motors_ECU.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





























./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega32a -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega32a" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="esp_motors_ECU.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega32a -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega32a"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "esp_motors_ECU.elf" "esp_motors_ECU.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "esp_motors_ECU.elf" "esp_motors_ECU.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "esp_motors_ECU.elf" > "esp_motors_ECU.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "esp_motors_ECU.elf" "esp_motors_ECU.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "esp_motors_ECU.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "esp_motors_ECU.elf" "esp_motors_ECU.a" "esp_motors_ECU.hex" "esp_motors_ECU.lss" "esp_motors_ECU.eep" "esp_motors_ECU.map" "esp_motors_ECU.srec" "esp_motors_ECU.usersignatures"
	