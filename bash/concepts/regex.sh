#!/bin/bash

###############################################################################
# This file explains briefly why using native bash regex is a bad idea, and
# what to use instead.
# Remember: ALWAYS DEFINE A REGEX INSIDE A VARIABLE FIRST.
#
# TL;DR: Use "grep -P" for regex (using the Perl regex engine). 
###############################################################################

# A regex comparison can be made with the double bracket operator, and the "=~".
regex="^h\w+o$"
if [[ "hello" =~ $regex ]]; then
    echo "\"hello\" starts with \"h\" and ends with \"o\""
fi

# That's great, now let's get a little fancy.
regex="\d+"
if [[ "123" =~ $regex ]]; then
    echo ""
else
    echo "\"123\" doesn't have digits??"
fi

# As you can see, bash regex is quite limited in it's functionality.
# There are several Regex engines, each one with it's own features. By default,
# the [[ operator and grep command use the GNU BRE / POSIX BRE (Basic Regex engine)
# or GNU ERE / POSIX ERE (Extended Regex Engine).
# However, the most complete one is the Perl Engine, which is only available with
# the "-P" option in grep. Therefore, the same statement is true with grep!
if echo "123" | grep -P $regex &>/dev/null; then
    echo "Using Perl regex engine, digits are detected!"
fi

# A simple substitution algorithm can be made with shell parameter expansion.
# The syntax is: ${variable/pattern_to_search/pattern_to_replace}
# Use one "/" to replace the first occurrence, double "//" for all occurrences.
var="Hello world"
echo "Let's replace the first ocurrence of the letter \"L\" in hello world \
with \"K\": ${var/l/K}"
echo "Now, all the \"L\": ${var//l/K}"
