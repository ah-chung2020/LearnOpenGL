#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)

message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
  
#本工程的名字
project(LearnOpenGL)

add_definitions(-DDEBUG)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)

file(GLOB copyAssets "./assets")
file(COPY ${copyAssets} DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(shader)

#本工程所有cpp文件编译链接，生成exe
add_executable(LearnOpenGl "main.cpp" "glad.c" "Base.h")

target_link_libraries(LearnOpenGl "${PROJECT_SOURCE_DIR}/lib/glfw3.lib" wrapperLib shaderLib appLib)

