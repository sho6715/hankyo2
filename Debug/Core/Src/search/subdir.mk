################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/search/map_cmd.c \
../Core/Src/search/map_flash.c \
../Core/Src/search/search.c 

OBJS += \
./Core/Src/search/map_cmd.o \
./Core/Src/search/map_flash.o \
./Core/Src/search/search.o 

C_DEPS += \
./Core/Src/search/map_cmd.d \
./Core/Src/search/map_flash.d \
./Core/Src/search/search.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/search/%.o: ../Core/Src/search/%.c Core/Src/search/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DHSE_VALUE=8000000 -DLSI_VALUE=32000 -DHSE_STARTUP_TIMEOUT=100 -DHSI_VALUE=16000000 -DDEBUG -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DDATA_CACHE_ENABLE=1 -DEXTERNAL_CLOCK_VALUE=12288000 -DSTM32G474xx -DVDD_VALUE=3300 -DINSTRUCTION_CACHE_ENABLE=1 -DUSE_FULL_LL_DRIVER -DPREFETCH_ENABLE=0 -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-search

clean-Core-2f-Src-2f-search:
	-$(RM) ./Core/Src/search/map_cmd.d ./Core/Src/search/map_cmd.o ./Core/Src/search/map_flash.d ./Core/Src/search/map_flash.o ./Core/Src/search/search.d ./Core/Src/search/search.o

.PHONY: clean-Core-2f-Src-2f-search

