#!/bin/bash

sourcelist="sourcelist.cmake"

echo "SET(SOURCES \${SOURCES}" > $sourcelist

find ./src -type f >> $sourcelist

echo ")" >> $sourcelist