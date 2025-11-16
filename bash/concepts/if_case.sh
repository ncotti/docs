#!/bin/bash

###############################################################################
# This file demonstrates how to write conditional statements.
#
# A condition is evaluated as "True" if the return value of a script is 0. Any
# other value will be considered as False. However, there is a special script
# that evaluates a expression and, if it's true, returns 0: "test" or "[".
#
# The return value of a function can be read with "$?" just after it's execution.
###############################################################################

# This operation returns 0
if cat if-case.sh &> /dev/null; then
    # return value == 0
	echo "Return value of first if: $?"
else
    # return value != 0
    echo "Never come here"
fi

# This operation doesn't return 0, the file doesn't exist.
if cat file_not_found &> /dev/null; then
	# return value == 0
	echo "Never come here"
else
	#return value != 0
	echo "Return value of second if: $?"
fi

# The command /usr/bin/test evaluates the expression, and returns 0 if it's true.
# Check the "man test" for all possible flags.
echo "12 > 55?"
if test 12 -gt 55; then
	echo "True"
else
	echo "False"
fi

# The command /usr/bin/[ (yes, an opening bracket is it's name), works exactly
# as /usr/bin/test, but it receives as it's last argument a closing bracket "]".
# So remember, there must be spaces between the brackets: the "[" is a command, and
# "]" is an argument.
echo "12 < 55?"
if [ 12 -lt 55 ]; then
	echo "True"
else
	echo "False"
fi

# Clasical numerical operation can be made with the double parenthesis "((" operator.
echo "2^3 == 8?"
if (( 2**3 == 7 )); then
	echo "Not here"
elif (( 2**3 == 8 )); then
	echo "True"
else
	echo "Not here"
fi

# The double bracket operator "[[" allows for regex string comparison
# and exact comparisons:
if [[ "color" =~ colo(u)?r ]] && [[ "colour" =~ colo(u)?r ]]; then
	echo "color or colour are valid"
fi

# Switch case statements have this weird syntax. The "*" is
# the default case.
case "c" in
	a) echo "Found option a";;
	b) echo "Found option b";;
	c) echo "Found option c";;
	*) echo "Unknown option";;
esac


