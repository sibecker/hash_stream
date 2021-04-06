add_library(hash STATIC city.cc city_hash.cpp functors.cpp sha2.c siphash.cpp SpookyV2.cpp xxhash.c)
target_include_directories(hash PUBLIC ../include)
