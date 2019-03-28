cmake_minimum_required(VERSION 3.0)  
project( Geometrytransfer )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

set( src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

# Eigen
include_directories( "/usr/include/eigen3" )

include_directories( ${PROJECT_SOURCE_DIR}/include )

add_executable( Geometrytransfer ${src_dir}main.cpp )