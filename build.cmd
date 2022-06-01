@echo off
cd .\src
clang -Weverything -Wno-c++98-compat -std=c++20 -O2 .\remv.cpp
