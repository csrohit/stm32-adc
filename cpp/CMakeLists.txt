# STM32 Minimal CMake project for C/C++ projects
cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Toolchain configuration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PREFIX arm-none-eabi)
# set(FLAGS
# "-fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections")
# set(CPP_FLAGS
# "-fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc ${FLAGS})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++ ${FLAGS} ${CPP_FLAGS})
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}-size)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)

set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set project name
project(adc_demo LANGUAGES CXX C)
set(EXECUTABLE ${CMAKE_PROJECT_NAME})

set(CMAKE_VERBOSE_MAKEFILE ON)

# Set microcontroller information
set(MCU_FAMILY STM32F1xx)
set(MCU_MODEL STM32F103xB)
set(CPU_PARAMETERS
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/startup_stm32f1.c)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1.ld)


# enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# add directories to look for header files
set(PROJECT_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)


# add source files for project
add_executable(${EXECUTABLE}
    src/main.cpp
    src/system_stm32f1xx.c
    src/printf.c
    ${STARTUP_SCRIPT}
)

set(C_FLAGS
    ${CPU_PARAMETERS}
    -nostdlib
    # -fno-tree-loop-distribute-patterns
    # -Wall
    # -Wextra
    # -Wpedantic
    # -Wshadow
    # -Wdouble-promotion
    # -Wformat=2
    # -Wformat-truncation
    # -Wundef
    -fno-common
    # -Wno-unused-parameter
)

set(CXX_FLAGS
    ${C_FLAGS}
    -fno-exceptions
    -fno-rtti
    -nostdlib
    # -Wconversion # STM libraries!
    -Wno-volatile
    # -Wold-style-cast
    # -Wuseless-cast
    # -Wsuggest-override
)

# set c flags with space delimited flags
string (REPLACE ";" " " CMAKE_C_FLAGS "${C_FLAGS}")
string (REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG
    "-g -gdwarf-2"
)
set(CMAKE_CXX_FLAGS_DEBUG
    "-g -gdwarf-2"
)


# add directories for searching libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/gpio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/usart)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/system)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/rcc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/adc)

# link libraries with the executable
target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
    gpio
    usart
    system
    adc
    rcc
)

# add definitions to be passed to compiler
target_compile_definitions(${EXECUTABLE}
    PUBLIC
        $<$<CONFIG:Debug>:DEBUG>
        ${MCU_MODEL}
)


# add include directories 
target_include_directories(${EXECUTABLE}
    PRIVATE
        ${PROJECT_INCLUDE_DIRECTORIES}
)

# add flags for linker
target_link_options(${EXECUTABLE} 
    PRIVATE
        -T${MCU_LINKER_SCRIPT}
        -Wl,-Map=${CMAKE_PROJECT_NAME}.map,--gc-sections,-cref,--print-memory-usage
)


# add target for flashing the micro-controller
add_custom_target(flash
    COMMAND st-flash write ${EXECUTABLE}.bin 0x8000000
)

# generated binaries for flashinh
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
