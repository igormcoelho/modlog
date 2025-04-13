cmake_minimum_required(VERSION 4.0.0)
set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
#set(CMAKE_CXX_COMPILER /usr/bin/clang++-19)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
set(CMAKE_CXX_MODULE_STD 1)

project(example_project VERSION 0.1.0 LANGUAGES CXX)

Include(FetchContent)

FetchContent_Declare(modlog GIT_REPOSITORY https://github.com/igormcoelho/modlog.git GIT_TAG c481efe76d8cb372351c0ee9af863b7e016c59e8)
FetchContent_MakeAvailable(modlog)

add_executable(modlog1 main.cpp)
target_link_libraries(modlog1 PRIVATE modlog_module)



