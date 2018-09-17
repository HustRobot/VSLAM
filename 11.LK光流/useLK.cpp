/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch8 学习记录
 *                           光流法Lucas-Kanade optical flow algorithm
 * Reference               : https://docs.opencv.org/3.3.1/dd/d1a/group__imgproc__feature.html#ga1d6bb77486c8f92d79c8793ad995d541
 *                           https://docs.opencv.org/3.3.1/df/d74/classcv_1_1FastFeatureDetector.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.22, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/

/*-----------------------------[Note]---------------------------
 * 灰度不变假设
 *  I(x+dx, y+dy, t+dt) = I(x, y, t) 同一个空间点的像素灰度值 在各个时间点的图像上是固定不变的假设
 * 泰勒展开  I(x+dx, y+dy, t+dt) = I(x, y, t)   +  I对x的偏导数 * dx + I对y的偏导数 * dy + I对t的偏导数 * dt =  I(x, y, t) 
 *  得 I对x的偏导数 * dx + I对y的偏导数 * dy + I对t的偏导数 * dt = 0
 * 两边同时除以 dt   
 *  得 I对x的偏导数 * dx/dt + I对y的偏导数 * dy/dt  = - I对t的偏导数
 * 其中 dx/dt  = 像素运动的速度 x轴分量 u； 
 *     dy/dt =  像素运动的速度 y轴分量 v；
 *     I对x的偏导数   为像素在该点 x方向 梯度 Ix ；
 *     I对y的偏导数   为像素在该点 y方向 梯度  Iy；
 *     I对t的偏导数   为两个时间点图像 对应点 灰度变化 It
--------------------------------------------------------------*/


/// Include Files
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <chrono>
using namespace std; 

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>





/// Global Variables




/// Function Declaration




/// Function Definitions

/**
 * @function main
 * @brief 
 * @param  None
 * @retval None
 */

int main( int argc, char** argv )
{
    //--载入数据集
    if ( argc != 2 )
    {
        cout<<"usage: useLK path_to_dataset"<<endl;
        return 1;
    }
    string path_to_dataset = argv[1];
    string associate_file = path_to_dataset + "/associate.txt";
    
    ifstream fin( associate_file );
    if ( !fin )
    {
        cerr<<"I cann't find associate.txt!"<<endl;  //学会一条新函数
        return 1;
    }
    
    string rgb_file, depth_file, time_rgb, time_depth;
    list< cv::Point2f > keypoints;      // 因为要删除跟踪失败的点，使用list
    cv::Mat color, depth, last_color;
    


    
    for ( int index=0; index<100; index++ )
    {
        fin>>time_rgb>>rgb_file>>time_depth>>depth_file;
        color = cv::imread( path_to_dataset+"/"+rgb_file );
        depth = cv::imread( path_to_dataset+"/"+depth_file, -1 );
        if (index ==0 )
        {
            // 对第一帧提取FAST特征点
            vector<cv::KeyPoint> kps;
            cv::Ptr<cv::FastFeatureDetector> detector = cv::FastFeatureDetector::create();
            detector->detect( color, kps );
            for ( auto kp:kps )
                keypoints.push_back( kp.pt );
            last_color = color;
            continue;
        }

        if ( color.data==nullptr || depth.data==nullptr )//读取错误，跳出
            continue;
        
        //对其他帧用LK跟踪特征点
        vector<cv::Point2f> next_keypoints; //下一帧
        vector<cv::Point2f> prev_keypoints; //上一帧
        for ( auto kp:keypoints )
            prev_keypoints.push_back(kp);
        vector<unsigned char> status;
        vector<float> error; 
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        cv::calcOpticalFlowPyrLK( last_color, color, prev_keypoints, next_keypoints, status, error );
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
        cout<<"LK Flow use time："<<time_used.count()<<" seconds."<<endl;
        //把跟丢的点删掉
        int i=0; 
        for ( auto iter=keypoints.begin(); iter!=keypoints.end(); i++)
        {
            if ( status[i] == 0 )
            {
                iter = keypoints.erase(iter);
                continue;
            }
            *iter = next_keypoints[i];
            iter++;
        }
        cout<<"tracked keypoints: "<<keypoints.size()<<endl;
        if (keypoints.size() == 0)
        {
            cout<<"all keypoints are lost.所有特征点都已丢失"<<endl;
            break; 
        }
        // 画出 keypoints
        cv::Mat img_show = color.clone();
        for ( auto kp:keypoints )
            cv::circle(img_show, kp, 10, cv::Scalar(0, 240, 0), 1);
        cv::imshow("corners", img_show);
        cv::waitKey(0);
        last_color = color;
    }


    return 0;
}