/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch8 学习记录
 *                           光流法Lucas-Kanade optical flow algorithm
 * Reference               : https://docs.opencv.org/3.3.1/dd/d1a/group__imgproc__feature.html#ga1d6bb77486c8f92d79c8793ad995d541
 *                           https://docs.opencv.org/3.3.1/df/d74/classcv_1_1FastFeatureDetector.html
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.22, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/



# 0.准备数据集
将/tool文件夹下的脚本associate.py复制到/data/数据集文件夹，运行
$ python associate.py rgb.txt depth.txt > associate.txt




# 1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make




2.Usage:
./useLK <path_to_data>

eg：
./useLK ../../data/rgbd_dataset_freiburg1_desk
