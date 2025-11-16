#!/bin/bash

# Please, refer to sourcing.sh

if [ -n "$parent_var" ]; then
    echo "The parent variable is seen by the child (with SHLVL=$SHLVL): $parent_var";
    echo "From the child, i will try to modify the value of the parent variable"
    parent_var="Son"
    child_var="Child"
    export parent_var
else
    echo "No parent variable."
fi