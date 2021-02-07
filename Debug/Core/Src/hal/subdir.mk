################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/hal/CTRL.c \
../Core/Src/hal/DCM.c \
../Core/Src/hal/encoder.c \
../Core/Src/hal/gyro.c \
../Core/Src/hal/init.c \
../Core/Src/hal/log.c \
../Core/Src/hal/mode.c \
../Core/Src/hal/run.c \
../Core/Src/hal/sen_batt.c \
../Core/Src/hal/sen_dist.c 

OBJS += \
./Core/Src/hal/CTRL.o \
./Core/Src/hal/DCM.o \
./Core/Src/hal/encoder.o \
./Core/Src/hal/gyro.o \
./Core/Src/hal/init.o \
./Core/Src/hal/log.o \
./Core/Src/hal/mode.o \
./Core/Src/hal/run.o \
./Core/Src/hal/sen_batt.o \
./Core/Src/hal/sen_dist.o 

C_DEPS += \
./Core/Src/hal/CTRL.d \
./Core/Src/hal/DCM.d \
./Core/Src/hal/encoder.d \
./Core/Src/hal/gyro.d \
./Core/Src/hal/init.d \
./Core/Src/hal/log.d \
./Core/Src/hal/mode.d \
./Core/Src/hal/run.d \
./Core/Src/hal/sen_batt.d \
./Core/Src/hal/sen_dist.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/hal/CTRL.o: ../Core/Src/hal/CTRL.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/CTRL.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/DCM.o: ../Core/Src/hal/DCM.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/DCM.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/encoder.o: ../Core/Src/hal/encoder.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/encoder.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/gyro.o: ../Core/Src/hal/gyro.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/gyro.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/init.o: ../Core/Src/hal/init.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/init.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/log.o: ../Core/Src/hal/log.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/log.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/mode.o: ../Core/Src/hal/mode.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/mode.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/run.o: ../Core/Src/hal/run.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/run.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/sen_batt.o: ../Core/Src/hal/sen_batt.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/sen_batt.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/hal/sen_dist.o: ../Core/Src/hal/sen_dist.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_VALUE=8000000' '-DLSI_VALUE=32000' '-DHSE_STARTUP_TIMEOUT=100' '-DHSI_VALUE=16000000' -DDEBUG '-DLSE_STARTUP_TIMEOUT=5000' '-DLSE_VALUE=32768' '-DDATA_CACHE_ENABLE=1' '-DEXTERNAL_CLOCK_VALUE=12288000' -DSTM32G474xx '-DVDD_VALUE=3300' '-DINSTRUCTION_CACHE_ENABLE=1' -DUSE_FULL_LL_DRIVER '-DPREFETCH_ENABLE=0' -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/hal/sen_dist.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

