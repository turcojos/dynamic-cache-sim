#!/bin/bash

make
./cache_sim traces/large_nu_32000_1000
./cache_sim traces/large_nu_32000_2000
./cache_sim traces/large_nu_32000_4000
./cache_sim traces/large_nu_32000_6000
./cache_sim traces/large_nu_32000_8000
./cache_sim traces/large_nu_32000_10000

./cache_sim traces/large_u

./cache_sim traces/large_p_32000_8
./cache_sim traces/large_p_32000_16
./cache_sim traces/large_p_32000_32
./cache_sim traces/large_p_32000_64
