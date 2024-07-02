################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/SCPI/libscpi/src/error.c \
../Core/BSP/SCPI/libscpi/src/expression.c \
../Core/BSP/SCPI/libscpi/src/fifo.c \
../Core/BSP/SCPI/libscpi/src/ieee488.c \
../Core/BSP/SCPI/libscpi/src/lexer.c \
../Core/BSP/SCPI/libscpi/src/minimal.c \
../Core/BSP/SCPI/libscpi/src/parser.c \
../Core/BSP/SCPI/libscpi/src/units.c \
../Core/BSP/SCPI/libscpi/src/utils.c 

OBJS += \
./Core/BSP/SCPI/libscpi/src/error.o \
./Core/BSP/SCPI/libscpi/src/expression.o \
./Core/BSP/SCPI/libscpi/src/fifo.o \
./Core/BSP/SCPI/libscpi/src/ieee488.o \
./Core/BSP/SCPI/libscpi/src/lexer.o \
./Core/BSP/SCPI/libscpi/src/minimal.o \
./Core/BSP/SCPI/libscpi/src/parser.o \
./Core/BSP/SCPI/libscpi/src/units.o \
./Core/BSP/SCPI/libscpi/src/utils.o 

C_DEPS += \
./Core/BSP/SCPI/libscpi/src/error.d \
./Core/BSP/SCPI/libscpi/src/expression.d \
./Core/BSP/SCPI/libscpi/src/fifo.d \
./Core/BSP/SCPI/libscpi/src/ieee488.d \
./Core/BSP/SCPI/libscpi/src/lexer.d \
./Core/BSP/SCPI/libscpi/src/minimal.d \
./Core/BSP/SCPI/libscpi/src/parser.d \
./Core/BSP/SCPI/libscpi/src/units.d \
./Core/BSP/SCPI/libscpi/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/SCPI/libscpi/src/%.o Core/BSP/SCPI/libscpi/src/%.su Core/BSP/SCPI/libscpi/src/%.cyclo: ../Core/BSP/SCPI/libscpi/src/%.c Core/BSP/SCPI/libscpi/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DDATA_IN_D2_SRAM -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/Inc" -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc/scpi" -I"/home/grzegorz/git/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-SCPI-2f-libscpi-2f-src

clean-Core-2f-BSP-2f-SCPI-2f-libscpi-2f-src:
	-$(RM) ./Core/BSP/SCPI/libscpi/src/error.cyclo ./Core/BSP/SCPI/libscpi/src/error.d ./Core/BSP/SCPI/libscpi/src/error.o ./Core/BSP/SCPI/libscpi/src/error.su ./Core/BSP/SCPI/libscpi/src/expression.cyclo ./Core/BSP/SCPI/libscpi/src/expression.d ./Core/BSP/SCPI/libscpi/src/expression.o ./Core/BSP/SCPI/libscpi/src/expression.su ./Core/BSP/SCPI/libscpi/src/fifo.cyclo ./Core/BSP/SCPI/libscpi/src/fifo.d ./Core/BSP/SCPI/libscpi/src/fifo.o ./Core/BSP/SCPI/libscpi/src/fifo.su ./Core/BSP/SCPI/libscpi/src/ieee488.cyclo ./Core/BSP/SCPI/libscpi/src/ieee488.d ./Core/BSP/SCPI/libscpi/src/ieee488.o ./Core/BSP/SCPI/libscpi/src/ieee488.su ./Core/BSP/SCPI/libscpi/src/lexer.cyclo ./Core/BSP/SCPI/libscpi/src/lexer.d ./Core/BSP/SCPI/libscpi/src/lexer.o ./Core/BSP/SCPI/libscpi/src/lexer.su ./Core/BSP/SCPI/libscpi/src/minimal.cyclo ./Core/BSP/SCPI/libscpi/src/minimal.d ./Core/BSP/SCPI/libscpi/src/minimal.o ./Core/BSP/SCPI/libscpi/src/minimal.su ./Core/BSP/SCPI/libscpi/src/parser.cyclo ./Core/BSP/SCPI/libscpi/src/parser.d ./Core/BSP/SCPI/libscpi/src/parser.o ./Core/BSP/SCPI/libscpi/src/parser.su ./Core/BSP/SCPI/libscpi/src/units.cyclo ./Core/BSP/SCPI/libscpi/src/units.d ./Core/BSP/SCPI/libscpi/src/units.o ./Core/BSP/SCPI/libscpi/src/units.su ./Core/BSP/SCPI/libscpi/src/utils.cyclo ./Core/BSP/SCPI/libscpi/src/utils.d ./Core/BSP/SCPI/libscpi/src/utils.o ./Core/BSP/SCPI/libscpi/src/utils.su

.PHONY: clean-Core-2f-BSP-2f-SCPI-2f-libscpi-2f-src

