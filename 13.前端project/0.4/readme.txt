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



0.准备Dataset

将/tool文件夹脚本associate.py复制到/data文件夹，运行
python associate.py rgb.txt depth.txt > associate.txt

修改/config/default.yaml中的数据集路径，似乎需要使用绝对路径？




1.Compile this program:

* cmake编译
mkdir build
cd build
cmake ..
make




2.Usage:

bin/run_vo <path_to_yaml> 
eg:
bin/run_vo config/default.yaml 


3.备注：
# 完整的linux程序工程结构
1. /bin             存放可执行的二进制文件
2. /include/myslam  存放slam工程模块的头文件，只要是.h 引用头文件时时
                    需写 include "myslam/xxx.h"不容易和其他库混淆
3. /src             存放源代码文件.cpp文件
4. /test            存放测试用的文件.cpp文件，其实完全也可以放到/src里面去
5. /lib             存放编译好的库文件
6. /config          存放配置文件，比如相机内参，数据集地址之类信息
7. /cmake_modules   存放第三方库的cmake文件 例如使用g2o eigen库时



0.1版本 类
Frame     帧                Frame::Ptr  frame 
Camera    相机模型          Camera::Ptr camera_
MapPoint  特征点/路标点     MapPoint::Ptr map_point 
Map       管理特征点   保存所有的特征点/路标 和关键帧
Config    提供配置参数