/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch7 学习记录
 *                           求解3D到2D点运动
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.20, William Yu
                             ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/


# 1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make




# 2.Usage:
./pose_estimation_3d2d <path_to_image1> <path_to_image2> <path_to_image1_depth> <path_to_image2_depth>
