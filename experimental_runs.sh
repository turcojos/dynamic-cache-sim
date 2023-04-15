#!/bin/bash

make

./cache_sim traces/large_p_32000_8
./cache_sim traces/large_p_32000_16
./cache_sim traces/large_p_32000_32
./cache_sim traces/large_p_32000_64
