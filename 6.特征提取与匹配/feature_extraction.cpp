/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch7 学习记录
 *                              特征点匹配
 * Reference               : https://docs.opencv.org/3.3.1/dc/d16/tutorial_akaze_tracking.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.12, William Yu
                            
=================================================================================*/

/// Include Files
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


/*-----------------------------[Note]---------------------------

references： https://docs.opencv.org/3.3.1/dc/d16/tutorial_akaze_tracking.html

--------------------------------------------------------------*/




/// Function Definitions

/**
 * @function main
 * @author William Yu
 * @brief 
 * @param  None
 * @retval None
 */
int main ( int argc, char** argv )
{
    if ( argc != 3 )
    {
        cout<<"usage: feature_extraction img1 img2"<<endl;
        return 1;
    }

    //--读取图像
    Mat img_1 = imread ( argv[1], CV_LOAD_IMAGE_COLOR );
    Mat img_2 = imread ( argv[2], CV_LOAD_IMAGE_COLOR );

    //--初始化
    std::vector<KeyPoint> keypoints_1, keypoints_2; //关键点容器
    Mat descriptors_1, descriptors_2; //描述子
    Ptr<FeatureDetector> detector = ORB::create(); //ORB特征点
    Ptr<DescriptorExtractor> descriptor = ORB::create(); //ORB描述子
    //如果想使用其他特征点和描述子的句柄：
    // Ptr<FeatureDetector> detector = FeatureDetector::create(detector_name);
    // Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create(descriptor_name);
   
    //汉明距离匹配
    Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );


    //--[1]:检测 Oriented FAST 角点位置
    detector->detect ( img_1,keypoints_1 );
    detector->detect ( img_2,keypoints_2 );

    //--[2]:根据角点位置计算 BRIEF 描述子
    descriptor->compute ( img_1, keypoints_1, descriptors_1 );
    descriptor->compute ( img_2, keypoints_2, descriptors_2 );
    
    //绘制特征点
    Mat outimg1;
    drawKeypoints( img_1, keypoints_1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    namedWindow("ORB", WINDOW_NORMAL);
    imshow("ORB",outimg1);

    //--[3]:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
    vector<DMatch> matches;
    //BFMatcher matcher ( NORM_HAMMING );
    matcher->match ( descriptors_1, descriptors_2, matches );

    //--[4]:匹配点对筛选
    double min_dist=10000, max_dist=0;

    //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if ( dist < min_dist ) min_dist = dist;//最短距离，最相似
        if ( dist > max_dist ) max_dist = dist;//最长距离，最不相似
    }
    
    // 仅供娱乐的写法
    min_dist = min_element( matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance<m2.distance;} )->distance;
    max_dist = max_element( matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance<m2.distance;} )->distance;

    printf ( "-- Max dist : %f \n", max_dist );
    printf ( "-- Min dist : %f \n", min_dist );

    //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值30作为下限.
    std::vector< DMatch > good_matches;
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        if ( matches[i].distance <= max ( 2*min_dist, 30.0 ) )
        {
            good_matches.push_back ( matches[i] );
        }
    }

    //--[5]:绘制匹配结果
    Mat img_match;
    Mat img_goodmatch;
    drawMatches ( img_1, keypoints_1, img_2, keypoints_2, matches, img_match );
    drawMatches ( img_1, keypoints_1, img_2, keypoints_2, good_matches, img_goodmatch );
    namedWindow("img_match", WINDOW_NORMAL);
    namedWindow("img_goodmatch", WINDOW_NORMAL);
    imshow ( "img_match", img_match );
    imshow ( "img_goodmatch", img_goodmatch );
    waitKey(0);

    return 0;
}
