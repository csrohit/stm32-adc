# specify name of library
set(LIB_NAME rcc)
add_library(${LIB_NAME} "")

# add sources for library
target_sources(${LIB_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/rcc.cpp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/rcc.h
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
