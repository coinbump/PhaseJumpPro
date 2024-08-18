#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdio.h>

#define PJLog(...)       \
    printf(__VA_ARGS__); \
    std::cout << std::endl;

// TODO: fix this (not working)
// TODO: Rename to ALL CAPS?
// #ifdef DEBUG
// #define PJLog(...) printf(__VA_ARGS__); printf("\n");
// #else
// #define PJLog(...) {}
// #endif

#endif
