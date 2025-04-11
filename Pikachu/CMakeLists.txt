cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

project(Pikachu C ASM)

set(SOURCES
    Core/Src/main.c
)

include_directories(
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32F1xx/Include
    Drivers/STM32F1xx_HAL_Driver/Inc
    src
)

add_executable(Pikachu.elf ${SOURCES})
target_link_options(Pikachu.elf PRIVATE -TSTM32F103C8TX_FLASH.ld)

