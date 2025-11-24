cmake_minimum_required(VERSION 3.10)


project(AuroraServer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C++ 标准为 C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES
    "include/PlatformDef.h"
    "include/AuroraLog.h"
    "include/AuroraPackage.h"
    "include/AuroraServer.h"
    "include/EventServer.h"
    "include/AuroraServerConfig.h"
)

set(SOURCE_FILES
    "src/main.cpp"
    "src/AuroraLog.cpp"
    "src/AuroraPackage.cpp"
    "src/EventServer.cpp"
    "src/AuroraServer.cpp"
    "src/AuroraServerConfig.cpp"
)

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)

    #添加visual studio 文件过滤器
    foreach(header ${PROJECT_HEADERS})
        string(REGEX REPLACE "/[^/]*$" "" file_filter ${header})
        string(REPLACE "include" "" file_group_path ${file_filter})
        source_group("Header Files/${file_group_path}" FILES "${header}")
        # message("header_filter=${file_filter}")
    endforeach()

    foreach(source ${PROJECT_SOURCE})
        string(REGEX REPLACE "/[^/]*$" "" file_filter ${source})
        string(REPLACE "src" "" file_group_path ${file_filter})
        source_group("Source Files/${file_group_path}" FILES "${source}")
        # message("source_file_filter=${file_filter}")
    endforeach()
endif(MSVC)


include_directories(
    "include"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  message("using clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  message("using gcc")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  message("using MSVC")
endif()

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})



if(MSVC)
    target_include_directories(${PROJECT_NAME} PRIVATE
        "D:/libevent/include"
    )

    target_link_directories(${PROJECT_NAME} PRIVATE
        "D:/libevent/lib"
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32
        Iphlpapi
        event
        event_core
        event_extra
        event_openssl
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        event
        event_core
        event_extra
        event_openssl
    )
endif(MSVC)




