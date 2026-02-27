cmake_minimum_required(VERSION 3.16)
project(pwatsql LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(sqlite3 STATIC
  third_party/sqlite3/sqlite3.c
)

target_include_directories(sqlite3 PUBLIC
  third_party/sqlite3
)

find_package(Threads REQUIRED)
target_link_libraries(sqlite3 PUBLIC Threads::Threads dl)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
