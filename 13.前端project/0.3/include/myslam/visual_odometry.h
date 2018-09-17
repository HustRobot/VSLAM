/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.26, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/

#ifndef VISUALODOMETRY_H
#define VISUALODOMETRY_H

#include "myslam/common_include.h"
#include "myslam/map.h"

#include <opencv2/features2d/features2d.hpp>

namespace myslam 
{
class VisualOdometry
{
public:
    typedef shared_ptr<VisualOdometry> Ptr;
    enum VOState {
        INITIALIZING=-1,
        OK=0,
        LOST
    };
    
    VOState     state_;     // current VO status
    Map::Ptr    map_;       // map with all frames and map points
    
    Frame::Ptr  ref_;       // reference key-frame 
    Frame::Ptr  curr_;      // current frame 
    
    cv::Ptr<cv::ORB> orb_;  // orb detector and computer 
    vector<cv::Point3f>     pts_3d_ref_;        // 3d points in reference frame 
    vector<cv::KeyPoint>    keypoints_curr_;    // keypoints in current frame
    Mat                     descriptors_curr_;  // descriptor in current frame 
    Mat                     descriptors_ref_;   // descriptor in reference frame 
    vector<cv::DMatch>      feature_matches_;   // feature matches 
    cv::FlannBasedMatcher   matcher_flann_;     // flann matcher
   
    SE3 T_c_r_estimated_;    // the estimated pose of current frame 
    int num_inliers_;        // number of inlier features in icp
    int num_lost_;           // number of lost times
    
    // parameters 
    int num_of_features_;   // number of features
    double scale_factor_;   // scale in image pyramid
    int level_pyramid_;     // number of pyramid levels
    float match_ratio_;     // ratio for selecting  good matches
    int max_num_lost_;      // max number of continuous lost times
    int min_inliers_;       // minimum inliers
    double key_frame_min_rot;   // minimal rotation of two key-frames
    double key_frame_min_trans; // minimal translation of two key-frames
    double  map_point_erase_ratio_; // remove map point ratio
    
public: // functions 
    VisualOdometry();
    ~VisualOdometry();
    
    bool addFrame( Frame::Ptr frame );      // add a new frame 
    
protected:  
    // inner operation 
    void extractKeyPoints();
    void computeDescriptors(); 
    void featureMatching();
    void setRef3DPoints();
    void poseEstimationPnP(); 
    
    void addKeyFrame();
    bool checkEstimatedPose(); 
    bool checkKeyFrame();
    
};
}

#endif // VISUALODOMETRY_H
