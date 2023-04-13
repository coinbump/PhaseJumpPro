#ifndef LOG_H
#define LOG_H

#ifdef DEBUG
#define PJLog(...) printf(__VA_ARGS__); printf("\n");
#else
#define PJLog(...) {}
#endif

#endif
