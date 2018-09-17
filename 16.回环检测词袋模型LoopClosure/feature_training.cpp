/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch12 学习记录
 *                           回环检测-创建小规模字典
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.05.06, William Yu
                             ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/


//--训练小字典，回环检测这几部分都比较简单，DBoW3操作学习很快
//但是现在的问题是，还不知道检测出会还之后如何通知后端进行地图修正，还是去跑开源算法吧---vslam真的太复杂了

/// Include Files
#include "DBoW3/DBoW3.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace cv;
using namespace std;




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
    // 读取图片
    cout<<"reading images... "<<endl;
    vector<Mat> images; 
    for ( int i=0; i<10; i++ )
    {
        string path = "../data/"+to_string(i+1)+".png";//注意数据集路径
        images.push_back( imread(path) );
    }

    // detect ORB features
    cout<<"detecting ORB features... "<<endl;
    Ptr< Feature2D > detector = ORB::create();
    vector<Mat> descriptors;
    for ( Mat& image:images )
    {
        vector<KeyPoint> keypoints; 
        Mat descriptor;
        detector->detectAndCompute( image, Mat(), keypoints, descriptor );
        descriptors.push_back( descriptor );
    }
    
    // create vocabulary 
    //--DBoW3的简单操作
    cout<<"creating vocabulary ... "<<endl;
    DBoW3::Vocabulary vocab;
    vocab.create( descriptors );
    cout<<"vocabulary info: "<<vocab<<endl;
    vocab.save( "vocabulary.yml.gz" );
    cout<<"done"<<endl;
    
    return 0;
}