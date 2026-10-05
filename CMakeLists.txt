cmake_minimum_required(VERSION 3.21)

# The project is pinned to the Ninja generator. CMakePresets.json already selects it, but a hand written
# `cmake -B build` would silently pick a different one, and the reason Ninja is required is not cosmetic:
# the shared library link step is the only place where the platform specific system libraries are named, and
# the generator decides the exact link command line. Rather than let the two drift apart unnoticed, reject
# anything else up front. Set -DTENET_ALLOW_ANY_GENERATOR=ON to opt out (useful for IDE driven builds that
# only offer Makefile generators).
if(NOT CMAKE_GENERATOR MATCHES "Ninja")
  if(TENET_ALLOW_ANY_GENERATOR)
    message(WARNING
      "TENET_ALLOW_ANY_GENERATOR is ON, building with '${CMAKE_GENERATOR}' instead of Ninja. "
      "This is unsupported; if the link step behaves differently than CI, start here.")
  else()
    message(FATAL_ERROR
      "tenet-lib requires the Ninja generator, but CMake selected '${CMAKE_GENERATOR}'.\n"
      "Use one of:\n"
      "  cmake --preset default && cmake --build --preset default\n"
      "  cmake --preset release && cmake --build --preset release\n"
      "  cmake --preset debug   && cmake --build --preset debug\n"
      "To bypass this check anyway, add -DTENET_ALLOW_ANY_GENERATOR=ON.")
  endif()
endif()

project(tenet_lib C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# A single shared library, libtenet, plus the vendored rpmalloc allocator it hands out memory from.
# Only one platform backend is compiled in: the epoll based Linux one, the kqueue based macOS one, or the
# wepoll/IOCP based Windows one. lib_win.c includes wepoll.h itself, so wepoll.c joins the source list on
# Windows only.
set(TENET_LIB_SOURCES src/share.c src/rpmalloc.c)
if(WIN32)
    list(APPEND TENET_LIB_SOURCES src/lib_win.c src/wepoll.c)
    # Advapi32 supplies the NT APIs wepoll issues directly; ws2_32 is Winsock.
    set(TENET_LIB_SYSTEM_LIBS Advapi32 ws2_32)
elseif(APPLE)
    list(APPEND TENET_LIB_SOURCES src/lib_macos.c)
    set(TENET_LIB_SYSTEM_LIBS "")
elseif(UNIX)
    list(APPEND TENET_LIB_SOURCES src/lib_linux.c)
    set(TENET_LIB_SYSTEM_LIBS "")
    # lib_linux.c reads epoll_event.ulong through the GNU extensions for it.
    set(TENET_LIB_DEFINITIONS _GNU_SOURCE)
else()
    message(FATAL_ERROR "Unsupported platform: only Windows, macOS and Linux have a lib_*.c backend")
endif()

add_library(tenet SHARED ${TENET_LIB_SOURCES})
target_link_libraries(tenet PRIVATE ${TENET_LIB_SYSTEM_LIBS})
target_compile_definitions(tenet PRIVATE ${TENET_LIB_DEFINITIONS})

# -march=native tunes for the build machine and is therefore off by default: the prebuilt binaries in lib/
# are committed and reused by other machines, and a library tuned for one microarchitecture can fault on an
# older one. Turn it on for a local, machine specific build.
option(TENET_NATIVE_ARCH "Tune for the build machine with -march=native (do not use for committed binaries)" OFF)

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(tenet PRIVATE
        -Wall -Wextra -Werror -Wvla -pedantic
        # Only the functions marked EXPORT_SYMBOL in share.h leave the library, which keeps the rpmalloc
        # internals and the backend helpers from becoming part of the ABI.
        -fvisibility=hidden
        # The lib_*.h headers declare their exported functions as "int l_epoll_create();" rather than
        # "(void)", which current Clang rejects under -Werror because an empty parameter list is deprecated
        # in every version of C. -Werror is kept for everything else; the declarations themselves are left
        # alone because rewriting ~100 of them is a source level ABI decision, not a build system change.
        -Wno-strict-prototypes
        $<$<BOOL:${TENET_NATIVE_ARCH}>:-march=native>
    )
endif()

if(TENET_NATIVE_ARCH AND NOT CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    message(WARNING "TENET_NATIVE_ARCH only applies to Clang and GCC, ignoring it for ${CMAKE_C_COMPILER_ID}")
endif()

option(TENET_STRIP "Strip the symbol table and debug info from the shared library" ON)

# Release builds strip the symbol table and debug info, matching the -O3 -g0 -Wl,-s of the original hand
# written commands. A stripped library is what lib/ has always shipped, so this defaults to ON; turn it off
# when profiling or when you want a backtrace to name library internals.
if(NOT MSVC AND TENET_STRIP)
    target_link_options(tenet PRIVATE -Wl,-s)
    target_compile_options(tenet PRIVATE -g0)
endif()

# The library is built straight into lib/ next to the prebuilt third party binaries, because that directory is
# what the Java side points TENET_LIBRARY_PATH at. NativeUtil resolves both the bare and the lib prefixed name,
# but lib/ has always shipped libtenet.so and libtenet.dylib on Unix and tenet.dll on Windows, so keep those
# exact names rather than relying on the fallback: on Windows the "lib" prefix has to be suppressed explicitly,
# including for MinGW which would otherwise emit libtenet.dll.
set_target_properties(tenet PROPERTIES
    OUTPUT_NAME tenet
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
if(WIN32)
    set_target_properties(tenet PROPERTIES PREFIX "")
endif()

install(TARGETS tenet
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)