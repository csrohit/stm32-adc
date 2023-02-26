# specify name of library
add_library(gpio "")

# add sources for library
target_sources(gpio
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gpio.cpp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/gpio.h
)

# add this path to include directories for the target
target_include_directories(gpio
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/include
)
