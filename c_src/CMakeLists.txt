cmake_minimum_required(VERSION 3.12)
project(julia_demo VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(Python3 COMPONENTS Interpreter Development)
find_package(Boost REQUIRED COMPONENTS "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")

add_library(julia_demo MODULE demo.cpp)
set_target_properties(julia_demo PROPERTIES PREFIX "")

target_include_directories(julia_demo PRIVATE ${Boost_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
target_link_libraries(julia_demo ${Boost_LIBRARIES} ${Python3_LIBRARIES})
