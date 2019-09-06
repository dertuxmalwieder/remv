@echo off
cd .\src
clang -Weverything -Wno-c++98-compat -O2 .\remv.cpp
