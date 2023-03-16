# specify name of library
set(LIB_NAME adc)
add_library(${LIB_NAME} "")

# add sources for library
target_sources(${LIB_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/adc.cpp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/adc.h
)

# add this path to include directories for the target
target_include_directories(${LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(
    ${LIB_NAME}
    PUBLIC
    ${MCU_MODEL}
)


