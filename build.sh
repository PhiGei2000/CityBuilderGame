#!/bin/bash

if [[ ! -e ./build ]]; then
    echo create build directory

    mkdir ./build
fi

cd ./build
cmake -DCMAKE_BUILD_TYPE:STRING=Debug ..

cmake --build $PWD --config Debug --target all --

if [[ ! -e ./res ]]; then
    echo linking resource directory
    ln -s ../res ./res
fi