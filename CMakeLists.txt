cmake_minimum_required(VERSION 3.15)
project(EgoisticOS)

set(CMAKE_CXX_STANDARD 99)

set(MINGW32_64_GCC /usr/bin/x86_64-w64-mingw32-gcc)
set(NATIVE_GCC /usr/bin/cc)
set(CMAKE_C_COMPILER ${MINGW32_64_GCC})
set(CMAKE_C_COMPILER ${NATIVE_GCC})

if (${CMAKE_C_COMPILER} STREQUAL ${MINGW32_64_GCC})
    add_subdirectory(boot)
endif ()

if (${CMAKE_C_COMPILER} STREQUAL ${NATIVE_GCC})
    add_subdirectory(kernel)
endif ()
