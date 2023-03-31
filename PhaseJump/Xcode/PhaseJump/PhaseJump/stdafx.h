#ifndef PRECOMPILEDHEADER_H
#define PRECOMPILEDHEADER_H

#ifdef DEBUG
#define PJLog(...) printf(__VA_ARGS__); printf("\n");
#else
#define PJLog(...) {}
#endif

#endif
