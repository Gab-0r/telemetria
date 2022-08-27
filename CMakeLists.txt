# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "C:/Users/j_gab/Documents/Pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(telemetria C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(telemetria telemetria.cpp hw_config.c sd_manager.cpp mpuObject.cpp mpu9250.c)

add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)


pico_set_program_name(telemetria "telemetria")
pico_set_program_version(telemetria "0.1")

pico_enable_stdio_uart(telemetria 0)
pico_enable_stdio_usb(telemetria 1)

# Add the standard library to the build
target_link_libraries(telemetria pico_stdlib FatFs_SPI)

pico_add_extra_outputs(telemetria)

