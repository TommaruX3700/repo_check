cmake_minimum_required(VERSION 3.8)
project("repo_check")

add_executable(repo_check ./main.cpp)

target_include_directories(repo_check PRIVATE
    ./headers
)

file(GLOB SOURCES ./src/*.cpp)

target_sources(repo_check PRIVATE ${SOURCES})