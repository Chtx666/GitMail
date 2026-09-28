cmake_minimum_required(VERSION 3.20)

project(GitMail LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GitMail
    src/main.cpp
    src/all.cpp
)

target_include_directories(GitMail
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

if(MINGW)
    target_link_options(GitMail PRIVATE -static -static-libgcc -static-libstdc++)
endif()
