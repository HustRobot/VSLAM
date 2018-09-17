/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲/ch8 学习记录
 *                           直接法与半稠密直接法
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.04.22, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/


# 0.准备数据集
将/tool文件夹下的脚本associate.py复制到/data/数据集文件夹，运行
$ python associate.py rgb.txt depth.txt > associate.txt





1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make




2.Usage:

* 直接法
./direct_sparse <path_to_data> 


* 半稠密直接法
./direct_semidense <path_to_data> 


eg：
./direct_sparse ../../data/rgbd_dataset_freiburg1_desk
./direct_semidense ../../data/rgbd_dataset_freiburg1_desk