/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2019.01.08, William Yu
                             ver1.1, 2019.01.13, William Yu, add notes
=================================================================================*/


/*-----------------------------[Note]---------------------------
 * 
--------------------------------------------------------------*/


/// Include Files
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/SVD>
#include <g2o/core/base_vertex.h>
#include <g2o/core/base_unary_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/solvers/eigen/linear_solver_eigen.h>
#include <g2o/types/sba/types_six_dof_expmap.h>
#include <chrono>

using namespace std;
using namespace cv;


/// Global Variables





/// Function Declaration

void find_feature_matches (
    const Mat& img_1, const Mat& img_2,
    std::vector<KeyPoint>& keypoints_1,
    std::vector<KeyPoint>& keypoints_2,
    std::vector< DMatch >& matches 
);

// 像素坐标转相机归一化坐标
Point2d pixel2cam ( const Point2d& p, const Mat& K );

void pose_estimation_3d3d (
    const vector<Point3f>& pts1,
    const vector<Point3f>& pts2,
    Mat& R, Mat& t
);






/// Function definitions

/**
 * @function main
 * @brief 
 */
int main ( int argc, char** argv )
{
    if ( argc != 5 )
    {
        cout<<"usage: pose_estimation_3d3d img1 img2 depth1 depth2"<<endl;
        return 1;
    }
    //-- 内参矩阵 
    //--调参：相机内参--
    //        | fx,  0, cx, | cx主点偏移
    //   K =  |  0, fy, cy, | 
    //        |  0,  0,  1  |
    Mat K = ( Mat_<double> ( 3,3 ) << 518.0,     0, 325.5,
                                          0, 519.0, 253.5, 
                                          0,     0,     1 );
    double depthScale = 1000.0;

    //-- 读取图像
    Mat img_1 = imread ( argv[1], CV_LOAD_IMAGE_COLOR );
    Mat img_2 = imread ( argv[2], CV_LOAD_IMAGE_COLOR );

    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;
    find_feature_matches ( img_1, img_2, keypoints_1, keypoints_2, matches );
    cout<<"一共找到了"<<matches.size() <<"组匹配点"<<endl;

    // 建立3D点
    Mat depth1 = imread ( argv[3], CV_LOAD_IMAGE_UNCHANGED );       // 深度图为16位无符号数，单通道图像
    Mat depth2 = imread ( argv[4], CV_LOAD_IMAGE_UNCHANGED );       // 深度图为16位无符号数，单通道图像
    
    vector<Point3f> pts1, pts2;
    for ( DMatch m:matches )
    {
        ushort d1 = depth1.ptr<unsigned short> ( int ( keypoints_1[m.queryIdx].pt.y ) ) [ int ( keypoints_1[m.queryIdx].pt.x ) ];
        ushort d2 = depth2.ptr<unsigned short> ( int ( keypoints_2[m.trainIdx].pt.y ) ) [ int ( keypoints_2[m.trainIdx].pt.x ) ];
        if ( d1==0 || d2==0 )   // bad depth //深度值为0，无法使用
            continue;
        Point2d p1 = pixel2cam ( keypoints_1[m.queryIdx].pt, K );//是测试图像的特征点描述子（descriptor）的下标，同时也是描述符对应特征点（keypoint)的下标。
        Point2d p2 = pixel2cam ( keypoints_2[m.trainIdx].pt, K );//是样本图像的特征点描述子的下标，同样也是相应的特征点的下标。
        float dd1 = float ( d1 ) /depthScale;
        float dd2 = float ( d2 ) /depthScale;
        pts1.push_back ( Point3f ( p1.x*dd1, p1.y*dd1, dd1 ) );
        pts2.push_back ( Point3f ( p2.x*dd2, p2.y*dd2, dd2 ) );
    }

    cout<<"3d-3d 点对: "<<pts1.size() <<endl;
    Mat R, t;
    pose_estimation_3d3d ( pts1, pts2, R, t );
    cout<<"---------------------------------------------"<<endl;
    cout<<"ICP via SVD results: "<<endl;
    cout<<"R_12 = "<<R<<endl;
    cout<<"t_12 = "<<t<<endl;
    /*-----------------------------[Note]---------------------------
    * pts1 = R*pts2 + t
    * 则有pts2 = R_inv*pts1 - R_inv*t
    * 对应pts2 = R`*pts1 + t`
    * 即逆运动R` = R_inv
    *        t` = - R_inv*t
    * Notice:旋转矩阵R为正交矩阵，
    * 所有又有 R^(-1) = R^T，逆等于转置
    --------------------------------------------------------------*/
    cout<<"逆运动R：R` = R_inv = "<<R.t() <<endl;   //Mat.t()为矩阵求转置
    //cout<<"逆运动R：R` = R_inv = "<<R.inv() <<endl;  //Mat.inv()为矩阵求逆 
    cout<<"逆运动t：t` = - R_inv*t = "<<-R.t() *t <<endl;

    cout<<"---------------------------------------------"<<endl;
    //--校验5组 p1 = R*p2 + t  和  p2 = R_inv*p1 - R_inv*t
    for ( int i=0; i<5; i++ )
    {
        cout<<"p1 = "<<pts1[i]<<endl;
        cout<<"p2 = "<<pts2[i]<<endl;
        cout<<"(R*p2+t) = "<<
            R * (Mat_<double>(3,1)<<pts2[i].x, pts2[i].y, pts2[i].z) + t << endl;
        cout<<"(R`*p1+t`) = "<<
            R.inv() * (Mat_<double>(3,1)<<pts1[i].x, pts1[i].y, pts1[i].z) -R.inv() *t << endl<< endl;
    }
}





/**
 * @function find_feature_matches
 * @brief 特征点匹配与筛选
 * @param  const Mat& img_1, const Mat& img_2,
                            std::vector<KeyPoint>& keypoints_1,
                            std::vector<KeyPoint>& keypoints_2,
                            std::vector< DMatch >& good_matches
 * @retval None
 */

void find_feature_matches ( const Mat& img_1, const Mat& img_2,
                            std::vector<KeyPoint>& keypoints_1,
                            std::vector<KeyPoint>& keypoints_2,
                            std::vector< DMatch >& good_matches )
{
    //-- 初始化
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create();
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create("BruteForce-Hamming");


    //--[1]:检测 Oriented FAST 角点位置
    detector->detect ( img_1,keypoints_1 );
    detector->detect ( img_2,keypoints_2 );

    //--[2]:根据角点位置计算 BRIEF 描述子
    descriptor->compute ( img_1, keypoints_1, descriptors_1 );
    descriptor->compute ( img_2, keypoints_2, descriptors_2 );
    
    //绘制特征点
    Mat outimg1, outimg2;
    drawKeypoints( img_1, keypoints_1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    namedWindow("ORB_img1", WINDOW_NORMAL);
    imshow("ORB_img1",outimg1);
    drawKeypoints( img_2, keypoints_2, outimg2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    namedWindow("ORB_img2", WINDOW_NORMAL);
    imshow("ORB_img2",outimg2);
    cout<<"Img1特征点数"<<keypoints_1.size() <<endl;//500个特征点
    cout<<"Img2特征点数"<<keypoints_2.size() <<endl;
    
    //--[3]:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
    vector<DMatch> matches;
    //BFMatcher matcher ( NORM_HAMMING );
    matcher->match ( descriptors_1, descriptors_2, matches );
    cout<<"初步匹配"<<matches.size() <<"对匹配点"<<endl; //500对匹配

    //--[4]:匹配点对筛选
    //--筛选要求：1.确保都是正确匹配，删去错误匹配
    //          2.确保正确匹配的前提下，保证点对数足够多

    //--调参：筛选阈值--
    double min_dist=10000, max_dist=0;
    //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if ( dist < min_dist ) min_dist = dist;//最短距离，最相似
        if ( dist > max_dist ) max_dist = dist;//最长距离，最不相似
    }
    printf ( "-- Max dist : %f \n", max_dist );
    printf ( "-- Min dist : %f \n", min_dist );

    //--调参：筛选阈值--
    //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.
    //但有时候最小距离会非常小,设置一个经验值30作为下限.
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        if ( matches[i].distance <= max ( 2*min_dist, 30.0 ) )
        {
            good_matches.push_back ( matches[i] );
        }
    }

    //--TODO：调参：逐点检查--


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
}




/**
 * @function pixel2cam
 * @brief 像素坐标转相机归一化坐标
 * @param  const Point2d& p, const Mat& K
 * @retval Point2d
 */
Point2d pixel2cam ( const Point2d& p, const Mat& K )
{
    return Point2d
           (
               ( p.x - K.at<double> ( 0,2 ) ) / K.at<double> ( 0,0 ),
               ( p.y - K.at<double> ( 1,2 ) ) / K.at<double> ( 1,1 )
           );
}





/**
 * @function pose_estimation_3d3d
 * @brief   求解R、t
 * @param   const vector<Point3f>& pts1,
            const vector<Point3f>& pts2,
            Mat& R, Mat& t
 * @retval None
 */
void pose_estimation_3d3d (
    const vector<Point3f>& pts1,
    const vector<Point3f>& pts2,
    Mat& R, Mat& t
)
{
    Point3f p1, p2;     // center of mass
    int N = pts1.size();
    for ( int i=0; i<N; i++ )
    {
        p1 += pts1[i];
        p2 += pts2[i];
    }
    p1 = Point3f( Vec3f(p1) /  N);
    p2 = Point3f( Vec3f(p2) / N);
    vector<Point3f>     q1 ( N ), q2 ( N ); // remove the center
    for ( int i=0; i<N; i++ )
    {
        q1[i] = pts1[i] - p1;
        q2[i] = pts2[i] - p2;
    }

    // compute q1*q2^T
    Eigen::Matrix3d W = Eigen::Matrix3d::Zero();
    for ( int i=0; i<N; i++ )
    {
        W += Eigen::Vector3d ( q1[i].x, q1[i].y, q1[i].z ) * Eigen::Vector3d ( q2[i].x, q2[i].y, q2[i].z ).transpose();
    }
    cout<<"W="<<W<<endl;

    // SVD on W
    Eigen::JacobiSVD<Eigen::Matrix3d> svd ( W, Eigen::ComputeFullU|Eigen::ComputeFullV );
    Eigen::Matrix3d U = svd.matrixU();
    Eigen::Matrix3d V = svd.matrixV();
    
    if (U.determinant() * V.determinant() < 0)
	{
        for (int x = 0; x < 3; ++x)
        {
            U(x, 2) *= -1;
        }
	}
    
    cout<<"U="<<U<<endl;
    cout<<"V="<<V<<endl;

    Eigen::Matrix3d R_ = U* ( V.transpose() );
    Eigen::Vector3d t_ = Eigen::Vector3d ( p1.x, p1.y, p1.z ) - R_ * Eigen::Vector3d ( p2.x, p2.y, p2.z );

    // convert to cv::Mat
    R = ( Mat_<double> ( 3,3 ) <<
          R_ ( 0,0 ), R_ ( 0,1 ), R_ ( 0,2 ),
          R_ ( 1,0 ), R_ ( 1,1 ), R_ ( 1,2 ),
          R_ ( 2,0 ), R_ ( 2,1 ), R_ ( 2,2 )
        );
    t = ( Mat_<double> ( 3,1 ) << t_ ( 0,0 ), t_ ( 1,0 ), t_ ( 2,0 ) );
}