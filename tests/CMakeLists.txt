add_library(hash_test STATIC hash_test.cpp)
target_link_libraries(hash_test PUBLIC hash)

add_executable(dates dates.cpp)
target_link_libraries(dates PUBLIC hash_test)
add_test(NAME dates COMMAND dates)

# add_executable(example example.cpp)
# add_test(NAME example COMMAND example)

add_executable(m16 m16.cpp)
target_link_libraries(m16 PUBLIC hash_test)
add_test(NAME m16 COMMAND m16)

add_executable(pairii pairii.cpp)
target_link_libraries(pairii PUBLIC hash_test)
add_test(NAME pairii COMMAND pairii)

add_executable(sherlock sherlock.cpp)
target_link_libraries(sherlock PUBLIC hash_test)
add_test(NAME sherlock COMMAND sherlock)

add_executable(sizes sizes.cpp)
target_link_libraries(sizes PUBLIC hash)
add_test(NAME sizes COMMAND sizes)

add_executable(test1 test.cpp)
target_link_libraries(test1 PUBLIC hash hash_test)
add_test(NAME test1 COMMAND test1)

add_executable(test2 test2.cpp)
target_link_libraries(test2 PUBLIC hash)
add_test(NAME test2 COMMAND test2)

# add_executable(test3 test3.cpp)
# add_test(NAME test3 COMMAND test3)

add_executable(test4 test4.cpp Handle.cpp)
target_link_libraries(test4 PUBLIC hash)
add_test(NAME test4 COMMAND test4)
