cmake_minimum_required(VERSION 3.13)


project(marl_sandbox C)

add_executable(marl_sandbox main.c)

target_include_directories(marl_sandbox PRIVATE ${CMAKE_SOURCE_DIR}/../marl_lib/include)
target_link_libraries(marl_sandbox ${CMAKE_SOURCE_DIR}/../marl_lib/build/libmarl_lib.so)


