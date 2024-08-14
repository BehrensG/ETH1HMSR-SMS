################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/Src/ADC.c \
../Core/BSP/Src/BSP.c \
../Core/BSP/Src/EE24.c \
../Core/BSP/Src/EEPROM.c \
../Core/BSP/Src/FloatToString.c \
../Core/BSP/Src/GPIO.c \
../Core/BSP/Src/HMSR-SMS.c \
../Core/BSP/Src/LED.c \
../Core/BSP/Src/SCPI_ADC.c \
../Core/BSP/Src/SCPI_Calculate.c \
../Core/BSP/Src/SCPI_Calibration.c \
../Core/BSP/Src/SCPI_Def.c \
../Core/BSP/Src/SCPI_Format.c \
../Core/BSP/Src/SCPI_Measure.c \
../Core/BSP/Src/SCPI_Server.c \
../Core/BSP/Src/SCPI_System.c \
../Core/BSP/Src/UDP.c \
../Core/BSP/Src/Utility.c \
../Core/BSP/Src/printf.c 

OBJS += \
./Core/BSP/Src/ADC.o \
./Core/BSP/Src/BSP.o \
./Core/BSP/Src/EE24.o \
./Core/BSP/Src/EEPROM.o \
./Core/BSP/Src/FloatToString.o \
./Core/BSP/Src/GPIO.o \
./Core/BSP/Src/HMSR-SMS.o \
./Core/BSP/Src/LED.o \
./Core/BSP/Src/SCPI_ADC.o \
./Core/BSP/Src/SCPI_Calculate.o \
./Core/BSP/Src/SCPI_Calibration.o \
./Core/BSP/Src/SCPI_Def.o \
./Core/BSP/Src/SCPI_Format.o \
./Core/BSP/Src/SCPI_Measure.o \
./Core/BSP/Src/SCPI_Server.o \
./Core/BSP/Src/SCPI_System.o \
./Core/BSP/Src/UDP.o \
./Core/BSP/Src/Utility.o \
./Core/BSP/Src/printf.o 

C_DEPS += \
./Core/BSP/Src/ADC.d \
./Core/BSP/Src/BSP.d \
./Core/BSP/Src/EE24.d \
./Core/BSP/Src/EEPROM.d \
./Core/BSP/Src/FloatToString.d \
./Core/BSP/Src/GPIO.d \
./Core/BSP/Src/HMSR-SMS.d \
./Core/BSP/Src/LED.d \
./Core/BSP/Src/SCPI_ADC.d \
./Core/BSP/Src/SCPI_Calculate.d \
./Core/BSP/Src/SCPI_Calibration.d \
./Core/BSP/Src/SCPI_Def.d \
./Core/BSP/Src/SCPI_Format.d \
./Core/BSP/Src/SCPI_Measure.d \
./Core/BSP/Src/SCPI_Server.d \
./Core/BSP/Src/SCPI_System.d \
./Core/BSP/Src/UDP.d \
./Core/BSP/Src/Utility.d \
./Core/BSP/Src/printf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/Src/%.o Core/BSP/Src/%.su Core/BSP/Src/%.cyclo: ../Core/BSP/Src/%.c Core/BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DDATA_IN_D2_SRAM -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/HTTP/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/multicastDNS/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/HiSLIP/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc/scpi" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/LwIP/src/apps/http -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-Src

clean-Core-2f-BSP-2f-Src:
	-$(RM) ./Core/BSP/Src/ADC.cyclo ./Core/BSP/Src/ADC.d ./Core/BSP/Src/ADC.o ./Core/BSP/Src/ADC.su ./Core/BSP/Src/BSP.cyclo ./Core/BSP/Src/BSP.d ./Core/BSP/Src/BSP.o ./Core/BSP/Src/BSP.su ./Core/BSP/Src/EE24.cyclo ./Core/BSP/Src/EE24.d ./Core/BSP/Src/EE24.o ./Core/BSP/Src/EE24.su ./Core/BSP/Src/EEPROM.cyclo ./Core/BSP/Src/EEPROM.d ./Core/BSP/Src/EEPROM.o ./Core/BSP/Src/EEPROM.su ./Core/BSP/Src/FloatToString.cyclo ./Core/BSP/Src/FloatToString.d ./Core/BSP/Src/FloatToString.o ./Core/BSP/Src/FloatToString.su ./Core/BSP/Src/GPIO.cyclo ./Core/BSP/Src/GPIO.d ./Core/BSP/Src/GPIO.o ./Core/BSP/Src/GPIO.su ./Core/BSP/Src/HMSR-SMS.cyclo ./Core/BSP/Src/HMSR-SMS.d ./Core/BSP/Src/HMSR-SMS.o ./Core/BSP/Src/HMSR-SMS.su ./Core/BSP/Src/LED.cyclo ./Core/BSP/Src/LED.d ./Core/BSP/Src/LED.o ./Core/BSP/Src/LED.su ./Core/BSP/Src/SCPI_ADC.cyclo ./Core/BSP/Src/SCPI_ADC.d ./Core/BSP/Src/SCPI_ADC.o ./Core/BSP/Src/SCPI_ADC.su ./Core/BSP/Src/SCPI_Calculate.cyclo ./Core/BSP/Src/SCPI_Calculate.d ./Core/BSP/Src/SCPI_Calculate.o ./Core/BSP/Src/SCPI_Calculate.su ./Core/BSP/Src/SCPI_Calibration.cyclo ./Core/BSP/Src/SCPI_Calibration.d ./Core/BSP/Src/SCPI_Calibration.o ./Core/BSP/Src/SCPI_Calibration.su ./Core/BSP/Src/SCPI_Def.cyclo ./Core/BSP/Src/SCPI_Def.d ./Core/BSP/Src/SCPI_Def.o ./Core/BSP/Src/SCPI_Def.su ./Core/BSP/Src/SCPI_Format.cyclo ./Core/BSP/Src/SCPI_Format.d ./Core/BSP/Src/SCPI_Format.o ./Core/BSP/Src/SCPI_Format.su ./Core/BSP/Src/SCPI_Measure.cyclo ./Core/BSP/Src/SCPI_Measure.d ./Core/BSP/Src/SCPI_Measure.o ./Core/BSP/Src/SCPI_Measure.su ./Core/BSP/Src/SCPI_Server.cyclo ./Core/BSP/Src/SCPI_Server.d ./Core/BSP/Src/SCPI_Server.o ./Core/BSP/Src/SCPI_Server.su ./Core/BSP/Src/SCPI_System.cyclo ./Core/BSP/Src/SCPI_System.d ./Core/BSP/Src/SCPI_System.o ./Core/BSP/Src/SCPI_System.su ./Core/BSP/Src/UDP.cyclo ./Core/BSP/Src/UDP.d ./Core/BSP/Src/UDP.o ./Core/BSP/Src/UDP.su ./Core/BSP/Src/Utility.cyclo ./Core/BSP/Src/Utility.d ./Core/BSP/Src/Utility.o ./Core/BSP/Src/Utility.su ./Core/BSP/Src/printf.cyclo ./Core/BSP/Src/printf.d ./Core/BSP/Src/printf.o ./Core/BSP/Src/printf.su

.PHONY: clean-Core-2f-BSP-2f-Src

