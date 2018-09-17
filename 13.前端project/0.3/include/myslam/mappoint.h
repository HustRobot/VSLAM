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

#ifndef MAPPOINT_H
#define MAPPOINT_H

namespace myslam
{
    
class Frame;
class MapPoint
{
public:
    typedef shared_ptr<MapPoint> Ptr;
    unsigned long      id_;        // ID
    bool        good_;      // wheter a good point 
    Vector3d    pos_;       // Position in world
    Vector3d    norm_;      // Normal of viewing direction 
    Mat         descriptor_; // Descriptor for matching 
    
    list<Frame*>    observed_frames_;   // frames that can observe this point 
    
    int         observed_times_;    // being observed by feature matching algo.
    int         matched_times_;     // being an inliner in pose estimation
    
    MapPoint();
    MapPoint( long id, Vector3d position, Vector3d norm );
    
    static MapPoint::Ptr createMapPoint();
};
}

#endif // MAPPOINT_H
