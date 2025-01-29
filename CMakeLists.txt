cmake_minimum_required(VERSION 3.8)
project("repo_check")

add_executable(repo_check ./main.cpp)

target_include_directories(repo_check PRIVATE
    ./headers
)

file(GLOB SOURCES ./src/*.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configuration.json ${CMAKE_CURRENT_BINARY_DIR}/configuration.json COPYONLY)

target_sources(repo_check PRIVATE ${SOURCES})