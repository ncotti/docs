#!/bin/bash

# Test cases for short options only and arguments only.

tested_file="../example_getopts.sh"

test_no_options_no_args() {
    local real_output expected_output
    real_output=$($tested_file)
    expected_output=""
    assertEquals "$expected_output" "$real_output"
}

test_no_options_with_args() {
    local real_output expected_output
    real_output=$($tested_file arg1 arg2 arg3)
    expected_output="Argument: arg1
Argument: arg2
Argument: arg3"
    assertEquals "$expected_output" "$real_output"
}

test_no_options_with_args_and_double_dash() {
    local real_output expected_output
    real_output=$($tested_file -- arg1 arg2)
    expected_output="Argument: arg1
Argument: arg2"
    assertEquals "$expected_output" "$real_output"
}

test_only_short_options() {
    local real_output expected_output
    real_output=$($tested_file -a -b)
    expected_output="Option -a
Option -b"
    assertEquals "$expected_output" "$real_output"
}

test_short_options_with_required_args() {
    local real_output expected_output
    real_output=$($tested_file -c hi -dhello -ab arg1)
    expected_output="Option -c with value: hi
Option -d with value: hello
Option -a
Option -b
Argument: arg1"
    assertEquals "$expected_output" "$real_output"
}

test_long_options_with_no_args() {
    local real_output expected_output
    real_output=$($tested_file --verbose --clear arg1)
    expected_output="Option --verbose
Option --clear
Argument: arg1"
    assertEquals "$expected_output" "$real_output"
}

test_long_options_with_required_args() {
    local real_output expected_output
    real_output=$($tested_file --verbose --number=5 --string=hi)
    expected_output="Option --verbose
Option --number with value: 5
Option --string with value: hi"
    assertEquals "$expected_output" "$real_output"
}

test_undefined_short_options() {
    local real_output expected_output
    real_output=$($tested_file -ef)
    expected_output="Unknown option: -e"
    assertEquals "$expected_output" "$real_output"
}

test_undefined_long_options() {
    local real_output expected_output
    real_output=$($tested_file --aaa --bbb)
    expected_output="Unknown option: --aaa"
    assertEquals "$expected_output" "$real_output"
}

test_short_options_missing_required_args() {
    local real_output expected_output
    real_output=$($tested_file -c)
    expected_output="Missing value for option -c"
    assertEquals "$expected_output" "$real_output"

    real_output=$($tested_file -c -a)
    assertEquals "$expected_output" "$real_output"

    real_output=$($tested_file -c -- arg1)
    assertEquals "$expected_output" "$real_output"
}

test_long_options_missing_required_args() {
    local real_output expected_output
    real_output=$($tested_file --number -a)
    expected_output="Missing value for option --number"
    assertEquals "$expected_output" "$real_output"
}


source shunit2
