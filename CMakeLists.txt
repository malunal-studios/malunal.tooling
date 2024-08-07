cmake_minimum_required(VERSION 3.16)
project(simular.tooling VERSION 1.0.2 LANGUAGES CXX)

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${PROJECT_NAME} INTERFACE)
add_library(simular::tooling ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include)

option(SIMULAR_TOOLING_BUILD_EXAMPLE "Build example" OFF)
if(SIMULAR_TOOLING_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
