![logo](https://github.com/microhardsmith/tenet-lib/blob/master/tenet.png)

## Introduction

This repository contains the C source code and dynamic library for tenet project.
The `src` folder contains all the source code for generating `libtenet` dynamic library.
The `lib` folder contains some prebuilt dynamic library which might just fit into your system.

The tenet project would be using some well known open-source librarys if needed, including:

- [deflate](https://github.com/ebiggers/libdeflate)
- [brotli](https://github.com/google/brotli/tree/master) 
- [sqlite](https://github.com/sqlite/sqlite)
- [duckdb](https://github.com/duckdb/duckdb)
- [openssl](https://github.com/openssl/openssl)

If you are using features with these libraries involved, you are recommended to build them yourself, and put them under the same folder with `libtenet`, for your application to load them. 

## Build

The build is driven by CMake and pinned to the [Ninja](https://ninja-build.org/) generator.

### Requirements

- CMake 3.21 or newer
- Ninja
- A C17 capable compiler (GCC, Clang, or MSVC). No particular compiler is selected for you, CMake uses `cc`/`cl`.

### Commands

``` shell
cmake --preset default
cmake --build --preset default
```

That produces `lib/libtenet.so`, `lib/libtenet.dylib` or `lib/tenet.dll`, depending on the platform.

### Presets

| Preset     | Result                                                                   |
|------------|--------------------------------------------------------------------------|
| `default`  | Release, optimised and stripped. The binary normally committed to `lib/`. |
| `release`  | Release, but the symbol table is kept for backtraces.                    |
| `debug`    | Unoptimised with full debug info.                                        |
| `native`   | Release tuned for the current CPU with `-march=native`.                  |

Configure and build are separate steps, so each preset needs its own pair:

``` shell
cmake --preset debug
cmake --build --preset debug
```

Every preset writes to the same `lib/` directory, so building one preset replaces the library produced by
another. Copy the result aside first if you want to keep both.

### Ninja is not optional

`CMakeLists.txt` rejects any other generator outright. The link step is the only place where the platform
specific system libraries are named, and the generator decides the exact link command line, so letting the two
drift apart unnoticed tends to surface as a link error on one platform only. If a tool you need insists on a
different generator, add `-DTENET_ALLOW_ANY_GENERATOR=ON` to bypass the check.

A plain `cmake -B build -G Ninja` also works, but the presets additionally set the build type, the binary
directory and the symbol stripping.

### Options

| Option                     | Default | Effect                                                                          |
|----------------------------|---------|---------------------------------------------------------------------------------|
| `TENET_NATIVE_ARCH`        | `OFF`   | Adds `-march=native`. Machine specific, see the warning below.                   |
| `TENET_STRIP`              | `ON`    | Strips the symbol table, matching the `-O3 -g0 -Wl,-s` of the previous commands. |
| `TENET_ALLOW_ANY_GENERATOR`| `OFF`   | Permits a non-Ninja generator.                                                   |

`-march=native` optimises for whichever CPU ran the build, so the resulting library can fault on an older
machine. Leave it off for anything you commit or hand to someone else; the `native` preset turns it on.

### Platform sources

Only one backend is compiled in, selected by CMake:

| Platform | Backend             | Extra dependencies |
|----------|---------------------|--------------------|
| Linux    | `src/lib_linux.c`   | epoll              |
| macOS    | `src/lib_macos.c`   | kqueue             |
| Windows  | `src/lib_win.c`, `src/wepoll.c` | `Advapi32`, `ws2_32` |

`src/share.c` and the vendored `src/rpmalloc.c` are always part of the library. Only the functions marked
`EXPORT_SYMBOL` in `src/share.h` are meant to be public API.


## Usage

Specify the system propeties for your tenet application with `-DTENET_LIBRARY_PATH=/path/to/lib`.