# CMake: A little messy but gets the job done

[CMake][cmake] is powerful, but hard to master. The main issue with CMake is that its syntax is too convoluted, and to do anything you have to consult the documentation. Whenever I learn CMake, I automatically forget how to use it the next week, and I have to return to the tutorials.

Next, a quick guide is provided to learn how to compile C and C++ projects, together with an associated [example][example] in this repository.

## Quick guide

Below is a list of the commands needed to build, install and execute a CMake project, with a `CMakeLists.txt` file:

```bash
mkdir build
cd build
cmake ..
cmake --build .
cmake --install . --prefix $(pwd)/../install
../install/bin/tuto
```

## CMakeLists.txt reference

All compilation, linking, and installation instructions must be written in a `CMakeLists.txt` file in the project's root directory.

### Initial setup

Setup the CMake version, C language standard, and the project's name.

```cmake
cmake_minimum_required(VERSION <version_number>)

project([PROJECT_NAME])

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

### Adding source files

A `target` represents an executable file to be compiled. The list of required source files to compile that target must be provided.

```cmake
add_executable(
    [target_name]
    [sources ...]
)
```

### Adding a library

If the target requires linking to a library, it can be created and then added for linking as follows.

```cmake
add_library(
    [library_target_name]
    [sources...]
)

target_link_libraries(
    <target_name>
    PUBLIC|PRIVATE|INTERFACE <library_target_name>
)
```

The `PUBLIC|PRIVATE|INTERFACE` establishes the way in which the library will be included:

1. `PUBLIC`: Can be seen by the target and every other target that links with it.

2. `PRIVATE`: Can only be seen by the calling target.

3. `INTERFACE`: Can only be seen by subsequent targets, not by the target that included it.

### Adding header files

An include directory, where all `*.h` headers files are stored, can be defined with:

```cmake
target_include_directories ([target_name] [PUBLIC|PRIVATE|INTERFACE] [header_folders ...])
```

### Miscellaneous

It's possible to define a subdirectory with a nested `CMakeLists.txt` file to be executed with:

```cmake
add_subdirectory([path_to_CMakeLists.txt])
```

A custom linux shell command can be executed with the following syntax:

```cmake
add_custom_command(
    OUTPUT [output_file]
    WORKING_DIRECTORY [path]
    COMMAND [linux command]
    COMMENT [What to print in terminal]
    DEPENDS [file]
)
```

### Installation commands

The installation process consists of cleaning up and moving around the binaries generated, as a mean to expose only useful files to the user, and hide all the compilation ones.

```cmake
install(
    TARGETS|FILES|DIRECTORY [element]
    DESTINATION [directory_path]
)
```

### Variables

A variable, accessible from the `CMakeList.txt` file, can be created as:

```cmake
set([VAR_NAME] [var_value])
```

And be read as follows:

```cmake
${VAR_NAME}
```

Some common global variables defined for every CMake project are:

```cmake
${CMAKE_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_BINARY_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${PROJECT_SOURCE_DIR}
${PROJECT_BINARY_DIR}
```

<!-- External links -->
[cmake]: https://cmake.org/

<!-- Internal links -->
[example]: /cmake/
