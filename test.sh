#!/bin/sh

if cmake --build build; then
    ./build/nbpp --dt-e "$@"
else
    echo "[ERROR]: Compilation terminated with non-zero exit code."
fi
