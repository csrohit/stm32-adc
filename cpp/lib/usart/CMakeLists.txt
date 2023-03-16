# specify name of library
set(LIB_NAME usart)
add_library(${LIB_NAME} "")

# get_cmake_property(_variableNames VARIABLES) list (SORT _variableNames)
# foreach (_variableName ${_variableNames}) message(STATUS
# "${_variableName}=${${_variableName}}") endforeach()

# add sources for library
target_sources(
  ${LIB_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/usart.cpp
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/usart.h)

# add this path to include directories for the target
target_include_directories(
  ${LIB_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/system
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${LIB_NAME} PUBLIC ${MCU_MODEL})

message(STATUS "mcu_model}=${MCU_MODEL}")
