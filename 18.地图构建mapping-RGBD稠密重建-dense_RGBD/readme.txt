/*=================================================================================
 *	                    Copyleft! 2018 William Yu
 *          Some rights reserved：CC(creativecommons.org)BY-NC-SA
 *                      Copyleft! 2018 William Yu
 *      版权部分所有，遵循CC(creativecommons.org)BY-NC-SA协议授权方式使用
 *
 * Filename                : 
 * Description             : 视觉SLAM十四讲 学习记录
 *                           地图构建-(RGBD稠密重建+八叉树地图构建)
 * Reference               : 
 * Programmer(s)           : William Yu, windmillyucong@163.com
 * Company                 : HUST, DMET国家重点实验室FOCUS团队
 * Modification History	   : ver1.0, 2018.05.10, William Yu
 *                           ver1.1, 2018.05.12, William Yu, add notes
=================================================================================*/


# 0.Requirements
 - Octomap

## install Octomap
可以下载Github源码，https://github.com/Octomap/octomap.git 
或者使用/3rdapart文件夹下源码
建议将源码复制在~/src文件夹下编译安装，install之后源码文件夹不建议删除


* 安装依赖项
sudo apt-get install doxygen

* 获取源码，Github下载，或者使用/3rdparty文件夹下源码
cd ~/src
git clone https://github.com/Octomap/octomap.git


* 编译安装
cd octomap
mkdir build
cd build
camke ..
make -j4
sudo make install








# 1.To build the code : 
mkdir build
cd ./build
cmake ..
make





# 2.Usage:

* 拼接点云（带滤波）
cd build
./pointcloud_mapping
输出点云地图map.pcd

* 查看点云地图:
$ pcl_viewer map.pcd


* 八叉树地图构建
cd build
./octomap_mapping
输出cotmap.bt

* 查看八叉树地图:
$ octovis cotmap.bt
暂且看不了，我的电脑...女朋友发脾气了，哄不好，心塞 =_=

======================20180920更新 Sunnie======================
你的output文件夹里八叉树地图名字是octomap.bt,不是cotmap.bt
是不是被女朋友气傻了 →_→
