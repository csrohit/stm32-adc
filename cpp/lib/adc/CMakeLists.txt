# specify name of library
set(LIB_NAME lib_adc)
add_library(${LIB_NAME} "")

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

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

message(STATUS "mcu_model}=${MCU_MODEL}")

