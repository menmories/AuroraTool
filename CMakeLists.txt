cmake_minimum_required(VERSION 3.10)


project(AuroraTool)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)

else()
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
endif(MSVC)

# 设置 C++ 标准为 C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(AuroraServer)
add_subdirectory(AuroraProxy)
add_subdirectory(AuroraClient)

