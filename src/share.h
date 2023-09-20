#ifndef TENET_SHARED_H
#define TENET_SHARED_H

#if defined(_WIN32)
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif

EXPORT_SYMBOL FILE *get_stdout();

EXPORT_SYMBOL FILE *get_stderr();

#endif
