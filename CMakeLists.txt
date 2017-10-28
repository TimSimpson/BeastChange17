project (BeastChange17 CXX)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# macro(add_exe FILENAME)
#     add_executable(${FILENAME}
#                    ${CMAKE_CURRENT_SOURCE_DIR}/src/${FILENAME}.cpp)
#     target_link_libraries(${FILENAME} lp3_deps::boost-coroutine lp3_deps::boost-context lp3_deps::boost-system)
# endmacro(add_exe)

# add_exe(asio_example1)
# add_exe(asio_example2)
# add_exe(exceptions)
# add_exe(fibonacci)
# add_exe(fibonacci2)
# add_exe(push_type_range)
# add_exe(reverse_fibonacci)
# add_exe(symmetric_coro)

add_executable(beast_example "${CMAKE_CURRENT_SOURCE_DIR}/src/beast_example.cpp")
target_link_libraries(beast_example lp3_deps::boost-beast lp3_deps::boost-coroutine lp3_deps::boost-context lp3_deps::boost-system)
