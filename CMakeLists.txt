cmake_minimum_required(VERSION 3.12)
project(hash_append)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES MSVC)
    message("Appplying MSVC specific settings")
    add_compile_definitions(SHA2_USE_INTTYPES_H)
    add_compile_definitions(BYTE_ORDER=1234 LITTLE_ENDIAN=1234 BIG_ENDIAN=4321)
    add_compile_definitions(__BYTE_ORDER__=1234 __ORDER_LITTLE_ENDIAN__=1234 __ORDER_BIG_ENDIAN__=4321)
endif()

add_library(hash STATIC city.cc sha2.c siphash.cpp SpookyV2.cpp xxhash.c)

add_library(hash_test STATIC hash_test.cpp)

add_executable(dates dates.cpp)
target_link_libraries(dates PUBLIC hash_test hash)
add_test(NAME dates COMMAND dates)

add_executable(m16 m16.cpp)
target_link_libraries(m16 PUBLIC hash_test)
add_test(NAME m16 COMMAND m16)

add_executable(pairii pairii.cpp)
target_link_libraries(pairii PUBLIC hash_test)
add_test(NAME pairii COMMAND pairii)

add_executable(sherlock sherlock.cpp)
target_link_libraries(sherlock PUBLIC hash_test hash)
add_test(NAME sherlock COMMAND sherlock)

add_executable(sizes sizes.cpp)
add_test(NAME sizes COMMAND sizes)

add_executable(test1 test.cpp hash_functors.cpp)
target_link_libraries(test1 PUBLIC hash hash_test)
add_test(NAME test1 COMMAND test1)

add_executable(test2 test2.cpp)
add_test(NAME test2 COMMAND test2)

# add_executable(test3 test3.cpp)
# add_test(NAME test3 COMMAND test3)

add_executable(test4 test4.cpp Handle.cpp)
target_link_libraries(test4 PUBLIC hash)
add_test(NAME test4 COMMAND test4)
