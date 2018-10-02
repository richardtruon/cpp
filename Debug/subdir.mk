################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Geometry.cpp \
../Program.cpp \
../RenderingEngine.cpp \
../Scene.cpp \
../ShaderTools.cpp \
../main.cpp 

C_SRCS += \
../glad.c 

OBJS += \
./Geometry.o \
./Program.o \
./RenderingEngine.o \
./Scene.o \
./ShaderTools.o \
./glad.o \
./main.o 

CPP_DEPS += \
./Geometry.d \
./Program.d \
./RenderingEngine.d \
./Scene.d \
./ShaderTools.d \
./main.d 

C_DEPS += \
./glad.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ugb/eric.dimond/cpsc_453/A1/middleware/glad/include" -I"/home/ugb/eric.dimond/cpsc_453/A1/middleware/glfw/include" -I"/home/ugb/eric.dimond/cpsc_453/A1/middleware/glm-0.9.8.2" -I"/home/ugb/eric.dimond/cpsc_453/A1/middleware/stb" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/ugb/eric.dimond/cpsc_453/A1/middleware/glad/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


