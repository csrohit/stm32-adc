set(LIB_NAME gpio)
# specify name of library
add_library(${LIB_NAME} "")

# add sources for library
target_sources(${LIB_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gpio_port.cpp
    ${CMAKE_CURRENT_LIST_DIR}/gpio_pin.cpp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/gpio_port.h
    ${CMAKE_CURRENT_LIST_DIR}/gpio_pin.h
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

