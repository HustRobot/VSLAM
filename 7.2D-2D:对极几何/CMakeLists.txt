#1.cmake verson，指定cmake版本 
cmake_minimum_required( VERSION 2.8 )

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
project( vo1 )

#3.指定编译模式
set( CMAKE_BUILD_TYPE "Release" )
set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

#4.查找库
find_package( OpenCV 3.1 REQUIRED )


#5.head file path，头文件目录
include_directories( 
    ${OpenCV_INCLUDE_DIRS} 
)


#6.add executable file，添加要编译的可执行文件
add_executable( pose_estimation_2d2d pose_estimation_2d2d.cpp )

#7.add link library，添加可执行文件所需要的库
target_link_libraries( pose_estimation_2d2d ${OpenCV_LIBS} )