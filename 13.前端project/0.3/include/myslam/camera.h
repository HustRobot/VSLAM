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
#ifndef CAMERA_H
#define CAMERA_H

#include "myslam/common_include.h"

namespace myslam
{

// Pinhole RGBD camera model
class Camera
{
public:
    typedef std::shared_ptr<Camera> Ptr;
    float   fx_, fy_, cx_, cy_, depth_scale_;

    Camera();
    Camera ( float fx, float fy, float cx, float cy, float depth_scale=0 ) :
        fx_ ( fx ), fy_ ( fy ), cx_ ( cx ), cy_ ( cy ), depth_scale_ ( depth_scale )
    {}

    // coordinate transform: world, camera, pixel
    Vector3d world2camera( const Vector3d& p_w, const SE3& T_c_w );
    Vector3d camera2world( const Vector3d& p_c, const SE3& T_c_w );
    Vector2d camera2pixel( const Vector3d& p_c );
    Vector3d pixel2camera( const Vector2d& p_p, double depth=1 ); 
    Vector3d pixel2world ( const Vector2d& p_p, const SE3& T_c_w, double depth=1 );
    Vector2d world2pixel ( const Vector3d& p_w, const SE3& T_c_w );

};

}
#endif // CAMERA_H
