cmake_minimum_required(VERSION 3.10)

project(PocEngine VERSION 0.1.0)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(PROJECT_3RD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third-party")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM 64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(PLATFORM 32)
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/poc-engine")
add_subdirectory("${PROJECT_SOURCE_DIR}/demos/demo-01-simple-window")
add_subdirectory("${PROJECT_SOURCE_DIR}/demos/demo-02-draw-simple-shape")
