/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch12 学习记录
 *                           回环检测-创建大规模字典
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.05.06, William Yu
                             ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/

//--创建大规模字典

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
    string dataset_dir = argv[1];
    ifstream fin ( dataset_dir+"/associate.txt" );
    if ( !fin )
    {
        cout<<"please generate the associate file called associate.txt!"<<endl;
        return 1;
    }

    //--读入数据集
    vector<string> rgb_files, depth_files;
    vector<double> rgb_times, depth_times;
    while ( !fin.eof() )
    {
        string rgb_time, rgb_file, depth_time, depth_file;
        fin>>rgb_time>>rgb_file>>depth_time>>depth_file;
        rgb_times.push_back ( atof ( rgb_time.c_str() ) );
        depth_times.push_back ( atof ( depth_time.c_str() ) );
        rgb_files.push_back ( dataset_dir+"/"+rgb_file );
        depth_files.push_back ( dataset_dir+"/"+depth_file );

        if ( fin.good() == false )
            break;
    }
    fin.close();
    




    
    cout<<"generating features ... "<<endl;
    vector<Mat> descriptors;
    Ptr< Feature2D > detector = ORB::create();
    int index = 1;
    for ( string rgb_file:rgb_files )
    {
        Mat image = imread(rgb_file);
        vector<KeyPoint> keypoints; 
        Mat descriptor;
        detector->detectAndCompute( image, Mat(), keypoints, descriptor );
        descriptors.push_back( descriptor );
        cout<<"extracting features from image " << index++ <<endl;
    }
    cout<<"extract total "<<descriptors.size()*500<<" features."<<endl;
    





    //--create vocabulary 
    //--创建字典
    cout<<"creating vocabulary, please wait ... "<<endl;
    DBoW3::Vocabulary vocab;
    vocab.create( descriptors );
    cout<<"vocabulary info: "<<vocab<<endl;
    vocab.save( "vocab_larger.yml.gz" );
    cout<<"done"<<endl;
    
    return 0;
}