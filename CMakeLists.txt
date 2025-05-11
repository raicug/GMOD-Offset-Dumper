cmake_minimum_required(VERSION 3.10)
project(gmod_dumper)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(DIRECTX_SDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)")

include_directories("${DIRECTX_SDK_DIR}/Include")

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    link_directories("${DIRECTX_SDK_DIR}/Lib/x64")
else ()
    link_directories("${DIRECTX_SDK_DIR}/Lib/x86")
endif ()

if (MSVC)
    add_definitions(
            -D_AMD64_
            -D_WIN64
            -D_CRT_SECURE_NO_WARNINGS
            -DBUILDING_DLL
            -DUNICODE
            -D_UNICODE
    )
    add_compile_options(
            /WX-
            /wd4244
    )

    set(CMAKE_GENERATOR_PLATFORM x64)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(math_sources
        math/math.h
        math/math.cpp
        math/vector.h
        math/vector.cpp
)

set(sdk_sources
    sdk/interfaces.h
    sdk/interfaces.cpp
    sdk/utils/md5_check_sum.cpp
    sdk/utils/md5_check_sum.h
    sdk/utils/netvars.cpp
    sdk/utils/netvars.h
)

add_library(gmod_dumper SHARED library.cpp
        ${sdk_sources}
)

target_link_libraries(gmod_dumper
        winmm
        d3d9
        d3dx9
        dwmapi
        wininet
)

set_target_properties(gmod_dumper PROPERTIES
        OUTPUT_NAME "libdumper"
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)