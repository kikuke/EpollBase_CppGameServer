message("#KikukeLib#")
add_subdirectory(lsys)
add_subdirectory(service)
add_subdirectory(struct)
add_subdirectory(utility)

file(GLOB_RECURSE KIKUKELIB "*.h" "*.c" "*.cpp")

message("KikukeLib: ${KIKUKELIB}")

add_library(KikukeLib STATIC ${KIKUKELIB})
target_include_directories(KikukeLib PUBLIC lsys Server_Define service struct utility)