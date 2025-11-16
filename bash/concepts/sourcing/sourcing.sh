#!/bin/bash

###############################################################################
# This file demonstrates how child shells and sourcing works with local
# and global variables.
#
# Remember that the $SHLVL shows how many times the "bash" command was called.
###############################################################################

# The parent variable won't be seen by the child process, unless exported.
parent_var="Dad"
bash "$PWD/child_shell.sh"

# Now it will be seen.
export parent_var
bash "$PWD/child_shell.sh"

# And no matter the changes, a child shell can never modify the parent's values.
echo "Parent variable is still the same: $parent_var"
echo "No child variable recognized: $child_var"
echo ""

# source (or it's alias dot ".") executes the shell script inside the same process.
# Therefore, all variables remain, and changes from the son are reflected on
# the father. Same SHLVL value while executing.
source "$PWD/child_shell.sh"
echo "Parent variable changed: $parent_var"
echo "Child variable recognized: $child_var"



