message("Client CMake")

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(client_test PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(client_test "${CMAKE_THREAD_LIBS_INIT}")
endif()

add_executable(client_test main.cpp)
target_link_libraries(client_test PUBLIC KikukeLib)