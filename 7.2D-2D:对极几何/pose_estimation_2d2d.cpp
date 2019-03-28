/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch7 学习记录
 *                           使用2D-2D的特征匹配估计相机运动
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.20, William Yu
                            
=================================================================================*/


/*-----------------------------[Note]---------------------------
*对极几何求解两组单目相机2D图像
 * 2D点对求两相机的 旋转和平移矩阵
 * 空间点 P  两相机 像素点对  p1  p2 两相机 归一化平面上的点对 x1 x2 与P点对应
 * 相机内参数 K  两镜头旋转平移矩阵  R t 或者 变换矩阵 T
 * 公式：
 *  p1 = KP  (世界坐标系)     
 *  p2 = K( RP + t)  = KTP
 *  x1 =  K逆* p1  
 *  x2 =  K逆* p2  
 * 相机坐标系下 归一化平面上的点     
 *  x1= (px -cx)/fx    
 *  x2= (py -cy)/fy
 *  x1 = P     
 *  x2 =  R  x1  + t
 *  t 外积  x2  = t 外积 R  x1 ；
 *  t 外积 t   =0    
 *  sin(cet) =0 垂线段投影
 *  x2转置 *  t 外积  x2 = x2转置 * t 外积 R  x1   = 0
 * 有 
 *  x2转置 * t 外积 R  x1   = x2转置 * E * x1 =  0 ； E 为本质矩阵
 *  p2转置 * K 转置逆 * t 外积 R * K逆 * p1  = p2转置 * F * p1 =  0 ； F 为基础矩阵
 *  x2转置 * E * x1 =  0   
 *  x1 x2  为 由 像素坐标转化的归一化坐标
 *  一个点对一个约束，8点法  可以算出 E的各个元素 ，
 *  再 奇异值分解 E 得到 R t
 * 
 * 
 * 单应矩阵描述了两个平面间的映射关系
 *  p2 = K( RP + t)       
 * 有 P在极平面上  平面方程  n转置 * P + d = 0 
 * 得 - n转置 * P/d  =1 
 *   p2 = K( RP - t *n转置 * P/d)   = K( R - t *n转置 * /d)*P = H *p1
 *   p2 = H *p1
 * 一个点对 2个约束，4点法求解  单应矩阵 H 再对 H进行分解
--------------------------------------------------------------*/


/// Include Files
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;




/// Global Variables





/// Function Header


// 特征点匹配
void find_feature_matches (
    const Mat& img_1, const Mat& img_2,
    std::vector<KeyPoint>& keypoints_1,
    std::vector<KeyPoint>& keypoints_2,
    std::vector< DMatch >& matches );
// 旋转平移矩阵计算
void pose_estimation_2d2d (
    std::vector<KeyPoint> keypoints_1,
    std::vector<KeyPoint> keypoints_2,
    std::vector< DMatch > matches,
    Mat& R, Mat& t );

// 像素坐标转相机归一化坐标
Point2d pixel2cam ( const Point2d& p, const Mat& K );






/// Function Definitions

/**
 * @function main
 * @brief 
 * @param  None
 * @retval None
 */
int main ( int argc, char** argv )
{
    if ( argc != 3 )
    {
        cout<<"usage: pose_estimation_2d2d img1 img2"<<endl;
        return 1;
    }
    //--读取图像
    Mat img_1 = imread ( argv[1], CV_LOAD_IMAGE_COLOR );
    Mat img_2 = imread ( argv[2], CV_LOAD_IMAGE_COLOR );

    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;
    find_feature_matches ( img_1, img_2, keypoints_1, keypoints_2, matches );
    cout<<"一共找到了"<<matches.size() <<"组匹配点"<<endl;


    //--[5]:绘制匹配结果
    Mat img_match;
    drawMatches ( img_1, keypoints_1, img_2, keypoints_2, matches, img_match );
    namedWindow("img_match", WINDOW_NORMAL);
    imshow ( "img_match", img_match );
    waitKey(0);

    //-- 估计两张图像间运动
    Mat R,t; //旋转平移矩阵
    pose_estimation_2d2d ( keypoints_1, keypoints_2, matches, R, t );

    //-- 验证E=t^R*scale
    Mat t_x = ( Mat_<double> ( 3,3 ) <<
                0,                      -t.at<double> ( 2,0 ),     t.at<double> ( 1,0 ),
                t.at<double> ( 2,0 ),      0,                      -t.at<double> ( 0,0 ),
                -t.at<double> ( 1,0 ),     t.at<double> ( 0,0 ),      0 );

    cout<<"E = t^R = "<<endl<<t_x*R<<endl;

    //-- 验证对极约束
    Mat K = ( Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1 );
    for ( DMatch m: matches )
    {
        //像素坐标转相机归一化坐标
        Point2d pt1 = pixel2cam ( keypoints_1[ m.queryIdx ].pt, K );
        Mat y1 = ( Mat_<double> ( 3,1 ) << pt1.x, pt1.y, 1 );
        Point2d pt2 = pixel2cam ( keypoints_2[ m.trainIdx ].pt, K );
        Mat y2 = ( Mat_<double> ( 3,1 ) << pt2.x, pt2.y, 1 );
        Mat d = y2.t() * t_x * R * y1;
        cout << "epipolar constraint = " << d << endl;
    }
    return 0;
}











/**
 * @function find_feature_matches
 * @author 特征点匹配
 * @brief 
 * @param  const Mat& img_1, const Mat& img_2,
                            std::vector<KeyPoint>& keypoints_1,
                            std::vector<KeyPoint>& keypoints_2,
                            std::vector< DMatch >& matches
 * @retval None
 */
void find_feature_matches ( const Mat& img_1, const Mat& img_2,
                            std::vector<KeyPoint>& keypoints_1,
                            std::vector<KeyPoint>& keypoints_2,
                            std::vector< DMatch >& matches )
{
    //-- 初始化
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );
    //-- [1]:检测 Oriented FAST 角点位置
    detector->detect ( img_1,keypoints_1 );
    detector->detect ( img_2,keypoints_2 );

    //-- [2]:根据角点位置计算 BRIEF 描述子
    descriptor->compute ( img_1, keypoints_1, descriptors_1 );
    descriptor->compute ( img_2, keypoints_2, descriptors_2 );

    //-- [3]:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
    vector<DMatch> match;
    //BFMatcher matcher ( NORM_HAMMING );
    matcher->match ( descriptors_1, descriptors_2, match );

    //-- [4]:匹配点对筛选
    double min_dist=10000, max_dist=0;

    //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = match[i].distance;
        if ( dist < min_dist ) min_dist = dist;
        if ( dist > max_dist ) max_dist = dist;
    }

    printf ( "-- Max dist : %f \n", max_dist );
    printf ( "-- Min dist : %f \n", min_dist );

    //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值30作为下限.
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        if ( match[i].distance <= max ( 2*min_dist, 30.0 ) )
        {
            matches.push_back ( match[i] );
        }
    }
}











/**
 * @function pixel2cam
 * @brief 像素坐标转相机归一化坐标
 * @param  const Point2d& p, const Mat& K
 * @retval Point2d
 */
// x1 =  K^(-1) * p1;  x2 =  K^(-1)* p2
Point2d pixel2cam ( const Point2d& p, const Mat& K )
{
    return Point2d
           (
               ( p.x - K.at<double> ( 0,2 ) ) / K.at<double> ( 0,0 ),
               ( p.y - K.at<double> ( 1,2 ) ) / K.at<double> ( 1,1 )
           );
}










/**
 * @function pose_estimation_2d2d
 * @author 
 * @brief 对极几何
 * @param  std::vector<KeyPoint> keypoints_1,std::vector<KeyPoint> keypoints_2,std::vector< DMatch > matches,Mat& R, Mat& t
 * @retval None
 */
void pose_estimation_2d2d ( std::vector<KeyPoint> keypoints_1,
                            std::vector<KeyPoint> keypoints_2,
                            std::vector< DMatch > matches,
                            Mat& R, Mat& t )
{
    // 相机内参,使用的是TUM Freiburg2相机
    //   [fx 0 cx
    //     0 fy cy
    //     0 0  1]
    Mat K = ( Mat_<double> ( 3,3 ) << 520.9, 0, 325.1, 
                                    0, 521.0, 249.7,
                                    0, 0, 1 );

    //-- 把匹配点转换为vector<Point2f>的形式
    vector<Point2f> points1;
    vector<Point2f> points2;
    for ( int i = 0; i < ( int ) matches.size(); i++ )
    {
        points1.push_back ( keypoints_1[matches[i].queryIdx].pt );
        points2.push_back ( keypoints_2[matches[i].trainIdx].pt );
    }

    //-- 计算基础矩阵F
    //公式：p2转置 * F * p1 = 0
    Mat fundamental_matrix;
    fundamental_matrix = findFundamentalMat ( points1, points2, CV_FM_8POINT );
    cout<<"fundamental_matrix is "<<endl<< fundamental_matrix<<endl;

    //-- 计算本质矩阵E
    //公式： x2转置 * E * x1 = 0 
    Point2d principal_point ( 325.1, 249.7 );	//相机光心, TUM dataset标定值
    double focal_length = 521;			//相机焦距, TUM dataset标定值
    Mat essential_matrix;
    essential_matrix = findEssentialMat ( points1, points2, focal_length, principal_point );
    cout<<"essential_matrix is "<<endl<< essential_matrix<<endl;

    //-- 计算单应矩阵H
    //公式：p2 = H *p1
    Mat homography_matrix;
    homography_matrix = findHomography ( points1, points2, RANSAC, 3 );
    cout<<"homography_matrix is "<<endl<<homography_matrix<<endl;

    //-- 从本质矩阵中恢复旋转和平移信息
    //奇异值分解
    recoverPose ( essential_matrix, points1, points2, R, t, focal_length, principal_point );
    cout<<"R is "<<endl<<R<<endl;
    cout<<"t is "<<endl<<t<<endl;
    
}
