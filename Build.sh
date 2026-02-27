#!/usr/bin/env bash

rm -rf build
mkdir -p build
cd build
cmake -G "Unix Makefiles" ..
make
