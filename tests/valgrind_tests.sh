#!/bin/bash

for filename in *; do
    if test -f "$filename" then
        valgrind --leak-check=full --error-exitcode=1
    fi
done
