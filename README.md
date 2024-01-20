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

Just install `clang` and run a single command:

``` shell
make
```

then the dynamic library would be generated under the `lib` dir.

## Usage

Specify the system propeties for your tenet application with `-DTENET_LIBRARY_PATH=/path/to/lib`.