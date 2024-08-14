################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/HTTP/Src/HTTP_Control.c \
../Core/BSP/HTTP/Src/HTTP_Home.c \
../Core/BSP/HTTP/Src/HTTP_Index.c \
../Core/BSP/HTTP/Src/HTTP_LXI_Idn.c \
../Core/BSP/HTTP/Src/HTTP_Service.c \
../Core/BSP/HTTP/Src/HTTP_Setup.c \
../Core/BSP/HTTP/Src/HTTP_Task.c \
../Core/BSP/HTTP/Src/fsdata.c 

OBJS += \
./Core/BSP/HTTP/Src/HTTP_Control.o \
./Core/BSP/HTTP/Src/HTTP_Home.o \
./Core/BSP/HTTP/Src/HTTP_Index.o \
./Core/BSP/HTTP/Src/HTTP_LXI_Idn.o \
./Core/BSP/HTTP/Src/HTTP_Service.o \
./Core/BSP/HTTP/Src/HTTP_Setup.o \
./Core/BSP/HTTP/Src/HTTP_Task.o \
./Core/BSP/HTTP/Src/fsdata.o 

C_DEPS += \
./Core/BSP/HTTP/Src/HTTP_Control.d \
./Core/BSP/HTTP/Src/HTTP_Home.d \
./Core/BSP/HTTP/Src/HTTP_Index.d \
./Core/BSP/HTTP/Src/HTTP_LXI_Idn.d \
./Core/BSP/HTTP/Src/HTTP_Service.d \
./Core/BSP/HTTP/Src/HTTP_Setup.d \
./Core/BSP/HTTP/Src/HTTP_Task.d \
./Core/BSP/HTTP/Src/fsdata.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/HTTP/Src/%.o Core/BSP/HTTP/Src/%.su Core/BSP/HTTP/Src/%.cyclo: ../Core/BSP/HTTP/Src/%.c Core/BSP/HTTP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DDATA_IN_D2_SRAM -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/HTTP/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/multicastDNS/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/HiSLIP/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/Inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc" -I"D:/tmp/ETH1HMSR-SMS/SOFTWARE/ETH1HMSR-SMS/Core/BSP/SCPI/libscpi/inc/scpi" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../LWIP/App -I../LWIP/Target -I../Middlewares/Third_Party/LwIP/src/include -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/Components/lan8742 -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/Third_Party/LwIP/src/include/netif -I../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/LwIP/src/apps/http -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-HTTP-2f-Src

clean-Core-2f-BSP-2f-HTTP-2f-Src:
	-$(RM) ./Core/BSP/HTTP/Src/HTTP_Control.cyclo ./Core/BSP/HTTP/Src/HTTP_Control.d ./Core/BSP/HTTP/Src/HTTP_Control.o ./Core/BSP/HTTP/Src/HTTP_Control.su ./Core/BSP/HTTP/Src/HTTP_Home.cyclo ./Core/BSP/HTTP/Src/HTTP_Home.d ./Core/BSP/HTTP/Src/HTTP_Home.o ./Core/BSP/HTTP/Src/HTTP_Home.su ./Core/BSP/HTTP/Src/HTTP_Index.cyclo ./Core/BSP/HTTP/Src/HTTP_Index.d ./Core/BSP/HTTP/Src/HTTP_Index.o ./Core/BSP/HTTP/Src/HTTP_Index.su ./Core/BSP/HTTP/Src/HTTP_LXI_Idn.cyclo ./Core/BSP/HTTP/Src/HTTP_LXI_Idn.d ./Core/BSP/HTTP/Src/HTTP_LXI_Idn.o ./Core/BSP/HTTP/Src/HTTP_LXI_Idn.su ./Core/BSP/HTTP/Src/HTTP_Service.cyclo ./Core/BSP/HTTP/Src/HTTP_Service.d ./Core/BSP/HTTP/Src/HTTP_Service.o ./Core/BSP/HTTP/Src/HTTP_Service.su ./Core/BSP/HTTP/Src/HTTP_Setup.cyclo ./Core/BSP/HTTP/Src/HTTP_Setup.d ./Core/BSP/HTTP/Src/HTTP_Setup.o ./Core/BSP/HTTP/Src/HTTP_Setup.su ./Core/BSP/HTTP/Src/HTTP_Task.cyclo ./Core/BSP/HTTP/Src/HTTP_Task.d ./Core/BSP/HTTP/Src/HTTP_Task.o ./Core/BSP/HTTP/Src/HTTP_Task.su ./Core/BSP/HTTP/Src/fsdata.cyclo ./Core/BSP/HTTP/Src/fsdata.d ./Core/BSP/HTTP/Src/fsdata.o ./Core/BSP/HTTP/Src/fsdata.su

.PHONY: clean-Core-2f-BSP-2f-HTTP-2f-Src

