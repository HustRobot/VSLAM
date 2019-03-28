#include <iostream>
#include <stdlib.h>
#include <Eigen/Geometry>
#include <Eigen/Core>
#include <vector>
#include <math.h>
 
using namespace std;

Eigen::Quaterniond euler2Quaternion(const double roll, const double pitch, const double yaw);
Eigen::Vector3d Quaterniond2Euler(const double x,const double y,const double z,const double w);
Eigen::Matrix3d Quaternion2RotationMatrix(const double x,const double y,const double z,const double w);
Eigen::Quaterniond rotationMatrix2Quaterniond(Eigen::Matrix3d R);
Eigen::Matrix3d euler2RotationMatrix(const double roll, const double pitch, const double yaw);
Eigen::Vector3d RotationMatrix2euler(Eigen::Matrix3d R);

/**
 * @brief 欧拉角转四元数
 * @param  
 * @retval 
 */
Eigen::Quaterniond euler2Quaternion(const double roll, const double pitch, const double yaw)
{
    Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitX());
 
    Eigen::Quaterniond q = rollAngle * yawAngle * pitchAngle;
    cout << "Euler2Quaternion result is:" <<endl;
    cout << "x = " << q.x() <<endl;
    cout << "y = " << q.y() <<endl;
    cout << "z = " << q.z() <<endl;
    cout << "w = " << q.w() <<endl<<endl;
    return q;
}

/**
 * @brief 四元数转欧拉角
 * @param q = ( x_i, y_j, z_k, w) //前三个虚部，后一个实部
 * @retval Eigen::Vector3d
 */
Eigen::Vector3d Quaterniond2Euler(const double x,const double y,const double z,const double w)
{
    Eigen::Quaterniond q;
    q.x() = x;
    q.y() = y;
    q.z() = z;
    q.w() = w;
    Eigen::Vector3d euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
    cout << "Quaterniond2Euler result is:" <<endl;
    cout << "x = "<< euler[2] << endl ;
    cout << "y = "<< euler[1] << endl ;
    cout << "z = "<< euler[0] << endl << endl;
}


/**
 * @brief 四元数转旋转矩阵
 * @param q = ( x_i, y_j, z_k, w) //前三个虚部，后一个实部
 * @retval Eigen::Matrix3d
 */
Eigen::Matrix3d Quaternion2RotationMatrix(const double x,const double y,const double z,const double w)
{
    Eigen::Quaterniond q;
    q.x() = x;
    q.y() = y;
    q.z() = z;
    q.w() = w;
    Eigen::Matrix3d R = q.normalized().toRotationMatrix();
    cout << "Quaternion2RotationMatrix result is:" <<endl;
    cout << "R = " << endl << R << endl<< endl;
    return R;
}

/**
 * @brief 旋转矩阵转四元数
 * @param 
 * @retval 
 */
Eigen::Quaterniond rotationMatrix2Quaterniond(Eigen::Matrix3d R)
{
    Eigen::Quaterniond q = Eigen::Quaterniond(R);
    q.normalize();
    cout << "RotationMatrix2Quaterniond result is:" <<endl;
    cout << "x = " << q.x() <<endl;
    cout << "y = " << q.y() <<endl;
    cout << "z = " << q.z() <<endl;
    cout << "w = " << q.w() <<endl<<endl;
    return q;
}

/**
 * @brief 欧拉角转旋转矩阵
 * @param 
 * @retval 
 */
Eigen::Matrix3d euler2RotationMatrix(const double roll, const double pitch, const double yaw)
{
    Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitX());
    Eigen::Quaterniond q = rollAngle * yawAngle * pitchAngle;
    Eigen::Matrix3d R = q.matrix();
    cout << "Euler2RotationMatrix result is:" <<endl;
    cout << "R = " << endl << R << endl<<endl;
    return R;
}

/**
 * @brief 旋转矩阵转欧拉角
 * @param 
 * @retval 
 */
Eigen::Vector3d RotationMatrix2euler(Eigen::Matrix3d R)
{
    Eigen::Matrix3d m;
    m = R;
    Eigen::Vector3d euler = m.eulerAngles(0, 1, 2);
    cout << "RotationMatrix2euler result is:" << endl;
    cout << "x = "<< euler[2] << endl ;
    cout << "y = "<< euler[1] << endl ;
    cout << "z = "<< euler[0] << endl << endl;
    
    //---2019.01.17 William Yu add, radian_to_angle.
    double Pi=3.1415926;
    cout << "x = "<< euler[2] * (180/Pi) << endl ;
    cout << "y = "<< euler[1] * (180/Pi) << endl ;
    cout << "z = "<< euler[0] * (180/Pi) << endl << endl;
    
    return euler;
}


/*------------------e.g.-----------------------

///-- main.cpp:
#include "./include/eigenGeometryTransfer.h"
int main(int argc, char **argv)
{
    //this is euler2Quaternion transform function,please input your euler angle//
    euler2Quaternion(0,0,0);

    //this is Quaternion2Euler transform function,please input your Quaternion parameter//
    Quaterniond2Euler(0,0,0,1);

    //this is Quaternion2RotationMatrix transform function,please input your Quaternion parameter//
    Quaternion2RotationMatrix(0,0,0,1);

    //this is rotationMatrix2Euler transform function,please input your RotationMatrix parameter like following//
    Eigen::Vector3d x_axiz,y_axiz,z_axiz;
    x_axiz << 1,0,0;
    y_axiz << 0,1,0;
    z_axiz << 0,0,1;
    Eigen::Matrix3d R;
    R << x_axiz,y_axiz,z_axiz;
    rotationMatrix2Quaterniond(R);

    //this is euler2RotationMatrix transform function,please input your euler angle for the function parameter//
    euler2RotationMatrix(0,0,0);

    //this is RotationMatrix2euler transform function,please input your euler angle for the function parameter//
    RotationMatrix2euler(R);

    cout << "All transform is done!" << endl;
}

///-- CMakeLists.txt:

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

------------------e.g.-----------------------*/
