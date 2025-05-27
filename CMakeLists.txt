cmake_minimum_required(VERSION 3.10.0)
project(CreateShmem VERSION 0.1.0 LANGUAGES C CXX)

add_executable(CreateShmem create_shmem.cpp)

include(CTest)
enable_testing()

set_property(TARGET CreateShmem PROPERTY CXX_STANDARD 17)