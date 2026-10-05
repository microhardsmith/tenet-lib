#ifndef TENET_SHARED_H
#define TENET_SHARED_H

#if defined(_WIN32)
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif

// Branch hints. Only GCC and Clang expose __builtin_expect, so MSVC gets a plain pass-through. Both
// spellings evaluate their argument exactly once and yield the same normalised 0/1 result, so the two
// branches cannot drift apart in behaviour; the MSVC one merely stops steering the optimizer. Both
// spellings are used heavily in the backends, always in boolean position.
#if defined(__GNUC__) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (!!(x))
#define unlikely(x) (!!(x))
#endif

#include <stdio.h>

EXPORT_SYMBOL FILE *get_stdout();

EXPORT_SYMBOL FILE *get_stderr();

EXPORT_SYMBOL int get_fbf();

EXPORT_SYMBOL int get_lbf();

EXPORT_SYMBOL int get_nbf();

EXPORT_SYMBOL int rp_initialize();

EXPORT_SYMBOL void rp_tinitialize();

EXPORT_SYMBOL void rp_tfinalize();

EXPORT_SYMBOL void rp_finalize();

EXPORT_SYMBOL void *rp_malloc(size_t size);

EXPORT_SYMBOL void rp_free(void *ptr);

EXPORT_SYMBOL void *rp_realloc(void *ptr, size_t size);

#endif
