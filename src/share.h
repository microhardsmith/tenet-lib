#ifndef TENET_SHARED_H
#define TENET_SHARED_H

#if defined(_WIN32)
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#include <stdio.h>

EXPORT_SYMBOL FILE *get_stdout();

EXPORT_SYMBOL FILE *get_stderr();

EXPORT_SYMBOL int get_fbf();

EXPORT_SYMBOL int get_lbf();

EXPORT_SYMBOL int get_nbf();

#endif
