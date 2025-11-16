# Setup
cmake_minimum_required(VERSION 3.10)
project(Tutorial)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the math directory
add_subdirectory(math)

# Create the binary target "tuto". This will be the executable file.
add_executable(
    tuto
    ${PROJECT_SOURCE_DIR}/src/tutorial.cpp
    "${PROJECT_SOURCE_DIR}/test.txt"
)

# Link to the library in the "math" folder.
target_link_libraries(
    tuto
    PUBLIC MathFunctions
)

# Add the include path to "include".
target_include_directories(
    tuto
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Define a custom command to execute
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/test.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND echo "Custom Linux command was executed inside compilation proccess." >> test.txt
    COMMENT "Creating test.txt\n"
)

# Add the binary in the "bin" folder when installing
install(
    TARGETS tuto
    DESTINATION bin
)
