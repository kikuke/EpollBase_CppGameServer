cmake_minimum_required(VERSION 3.0.0)
project(EpollServer VERSION 0.1.0)

message("Root CMake")
include(CTest)
enable_testing()

add_subdirectory(KikukeLib)
add_subdirectory(Server)
add_subdirectory(Client)

message("END")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)