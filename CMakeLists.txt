cmake_minimum_required(VERSION 3.10)
project(toyc_compiler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

add_executable(lexer src/lexer.cpp)

set_target_properties(lexer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

install(TARGETS lexer
    RUNTIME DESTINATION bin
)