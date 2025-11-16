#!/bin/bash

###############################################################################
# This file shows syntax for function definitions and functions calling.
###############################################################################

# This two ways of declaring a function are equal.
# The exit value of a function must be an integer from 0 to 255.
function foo1() {
    echo "Hello from function foo1"
    return 1
}

foo2() {
    echo "Hello from function foo2"
    return 2
}

#Functions are called as normal commands, from it's name.
foo1
echo "foo1 exit status: $?"
foo2
echo "foo2 exit status: $?"

# Pass arguments to a function in the same way as if 
# they were command line arguments.
# Return any value from a function by writing to stdout, and assigning the
# function to a variable with $() syntax.
foo3() {
    echo $(( $1 + $2 ))
    return 0
}
echo "The function foo3 returned 1 + 2 = $(foo3 1 2)"

# By default, variables from outside and inside the function share names.
foo4() {
    var_dad="Son, modified from foo4"
    var_fun="Hello from foo4"
}
var_dad="Dad"
foo4
echo "Variable changed inside function foo4: $var_dad"
echo "Variable created inside function foo4: $var_fun"

# To prevent this, use the keyword "local"
foo5() {
    local var_dad="Son, modified from foo5"
    local var_fun="Hello from foo5"
}
echo "Variables didn't change inside foo5: $var_dad; $var_fun"




