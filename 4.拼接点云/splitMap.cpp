/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch5 学习记录
 * Reference               : 分切点云
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2019.01.02, William Yu
                            
=================================================================================*/

/// Include Files
#include <iostream>
#include <fstream>
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Eigen/Geometry> 
#include <boost/format.hpp>  // for formating strings
#include <pcl/point_types.h> 
#include <pcl/io/pcd_io.h> 
#include <pcl/visualization/pcl_visualizer.h>



/// Function Definitions

/**
 * @function main
 * @author William Yu
 * @brief 
 */
int main( int argc, char** argv )
{
    // 相机内参 
    double cx = 325.5;
    double cy = 253.5;
    double fx = 518.0;
    double fy = 519.0;
    double depthScale = 1000.0;
    
    cout<<"读入点云..."<<endl;
    pcl::PCDReader reader;
    // 定义点云使用的格式：这里用的是XYZRGB
    typedef pcl::PointXYZRGB PointT; 
    typedef pcl::PointCloud<PointT> PointCloud;
    PointCloud::Ptr pointCloud( new PointCloud ); 
    reader.read ("../output/map.pcd", *pointCloud);
    int size_pointCloud = pointCloud->points.size()/10;//只是用前面的一些点，使用全部点会报错，原因不明。应该是内存溢出。
    std::cout << "PointCloud before filtering has: " << size_pointCloud << " data points." << std::endl;

    cv::Mat color(480,640,CV_8UC3,cv::Scalar(0,0,0) );
    cv::Mat depth(480,640,CV_8UC1,cv::Scalar(255));
    
    //--每个世界坐标转换到像素坐标
    for ( size_t p_i=0; p_i<540920; ++p_i )
    {   
        PointT p = pointCloud->points[p_i];//由点云中载入点
        
        //内参修正
        Eigen::Vector3d point; 
        point[0] = p.x;
        point[1] = p.y;
        point[2] = p.z;
        
        unsigned int d = depthScale * point[2];
        int u = fx * point[0] / point[2] + cx;
        int v = fy * point[1] / point[2] + cy; 
        int d_rawdata = depth.ptr<unsigned short> (v)[u];
        std::cout << p_i << std::endl;
        // std::cout << " u:" << u
        // << " v:" << v
        // << " d:" << d
        // << std::endl;
        // std::cout << " b:" << (int)p.b
        // << " g:" << (int)p.g
        // << " r:" << (int)p.r
        // << std::endl;

        //如果新点z轴更高，跳过
        if ( d_rawdata < d ) continue; //取下侧，仰视图
        //如果新点更低，更新RGBD
        depth.ptr<unsigned short> (v)[u] = d; // 深度值，其实深度值根本获取不出来
        color.data[ v*color.step+u*color.channels() ] = p.b;
        color.data[ v*color.step+u*color.channels()+1 ] = p.g;
        color.data[ v*color.step+u*color.channels()+2 ] = p.r;

        //释放资源
        //delete []PointCloud;
    }
    
    cv::imshow("splitimg",color);
    
    char key = (char)cv::waitKey(0); 
    if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
        return 0;
}
