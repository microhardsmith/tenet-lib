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

All presets share a single `build/` directory, so configure once with a preset and then build with the
ordinary command:

``` shell
cmake --preset default
cmake --build build
```

That produces `lib/libtenet.so`, `lib/libtenet.dylib` or `lib/tenet.dll`, depending on the platform.

### Presets

| Preset     | Result                                                                   |
|------------|--------------------------------------------------------------------------|
| `default`  | Release, optimised and stripped. The binary normally committed to `lib/`. |
| `release`  | Release, but the symbol table is kept for backtraces.                    |
| `debug`    | Unoptimised with full debug info.                                        |
| `native`   | Release tuned for the current CPU with `-march=native`.                  |

A preset only decides how `build/` is configured. To switch, configure the new preset and build again:

``` shell
cmake --preset debug
cmake --build build
```

Because every preset writes into the same `build/` and the same `lib/` directory, switching presets replaces
the previous library in place. Copy the result aside first if you want to keep more than one.

Every preset pins the full set of `TENET_*` cache variables, including the ones it does not change. This matters
because `build/` is shared: a variable left behind in the cache would otherwise survive into the next
configuration and silently produce the wrong artifact, for instance keeping a Release build unstripped because a
`debug` configure had been there before.

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

### Windows and MSVC

The Windows backend builds with MSVC as well as with MinGW or clang-cl. CMake picks whichever toolchain you
point it at:

``` shell
cmake --preset default
cmake --build build
```

or explicitly:

``` shell
cmake --preset default -DCMAKE_C_COMPILER=cl
cmake --build build
```

Three things are worth knowing:

- `likely()` and `unlikely()` in `src/share.h` fall back to a plain pass-through where `__builtin_expect` does
  not exist. Both spellings normalise their argument to 0 or 1 and evaluate it exactly once, so the MSVC build
  behaves identically to the GCC and Clang builds, minus the branch hints. You will see slightly worse code
  generation, no behaviour change.
- MSVC gets `/W4` but not `/WX`. Warnings are not promoted to errors here because `lib_win.c` and `wepoll.c`
  cannot be compiled on Linux or macOS, so MSVC specific diagnostics are the one part of this build that CI on
  those platforms cannot vouch for. Verify a clean `/W4` build on Windows before adding `/WX`.
- `/std:c17` is emitted for you by `CMAKE_C_STANDARD`, so a Visual Studio 2019 16.8 or newer toolset is
  required. Older toolsets silently fall back to an older C standard and then fail on C99 constructs.

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