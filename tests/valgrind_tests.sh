#!/bin/bash

for filename in bin/*; do
    if [ -f "$filename" -a -x "$filename" ]; then
        valgrind --leak-check=full --error-exitcode=1 ./"$filename"
    fi
done
