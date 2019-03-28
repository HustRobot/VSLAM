cmake_minimum_required(VERSION 3.0) 
project( baseSLAM )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )

set( src_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

include_directories( ${PROJECT_SOURCE_DIR}/include )

# g2o  添加cmake模块
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

# opencv 
find_package( OpenCV 3.1 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# eigen 
include_directories( "/usr/include/eigen3/" )

find_package( G2O REQUIRED )
find_package( CSparse REQUIRED )

# pcl 
find_package( PCL REQUIRED COMPONENT common io filters visualization)
include_directories( ${PCL_INCLUDE_DIRS} )
add_definitions( ${PCL_DEFINITIONS} )

# octomap 
find_package( octomap REQUIRED )
include_directories( ${OCTOMAP_INCLUDE_DIRS} )

include_directories( 
    ${OpenCV_INCLUDE_DIRS} 
    ${G2O_INCLUDE_DIRS}
    ${CSPARSE_INCLUDE_DIR}
    "/usr/include/eigen3/"
)



add_executable( generate_pointcloud ${src_dir}generatePointCloud.cpp )
target_link_libraries( generate_pointcloud 
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
)

add_executable( slambase ${src_dir}slambase.cpp )
target_link_libraries( slambase 
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
)

add_executable( detectFeatures ${src_dir}detectFeatures.cpp )
target_link_libraries( detectFeatures 
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
)

add_executable( joinPointCloud ${src_dir}joinPointCloud.cpp )
target_link_libraries( joinPointCloud
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
)

add_executable( main ${src_dir}main.cpp )