cmake_minimum_required(VERSION 3.10)
project(psql_benchmark_scenarios C)

set(CMAKE_C_STANDARD 99)

include (FindPkgConfig)
find_package(PostgreSQL REQUIRED)

include_directories(${PostgreSQL_INCLUDE_DIRS})


add_executable(psql_benchmark_scenarios main.c shared.c pgman.c)

target_link_libraries(psql_benchmark_scenarios ${PostgreSQL_LIBRARIES})
