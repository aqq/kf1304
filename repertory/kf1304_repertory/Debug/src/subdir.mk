################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/GlobalHelper.cpp \
../src/StringHelper.cpp \
../src/TextLogger.cpp \
../src/kf1304_master.cpp \
../src/kf1304_repertory.cpp \
../src/master.cpp \
../src/rep.cpp \
../src/repertory.cpp \
../src/slaver.cpp 

OBJS += \
./src/GlobalHelper.o \
./src/StringHelper.o \
./src/TextLogger.o \
./src/kf1304_master.o \
./src/kf1304_repertory.o \
./src/master.o \
./src/rep.o \
./src/repertory.o \
./src/slaver.o 

CPP_DEPS += \
./src/GlobalHelper.d \
./src/StringHelper.d \
./src/TextLogger.d \
./src/kf1304_master.d \
./src/kf1304_repertory.d \
./src/master.d \
./src/rep.d \
./src/repertory.d \
./src/slaver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

