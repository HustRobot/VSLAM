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

#include "myslam/common_include.h"
#include "myslam/mappoint.h"

namespace myslam
{

MapPoint::MapPoint()
: id_(-1), pos_(Vector3d(0,0,0)), norm_(Vector3d(0,0,0)), good_(true), visible_times_(0), matched_times_(0)
{

}

MapPoint::MapPoint ( long unsigned int id, const Vector3d& position, const Vector3d& norm, Frame* frame, const Mat& descriptor )
: id_(id), pos_(position), norm_(norm), good_(true), visible_times_(1), matched_times_(1), descriptor_(descriptor)
{
    observed_frames_.push_back(frame);
}

MapPoint::Ptr MapPoint::createMapPoint()
{
    return MapPoint::Ptr( 
        new MapPoint( factory_id_++, Vector3d(0,0,0), Vector3d(0,0,0) )
    );
}

MapPoint::Ptr MapPoint::createMapPoint ( 
    const Vector3d& pos_world, 
    const Vector3d& norm, 
    const Mat& descriptor, 
    Frame* frame )
{
    return MapPoint::Ptr( 
        new MapPoint( factory_id_++, pos_world, norm, frame, descriptor )
    );
}

unsigned long MapPoint::factory_id_ = 0;

}
