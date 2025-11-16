# We are comming from the parent "CMakeLists.txt" file
# Let's make a library

# First, define the library name and the source files to compile it
add_library(
    MathFunctions 
    math.cpp
)

# Let's add the include folder as PUBLIC, so whoever want's to 
# link to this library can access the header files
target_include_directories(
    MathFunctions
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Lets add the static library and the headers for installation.
# They will be stored in the "lib" folder and "include" folder respectively.
install(
    TARGETS MathFunctions 
    DESTINATION lib
)

install(
    FILES include/math.h 
    DESTINATION include
)