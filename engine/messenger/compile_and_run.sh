#!/usr/bin/env bash
set -e
#cmake .
#make
# cmake can't find boost - just don't spend time on this
g++ -o messenger-engine main.cpp utils.cpp -lboost_system
./messenger-engine
