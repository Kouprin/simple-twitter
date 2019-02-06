#!/usr/bin/env bash
set -e
#cmake .
#make
# cmake can't find boost - just don't spend time on this
g++ -o messenger-engine main.cpp utils.cpp data.cpp -lboost_system -O0
#cgdb ./messenger-engine
./messenger-engine
