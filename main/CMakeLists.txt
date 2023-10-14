cmake_minimum_required(VERSION 3.26)

# using: https://github.com/ObKo/stm32-cmake cmake helpers
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/third-party/stm32-cmake/cmake/stm32_gcc.cmake)
set(STM32_TOOLCHAIN_PATH "$ENV{HOMEDRIVE}$ENV{HOMEPATH}/MyDocuments/arm-tools/xpack-arm-none-eabi-gcc-12.2.1-1.2" CACHE STRING "ToolchainPath")
set(STM32_CUBE_F1_PATH "$ENV{HOMEDRIVE}$ENV{HOMEPATH}/MyDocuments/arm-tools/stm32cube-drivers/STM32CubeF1-1.8.5" CACHE STRING "PathToCubeRoot")
set(TARGET_TRIPLET arm-none-eabi CACHE STRING "")

project(cardian-hardware C CXX ASM)
set(MAIN_SOURCE_FILE main.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

find_package(CMSIS COMPONENTS STM32F1 REQUIRED)
find_package(HAL COMPONENTS STM32F1 LL_RCC LL_GPIO REQUIRED)

file(GLOB cardian-startups
    ${CMAKE_CURRENT_SOURCE_DIR}/startup/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/startup/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/startup/*.s
    ${CMAKE_CURRENT_SOURCE_DIR}/mcu/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mcu/*.cpp
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${cardian-startups}
    third-party/ringbuffer/ringbuffer.hpp
)

target_link_libraries(${PROJECT_NAME}
    HAL::STM32::F1::LL_GPIO
    HAL::STM32::F1::LL_RCC
    CMSIS::STM32::F1
    STM32::Nano # Reduced code size
    STM32::NoSys # Disable systemcalls
)

add_subdirectory(${PROJECT_SOURCE_DIR}/utils)
target_link_libraries(${PROJECT_NAME} lib-utils)

target_compile_definitions(${PROJECT_NAME} PRIVATE STM32F103xB)
# suppress GCC warnings
target_link_options(${PROJECT_NAME} PRIVATE "LINKER:--no-warn-rwx-segments")
add_definitions("-Wno-psabi")

stm32_add_linker_script(${PROJECT_NAME} PRIVATE stm32f103cbt.ld)
stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_srec_file(${PROJECT_NAME})
stm32_generate_binary_file(${PROJECT_NAME})
stm32_generate_hex_file(${PROJECT_NAME})
