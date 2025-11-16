#!/bin/bash

###############################################################################
# This file contains all posible ways to create a loop.
#
# Remember that the environmental variable IFS (Interal Field Separator),
# is the one which decides how to separate the elements in a loop. By default,
# the value of IFS=$' \t\n'
#
# "break" and "continue" statements are allowed inside loops.
###############################################################################

list_separated_with_dots="1.2.3.4.5"

# Normal for loop. The elements are separated following the value of "IFS".
IFS=$'\n'
for state in $(cat states.txt); do
    echo "The next state is ${state}"
done


# Demonstration of changing the IFS value.
IFS=$'.'
for number in $list_separated_with_dots; do
	echo "${number}"
done

# Iterate through files in this directory
IFS=$' \t\n'
for file in "$(pwd)"/*; do
	echo "File: ${file}"
done

# You can use C-style for loops using double parenthesis ((
for (( i = 0; i < 10; i++ )); do
	echo "C style for loop: ${i}"
done

# While loop
var=5
while (( var > 0 )); do
	(( var-- ))
	echo "While loop: $var"
done

# Until loop is like a while loop, but negated
var=5
until (( var <= 0 )); do
	(( var-- ))
	echo "Until loop: $var"
done
