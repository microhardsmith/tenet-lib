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

The project is relatively simple, there is no need for build commands to be synchronized between different operating systems, just run the following commands on your demand.

### Windows:

``` shell
clang -std=c17 -Wall -Wextra -Werror -Wvla -shared -march=native -O3 -g0 -fcolor-diagnostics -fansi-escape-codes -v .\src\share.c .\src\rpmalloc.c .\src\lib_win.c .\src\wepoll.c -lAdvapi32 -lws2_32 -pedantic -o .\lib\tenet.dll
```

### Linux:

``` shell
# Using gcc
gcc -std=c17 -Wall -Wextra -Werror -Wvla -shared -march=native -O3 -g0 -fPIC -flto -fvisibility=hidden -D_GNU_SOURCE -v ./src/share.c ./src/rpmalloc.c ./src/lib_linux.c -Wl,-s -pedantic -o ./lib/libtenet.so

# Using clang
clang -std=c17 -Wall -Wextra -Werror -Wvla -shared -march=native -O3 -g0 -fcolor-diagnostics -fansi-escape-codes -fPIC -flto -fvisibility=hidden -D_GNU_SOURCE -v ./src/share.c ./src/rpmalloc.c ./src/lib_linux.c -Wl,-s -pedantic -o ./lib/libtenet.so
```

### macOS

``` shell
clang -std=c17 -Wall -Wextra -Werror -Wvla -shared -march=native -O3 -g0 -fcolor-diagnostics -fansi-escape-codes -fPIC -flto -fvisibility=hidden -o ./lib/libtenet.dylib -v ./src/share.c ./src/rpmalloc.c ./src/lib_macos.c -Wl,-s -pedantic -o ./lib/libtenet.dylib
```

then the dynamic library would be generated under the `lib` dir.

## Usage

Specify the system propeties for your tenet application with `-DTENET_LIBRARY_PATH=/path/to/lib`.