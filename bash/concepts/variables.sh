#!/bin/bash

###############################################################################
# This file discusses local variables, subshells and math expressions.
###############################################################################

# A variable is defined with the equal "=" symbol.
# And can be used with "$var_name" or "${var_name}"
parent_var="Dad"

# It's possible to create a list of commands to be executed enclosing them with
# curly braces {} and ending each one with a semicolon ;. This command are executed
# in the same shell instance.
{ 
    echo -n "Inside curly braces (BASH_SUBSHELL=$BASH_SUBSHELL). Parent variable value: ";
    echo "$parent_var";
    var_inside_curly="I was created inside curly.";
    echo "$SHLVL"
}
echo "The variable created inside curly is: ${var_inside_curly}"

# Enclosing commands with parenthesis () creates a subshell to execute the commands.
# A subshell inherits all the parent's variables, but all variables created 
# inside will perish after completition, even if exported.
(
    echo -n "Inside parenthesis subshell (BASH_SUBSHELL=$BASH_SUBSHELL). Parent variable value: ";
    echo "$parent_var";
    var_inside_parenthesis="I was created inside parenthesis.";
    export var_inside_parenthesis;
)
echo "The variable created inside a subshell is (nothing): ${var_inside_parenthesis}"

# A variable can be assigned to a command stdout value, calling it within $()
var=$(pwd)
echo "Calling \$(pwd) command returned: $var"

# Enclosing commands with double parenthesis (( )) allows to make C like math operations.
# Variables inside a (( )) don't need to be referenced with a dollar sign $.
# Anything that it's not a number will be equal to "0".
var="5"
(( var = var + 10 ))
echo $((var + 10))

# The AND operator && will execute the second command only if the first command
# returns "0"
bad_command &>/dev/null && echo "Not printing, first command doesn't exist"
pwd &>/dev/null && echo "Hello from AND"

# The OR operator || will execute the second command only if the first command
# doesn't return "0"
pwd &>/dev/null || echo "Not printing, first one returns 0"
bad_command &>/dev/null || echo "Hello from OR"