message("Client CMake")

add_executable(client_test main.cpp)
target_link_libraries(client_test PUBLIC KikukeLib)