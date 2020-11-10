#!/bin/bash

# Script to help automate testing
# Creates the library, moves it to the test folder, then cleans up
make libcds.a
mv libcds.a ../test
cp -r include/ ../test/
make clean
