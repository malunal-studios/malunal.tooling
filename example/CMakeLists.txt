cmake_minimum_required(VERSION 3.16)
project(simular.tooling.example VERSION 1.0.0 LANGUAGES CXX)

add_executable(example testing.cpp)
target_link_libraries(example PRIVATE simular::tooling)
target_compile_definitions(example PRIVATE SIMULAR_TOOLING_ENABLE_MACROS)
