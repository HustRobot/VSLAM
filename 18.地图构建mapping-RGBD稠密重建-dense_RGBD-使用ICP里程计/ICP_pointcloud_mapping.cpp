/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 
 * Reference               : http://pointclouds.org/documentation/tutorials/voxel_grid.php
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2019.01.13, William Yu
 *                           
=================================================================================*/

/*-----------------------------[Note]---------------------------
 * 
--------------------------------------------------------------*/

/// Include files
#include <iostream>
#include <fstream>
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Geometry> 
#include <boost/format.hpp> 
#include <pcl/point_types.h> 
#include <pcl/io/pcd_io.h> 
#include <pcl/filters/voxel_grid.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/statistical_outlier_removal.h>


/// Global Variables
                        //r,g,b
vector<int> colors = {      0,0,0,//黑色
                            0, 0, 255,// 蓝色
                            0, 255, 0,// 绿色
                            255, 0, 0,// 红色
                            255, 255, 0//黄色
                    };

/// Function definitions

/**
 * @function main
 * @author William Yu
 * @brief 
 */
int main( int argc, char** argv )
{
    vector<cv::Mat> colorImgs, depthImgs;    // 彩色图和深度图
    //vector<Eigen::Isometry3d> poses;         // 相机位姿 由于库版本更新，所以修正为下行代码，高博的十四讲书中也需要修正
    vector<Eigen::Isometry3d, Eigen::aligned_allocator<Eigen::Isometry3d>> poses;         // 相机位姿
    
    ifstream fin("../data/pose_icp.txt");
    if (!fin)
    {
        cerr<<"cannot find pose file"<<endl;
        return 1;
    }
    
    for ( int i=0; i<5; i++ )
    {
        boost::format fmt( "../%s/%s/%d.%s" ); //图像文件格式
        colorImgs.push_back( cv::imread( (fmt%"data"%"color"%(i+1)%"png").str() ));
        depthImgs.push_back( cv::imread( (fmt%"data"%"depth"%(i+1)%"pgm").str(), -1 )); // 使用-1读取原始图像
        
        double data[7] = {0};
        for ( int i=0; i<7; i++ )
        {
            fin>>data[i];
        }
        Eigen::Quaterniond q( data[6], data[3], data[4], data[5] );
        Eigen::Isometry3d T(q);
        T.pretranslate( Eigen::Vector3d( data[0], data[1], data[2] ));
        poses.push_back( T );
    }
    


    // 计算点云并拼接
    // 相机内参 
    double cx = 325.5;
    double cy = 253.5;
    double fx = 518.0;
    double fy = 519.0;
    double depthScale = 1000.0;
    
    cout<<"正在将图像转换为点云..."<<endl;
    
    // 定义点云使用的格式：这里用的是XYZRGB
    typedef pcl::PointXYZRGB PointT; 
    typedef pcl::PointCloud<PointT> PointCloud;
    
    // 新建一个点云
    PointCloud::Ptr pointCloud( new PointCloud ); 
    for ( int i=0; i<5; i++ )
    {
        //用于滤波缓存
        PointCloud::Ptr current( new PointCloud );
        
        cout<<"转换图像中: "<<i+1<<endl; 
        cv::Mat color = colorImgs[i]; 
        cv::Mat depth = depthImgs[i];
        cv::imshow("color",color);
		cv::waitKey(0);//检查图片是否正常载入
        Eigen::Isometry3d T = poses[0];
        for ( int j=0; j<i+1; j++ )
        {
            T = T * poses[j];
        }
        std::cout<<"T"<<i+1<<"="<<endl << T.matrix() <<std::endl;
        for ( int v=0; v<color.rows; v++ )
            for ( int u=0; u<color.cols; u++ )
            {
                unsigned int d = depth.ptr<unsigned short> (v)[u]; // 深度值
                if ( d==0 ) continue; // 为0表示没有测量到
                if ( d >= 7000 ) continue; // 深度太大时不稳定，去掉，7m
                Eigen::Vector3d point; 
                point[2] = double(d)/depthScale; 
                point[0] = (u-cx)*point[2]/fx;
                point[1] = (v-cy)*point[2]/fy; 

                //外参转换
                Eigen::Vector3d pointWorld = T*point;
                
                PointT p ;
                p.x = pointWorld[0];
                p.y = pointWorld[1];
                p.z = pointWorld[2];
                // p.b = color.data[ v*color.step+u*color.channels() ];
                // p.g = color.data[ v*color.step+u*color.channels()+1 ];
                // p.r = color.data[ v*color.step+u*color.channels()+2 ];
                //用颜色区分5帧点云
                p.r = colors[3*i];
                p.g = colors[3*i+1];
                p.b = colors[3*i+2];
                

                current->points.push_back( p );
            }




        //--外点去除滤波
        //depth filter and statistical removal 
        PointCloud::Ptr tmp ( new PointCloud );
        pcl::StatisticalOutlierRemoval<PointT> statistical_filter;
        statistical_filter.setMeanK(50);
        statistical_filter.setStddevMulThresh(1.0);
        statistical_filter.setInputCloud(current);
        statistical_filter.filter( *tmp );
        (*pointCloud) += *tmp;
    }
    
    pointCloud->is_dense = false;
    cout<<"点云共有"<<pointCloud->size()<<"个点."<<endl;
    




    //--降采样滤波器
    // voxel filter 
    pcl::VoxelGrid<PointT> voxel_filter; 
    voxel_filter.setLeafSize( 0.01, 0.01, 0.01 );       // resolution 
    PointCloud::Ptr tmp ( new PointCloud );
    voxel_filter.setInputCloud( pointCloud );
    voxel_filter.filter( *tmp );
    tmp->swap( *pointCloud );
    




    cout<<"滤波之后，点云共有"<<pointCloud->size()<<"个点."<<endl;
    if(pointCloud->size() > 0)
        pcl::io::savePCDFileBinary("map.pcd", *pointCloud );

    return 0;
}