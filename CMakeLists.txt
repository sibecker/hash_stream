cmake_minimum_required(VERSION 3.12)
project(hash_stream)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES MSVC)
    message("Appplying MSVC specific settings")
    add_compile_definitions(SHA2_USE_INTTYPES_H)
    add_compile_definitions(BYTE_ORDER=1234 LITTLE_ENDIAN=1234 BIG_ENDIAN=4321)
    add_compile_definitions(__BYTE_ORDER__=1234 __ORDER_LITTLE_ENDIAN__=1234 __ORDER_BIG_ENDIAN__=4321)
endif()

add_subdirectory(src)
add_subdirectory(tests)
