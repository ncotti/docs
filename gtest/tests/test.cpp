#include "gtest/gtest.h"
#include "functions.h"

TEST(FactorialTest, ZeroInput) {
    EXPECT_EQ(factorial(0), 1);
}

TEST(FactorialTest, PositiveInput) {
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
    EXPECT_EQ(factorial(3), 6);
    EXPECT_EQ(factorial(8), 40320);
}

TEST(FactorialTest, NegativeInput) {
    EXPECT_EQ(factorial(-6), -1);
    EXPECT_EQ(factorial(-9), -1);
    EXPECT_EQ(factorial(-1), -1);
}