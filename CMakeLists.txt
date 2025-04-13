cmake_minimum_required(VERSION 4.0.0)
#set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
#set(MODLOG_GCC ON)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++-19)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
set(CMAKE_CXX_MODULE_STD 1)

project(modlog_project VERSION 0.2.0 LANGUAGES CXX)

add_library(modlog INTERFACE)
add_library(modlog::modlog ALIAS modlog)
target_include_directories(modlog INTERFACE include/)

add_library(modlog_module)
target_sources(modlog_module  PUBLIC  FILE_SET CXX_MODULES FILES   src/modlog.cppm)
target_link_libraries(modlog_module PRIVATE modlog)

add_executable(demo1 demo/demo1.cpp)
target_link_libraries(demo1 PRIVATE modlog)

add_executable(demo2 demo/demo2.cpp)
target_link_libraries(demo2 PRIVATE modlog_module)

add_definitions(-DNDEBUG)

add_executable(demo3 demo/demo3.cpp)
target_link_libraries(demo3 PRIVATE modlog)

add_executable(demo4 demo/demo4.cpp)
target_link_libraries(demo4 PRIVATE modlog)

add_executable(demo5 demo/demo5.cpp)
target_link_libraries(demo5 PRIVATE modlog)

if(MODLOG_GCC)
#add_definitions(-DMODLOG_STACKTRACE=1)
add_executable(demo5_stacktrace demo/demo5.cpp)
target_link_libraries(demo5_stacktrace PRIVATE modlog)
target_link_libraries(demo5_stacktrace PRIVATE -lstdc++exp)
target_compile_definitions(demo5_stacktrace PUBLIC -DMODLOG_STACKTRACE=1)
endif()



