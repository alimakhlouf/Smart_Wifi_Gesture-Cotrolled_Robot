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
../adc_api.c \
../ADC_lib.c \
../DHT11.c \
../dio_api.c \
../dio_mcu_config.c \
../hcsr04.c \
../main.c \
../mcp2515.c \
../millis_api.c \
../millis_mcu_config.c \
../MQ_2.c \
../spi_api.c \
../spi_mcu_config.c \
../uart_api.c \
../uart_mcu_config.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
adc_api.o \
ADC_lib.o \
DHT11.o \
dio_api.o \
dio_mcu_config.o \
hcsr04.o \
main.o \
mcp2515.o \
millis_api.o \
millis_mcu_config.o \
MQ_2.o \
spi_api.o \
spi_mcu_config.o \
uart_api.o \
uart_mcu_config.o

OBJS_AS_ARGS +=  \
adc_api.o \
ADC_lib.o \
DHT11.o \
dio_api.o \
dio_mcu_config.o \
hcsr04.o \
main.o \
mcp2515.o \
millis_api.o \
millis_mcu_config.o \
MQ_2.o \
spi_api.o \
spi_mcu_config.o \
uart_api.o \
uart_mcu_config.o

C_DEPS +=  \
adc_api.d \
ADC_lib.d \
DHT11.d \
dio_api.d \
dio_mcu_config.d \
hcsr04.d \
main.d \
mcp2515.d \
millis_api.d \
millis_mcu_config.d \
MQ_2.d \
spi_api.d \
spi_mcu_config.d \
uart_api.d \
uart_mcu_config.d

C_DEPS_AS_ARGS +=  \
adc_api.d \
ADC_lib.d \
DHT11.d \
dio_api.d \
dio_mcu_config.d \
hcsr04.d \
main.d \
mcp2515.d \
millis_api.d \
millis_mcu_config.d \
MQ_2.d \
spi_api.d \
spi_mcu_config.d \
uart_api.d \
uart_mcu_config.d

OUTPUT_FILE_PATH +=SENSORS_ECU.elf

OUTPUT_FILE_PATH_AS_ARGS +=SENSORS_ECU.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega16a -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega16a" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="SENSORS_ECU.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega16a -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega16a"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "SENSORS_ECU.elf" "SENSORS_ECU.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "SENSORS_ECU.elf" "SENSORS_ECU.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "SENSORS_ECU.elf" > "SENSORS_ECU.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "SENSORS_ECU.elf" "SENSORS_ECU.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "SENSORS_ECU.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SENSORS_ECU.elf" "SENSORS_ECU.a" "SENSORS_ECU.hex" "SENSORS_ECU.lss" "SENSORS_ECU.eep" "SENSORS_ECU.map" "SENSORS_ECU.srec" "SENSORS_ECU.usersignatures"
	