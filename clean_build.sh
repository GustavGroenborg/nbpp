#!/bin/sh

if [ -d "build/" ]; then
    echo "[INFO]: Cleaning 'build' directory"
    rm -rf build/*
else
    echo "[INFO]: Creating 'build' directory"
    mkdir build/
fi

if [ -z "$1" ]; then 
    vcpkg install
    cmake -B build -S . --preset debug
    cmake --build build
else
    vcpkg install
    cmake -B build -S . --preset "$1"
    cmake --build build
fi
