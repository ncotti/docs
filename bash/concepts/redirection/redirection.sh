#!/bin/bash

###############################################################################
# This file explains STDIN, STDOUT and STDERR, as well as the redirection
# operators <, <<, >, >>, |.
# All stdout and stderr will be redirected to namesake files.
#
# Remember that the file descriptors are:
# STDIN = 0
# STDOUT = 1
# STDERR = 2
# ALL = &
###############################################################################

# This line starts a new shell with stdout and stderr redirected to files.
exec 1>stdout.txt 2>stderr.txt

# Normally an echo statement prints to STDOUT
echo "This goes to stdout"

# But you can redirect that output to a file using the > or >>.
# > stands for truncating contents.
# >> stands for appending contents.
echo "This goes in a file" >output_file.txt

# You can redirect the stdout ouput to stderr, and the stderr to stdout!
echo "This redirects stdout to stderr" 1>&2
junk_command 2>&1

# You can write to stdout and a file piping with the "tee" command:
echo "This will go to the file and to stdout" | tee -a output_file.txt

# You can supress a command output entirely by redirecting to the special
# file /dev/null. We put the ampersand to redirect both stdout and stderr.
echo "This will never be printed" &>/dev/null

# The << operator stands for "inline" input redirection.
# It will read the input passed in between a starting and ending string.
# That string can be anything, but normally "EOF" is used
cat << EOF
Cat command ...
inline ...
redirection!
EOF

# The < operator will redirect the contents of a file as it was stdin.
cat <input_file.txt

# The bar | operator is called pipe. It can redirect the output of the first
# command directly onto the second, if the second reads from STDIN.
pwd | cat

