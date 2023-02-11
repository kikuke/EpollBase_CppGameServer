message("#KikukeLib#")

file(GLOB_RECURSE KIKUKELIB "*.h" "*.c" "*.cpp")

message("KikukeLib: ${KIKUKELIB}")

add_library(KikukeLib STATIC ${KIKUKELIB})
target_include_directories(KikukeLib PUBLIC lsys Server_Define service struct utility)