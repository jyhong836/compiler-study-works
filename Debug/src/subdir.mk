################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LexScan.cpp \
../src/Parsing.cpp \
../src/wordscan.cpp 

OBJS += \
./src/LexScan.o \
./src/Parsing.o \
./src/wordscan.o 

CPP_DEPS += \
./src/LexScan.d \
./src/Parsing.d \
./src/wordscan.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


