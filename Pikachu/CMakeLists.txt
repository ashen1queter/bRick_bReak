cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/stm32_gcc.cmake)

project(Pikachu C ASM)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(PROJECT_SOURCES
    core/src/main.c
)

add_executable(pikachu.elf ${PROJECT_SOURCES})

find_package(CMSIS 5.8.0 COMPONENTS STM32F1 REQUIRED)
find_package(HAL 1.8.0 COMPONENTS STM32F1 REQUIRED)

target_link_libraries(pikachu.elf
    CMSIS::STM32F1
    HAL::STM32::F1::GPIO
    HAL::STM32::F1::UART
    HAL::STM32::F1::USB
    CMSIS::STM32::F103C8T6
    STM32::NoSys
)

stm32_add_linker_script(pikachu.elf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/LinkerScript.ld)
stm32_print_size_of_target(pikachu.elf)
