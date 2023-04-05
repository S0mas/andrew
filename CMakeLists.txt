cmake_minimum_required(VERSION 3.0.0)
project(Bitstream VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Bitstream main.cpp Bitstream.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set_property(TARGET Bitstream PROPERTY CXX_STANDARD 20)
include(CPack)
