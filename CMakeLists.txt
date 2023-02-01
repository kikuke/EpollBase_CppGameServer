cmake_minimum_required(VERSION 3.0.0)
project(EpollServer VERSION 0.1.0)

include(CTest)
enable_testing()

#나중에 링크 세부설정 제대로 하기
file(GLOB PROJ_SRC "*.h" "*.c" "*.cpp")
add_executable(EpollServer main.cpp ${PROJ_SRC})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
