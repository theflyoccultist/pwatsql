cmake_minimum_required(VERSION 3.20)
project(pwatsql LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(sqlite3 STATIC
  third_party/sqlite3/sqlite3.c
)

target_include_directories(sqlite3 PUBLIC
  third_party/sqlite3
)

find_package(Threads REQUIRED)
target_link_libraries(sqlite3 PUBLIC Threads::Threads dl)

add_library(${PROJECT_NAME}
  src/AssetRepository.cpp
  src/Database.cpp
  src/Logs.cpp
  src/Statement.cpp
  src/Transaction.cpp
)

add_library(pwat::sqlite ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} 
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC sqlite3)

add_executable(example_basic examples/basic.cpp)
target_link_libraries(example_basic PRIVATE ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
