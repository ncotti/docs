# GoogleTest

[GoogleTest][gtest] (gtest) is a tool for unit and integration testing for C++ code. Only available with the CMake buildsystem.

## How to setup

As shown in the [Quickstart Guide][quickstart], first prepare a `CMakeLists.txt` file, including the gtest dependency. After that, a standard CMake project can be built with the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

And the tests can be executed with:

```bash
ctest
```

## Gtest API reference

In a test file, the include the header `#include "gtest/gtest.h"`. Test functions are declared with the `TEST` macro with two arguments: The name of the group of tests, and the name of the individual test:

```cpp
TEST(GroupTestName, TestName) {}
```

After that, the test is normal C++ code, where you can call the assertion functions to check whether your code behaves as expected.

```cpp
EXPECT_EQ(first_expression, second_expression);
EXPECT_TRUE(expression);
EXPECT_FALSE(expression);
EXPECT_STREQ(first_string, second_string);
EXPECT_THROW(expression, error_to_be_catched_in_a_try_catch)
```

The difference between `ASSERT_X` and `EXPECT_X` functions is that the former stops execution if the condition is not met, while the latter continues until the end of the test.

<!-- External links -->
[gtest]: https://google.github.io/googletest/
[quickstart]: https://google.github.io/googletest/quickstart-cmake.html
