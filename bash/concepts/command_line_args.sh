#!/bin/bash

###############################################################################
# This file explains basic usage of command line arguments.
#
# Command Line Arguments are accessed with "$1", where the number 
# is the ordinal position of the arguments.
###############################################################################

# The script name depends on the name used to call the script.
# For example, it's not the same to call "./args.sh" than "$PWD/args.sh"
echo "The script name is: $0"

# The number of arguments passed (not including the script name)
# is read from the "$#" variable.
echo "The amount of arguments passed (not including script name) is: $#"

# The variable "$*" treats all args (excluding script name) as a single word.
echo "This for loop will only run once for all arguments."
for arg in "$*"; do
    echo "$arg"
done

# The variable "$@" treats all args (excluding script name) as separate words.
echo "This for loop will run as many times as arguments are."
for arg in "$@"; do
    echo "$arg"
done

# Finally, the shift operator will move all arguments one step to the left.
# For example, if you had this args:
# ./args.sh 1 2 3
# After calling shift, you would have:
# 1 2 3
while [ -n "$1" ]; do
    echo "$1"
    shift
done