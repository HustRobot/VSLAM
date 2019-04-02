cmake_minimum_required( VERSION 2.8 )
project( ceres_curve_fitting )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

# 添加cmake模块以使用ceres库
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

# 寻找Ceres库并添加它的头文件
find_package( Ceres REQUIRED )
include_directories( ${CERES_INCLUDE_DIRS} )

# OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_DIRS} )

add_executable( curve_fitting main.cpp )
target_link_libraries( curve_fitting ${CERES_LIBRARIES} ${OpenCV_LIBS} )

add_executable( my_cerestest my_cerestest.cpp )
target_link_libraries( my_cerestest ${CERES_LIBRARIES} ${OpenCV_LIBS} )

add_executable( theme_test theme_test.cpp )
target_link_libraries( theme_test ${OpenCV_LIBS} )