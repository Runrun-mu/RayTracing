cmake_minimum_required(VERSION 3.10) 
project(raytracing)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Vector.hpp Color.hpp Ray.hpp Hitable.hpp Sphere.hpp Hitablelist.hpp Utils.hpp Camera.hpp Material.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
