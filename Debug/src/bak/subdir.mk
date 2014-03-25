################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bak/LexScan.cpp \
../src/bak/Parsing.cpp \
../src/bak/wordscan.cpp 

OBJS += \
./src/bak/LexScan.o \
./src/bak/Parsing.o \
./src/bak/wordscan.o 

CPP_DEPS += \
./src/bak/LexScan.d \
./src/bak/Parsing.d \
./src/bak/wordscan.d 


# Each subdirectory must supply rules for building sources it contributes
src/bak/%.o: ../src/bak/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


