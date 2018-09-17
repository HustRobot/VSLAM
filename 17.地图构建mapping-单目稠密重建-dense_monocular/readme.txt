/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 *                           地图构建-单目稠密重建
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.05.08, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/



# To build the code : 
mkdir build
cd ./build
cmake ..
make





# How to run the code :
cd ./build
./dense_mapping <path_to_data>
eg:
./dense_mapping ../../data/remode_test_data  真的！非常慢！我的电脑已然苍老！