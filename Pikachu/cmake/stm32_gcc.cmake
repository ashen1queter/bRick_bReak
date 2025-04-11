cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

project(Pikachu C ASM)

set(SOURCES
    Core/Src/main.c
    Core/Src/stm32f1xx_it.c
    Core/Src/system_stm32f1xx.c
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    # Add more HAL sources here as needed
)

include_directories(
    Core/Inc
    Drivers/STM32F1xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F1xx/Include
    Drivers/CMSIS/Include
)

add_executable(Pikachu.elf ${SOURCES})
target_link_options(Pikachu.elf PRIVATE -TSTM32F103C8TX_FLASH.ld)

