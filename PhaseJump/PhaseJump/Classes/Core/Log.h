#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define PJLog(...) printf(__VA_ARGS__); printf("\n");

// TODO: fix this (not working)
//#ifdef DEBUG
//#define PJLog(...) printf(__VA_ARGS__); printf("\n");
//#else
//#define PJLog(...) {}
//#endif

#endif
