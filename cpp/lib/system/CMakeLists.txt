# specify name of library
add_library(system "")

# add sources for library
target_sources(system
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/system.cpp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/system.h
)

# add this path to include directories for the target
target_include_directories(system
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/include
)


target_compile_definitions(
    system
    PUBLIC
    ${MCU_MODEL}
)