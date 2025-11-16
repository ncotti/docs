cmake_minimum_required(VERSION 3.15)

project(embedded)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_STANDARD 11)

set(
    COMPILER_FLAGS
    "-c;-g;-mcpu=cortex-m4;-mthumb;-Wall"
)

set(
    LINKER_FLAGS
    "-mcpu=cortex-m4;-mthumb;-Wall;--specs=nosys.specs;-nostdlib;-lgcc;-T${PROJECT_SOURCE_DIR}/STM32F401RETX_FLASH.ld"
)

add_executable(
    aaa 
    ${PROJECT_SOURCE_DIR}/core/src/main.c 
    ${PROJECT_SOURCE_DIR}/core/src/startup/startup_stm32f401retx.s 
    ${PROJECT_SOURCE_DIR}/core/src/stm32f4xx_it.c 
    ${PROJECT_SOURCE_DIR}/core/src/syscalls.c 
    ${PROJECT_SOURCE_DIR}/core/src/sysmem.c 
    ${PROJECT_SOURCE_DIR}/core/src/system_stm32f4xx.c
)

target_include_directories(
    aaa
    PUBLIC ${PROJECT_SOURCE_DIR}/core/inc
    ${PROJECT_SOURCE_DIR}/drivers/CMSIS/inc
    ${PROJECT_SOURCE_DIR}/drivers/CMSIS/device/inc
)


target_compile_options(
    aaa 
    PUBLIC "${COMPILER_FLAGS}"
)

target_link_options(
    aaa
    PUBLIC "${LINKER_FLAGS}"
)


