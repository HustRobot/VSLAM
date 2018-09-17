/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch5 学习记录
 * Reference               : 拼接点云
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.01, William Yu
                            
=================================================================================*/

/*-----------------------------[Note]---------------------------
输入参数：像素坐标，以及像素深度图 + 相机位姿
成像过程：
    现实世界物体坐标
        |———(外参数 变换矩阵Ｔ变换)——>  
    相机坐标系　
        |———(同/Z)—>归一化平面坐标系——>
    径向和切向畸变纠正
        |———(内参  平移 Cx Cy 缩放焦距Fx Fy)——>
    图像坐标系下　像素坐标
    u=Fx *X/Z + Cx 　　像素列位置坐标　
    v=Fy *Y/Z + Cy 　　像素列位置坐标　

    上述过程逆向推导：
    X=(u- Cx)*Z/Fx
    Y=(u- Cy)*Z/Fy
    Z轴归一化
    X=(u- Cx)*Z/Fx/depthScale
    Y=(u- Cy)*Z/Fy/depthScale
    Z=Z/depthScale
    外参T，得世界坐标　
    pointWorld = T*[X Y Z]
输出：世界坐标系下各像素世界坐标
--------------------------------------------------------------*/


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
 * @param  None
 * @retval None
 */
int main( int argc, char** argv )
{
    vector<cv::Mat> colorImgs, depthImgs;    // 彩色图和深度图
    vector<Eigen::Isometry3d, Eigen::aligned_allocator<Eigen::Isometry3d>> poses;         // 相机位姿
    
    
    ifstream fin("../data/pose.txt");
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
        
        double data[7] = {0};      //pose.txt的内容：x,y,z,Qx,Qy,Qz,Qw  xyz为位置，Q为四元数姿态，Qw为四元数实部
        for ( auto& d:data )    //读取txt文件的每一行
            fin>>d;
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
         cout<<"转换图像中: "<<i+1<<endl; 
         cv::Mat color = colorImgs[i]; 
         cv::Mat depth = depthImgs[i];
		 cv::imshow("color",color);
		 cv::waitKey(0);//检查图片是否正常载入
         Eigen::Isometry3d T = poses[i]; //每张图像对应的相机位姿

         //--每个像素坐标转换到世界坐标
         for ( int v=0; v<color.rows; v++ )
	        for ( int u=0; u<color.cols; u++ )
            {
                //内参修正
                unsigned int d = depth.ptr<unsigned short> (v)[u]; // 深度值
                if ( d==0 ) continue; // 为0表示没有测量到
                Eigen::Vector3d point; 
                point[2] = double(d)/depthScale; 
                point[0] = (u-cx)*point[2]/fx;
                point[1] = (v-cy)*point[2]/fy; 
                
                //外参转换
                Eigen::Vector3d pointWorld = T*point;
                
                //输出
                PointT p ;
                p.x = pointWorld[0];
                p.y = pointWorld[1];
                p.z = pointWorld[2];
                p.b = color.data[ v*color.step+u*color.channels() ];
                p.g = color.data[ v*color.step+u*color.channels()+1 ];
                p.r = color.data[ v*color.step+u*color.channels()+2 ];
                pointCloud->points.push_back( p );
            }
    }
    
    pointCloud->is_dense = false;
    cout<<"点云共有"<<pointCloud->size()<<"个点."<<endl;
    if(pointCloud->size() > 0)
        pcl::io::savePCDFileBinary("map.pcd", *pointCloud );

    return 0;
}
